#include "FaultToleranceController.h"

void Gradient_controller::Init(TConfigurationNode& node) {
  compassSensor = GetSensor<CCI_PositioningSensor>("positioning");
  motorActuator = GetActuator<CCI_DifferentialSteeringActuator>("differential_steering");
  proximitySensor = GetSensor<argos::CCI_FootBotProximitySensor>("footbot_proximity");

  movement = new MovementVector();
}

bool Gradient_controller::isColliding() {
  bool colliding = false;

  /* Get readings from proximity sensor */
  const argos::CCI_FootBotProximitySensor::TReadings& proximityReadings = proximitySensor->GetReadings();

  for(size_t i = 0; i < proximityReadings.size(); ++i) {
    colliding |= proximityReadings[i].Value > 0;
  }

  return colliding;
}

void Gradient_controller::ControlStep() {
  finished = movement->step();
}

void Gradient_controller::Reset() {
  finished = false;
  setupPosition(index);
}

bool Gradient_controller::IsFinished() {
  return finished;
}

void Gradient_controller::SetIndex(int index) {
  this->index = index;
  setupPosition(index);
}

void Gradient_controller::setupPosition(int index) {

  double rmax = 2;
  double rmin = 2;

  int level = 0;
  int innerswarmcount = 0;
  int levelchildrencount = 0;
  double radius = 0;
  while (innerswarmcount < index) {
    level = level + 1;
    radius = ((level + 1) * rmax);
    double levelangle = 2 * asin(rmin / (2 * radius));
    levelchildrencount = int((M_PI * 2) / levelangle);
    innerswarmcount = innerswarmcount + levelchildrencount;
  }

  double trueangle = 0;
  if(levelchildrencount > 0) {
    trueangle = (M_PI * 2) / levelchildrencount;
    radius = radius - rmax;
  }

  double x = radius * cos((index - (innerswarmcount - levelchildrencount)) * trueangle);
  double y = radius * sin((index - (innerswarmcount - levelchildrencount)) * trueangle);

  movement->reset();
  MoveTo* staring_position = new MoveTo(compassSensor, motorActuator);
  staring_position->init(argos::CVector2(x, y));
  CollisionAvoidance* avoidance = new CollisionAvoidance(staring_position, proximitySensor, motorActuator);
  movement->add(avoidance);
}

REGISTER_CONTROLLER(Gradient_controller, "Gradient_controller")