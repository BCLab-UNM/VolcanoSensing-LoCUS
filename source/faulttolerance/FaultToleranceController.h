#ifndef GRADIENT_CONTROLLER_H_
#define GRADIENT_CONTROLLER_H_

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <stdlib.h>
#include "FaultToleranceLoopFunctions.h"
#include "MoveTo.h"
#include "MovementVector.h"
#include "Delay.h"
#include "WaypointUtil.h"
#include "CollisionAvoidance.h"
#include <math.h>

using namespace argos;
using namespace std;

class Gradient_controller : public CCI_Controller, public CLoopFunctions {

public:
  void Init(TConfigurationNode& node);
  void ControlStep();
  void Reset();
  void SetIndex(int index);
  bool IsFinished();

private:

  bool finished = false;
  CCI_PositioningSensor* compassSensor;
  CCI_DifferentialSteeringActuator* motorActuator;
  CCI_FootBotProximitySensor* proximitySensor;
  MovementVector* movement;
  bool first = true;

  bool isColliding();

  int index;

  void setupPosition(int index) ;
};

#endif /* GRADIENT_CONTROLLER_H_ */