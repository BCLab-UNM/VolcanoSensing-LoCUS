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
        if (controllers.at(failId)->IsFailed()) {
          j--;
        } else {
          controllers.at(failId)->fail();
        }
      }
    }
  }

  // TODO: movement?
}

void Gradient_loop_functions::Reset() {
  simulationTime = 0;
}

bool Gradient_loop_functions::IsExperimentFinished() {
  for(Spiri_controller* controller : controllers) {
    if(controller->GetReading().getValue() > 0.95) {
      return true;
    }
  }
  return false;
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