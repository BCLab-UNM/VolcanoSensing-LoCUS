#ifndef GRADIENT_LOOP_FUNCTIONS_H_
#define GRADIENT_LOOP_FUNCTIONS_H_

#include <vector>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/spiri/simulator/spiri_entity.h>
#include "SpiriController.h"
#include <list>

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
	std::vector<argos::CVector3> GetWaypoints() { return waypoints; }
	
	CColor GetFloorColor(const CVector2& p) { return CColor::WHITE; }
  bool coverage[1000][1000];
  double rmax = 2;

private:

  double rmin = 1;
  double stopRadius = 100;
	int points_per_rotation = 100;
  int fullshells = 1;
	int spiralIndex = 0;
	bool constellation_setup = false;
	vector<int> droneToFail;
	vector<int> droneToFailAtTime;
	Spiri_controller* rootController;
  std::vector<Spiri_controller*> controllers;
  std::vector<argos::CVector3> waypoints;

  long simulationTime = 0;
  int calculateParent(int level, argos::CVector3 childOffset);
	argos::CVector3 buildArchimedesSpiralWaypoint(int index, double radius);

  void loadDroneFailures(string basic_string);

  void initCoverage();

  void healFailedSwarm();

	vector<Spiri_controller*> getNextFailures();
};

#endif /* GRADIENT_LOOP_FUNCTIONS_H_ */