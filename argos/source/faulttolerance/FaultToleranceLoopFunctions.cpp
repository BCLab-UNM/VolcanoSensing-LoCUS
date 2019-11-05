#include "FaultToleranceLoopFunctions.h"
#include "SpiriController.h"
#include "SpokeSpiralSearch.h"

void Gradient_loop_functions::Init(TConfigurationNode& node) {

  TConfigurationNode simNode  = GetNode(node, "simulation");

  bool perturbPlume;
  string failureProbabilityString;
  string plumeFailureProbabilityString;
  GetNodeAttribute(simNode, "FailureProbability", failureProbabilityString);
  GetNodeAttribute(simNode, "PlumeFailureProbability", plumeFailureProbabilityString);
  failureProbability = stof(failureProbabilityString);
  plumeFailureProbability = stof(plumeFailureProbabilityString);
  GetNodeAttribute(simNode, "PerturbPlume", perturbPlume);
  long seed = 0;
  GetNodeAttributeOrDefault(simNode, "Seed", seed, (long)0);
  string droneFailureString;
  string randomFailureString;
  string emptyDroneFailureString = "";
  GetNodeAttributeOrDefault(simNode, "DroneFailures", droneFailureString, emptyDroneFailureString);
  GetNodeAttributeOrDefault(simNode, "RandomFailures", randomFailureString, emptyDroneFailureString);

  cout << "Configuration:"
          "\nradius:" << stopRadius <<
          "\nperturbPlume: " << perturbPlume <<
          "\nfailureProbability: " << failureProbability <<
          "\nplumeFailureProbability: " << plumeFailureProbability <<
          "\nseed:" << seed <<
          "\nfailures:" << droneFailureString <<
          "\nrandomfailures:" << randomFailureString << endl;

  srand(seed);

  int radius = 1000;

  int plumeX;
  int plumeY;

  do{
    plumeX = radius - (rand() % (radius * 2));
    plumeY = radius - (rand() % (radius * 2));
  } while(sqrt((plumeX * plumeX) + (plumeY * plumeY)) > radius);
  cout << "x: " << plumeX << " y:" << plumeY << " radius: " << sqrt((plumeX * plumeX) + (plumeY * plumeY)) << endl;

  plume.Init(plumeX, plumeY, perturbPlume);

  loadDroneFailures(droneFailureString, randomFailureString);

  CSpace::TMapPerType& spiris = GetSpace().GetEntitiesByType("spiri");

  for (auto &spiri : spiris) {
    CSpiriEntity &spiriEntity = *any_cast<CSpiriEntity *>(spiri.second);
    auto &controller = (Spiri_controller &) (spiriEntity.GetControllableEntity().GetController());

    controllers.push_back(&controller);

    controller.AddMovement(new SpokeSpiralSearch(&controller, radius / 10));
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
  for(Spiri_controller* controller : controllers) {
    if(!controller->IsFailed()) {
      double readingValue = controller->GetReading().getValue();
      if ((1.0 * rand() / RAND_MAX) <= failureProbability ||
              (1.0 * rand() / RAND_MAX) <= (plumeFailureProbability * readingValue)) {
        cout << "Controller " << controller->GetId() << " failed at " << simulationTime << endl;
        controller->fail();
      } else if ((1.0 * rand() / RAND_MAX) <= (plumeFailureProbability * readingValue)) {
        cout << "Controller " << controller->GetId() << " failed at " << simulationTime << " from plume." << endl;
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
}

void Gradient_loop_functions::Reset() {
  simulationTime = 0;
}

bool Gradient_loop_functions::IsExperimentFinished() {
  int activeDrones = 0;
  bool foundSource = false;
  CVector3 plumeLocation(plume.getXMax() / 10, plume.getYMax() / 10, 0);

  for(Spiri_controller* controller : controllers) {
    PositionReading reading = controller->GetReading();
    CVector3 plumeDistance = reading.getLocation() - plumeLocation;
    double distance = plumeDistance.Length();
    if(distance < 1) {
      foundSource = true;
    }
    if(!controller->IsFailed()) {
      activeDrones++;
    }
  }


  if (activeDrones == 0) {
    LOG << "failed:true" << endl;
  }

  return foundSource || simulationTime > 1000000 || activeDrones == 0;
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