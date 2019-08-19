#ifndef FAULT_TOLERANCE_DRONES_ARGOS_REPLACEWITHHEIR_H
#define FAULT_TOLERANCE_DRONES_ARGOS_REPLACEWITHHEIR_H


#include "SpiriController.h"

class ReplaceWithHeir : public virtual Movement, public virtual Finishable{

public:
  ReplaceWithHeir(Spiri_controller *controllerToReplace, CVector3 waypoint, std::vector<Spiri_controller*>* controllers):
                  controllerToReplace(controllerToReplace),
                  waypoint(waypoint),
                  controllers(controllers){}

  bool step();
  bool isFinished();

private:
  bool setup;
  bool finished;
  Movement* replacementMovement;
  Spiri_controller* controllerToReplace;
  argos::CVector3 waypoint;
  std::vector<Spiri_controller*>* controllers;
};


#endif //FAULT_TOLERANCE_DRONES_ARGOS_REPLACEWITHHEIR_H
