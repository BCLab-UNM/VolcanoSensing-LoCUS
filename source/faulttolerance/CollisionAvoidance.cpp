#include "CollisionAvoidance.h"

bool CollisionAvoidance::step() {
  bool colliding = false;

  /* Get readings from proximity sensor */
  const argos::CCI_FootBotProximitySensor::TReadings& proximityReadings = proximitySensor->GetReadings();

  for(size_t i = 0; i < proximityReadings.size(); ++i) {
    colliding |= proximityReadings[i].Value > 0.9;
  }

  if(colliding) {
    double left = ((double)rand() / RAND_MAX) * 16;
    double right = ((double)rand() / RAND_MAX) * 16;
    motorActuator->SetLinearVelocity(left, right);
    return false;
  } else {
    return delegate->step();
  }
}
