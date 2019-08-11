#include "SpiriController.h"
#include "WaitForChildren.h"
#include "FaultToleranceLoopFunctions.h"

void Spiri_controller::Init(TConfigurationNode& node) {
  compassSensor = GetSensor<argos::CCI_PositioningSensor>("positioning");
  positionActuator = GetActuator<argos::CCI_QuadRotorPositionActuator>("quadrotor_position");
  movement = new MovementVector();
}

void Spiri_controller::ControlStep() {
  finished = movement->step();
}

void Spiri_controller::Reset() {
  finished = false;
  setupPosition();
}

bool Spiri_controller::IsFinished() {
  return finished;
}

void Spiri_controller::Setup(int id, int level, argos::CVector3 offset, std::vector<Spiri_controller*>* controllers) {
  this->id = id;
  this->level = level;
  this->offset = offset;
  this->controllers = controllers;
  setupPosition();
}

void Spiri_controller::SetParent(int parentId) {
  this->parentId = parentId;
}

void Spiri_controller::AddChild(int childId) {
  children.push_back(childId);
}

void Spiri_controller::setupPosition() {
  movement->reset();
  MoveToPosition* starting_position = new MoveToPosition(positionActuator, compassSensor);
  starting_position->init(offset);
  movement->add(starting_position);
  WaitForChildren* wait = new WaitForChildren(this, controllers);
  movement->add(wait);
}

void Spiri_controller::AddRecursiveWaypoint(CVector3 vector3) {
  MoveToPosition* waypoint = new MoveToPosition(positionActuator, compassSensor);
  waypoint->init(argos::CVector3(vector3.GetX() + offset.GetX(),
          vector3.GetY() + offset.GetY(),
          vector3.GetZ() + offset.GetZ()));

  Gradient_loop_functions& loopFunctions = static_cast<Gradient_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions());
  for(int i = 0; i < 2; i++) {
    for(int j = 0; j < 2; j++) {
      loopFunctions.coverage[(int)(vector3.GetX() + offset.GetX() + i + 500)][(int)(vector3.GetY() + offset.GetY() + j + 500)] = true;
    }
  }


  movement->add(waypoint);
  WaitForChildren* wait = new WaitForChildren(this, controllers);
  movement->add(wait);

  for(int childId : children) {
    Spiri_controller* child = controllers->at(childId );
    child->AddRecursiveWaypoint(vector3);
  }
}

REGISTER_CONTROLLER(Spiri_controller, "Spiri_controller")