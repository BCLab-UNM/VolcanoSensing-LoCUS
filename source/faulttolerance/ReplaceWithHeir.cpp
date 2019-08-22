#include "ReplaceWithHeir.h"
#include "ParallelMovement.h"

bool ReplaceWithHeir::step() {

  if(!setup) {
    Spiri_controller *heir = controllerToReplace->heir;
    // TODO: heir->offset = controllerToReplace->offset;
    replacementMovement = heir->CreateOffsetMovement(waypoint);
    setup = true;
  }

  finished = replacementMovement->step();

  if(finished) {
    Spiri_controller *heir = controllerToReplace->heir;
    heir->replace(controllerToReplace);
  }

  return finished;
}

bool ReplaceWithHeir::isFinished() {
  return finished;
}
