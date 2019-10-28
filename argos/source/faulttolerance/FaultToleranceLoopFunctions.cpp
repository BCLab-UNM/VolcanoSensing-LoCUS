#include "FaultToleranceLoopFunctions.h"
#include "SpiriController.h"
#include "EmptyMovement.h"
#include "ThenMovement.h"
#include "ReplaceWithHeir.h"
#include "BalanceMovement.h"

void Gradient_loop_functions::Init(TConfigurationNode& node) {

  TConfigurationNode simNode  = GetNode(node, "simulation");

  bool perturbPlume;
  string failureProbabilityString;
  string plumeFailureProbabilityString;
  GetNodeAttribute(simNode, "FailureProbability", failureProbabilityString);
  GetNodeAttribute(simNode, "PlumeFailureProbability", plumeFailureProbabilityString);
  failureProbability = stof(failureProbabilityString);
  plumeFailureProbability = stof(plumeFailureProbabilityString);
  GetNodeAttribute(simNode, "RMin", rmin);
  GetNodeAttribute(simNode, "RMax", rmax);
  GetNodeAttribute(simNode, "PerturbPlume", perturbPlume);
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
          "\nperturbPlume: " << perturbPlume <<
          "\nfailureProbability: " << failureProbability <<
          "\nplumeFailureProbability: " << plumeFailureProbability <<
          "\nseed:" << seed <<
          "\nfailures:" << droneFailureString <<
          "\nrandomfailures:" << randomFailureString << endl;

  srand(seed);

  int plumeX;
  int plumeY;

  do{
    plumeX = radius - (rand() % (radius * 2));
    plumeY = radius - (rand() % (radius * 2));
  } while(sqrt((plumeX * plumeX) + (plumeY * plumeY)) > radius);

  LOG << "x: " << plumeX << " y:" << plumeY << " radius: " << sqrt((plumeX * plumeX) + (plumeY * plumeY)) << endl;

  plume.Init(plumeX, plumeY, perturbPlume);

  loadDroneFailures(droneFailureString, randomFailureString);

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
  fullshells = rootController->GetMinimumDepth();
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
  for(Spiri_controller* controller : controllers) {
    if(!controller->IsFailed()) {
      double readingValue = controller->GetReading().getValue();
      if ((1.0 * rand() / RAND_MAX) <= failureProbability ||
          (1.0 * rand() / RAND_MAX) <= (plumeFailureProbability * readingValue)) {
        LOG << "Controller " << controller->id << " failed at " << simulationTime << endl;
        controller->fail();
      } else if ((1.0 * rand() / RAND_MAX) <= (plumeFailureProbability * readingValue)) {
        LOG << "Controller " << controller->id << " failed at " << simulationTime << " from plume." << endl;
        controller->fail();
      }
    }
  }

  if (!encounteredPlume) {
    for(Spiri_controller* controller : controllers) {
      if(controller->GetReading().getValue() > 0.005) {
        encounteredPlume = true;
        LOG << "encountered plume: " << simulationTime << endl;
        break;
      }
    }
  }

  if (rootController->failureDetected()) {
    LOG << "Healing size " << rootController->GetSwarmSize() << endl;
    healing = true;
    healStart = simulationTime;
    healFailedSwarm();
  } else {
    if (rootController->IsFinished()) {
      if (!constellation_setup) {
        LOG << "Setup:" << simulationTime << endl;
        constellation_setup = true;
      }
      if(healing) {
        healing = false;
        LOG << "Healing took: " << (simulationTime - healStart) << endl;
      }
      int currentFullShells = rootController->GetMinimumDepth();
      std::vector<PositionReading> readings = rootController->getReadings(currentFullShells);
      for(PositionReading reading : readings) {
        readingQueue.push_front(reading);
      }
      while(readingQueue.size() > 100) {
        readingQueue.pop_back();
      }
      Eigen::Vector2f vector = linearRegression(readings);
      if(vector.norm() > 0) {
        currentPosition += argos::CVector3(vector(0),  vector(1), 0);
        currentPosition += argos::CVector3(rand() % 100 / 1000.0, rand() % 100 / 1000.0, 0);
        rootController->AddRecursiveWaypoint(currentPosition);
        waypoints.push_back(currentPosition);
      }
      else {
        argos::CVector3 waypoint = buildArchimedesSpiral(++spiralIndex, 2.0 * (fullshells - 0.5) * rmax);
        currentPosition = waypoint;
        rootController->AddRecursiveWaypoint(waypoint);
        waypoints.push_back(waypoint);
      }
    }
  }
}

void Gradient_loop_functions::healFailedSwarm() {

  Finishable* lastMovement = new EmptyMovement();
  argos::CVector3 waypoint = buildArchimedesSpiral(spiralIndex, 2.0 * (fullshells - 0.5) * rmax);
  for(Spiri_controller* failedController : getNextFailures()) {
    Movement* replaceWithHeir = new ReplaceWithHeir(this, failedController, waypoint, &controllers, swarmManager);
    ThenMovement* waitForPrevious = new ThenMovement(lastMovement, replaceWithHeir);
    failedController->AddMovement(waitForPrevious);
    failedController->AddWaitForChildren(&controllers);
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

    if(node->failed && !node->processedFail) {
      failedVector.push_back(node);
      node->processedFail = true;
    }

    for(ControllerBase* childController : swarmManager->GetChildren(node)) {
      nodes.push_back(dynamic_cast<Spiri_controller*>(childController));
    }
  }

  std::reverse(failedVector.begin(), failedVector.end());

  return failedVector;
}

argos::CVector3 Gradient_loop_functions::buildArchimedesSpiral(int index, double radius) {
  double altitude = 0;

  // https://www.comsol.com/blogs/how-to-build-a-parameterized-archimedean-spiral-geometry/
  double b = 2 * radius / (2 * M_PI);

  double angle = index * 2 * M_PI / 30;
  double point_radius = b * angle;
  double xoffset = point_radius * cos(angle); // in meters
  double yoffset = point_radius * sin(angle);

  return argos::CVector3(xoffset, yoffset, altitude);
}

void Gradient_loop_functions::Reset() {
  spiralIndex = 0;
  simulationTime = 0;
}

bool Gradient_loop_functions::IsExperimentFinished() {
  bool foundSource = false;
  int currentFullShells = rootController->GetMinimumDepth();
  currentFullShells = currentFullShells > 1 ? currentFullShells : 2;
  std::vector<PositionReading> readings = rootController->getReadings(currentFullShells);
  double minDistance = 1000000;
  CVector3 plumeLocation(plume.getXMax() / 10, plume.getYMax() / 10, 0);
  for(PositionReading reading : readings) {
    CVector3 plumeDistance = reading.getLocation() - plumeLocation;
    double distance = plumeDistance.Length();
    if(minDistance > distance) {
      minDistance = distance;
    }
    if(distance < 3) {
      foundSource = true;
    }
  }
  if (rootController->GetSwarmSize() == 0) {
    LOG << "failed:true" << endl;
  }
  return foundSource || simulationTime > 1000000 || rootController->GetSwarmSize() == 0;
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

  Eigen::VectorXf x = A.bdcSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(b);

  Eigen::Vector2f x2;
  x2 << x(1), x(2);
  x2.normalize();

  return x2;
}

REGISTER_LOOP_FUNCTIONS(Gradient_loop_functions, "Gradient_loop_functions")