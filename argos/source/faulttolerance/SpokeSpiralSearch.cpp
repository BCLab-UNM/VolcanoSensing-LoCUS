#include "SpokeSpiralSearch.h"

bool SpokeSpiralSearch::step() {

  controller->AddWaypoint(CVector3(0, 0, 10));

  double angle = controller->GetCurrentAngle() + ((2 * M_PI) / 1.6180339887);
  controller->SetCurrentAngle(angle);

  double randx = cos(angle);
  double randy = sin(angle);

  for(int i = 0; i < radius; i++) {
    controller->AddWaypoint(CVector3(i * randx, i * randy, 10));
  }

  for(int i = radius; i > 0; i--) {
    controller->AddWaypoint(CVector3(i * randx, i * randy, 10));
  }
  controller->AddWaypoint(CVector3(0, 0, 10));
  controller->AddMovement(new SpokeSpiralSearch(controller, radius));

  return true;
}