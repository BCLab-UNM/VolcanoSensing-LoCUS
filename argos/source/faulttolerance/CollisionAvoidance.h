#ifndef GRADIENT_ARGOS_COLLISIONAVOIDANCE_H
#define GRADIENT_ARGOS_COLLISIONAVOIDANCE_H

#include <argos3/plugins/robots/foot-bot/control_interface/ci_footbot_proximity_sensor.h>
#include <argos3/plugins/robots/generic/control_interface/ci_differential_steering_actuator.h>
#include <stdlib.h>
#include "Movement.h"

using namespace argos;
using namespace std;

class CollisionAvoidance : public virtual Movement {

public:
  CollisionAvoidance(Movement* inputDelegate, CCI_FootBotProximitySensor* inputProximitySensor, CCI_DifferentialSteeringActuator* motorActuator):
  delegate(inputDelegate),
  proximitySensor(inputProximitySensor),
  motorActuator(motorActuator)
  {}

  bool step();

private:
  Movement* delegate;
  CCI_FootBotProximitySensor* proximitySensor;
  CCI_DifferentialSteeringActuator* motorActuator;

};


#endif //GRADIENT_ARGOS_COLLISIONAVOIDANCE_H
