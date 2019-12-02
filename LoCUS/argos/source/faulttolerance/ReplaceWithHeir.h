#ifndef FAULT_TOLERANCE_DRONES_ARGOS_REPLACEWITHHEIR_H
#define FAULT_TOLERANCE_DRONES_ARGOS_REPLACEWITHHEIR_H


#include "SpiriController.h"
#include "FaultToleranceLoopFunctions.h"

class ReplaceWithHeir : public virtual Movement, public virtual Finishable{

public:
  ReplaceWithHeir(Gradient_loop_functions* loopFunctions, Spiri_controller *controllerToReplace, CVector3 waypoint, std::vector<Spiri_controller*>* controllers, SwarmManager* swarmManager):
          loopFunctions(loopFunctions),
          controllerToReplace(controllerToReplace),
          waypoint(waypoint),
          controllers(controllers),
          swarmManager(swarmManager){}

  bool step();
  bool isFinished();

private:
  bool setup;
  bool finished;
  Movement* replacementMovement;
  Gradient_loop_functions* loopFunctions;
  Spiri_controller* controllerToReplace;
  argos::CVector3 waypoint;
  std::vector<Spiri_controller*>* controllers;
  SwarmManager* swarmManager;
};


#endif //FAULT_TOLERANCE_DRONES_ARGOS_REPLACEWITHHEIR_H
