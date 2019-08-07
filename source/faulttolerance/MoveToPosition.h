#ifndef GRADIENT_ARGOS_MOVETO_H
#define GRADIENT_ARGOS_MOVETO_H

#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
#include "Movement.h"

class MoveToPosition : public virtual Movement {

public:

  MoveToPosition(argos::CCI_QuadRotorPositionActuator* positionActuator) {
    this->positionActuator = positionActuator;
  }

  void init(argos::CVector3 target, bool relative);
  bool step();

private:

  bool relative;
  argos::CVector3 target;
  argos::CCI_QuadRotorPositionActuator* positionActuator;

};


#endif //GRADIENT_ARGOS_MOVETO_H
