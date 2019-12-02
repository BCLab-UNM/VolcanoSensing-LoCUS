#ifndef GRADIENT_ARGOS_WAITFORCHILDREN_H
#define GRADIENT_ARGOS_WAITFORCHILDREN_H

#include <argos3/core/utility/math/vector2.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <stdlib.h>
#include "Movement.h"
#include "SpiriController.h"

class WaitForChildren : public virtual Movement {

public:

  WaitForChildren(Spiri_controller* root, std::vector<Spiri_controller*>* controllers) {
    this->controllers = controllers;
    this->root = root;
  }

  bool step();

private:

  std::vector<Spiri_controller*>* controllers;
  Spiri_controller* root;

};


#endif //GRADIENT_ARGOS_WAITFORCHILDREN_H
