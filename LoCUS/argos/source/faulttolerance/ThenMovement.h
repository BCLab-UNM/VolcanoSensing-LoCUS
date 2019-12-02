#ifndef FAULT_TOLERANCE_DRONES_ARGOS_THENMOVEMENT_H
#define FAULT_TOLERANCE_DRONES_ARGOS_THENMOVEMENT_H


#include <iostream>
#include "Movement.h"
#include "Finishable.h"

class ThenMovement : public virtual Movement, public virtual Finishable {

public:
  ThenMovement(Finishable* previous, Movement *next): previous(previous), next(next){}
  bool step() override;
  bool isFinished() {
    return previous->isFinished() && finished;
  }

private:
  bool finished = false;
  Finishable* previous;
  Movement* next;
};


#endif //FAULT_TOLERANCE_DRONES_ARGOS_THENMOVEMENT_H
