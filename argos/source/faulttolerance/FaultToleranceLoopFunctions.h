#ifndef GRADIENT_LOOP_FUNCTIONS_H_
#define GRADIENT_LOOP_FUNCTIONS_H_

#include <vector>
#include <queue>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/spiri/simulator/spiri_entity.h>
#include "SpiriController.h"
#include "SwarmManager.h"
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
  double rmax = 2;

  void SetRootController(Spiri_controller *toReplace, Spiri_controller *heir);

  Spiri_controller* getRootController() { return rootController;}

private:

  double thetaOffset = 0;
  const int radius = 1000;
  double rmin = 1;
  bool encounteredPlume = false;
  double failureProbability = 0;
  double plumeFailureProbability = 0;
  int fullshells = 1;
	int spiralIndex = 0;
	bool constellation_setup = false;
	vector<int> droneToFail;
	vector<int> droneToFailAtTime;
  vector<int> randomFail;
  vector<int> randomFailAtTime;
	Spiri_controller* rootController;
  std::vector<Spiri_controller*> controllers;
  std::vector<argos::CVector3> waypoints;
  std::vector<PositionReading> collectedReadings;
  SwarmManager* swarmManager;
  argos::CVector3 currentPosition;

  long simulationTime = 0;
  bool healing = false;
  long healStart = 0;
	argos::CVector3 buildArchimedesSpiral(int index, double radius);

  void loadDroneFailures(string basic_string, string randomFailureString);

  void healFailedSwarm();

	vector<Spiri_controller*> getNextFailures();

  SimplePlume plume = SimplePlume(1000, 400);

  Eigen::Vector2f linearRegression(vector<PositionReading> vector);
};

#endif /* GRADIENT_LOOP_FUNCTIONS_H_ */