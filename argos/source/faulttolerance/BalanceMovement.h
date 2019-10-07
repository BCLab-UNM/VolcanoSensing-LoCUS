#ifndef FAULT_TOLERANCE_DRONES_ARGOS_BALANCEMOVEMENT_H
#define FAULT_TOLERANCE_DRONES_ARGOS_BALANCEMOVEMENT_H

#include "Movement.h"
#include "Finishable.h"
#include "FaultToleranceLoopFunctions.h"

class BalanceMovement : public virtual Movement, public virtual Finishable {

public:
  BalanceMovement(Gradient_loop_functions* loopFunctions): loopFunctions(loopFunctions){}
  bool step();

  bool isFinished() {
    return finished;
  }

private:
  bool finished = false;
  Gradient_loop_functions* loopFunctions;
};
#endif //FAULT_TOLERANCE_DRONES_ARGOS_BALANCEMOVEMENT_H
