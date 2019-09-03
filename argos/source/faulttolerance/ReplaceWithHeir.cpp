#include "ReplaceWithHeir.h"
#include "ParallelMovement.h"
#include "EmptyMovement.h"

bool ReplaceWithHeir::step() {
  if (!setup) {
    if (controllerToReplace->heir == NULL) {
      // Remove from swarm
      Spiri_controller *parent = controllerToReplace->getParentController();
      LOG << "Removing " << controllerToReplace->id << " with no heir." << endl;
      swarmManager->RemoveChild(controllerToReplace);
      controllerToReplace->location = NULL;
      parent->SetupParentHeir();
      parent->Balance();
      replacementMovement = new EmptyMovement();
    } else {
      LOG << "Replacing " << controllerToReplace->id << " with heir " << controllerToReplace->heir->id << endl;
      Spiri_controller *heir = controllerToReplace->heir;
      replacementMovement = heir->CreateOffsetMovement(waypoint, controllerToReplace->location->getOffset());
      heir->replace(controllerToReplace);
      loopFunctions->SetRootController(controllerToReplace, heir);
    }
    setup = true;
  }
  finished = replacementMovement->step();

  return finished;
}

bool ReplaceWithHeir::isFinished() {
  return finished;
}