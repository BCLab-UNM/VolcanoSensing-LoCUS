#include "SpiriController.h"

void Spiri_controller::Init(TConfigurationNode& node) {
  positionActuator = GetActuator<argos::CCI_QuadRotorPositionActuator>("quadrotor_position");
  movement = new MovementVector();
}


void Spiri_controller::ControlStep() {
  finished = movement->step();
}

void Spiri_controller::Reset() {
  finished = false;
  setupPosition(index);
}


bool Spiri_controller::IsFinished() {
  return false;
}


void Spiri_controller::SetIndex(int index) {
  this->index = index;
  setupPosition(index);
}

void Spiri_controller::setupPosition(int index) {

  double rmax = 3;
  double rmin = 4;

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
  MoveToPosition* starting_position = new MoveToPosition(positionActuator);
  starting_position->init(argos::CVector3(x, y, 10), false);
  movement->add(starting_position);
  Delay* delay = new Delay(30);
  movement->add(delay);
  MoveToPosition* next_position1 = new MoveToPosition(positionActuator);
  next_position1->init(argos::CVector3(10, 0, 10), true);
  movement->add(next_position1);
  Delay* delay1 = new Delay(30);
  movement->add(delay1);
  MoveToPosition* next_position2 = new MoveToPosition(positionActuator);
  next_position2->init(argos::CVector3(-10, 10, 10), true);
  movement->add(next_position2);
  Delay* delay2 = new Delay(30);
  movement->add(delay2);
  MoveToPosition* next_position3 = new MoveToPosition(positionActuator);
  next_position3->init(argos::CVector3(-10, -10, 10), true);
  movement->add(next_position3);
  Delay* delay3 = new Delay(30);
  movement->add(delay3);
  MoveToPosition* next_position4 = new MoveToPosition(positionActuator);
  next_position4->init(argos::CVector3(10, -10, 10), true);
  movement->add(next_position4);
}

REGISTER_CONTROLLER(Spiri_controller, "Spiri_controller")