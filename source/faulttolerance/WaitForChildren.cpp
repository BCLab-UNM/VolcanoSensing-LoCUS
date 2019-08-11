#include "WaitForChildren.h"


bool WaitForChildren::step() {
  bool finished = true;

  for(int childId : root->GetChildren()) {
    finished &= controllers->at(childId)->IsFinished();
  }

  return finished;
}