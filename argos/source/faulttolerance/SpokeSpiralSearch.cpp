#include "SpokeSpiralSearch.h"

bool SpokeSpiralSearch::step() {

  controller->AddWaypoint(CVector3(0, 0, 10));

  double theta = (1.0 * rand() / RAND_MAX) * 2 * M_PI;

  double randx = cos(theta);
  double randy = sin(theta);

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