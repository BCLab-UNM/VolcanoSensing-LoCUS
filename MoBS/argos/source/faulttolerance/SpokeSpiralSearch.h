#ifndef UNCOORDINATED_CD_ARGOS_SPOKESPIRALSEARCH_H
#define UNCOORDINATED_CD_ARGOS_SPOKESPIRALSEARCH_H

#include "Movement.h"
#include "SpiriController.h"

class SpokeSpiralSearch : public virtual Movement {

public:
  SpokeSpiralSearch(Spiri_controller *controller, int radius)
          : controller(controller), radius(radius) {}

  bool step();

private:
  Spiri_controller* controller;
  int radius;
};


#endif //UNCOORDINATED_CD_ARGOS_SPOKESPIRALSEARCH_H
