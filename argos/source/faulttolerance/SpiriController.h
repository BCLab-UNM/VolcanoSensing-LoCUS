#ifndef SPIRI_CONTROLLER_H_
#define SPIRI_CONTROLLER_H_

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <stdlib.h>
#include "MovementVector.h"
#include "MoveToPosition.h"
#include "Delay.h"
#include "PositionReading.h"

using namespace argos;
using namespace std;

class Spiri_controller : public CCI_Controller, public CLoopFunctions {

public:
  void Init(TConfigurationNode& node);
  void ControlStep();
  bool IsFinished();
  void Reset();
  void fail();
  void AddWaypoint(CVector3 waypoint);
  void AddMovement(Movement *move);
  bool failureDetected();

  bool failed = false;

  std::vector<PositionReading> getReadings();

  argos::CCI_QuadRotorPositionActuator* GetPositionActuator() { return positionActuator;}

  argos::CCI_PositioningSensor* GetCompassSensor() { return compassSensor;}

private:

  bool stopped = false;
  bool finished = false;
  MovementVector* movement;
  argos::CCI_PositioningSensor* compassSensor;
  argos::CCI_QuadRotorPositionActuator* positionActuator;

  argos::CVector3 buildArchimedesSpiralWaypoint(int index, double radius);
};

#endif /* SPIRI_CONTROLLER_H_ */