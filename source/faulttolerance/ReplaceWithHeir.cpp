#include "ReplaceWithHeir.h"
#include "ParallelMovement.h"

bool ReplaceWithHeir::step() {

  if(!setup) {
    Spiri_controller *heir = controllerToReplace->heir;
    replacementMovement = heir->CreateOffsetMovement(waypoint, controllerToReplace->location->getOffset());
    heir->replace(controllerToReplace, waypoint);
    setup = true;
  }

  finished = replacementMovement->step();

  return finished;
}

bool ReplaceWithHeir::isFinished() {
  return finished;
}
