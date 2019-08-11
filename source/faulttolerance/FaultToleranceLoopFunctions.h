#ifndef GRADIENT_LOOP_FUNCTIONS_H_
#define GRADIENT_LOOP_FUNCTIONS_H_

#include <vector>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/spiri/simulator/spiri_entity.h>
#include "SpiriController.h"

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
	std::vector<argos::CVector3> GetWaypoints() { return waypoints; }
	
	CColor GetFloorColor(const CVector2& p) { return CColor::WHITE; }
  bool coverage[1000][1000];

private:

	int spiralIndex = 0;
	Spiri_controller* rootController;
  std::vector<Spiri_controller*> controllers;
  std::vector<argos::CVector3> waypoints;

  int calculateParent(int level, argos::CVector3 childOffset);
	argos::CVector3 buildArchimedesSpiralWaypoint(int index, int size, int loops, double radius);
};

#endif /* GRADIENT_LOOP_FUNCTIONS_H_ */