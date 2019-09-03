//
// Created by john on 3/22/19.
//

#ifndef GRADIENT_ARGOS_WAYPOINTUTIL_H
#define GRADIENT_ARGOS_WAYPOINTUTIL_H


#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include "Movement.h"
#include "MoveTo.h"

class WaypointUtil {
public:
  static std::vector<Movement*> buildDDSAWaypoints(
          argos::CCI_PositioningSensor* compassSensor,
          argos::CCI_DifferentialSteeringActuator* motorActuator,
          int size, int index, int loops, double radius);

  static std::vector<Movement*> buildLawnmowerWaypoints (
          argos::CCI_PositioningSensor* compassSensor,
          argos::CCI_DifferentialSteeringActuator* motorActuator,
          int size, int index, int cycles, double radius, int width, int height);
};


#endif //GRADIENT_ARGOS_WAYPOINTUTIL_H
