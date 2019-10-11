#include "SpokeSpiralSearch.h"

bool SpokeSpiralSearch::step() {

  controller->AddWaypoint(CVector3(0, 0, 10));
  CVector3 randomWaypoint = CVector3(rand() % 200 - 100, rand() % 200 - 100, 10);

  double length = randomWaypoint.Length();
  for(int i = 0; i < length; i++) {
    controller->AddWaypoint(CVector3(i * randomWaypoint.GetX() / length, i * randomWaypoint.GetY() / length, 10));
  }
  controller->AddWaypoint(randomWaypoint);
  controller->AddWaypoint(CVector3(0, 0, 10));
  controller->AddMovement(new SpokeSpiralSearch(controller));

  return true;
}