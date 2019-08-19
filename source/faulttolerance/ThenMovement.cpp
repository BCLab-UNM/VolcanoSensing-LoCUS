//
// Created by john on 8/18/19.
//

#include "ThenMovement.h"

bool ThenMovement::step() {
  bool one = previous->isFinished();
  bool two = false;
  if(one) {
    two = next->step();
  }
  finished = one && two;
  return finished;
}
