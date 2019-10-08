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
#include <eigen3/Eigen/Dense>

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

	std::vector<argos::CVector3> GetWaypoints() { return waypoints; }
	
	CColor GetFloorColor(const CVector2& p) { return CColor::WHITE; }

private:

  double stopRadius = 100;
	static const int points_per_rotation = 10;
	double radii[points_per_rotation];
  double loop[points_per_rotation];
	vector<int> droneToFail;
	vector<int> droneToFailAtTime;
  vector<int> randomFail;
  vector<int> randomFailAtTime;
  std::vector<Spiri_controller*> controllers;
  std::vector<argos::CVector3> waypoints;

  long simulationTime = 0;
	argos::CVector3 buildArchimedesSpiralWaypoint(int index, double radius);

  void loadDroneFailures(string basic_string, string randomFailureString);

  SimplePlume plume = SimplePlume(1000, 1000);
};

#endif /* GRADIENT_LOOP_FUNCTIONS_H_ */