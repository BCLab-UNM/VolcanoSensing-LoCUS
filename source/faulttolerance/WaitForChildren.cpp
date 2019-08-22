#include "WaitForChildren.h"


bool WaitForChildren::step() {
  bool finished = true;

  for(Spiri_controller* childController : root->getChildrenControllers()) {
    finished &= childController->IsFinished();
  }

  return finished;
}