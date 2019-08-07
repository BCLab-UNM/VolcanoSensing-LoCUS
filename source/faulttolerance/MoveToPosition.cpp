#include "MoveToPosition.h"

void MoveToPosition::init(argos::CVector3 target, bool relative) {
  this->target = target;
  this->relative = relative;
}

bool MoveToPosition::step() {
  if(relative) {
    positionActuator->SetRelativePosition(target);
  } else {
    positionActuator->SetAbsolutePosition(target);
    positionActuator->SetAbsoluteYaw(argos::CRadians::ZERO);
  }
  return true;
}