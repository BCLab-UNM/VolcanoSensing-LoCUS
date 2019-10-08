#ifndef UNCOORDINATED_CD_ARGOS_SPOKESPIRALSEARCH_H
#define UNCOORDINATED_CD_ARGOS_SPOKESPIRALSEARCH_H

#include "Movement.h"
#include "SpiriController.h"

class SpokeSpiralSearch : public virtual Movement {

public:
  SpokeSpiralSearch(Spiri_controller* controller): controller(controller) {}

  bool step();

private:
  Spiri_controller* controller;

  argos::CVector3 buildArchimedesSpiralWaypoint(int index, double radius);
};


#endif //UNCOORDINATED_CD_ARGOS_SPOKESPIRALSEARCH_H
