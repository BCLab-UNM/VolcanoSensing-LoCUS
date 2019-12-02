#ifndef GRADIENT_LOOP_FUNCTIONS_H_
#define GRADIENT_LOOP_FUNCTIONS_H_

#include <vector>
#include <queue>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/spiri/simulator/spiri_entity.h>
#include "SpiriController.h"
#include "SimplePlume.h"
#include "PositionReading.h"
#include <list>
#include <cstdlib>

using namespace argos;
using namespace std;

class Gradient_loop_functions : public CLoopFunctions {
public:
	Gradient_loop_functions(){};
  ~Gradient_loop_functions() {}
	void Init(TConfigurationNode& node);
	void Reset();
	void PostStep();
	bool IsExperimentFinished();
	void PostExperiment();
  void Destroy();
  SimplePlume getPlume() { return plume;}
  std::vector<Spiri_controller*> GetControllers() { return controllers;}

	std::vector<argos::CVector3> GetWaypoints() { return waypoints; }
	
	CColor GetFloorColor(const CVector2& p) { return CColor::WHITE; }

private:

  bool encounteredPlume = false;
  double failureProbability = 0;
  double plumeFailureProbability = 0;
	vector<int> droneToFail;
	vector<int> droneToFailAtTime;
  vector<int> randomFail;
  vector<int> randomFailAtTime;
  std::vector<Spiri_controller*> controllers;
  std::vector<argos::CVector3> waypoints;

  long simulationTime = 0;

  void loadDroneFailures(string basic_string, string randomFailureString);

  SimplePlume plume = SimplePlume(2000, 400);
};

#endif /* GRADIENT_LOOP_FUNCTIONS_H_ */