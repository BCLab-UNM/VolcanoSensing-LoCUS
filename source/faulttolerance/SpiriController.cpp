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
  addCoverage(CVector3(0, 0, 0));
  movement->add(starting_position);
  WaitForChildren* wait = new WaitForChildren(this, controllers);
  movement->add(wait);
}

void Spiri_controller::AddRecursiveWaypoint(CVector3 waypoint) {
  MoveToPosition* moveTo = new MoveToPosition(positionActuator, compassSensor);
  moveTo->init(argos::CVector3(waypoint.GetX() + offset.GetX(),
                               waypoint.GetY() + offset.GetY(),
                               waypoint.GetZ() + offset.GetZ()));

  addCoverage(waypoint);


  movement->add(CreateOffsetMovement(waypoint));
  WaitForChildren* wait = new WaitForChildren(this, controllers);
  movement->add(wait);

  for(int childId : children) {
    Spiri_controller* child = controllers->at(childId );
    child->AddRecursiveWaypoint(waypoint);
  }
}

MoveToPosition* Spiri_controller::CreateOffsetMovement(CVector3 waypoint) {
  MoveToPosition* moveTo = new MoveToPosition(positionActuator, compassSensor);
  moveTo->init(argos::CVector3(waypoint.GetX() + offset.GetX(),
                               waypoint.GetY() + offset.GetY(),
                               waypoint.GetZ() + offset.GetZ()));

  return moveTo;
}

void Spiri_controller::AddMovement(Movement *move) {
  movement->add(move);
}

void Spiri_controller::fail() {
  failed = true;
}

void Spiri_controller::addCoverage(CVector3 waypoint) {

  Gradient_loop_functions& loopFunctions = static_cast<Gradient_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions());
  for(int i = 0; i < loopFunctions.rmax; i++) {
    for(int j = -1; j < loopFunctions.rmax; j++) {
      loopFunctions.coverage[(int)(waypoint.GetX() + offset.GetX() + i + 500)][(int)(waypoint.GetY() + offset.GetY() + j + 500)] = true;
    }
  }
}

void Spiri_controller::SetupHeir() {
  heir = GetHeir();

  for (int childId : children) {
    Spiri_controller *child = controllers->at(childId);
    child->SetupHeir();
  }
}

void Spiri_controller::SetupParentHeir() {
  heir = GetHeir();

  if(parentId != NO_HEIR) {
    Spiri_controller *parent = controllers->at(parentId);
    parent->SetupParentHeir();
  }

}

int Spiri_controller::GetHeir() {
  if(children.empty()) {
    return NO_HEIR;
  }
  int successor = GetSuccessor();
  if (successor != NO_HEIR) {
    return successor;
  }
  return GetPredecessor();
}

int Spiri_controller::GetSuccessor() {
  if(children.size() > 1) {
    return controllers->at(children.at(1))->leftmost();
  }
  return NO_HEIR;
}

int Spiri_controller::GetPredecessor() {
  if(!children.empty()) {
    return controllers->at(children.at(0))->rightmost();
  }
  return NO_HEIR;
}

int Spiri_controller::leftmost() {
  if(!children.empty()) {
    return controllers->at(children.at(0))->leftmost();
  }
  return id;
}

int Spiri_controller::rightmost() {
  if(children.size() > 1) {
    return controllers->at(children.at(1))->rightmost();
  } else if(!children.empty()) {
    return controllers->at(children.at(0))->rightmost();
  }
  return id;
}

int Spiri_controller::GetMinimumHeight() {
  int childMinimumHeight = 0;
  if(!children.empty()) {
    childMinimumHeight = controllers->at(children.at(0))->GetMinimumHeight();
    for(int child : children) {
      int childHeight = controllers->at(child)->GetMinimumHeight();
      if(childMinimumHeight > childHeight) {
        childMinimumHeight = childHeight;
      }
    }
  }
  return childMinimumHeight + 1;
}

void Spiri_controller::replaceChild(int toReplace, int replacement) {
  std::replace(children.begin(), children.end(), toReplace, replacement);
}

void Spiri_controller::removeChild(int toRemove) {
  children.erase(std::remove(children.begin(), children.end(), toRemove));
}

void Spiri_controller::replace(Spiri_controller *target) {

  Spiri_controller *parent = controllers->at(parentId);
  parent->removeChild(id);
  offset = target->offset;
  children.clear();
  for(int child : target->children) {
    children.push_back(child);
    controllers->at(child)->SetParent(id);
  }
  target->heir = NO_HEIR;
  Spiri_controller *targetParent = controllers->at(target->parentId);
  targetParent->replaceChild(target->id, id);
  parentId = target->parentId;
  parent->SetupParentHeir();
}

bool Spiri_controller::failureDetected() {
  bool childFailed = false;

  for(int child : children) {
    childFailed |= controllers->at(child)->failureDetected();
  }

  return childFailed || failed;
}

REGISTER_CONTROLLER(Spiri_controller, "Spiri_controller")