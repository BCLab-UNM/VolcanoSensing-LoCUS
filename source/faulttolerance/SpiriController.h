#ifndef SPIRI_CONTROLLER_H_
#define SPIRI_CONTROLLER_H_

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
#include <stdlib.h>
#include "MovementVector.h"
#include "MoveToPosition.h"
#include "Delay.h"
#include <math.h>

using namespace argos;
using namespace std;

class Spiri_controller : public CCI_Controller, public CLoopFunctions {

public:
  void Init(TConfigurationNode& node);
  void ControlStep();
  bool IsFinished();
  void Reset();
  void SetIndex(int index);

private:

  bool finished = false;
  MovementVector* movement;
  argos::CCI_QuadRotorPositionActuator* positionActuator;
  int index;
  void setupPosition(int index) ;


};

#endif /* SPIRI_CONTROLLER_H_ */