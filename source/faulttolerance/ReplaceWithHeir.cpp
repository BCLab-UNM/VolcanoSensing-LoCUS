#include "ReplaceWithHeir.h"
#include "ParallelMovement.h"

bool ReplaceWithHeir::step() {

  if(!setup) {
    cout << "Replacing " << controllerToReplace->id << " with heir " << controllerToReplace->heir << endl;
    Spiri_controller *heir = controllers->at(controllerToReplace->heir);
    heir->offset = controllerToReplace->offset;
    replacementMovement = heir->CreateOffsetMovement(waypoint);
    setup = true;
  }

  finished = replacementMovement->step();

  if(finished) {
    Spiri_controller *heir = controllers->at(controllerToReplace->heir);
    heir->replace(controllerToReplace);
  }

  return finished;
}

bool ReplaceWithHeir::isFinished() {
  return finished;
}
