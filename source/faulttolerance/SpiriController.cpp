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
  addCoverage(CVector3(0, 0, 0));
  movement->add(starting_position);
  WaitForChildren* wait = new WaitForChildren(this, controllers);
  movement->add(wait);
}

void Spiri_controller::AddRecursiveWaypoint(CVector3 waypoint) {
  addCoverage(waypoint);

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

void Spiri_controller::addCoverage(CVector3 waypoint) {

  CVector3 offset = location->getOffset();
  Gradient_loop_functions& loopFunctions = static_cast<Gradient_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions());
  for(int i = 0; i < loopFunctions.rmax; i++) {
    for(int j = -1; j < loopFunctions.rmax; j++) {
      loopFunctions.coverage[(int)(waypoint.GetX() + offset.GetX() + i + 500)][(int)(waypoint.GetY() + offset.GetY() + j + 500)] = true;
    }
  }
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
    getParentController()->SetupParentHeir();
  }
}

void Spiri_controller::Balance() {

  if(location->getMaxChildrenSize() > 1) {
    int maxDepth;
    Spiri_controller* maxDepthChild;
    int minDepth;
    Spiri_controller* minDepthChild;

    do {
      maxDepth = 0;
      maxDepthChild = NULL;
      minDepth = 100000;
      minDepthChild = NULL;

      for (Spiri_controller* childController : getChildrenControllers()) {
        if(childController->CanInsert()) {
          int maximumControllerDepth = childController->GetMaximumDepth();
          if (maximumControllerDepth > maxDepth) {
            maxDepth = maximumControllerDepth;
            maxDepthChild = childController;
          }
          if(childController->CanInsert()) {
            int minimumControllerDepth = childController->GetMinimumDepth();
            if (minimumControllerDepth < minDepth) {
              minDepth = minimumControllerDepth;
              minDepthChild = childController;
            }
          }
        }
      }

      if(maxDepth - 1 > minDepth && maxDepthChild != NULL && minDepthChild != NULL) {
        Spiri_controller *childToMove = maxDepthChild->RemoveLeaf();
        //Spiri_controller *parent = swarmManager->GetParent();
        minDepthChild->Insert(childToMove);
        LOG << "Balancing at " << id << " moving " << childToMove->id << " into subtree of " << minDepthChild->id << endl;
      }
    }while(maxDepth - 1 > minDepth);
  }

  if(!location->IsRoot()) {
    getParentController()->Balance();
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
    if(controller->CanInsert()) {
      int maximumControllerDepth = controller->GetMaximumDepth();
      if (maximumControllerDepth > maxDepth) {
        maxDepth = maximumControllerDepth;
        maxDepthChild = controller;
      }
    }
  }

  if(maxDepthChild->getChildrenControllers().empty()) {
    swarmManager->RemoveChild(maxDepthChild);
    return maxDepthChild;
  } else {
    return maxDepthChild->RemoveLeaf();
  }
}

bool Spiri_controller::CanInsert() {
  vector<Spiri_controller*> childControllers = getChildrenControllers();
  if (location->getMaxChildrenSize() == 0) {
    return false;
  } else if(location->getMaxChildrenSize() == childControllers.size()) {
    for(Spiri_controller* childController : childControllers) {
      if(childController->CanInsert()) {
        return true;
      }
    }
    return false;
  } else {
    return true;
  }
}

void Spiri_controller::Insert(Spiri_controller *child) {
  vector<Spiri_controller*> childControllers = getChildrenControllers();
  if (childControllers.size() == location->getMaxChildrenSize()){
    int minDepth = 100000;
    Spiri_controller* minDepthChild = NULL;

    for (Spiri_controller* controller : childControllers) {
      if(controller->CanInsert()) {
        int controllerDepth = controller->GetMinimumDepth();
        if (controllerDepth < minDepth) {
          minDepth = controllerDepth;
          minDepthChild = controller;
        }
      }
    }

    minDepthChild->Insert(child);

  } else if(childControllers.size() < location->getMaxChildrenSize()) {
    child->location = swarmManager->AddChild(this, child);
  }
}

int Spiri_controller::GetMinimumDepth() {
  int childMinimumHeight = 0;
  if(swarmManager->GetChildren(this).size() == location->getMaxChildrenSize()) {
    bool foundMinimumHeight = false;
    childMinimumHeight = 10000;
    for(ControllerBase* childController: swarmManager->GetChildren(this)) {
      if (childController->CanInsert()) {
        foundMinimumHeight = true;
        int childHeight = childController->GetMinimumDepth();
        if (childMinimumHeight > childHeight) {
          childMinimumHeight = childHeight;
        }
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
  parent->SetupParentHeir();
  parent->Balance();
  Spiri_controller *heirParent = dynamic_cast<Spiri_controller*>(swarmManager->GetValue(location));
  heirParent->SetupParentHeir();
  heirParent->Balance();
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

REGISTER_CONTROLLER(Spiri_controller, "Spiri_controller")