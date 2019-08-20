#include "ReplaceWithHeir.h"
#include "ParallelMovement.h"

bool ReplaceWithHeir::step() {

  if(!setup) {
    Spiri_controller *heir = controllers->at(controllerToReplace->heir);
    heir->offset = controllerToReplace->offset;
    replacementMovement = heir->CreateOffsetMovement(waypoint);
    setup = true;
  }

  finished = replacementMovement->step();

  if(finished) {
    Spiri_controller *heir = controllers->at(controllerToReplace->heir);
    heir->replace(controllerToReplace);
    Spiri_controller *parent = controllers->at(controllerToReplace->parentId);
    while(parent->parentId != parent->NO_HEIR) {
      parent = controllers->at(parent->parentId);
    }
    parent->SetupHeir();
  }

  return finished;
}

bool ReplaceWithHeir::isFinished() {
  return finished;
}
