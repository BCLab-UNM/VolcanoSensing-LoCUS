#ifndef FAULT_TOLERANCE_DRONES_ARGOS_REPLACEROOTACTION_H
#define FAULT_TOLERANCE_DRONES_ARGOS_REPLACEROOTACTION_H

#include "Movement.h"
#include "SpiriController.h"
#include "FaultToleranceLoopFunctions.h"

class ReplaceRootAction : public virtual Movement {

public:
  ReplaceRootAction(Gradient_loop_functions* loopFunctions, Spiri_controller* heir): loopFunctions(loopFunctions), heir(heir){}
  bool step() override;

private:
  Gradient_loop_functions* loopFunctions;
  Spiri_controller* heir;
};


#endif //FAULT_TOLERANCE_DRONES_ARGOS_REPLACEROOTACTION_H
