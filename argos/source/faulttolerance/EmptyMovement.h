#ifndef FAULT_TOLERANCE_DRONES_ARGOS_EMPTYMOVEMENT_H
#define FAULT_TOLERANCE_DRONES_ARGOS_EMPTYMOVEMENT_H


#include "Movement.h"

class EmptyMovement : public virtual Movement, public virtual Finishable {

public:
  bool step() {
    return true;
  }

  bool isFinished() {
    return true;
  }
};


#endif //FAULT_TOLERANCE_DRONES_ARGOS_EMPTYMOVEMENT_H
