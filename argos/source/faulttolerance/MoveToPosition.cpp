#include "MoveToPosition.h"

void MoveToPosition::init(argos::CVector3 target) {
  this->target = target;
}

bool MoveToPosition::step() {
  if(!commanded) {
    positionActuator->SetAbsolutePosition(target);
    positionActuator->SetAbsoluteYaw(argos::CRadians::ZERO);
    commanded = true;
  }
  argos::CVector3 positionVector = compassSensor->GetReading().Position;

  finished = fabs(positionVector.GetX() - (target.GetX())) < 0.1 &&
          fabs(positionVector.GetY() - (target.GetY())) < 0.1;

  return finished;
}