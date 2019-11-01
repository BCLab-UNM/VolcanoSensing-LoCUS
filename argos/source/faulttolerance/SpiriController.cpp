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

void Spiri_controller::Setup(int id, SwarmLocation *location, std::vector<Spiri_controller*>* controllers, SwarmManager* swarmManager) {
  this->id = id;
  this->location = location;
  this->controllers = controllers;
  this->swarmManager = swarmManager;
  setupPosition();
}

void Spiri_controller::setupPosition() {
  movement->reset();
  MoveToPosition* starting_position = new MoveToPosition(positionActuator, compassSensor);
  starting_position->init(location->getOffset());
  movement->add(starting_position);
  WaitForChildren* wait = new WaitForChildren(this, controllers);
  movement->add(wait);
}

void Spiri_controller::AddRecursiveWaypoint(CVector3 waypoint) {
  movement->add(CreateOffsetMovement(waypoint, location->getOffset()));

  WaitForChildren* wait = new WaitForChildren(this, controllers);
  movement->add(wait);

  for(Spiri_controller* childController : getChildrenControllers()) {
    childController->AddRecursiveWaypoint(waypoint);
  }
}

MoveToPosition* Spiri_controller::CreateOffsetMovement(CVector3 waypoint, CVector3 offsetPosition) {
  MoveToPosition* moveTo = new MoveToPosition(positionActuator, compassSensor);
  moveTo->init(argos::CVector3(waypoint.GetX() + offsetPosition.GetX(),
                               waypoint.GetY() + offsetPosition.GetY(),
                               waypoint.GetZ() + offsetPosition.GetZ()));

  return moveTo;
}

void Spiri_controller::AddMovement(Movement *move) {
  movement->add(move);
}

void Spiri_controller::fail() {
  failed = true;
}

void Spiri_controller::SetupHeir() {
  heir = GetHeir();

  for (Spiri_controller *child : getChildrenControllers()) {
    child->SetupHeir();
  }
}

void Spiri_controller::SetupParentHeir() {
  heir = GetHeir();

  if(!location->IsRoot()) {
    Spiri_controller *parentController = getParentController();

    if(parentController != NULL) {
      parentController->SetupParentHeir();
    }
  }
}

void Spiri_controller::Balance() {

  if(location != NULL && location->getMaxChildrenSize() > 1) {
    int maxDepth;
    Spiri_controller* maxDepthChild;
    int minDepth;
    Spiri_controller* minDepthChild;

    do {
      maxDepth = 0;
      maxDepthChild = NULL;
      minDepth = 100000;
      minDepthChild = this;

      for (Spiri_controller* childController : getChildrenControllers()) {
          int maximumControllerDepth = childController->GetMaximumDepth();
        if (maximumControllerDepth > maxDepth) {
          maxDepth = maximumControllerDepth;
          maxDepthChild = childController;
        }
      }

      if(swarmManager->AreChildrenFull(this)) {
        for (Spiri_controller *childController : getChildrenControllers()) {
          int maximumControllerDepth = childController->GetMaximumDepth();
          if (maximumControllerDepth > maxDepth) {
            maxDepth = maximumControllerDepth;
            maxDepthChild = childController;
          }
          int minimumControllerDepth = childController->GetMinimumDepth();
          if (minimumControllerDepth < minDepth) {
            minDepth = minimumControllerDepth;
            minDepthChild = childController;
          }
        }
      } else {
        minDepth = 0;
      }

      if(maxDepth - 1 > minDepth && maxDepthChild != NULL && minDepthChild != NULL) {
        Spiri_controller *childToMove = maxDepthChild->RemoveLeaf();
        minDepthChild->Insert(childToMove);
        LOG << "Balancing at " << id << " moving " << childToMove->id << " into subtree of " << minDepthChild->id << endl;
      }
      LOG << "Max depth: " << maxDepth << " min depth: " << minDepth << endl;
     }while(maxDepth - 1 > minDepth);
  }

  if(location != NULL && !swarmManager->IsRoot(this)) {
    Spiri_controller *parentController = getParentController();

    if(parentController != NULL) {
      parentController->Balance();
    }
  }
}

Spiri_controller* Spiri_controller::RemoveLeaf() {
  vector<Spiri_controller*> childControllers = getChildrenControllers();
  if(childControllers.empty()) {
    return NULL;
  }

  int maxDepth = 0;
  Spiri_controller* maxDepthChild = NULL;

  for (Spiri_controller* controller : childControllers) {
    int maximumControllerDepth = controller->GetMaximumDepth();
    if (maximumControllerDepth > maxDepth) {
      maxDepth = maximumControllerDepth;
      maxDepthChild = controller;
    }
  }

  if(maxDepthChild->getChildrenControllers().empty()) {
    swarmManager->RemoveChild(maxDepthChild);
    return maxDepthChild;
  } else {
    return maxDepthChild->RemoveLeaf();
  }
}

void Spiri_controller::Insert(Spiri_controller *child) {
  vector<Spiri_controller*> childControllers = getChildrenControllers();
  if (childControllers.size() == location->getMaxChildrenSize()){
    int minDepth = 100000;
    Spiri_controller* minDepthChild = NULL;

    for (Spiri_controller* controller : childControllers) {
      int controllerDepth = controller->GetMinimumDepth();
      if (controllerDepth < minDepth) {
        minDepth = controllerDepth;
        minDepthChild = controller;
      }
    }

    minDepthChild->Insert(child);

  } else if(childControllers.size() < location->getMaxChildrenSize()) {
    child->location = swarmManager->AddChild(this, child);
  }
}

int Spiri_controller::GetMinimumDepth() {
  int childMinimumHeight = 0;
  if(location != NULL && swarmManager->GetChildren(this).size() == location->getMaxChildrenSize()) {
    bool foundMinimumHeight = false;
    childMinimumHeight = 10000;
    for(ControllerBase* childController: swarmManager->GetChildren(this)) {
      foundMinimumHeight = true;
      int childHeight = childController->GetMinimumDepth();
      if (childMinimumHeight > childHeight) {
        childMinimumHeight = childHeight;
      }
    }
    if(!foundMinimumHeight) {
      childMinimumHeight = 0;
    }
  }
  return childMinimumHeight + 1;
}

int Spiri_controller::GetMaximumDepth() {
  int depth = 1;
  for(Spiri_controller* childController : getChildrenControllers()) {
    int childDepth = childController->GetMaximumDepth() + 1;
    if(childDepth > depth) {
      depth = childDepth;
    }
  }
  return depth;
}

Spiri_controller* Spiri_controller::GetHeir() {
  if(getChildrenControllers().empty()) {
    return NULL;
  }
  Spiri_controller* successor = GetSuccessor();
  if (successor != NULL) {
    return successor;
  }
  return GetPredecessor();
}

Spiri_controller* Spiri_controller::GetSuccessor() {
  vector<Spiri_controller*> childControllers = getChildrenControllers();
  if(childControllers.size() > 1) {
    return childControllers.at(1)->leftmost();
  }
  return NULL;
}

Spiri_controller* Spiri_controller::GetPredecessor() {
  vector<Spiri_controller*> childControllers = getChildrenControllers();
  if(!childControllers.empty()) {
    return childControllers.at(0)->rightmost();
  }
  return NULL;
}

Spiri_controller* Spiri_controller::leftmost() {
  vector<Spiri_controller*> childControllers = getChildrenControllers();
  if(!childControllers.empty()) {
    return childControllers.at(0)->leftmost();
  }
  return this;
}

Spiri_controller* Spiri_controller::rightmost() {
  vector<Spiri_controller*> childControllers = getChildrenControllers();
  if(childControllers.size() > 1) {
    return childControllers.at(1)->rightmost();
  } else if(!childControllers.empty()) {
    return childControllers.at(0)->rightmost();
  }
  return this;
}

void Spiri_controller::replace(Spiri_controller *target) {
  SwarmLocation* parentLocation = location->GetParent();
  swarmManager->RemoveChild(target);
  location = target->location;
  swarmManager->UpdateLocation(this, location);
  target->location = NULL;
  target->heir = NULL;
  Spiri_controller *parent = dynamic_cast<Spiri_controller*>(swarmManager->GetValue(parentLocation));
  if(parent != NULL) {
    parent->SetupParentHeir();
  }
  Spiri_controller *heirParent = dynamic_cast<Spiri_controller*>(swarmManager->GetValue(location));
  if(heirParent != NULL) {
    heirParent->SetupParentHeir();
  }
}

bool Spiri_controller::failureDetected() {
  bool childFailed = false;

  for(Spiri_controller* childController : getChildrenControllers()) {
    childFailed |= childController->failureDetected();
  }

  if(failed) {
    LOG << "Failure detected at " << id << endl;
  }

  return childFailed || failed;
}

Spiri_controller *Spiri_controller::getParentController() {
  return dynamic_cast<Spiri_controller*>(swarmManager->GetParent(this));
}

vector<Spiri_controller *> Spiri_controller::getChildrenControllers() {
  vector<Spiri_controller *> castControllers;

  for(ControllerBase* base : swarmManager->GetChildren(this)) {
    castControllers.push_back(dynamic_cast<Spiri_controller*>(base));
  }

  return castControllers;
}

std::vector<PositionReading> Spiri_controller::getReadings(int depth) {
  std::vector<PositionReading> readings;

  readings.push_back(GetReading());

  if(depth > 1) {
    for (ControllerBase *base : swarmManager->GetChildren(this)) {
      for (PositionReading childReading : dynamic_cast<Spiri_controller *>(base)->getReadings(depth - 1)) {
        readings.push_back(childReading);
      }
    }
  }

  return readings;
}

PositionReading Spiri_controller::GetReading() {
  CVector3 position = compassSensor->GetReading().Position;

  Gradient_loop_functions& loopFunctions = static_cast<Gradient_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions());
  double valueAtPosition = loopFunctions.getPlume().getValue(position.GetX() * 10, position.GetY() * 10);
  if(isnan(valueAtPosition)) {
    valueAtPosition = 0;
  }
  return PositionReading(position, valueAtPosition);
}

void Spiri_controller::AddWaitForChildren(vector<Spiri_controller*> *controllers) {
  movement->add(new WaitForChildren(this, controllers));

  for(Spiri_controller* childController : getChildrenControllers()) {
    childController->AddWaitForChildren(controllers);
  }
}

int Spiri_controller::GetSwarmSize() {

  int swarmSize = 0;

  for(ControllerBase* base : swarmManager->GetChildren(this)) {
    swarmSize += base->GetSwarmSize();
  }

  return failed ? swarmSize : 1 + swarmSize;
}

int Spiri_controller::GetSwarmSize(int depth) {

  int swarmSize = 0;

  if(depth > 1) {
    for (ControllerBase *base : swarmManager->GetChildren(this)) {
      swarmSize += base->GetSwarmSize(depth - 1);
    }
  }

  return failed ? swarmSize : 1 + swarmSize;
}

REGISTER_CONTROLLER(Spiri_controller, "Spiri_controller")