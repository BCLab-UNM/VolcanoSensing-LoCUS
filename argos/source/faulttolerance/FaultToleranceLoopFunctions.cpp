#include "FaultToleranceLoopFunctions.h"
#include "SpiriController.h"
#include "SpokeSpiralSearch.h"

void Gradient_loop_functions::Init(TConfigurationNode& node) {

  plume.Init();

  TConfigurationNode simNode  = GetNode(node, "simulation");

  GetNodeAttribute(simNode, "StopCoverageRadius", stopRadius);
  long seed = 0;
  GetNodeAttributeOrDefault(simNode, "Seed", seed, (long)0);
  string droneFailureString;
  string randomFailureString;
  string emptyDroneFailureString = "";
  GetNodeAttributeOrDefault(simNode, "DroneFailures", droneFailureString, emptyDroneFailureString);
  GetNodeAttributeOrDefault(simNode, "RandomFailures", randomFailureString, emptyDroneFailureString);

  cout << "Configuration:"
          "\nradius:" << stopRadius <<
          "\nseed:" << seed <<
          "\nfailures:" << droneFailureString <<
          "\nrandomfailures:" << randomFailureString << endl;

  srand(seed);

  loadDroneFailures(droneFailureString, randomFailureString);

  CSpace::TMapPerType& spiris = GetSpace().GetEntitiesByType("spiri");

  for (auto &spiri : spiris) {
    CSpiriEntity &spiriEntity = *any_cast<CSpiriEntity *>(spiri.second);
    auto &controller = (Spiri_controller &) (spiriEntity.GetControllableEntity().GetController());

    controllers.push_back(&controller);

    controller.AddMovement(new SpokeSpiralSearch(&controller));
  }
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

  // TODO: movement?
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
  simulationTime = 0;
}

bool Gradient_loop_functions::IsExperimentFinished() {
  return false; // TODO
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

REGISTER_LOOP_FUNCTIONS(Gradient_loop_functions, "Gradient_loop_functions")