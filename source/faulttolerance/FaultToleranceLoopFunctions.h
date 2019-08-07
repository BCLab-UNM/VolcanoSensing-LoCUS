#ifndef GRADIENT_LOOP_FUNCTIONS_H_
#define GRADIENT_LOOP_FUNCTIONS_H_

#include <vector>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/spiri/simulator/spiri_entity.h>

using namespace argos;
using namespace std;

class Gradient_loop_functions : public CLoopFunctions {
public:
	Gradient_loop_functions(){};
  ~Gradient_loop_functions() {}
	void Init(TConfigurationNode& node);
  void Destroy();
	void Reset();
	bool IsExperimentFinished();
	
	CColor GetFloorColor(const CVector2& p) { return CColor::WHITE; }

private:

};

#endif /* GRADIENT_LOOP_FUNCTIONS_H_ */