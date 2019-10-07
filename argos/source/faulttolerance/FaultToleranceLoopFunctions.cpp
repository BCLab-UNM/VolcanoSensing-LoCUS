#include "FaultToleranceLoopFunctions.h"
#include "SpiriController.h"
#include "EmptyMovement.h"
#include "ThenMovement.h"
#include "ReplaceWithHeir.h"
#include "BalanceMovement.h"

void Gradient_loop_functions::Init(TConfigurationNode& node) {

  plume.Init();

  TConfigurationNode simNode  = GetNode(node, "simulation");

  GetNodeAttribute(simNode, "RMin", rmin);
  GetNodeAttribute(simNode, "RMax", rmax);
  GetNodeAttribute(simNode, "StopCoverageRadius", stopRadius);
  long seed = 0;
  GetNodeAttributeOrDefault(simNode, "Seed", seed, (long)0);
  string droneFailureString;
  string randomFailureString;
  string emptyDroneFailureString = "";
  GetNodeAttributeOrDefault(simNode, "DroneFailures", droneFailureString, emptyDroneFailureString);
  GetNodeAttributeOrDefault(simNode, "RandomFailures", randomFailureString, emptyDroneFailureString);

  cout << "Configuration:"
          "\nrmin:" << rmin <<
          "\nrmax:" << rmax <<
          "\nradius:" << stopRadius <<
          "\nseed:" << seed <<
          "\nfailures:" << droneFailureString <<
          "\nrandomfailures:" << randomFailureString << endl;

  srand(seed);

  loadDroneFailures(droneFailureString, randomFailureString);

  initCoverage();

  for(int i = 0; i < points_per_rotation; i++) {
    radii[i] = 0;
    loop[i] = -1;
  }

  swarmManager = new SwarmManager(rmin, rmax);

  CSpace::TMapPerType& spiris = GetSpace().GetEntitiesByType("spiri");

  bool first = true;
  for (auto &spiri : spiris) {
    CSpiriEntity &spiriEntity = *any_cast<CSpiriEntity *>(spiri.second);
    auto &controller = (Spiri_controller &) (spiriEntity.GetControllableEntity().GetController());

    if (first) {
      rootController = &controller;
      first = false;
    }

    controllers.push_back(&controller);
  }

  for(int i = 0; i < controllers.size(); i++) {
    SwarmLocation *location = swarmManager->GetLocation(i);
    Spiri_controller *controller = controllers.at(i);
    controller->Setup(i, location, &controllers, swarmManager);
    swarmManager->UpdateLocation(controller, location);
  }

  rootController->SetupHeir();
}

void Gradient_loop_functions::Destroy() {
  plume.Destroy();
}

void Gradient_loop_functions::PostStep() {
  simulationTime++;
  for (int i = 0; i < droneToFail.size(); i++) {
    int failId = droneToFail.at(i);
    int failTime = droneToFailAtTime.at(i);

    if (failId < controllers.size() && simulationTime == failTime) {
      controllers.at(failId)->fail();
    }
  }
  for (int i = 0; i < randomFail.size(); i++) {
    int failCount = randomFail.at(i);
    int failTime = randomFailAtTime.at(i);

    if (simulationTime == failTime) {
      for(int j = 0; j < failCount; j++) {
        int failId = rand() % controllers.size();
        if (controllers.at(failId)->failed) {
          j--;
        } else {
          controllers.at(failId)->fail();
        }
      }
    }
  }

  if (rootController->failureDetected()) {
    LOG << "Healing" << endl;
    healing = true;
    healStart = simulationTime;
    healFailedSwarm();
    spiralIndex--;
  } else {
    if (rootController->IsFinished()) {
      if (!constellation_setup) {
        LOG << "Setup:" << simulationTime << endl;
        constellation_setup = true;
      }
      int minimumDepth = rootController->GetMinimumDepth();
      if(fullshells != minimumDepth) {
        fullshells = minimumDepth;
        LOG << "Fullshells:" << fullshells << endl;
      }
      if(healing) {
        healing = false;
        LOG << "Healing took: " << (simulationTime - healStart) << endl;
      }

      std::vector<PositionReading> readings = rootController->getReadings();
      for(PositionReading reading : readings) {
        readingQueue.push_front(reading);
      }
      while(readingQueue.size() > 100) {
        readingQueue.pop_back();
      }
      Eigen::Vector2f vector = linearRegression(readings);
      if(vector.norm() > 0) {
        currentPosition += argos::CVector3(vector(0), vector(1), 0);
        cout << "currentPosition = " << currentPosition << endl;
        rootController->AddRecursiveWaypoint(currentPosition);
        waypoints.push_back(currentPosition);
      }
      else {
        argos::CVector3 waypoint = buildArchimedesSpiralWaypoint(++spiralIndex, 2.0 * (fullshells - 0.5) * rmax);
        currentPosition = waypoint;
        rootController->AddRecursiveWaypoint(waypoint);
        waypoints.push_back(waypoint);
      }
    }
  }
}

void Gradient_loop_functions::healFailedSwarm() {

  Finishable* lastMovement = new EmptyMovement();
  argos::CVector3 waypoint = buildArchimedesSpiralWaypoint(spiralIndex, 2.0 * (fullshells - 0.5) * rmax);
  for(Spiri_controller* failedController : getNextFailures()) {
    Movement* replaceWithHeir = new ReplaceWithHeir(this, failedController, waypoint, &controllers, swarmManager);
    ThenMovement* waitForPrevious = new ThenMovement(lastMovement, replaceWithHeir);
    failedController->AddMovement(waitForPrevious);
    lastMovement = waitForPrevious;
  }
  rootController->AddMovement(new BalanceMovement(this));
}

vector<Spiri_controller*> Gradient_loop_functions::getNextFailures() {

  // Find deepest failure by breadth first search and finding last failure
  std::list<Spiri_controller*> nodes;

  vector<Spiri_controller*> failedVector;

  nodes.push_back(rootController);

  while(!nodes.empty()) {
    Spiri_controller* node = nodes.front();
    nodes.pop_front();

    if(node->failed) {
      failedVector.push_back(node);
      node->failed = false;
    }

    for(ControllerBase* childController : swarmManager->GetChildren(node)) {
      nodes.push_back(dynamic_cast<Spiri_controller*>(childController));
    }
  }

  std::reverse(failedVector.begin(), failedVector.end());

  return failedVector;
}

argos::CVector3 Gradient_loop_functions::buildArchimedesSpiralWaypoint(int index, double radius) {
  int indexLoop = index / points_per_rotation;
  int indexMod = index % points_per_rotation;

  double angle = index * 2 * M_PI / points_per_rotation;

  if(loop[indexMod] != indexLoop) {
    // https://www.comsol.com/blogs/how-to-build-a-parameterized-archimedean-spiral-geometry/
    double b;
    double point_radius;

    if(index < points_per_rotation) {
      b = radius / (2 * M_PI);
      point_radius = b * angle;
    } else {
      point_radius = radii[indexMod] + radius;
    }
    radii[indexMod] = point_radius;
    loop[indexMod] = indexLoop;
  }

  double point_radius = radii[index % points_per_rotation];

  double altitude = 0;
  double xoffset = point_radius * cos(angle); // in meters
  double yoffset = point_radius * sin(angle);

  argos::CVector3 waypoint(xoffset, yoffset, altitude);

  return waypoint;
}

void Gradient_loop_functions::Reset() {
  spiralIndex = 0;
  simulationTime = 0;

  initCoverage();
}

bool Gradient_loop_functions::IsExperimentFinished() {
  bool surveyCovered = radii[spiralIndex % points_per_rotation] > stopRadius;
  return rootController->IsFinished() && surveyCovered;
}

void Gradient_loop_functions::PostExperiment() {
  LOG << "Time:" << simulationTime << endl;
  cout << "Time:" << simulationTime << endl;
}

void Gradient_loop_functions::loadDroneFailures(string droneFailureString, string randomFailureString) {

  vector<string> droneFailureStringTuples;
  Tokenize(droneFailureString, droneFailureStringTuples, ";");

  for (long i = 0; i < droneFailureStringTuples.size(); i++) {
    vector<string> failureTuple;
    Tokenize(droneFailureStringTuples.at(i), failureTuple, ",");

    if(failureTuple.size() >= 2) {
      droneToFail.push_back(atoi(failureTuple.at(0).c_str()));
      droneToFailAtTime.push_back(atoi(failureTuple.at(1).c_str()));
    }
  }

  vector<string> randomFailureStringTuples;
  Tokenize(randomFailureString, randomFailureStringTuples, ";");

  for (long i = 0; i < randomFailureStringTuples.size(); i++) {
    vector<string> failureTuple;
    Tokenize(randomFailureStringTuples.at(i), failureTuple, ",");

    if(failureTuple.size() >= 2) {
      randomFail.push_back(atoi(failureTuple.at(0).c_str()));
      randomFailAtTime.push_back(atoi(failureTuple.at(1).c_str()));
    }
  }
}

void Gradient_loop_functions::initCoverage() {
  for(int i = 0; i < 1000; i++) {
    for(int j = 0; j < 1000; j++) {
      coverage[i][j] = false;
    }
  }
}

void Gradient_loop_functions::SetRootController(Spiri_controller *toReplace, Spiri_controller *heir) {
  if(toReplace == rootController) {
    rootController = heir;
  }
}

Eigen::Vector2f Gradient_loop_functions::linearRegression(vector<PositionReading> readings) {

  Eigen::MatrixXf A(readings.size(), 3);
  Eigen::VectorXf b(readings.size());

  int i = 0;
  for(PositionReading reading : readings) {
    A(i, 0) = 1;
    A(i, 1) = reading.getLocation().GetX();
    A(i, 2) = reading.getLocation().GetY();
    //A(i, 3) = reading.getLocation().GetZ();
    b(i) = reading.getValue();
    i++;
  }

  cout << "A: " << A << endl;
  cout << "b: " << b << endl;

  Eigen::VectorXf x = (A.transpose() * A).ldlt().solve(A.transpose() * b);

  cout << "The solution using normal equations is:" << x << endl;

  Eigen::Vector2f x2;
  x2 << x(1), x(2);
  x2.normalize();

  return x2;
}

REGISTER_LOOP_FUNCTIONS(Gradient_loop_functions, "Gradient_loop_functions")