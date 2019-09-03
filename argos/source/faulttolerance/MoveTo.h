#ifndef GRADIENT_ARGOS_MOVETO_H
#define GRADIENT_ARGOS_MOVETO_H

#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include "Movement.h"

class MoveTo : public virtual Movement {

public:

  MoveTo(argos::CCI_PositioningSensor* compassSensor, argos::CCI_DifferentialSteeringActuator* motorActuator) {
    this->compassSensor = compassSensor;
    this->motorActuator = motorActuator;
  }

  void init(argos::CVector2 target);
  bool step();

private:

  void SetWheelSpeedsFromVector(const argos::CVector2& c_heading);
  argos::CVector2 target;

  argos::CCI_PositioningSensor* compassSensor;
  argos::CCI_DifferentialSteeringActuator* motorActuator;

};


#endif //GRADIENT_ARGOS_MOVETO_H
