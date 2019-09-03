#include "ParallelMovement.h"

bool ParallelMovement::step() {
  bool movementFinished = true;
  for(int i = 0; i < moves.size(); i++) {
    if(!finished.at(i)) {
      finished.at(i) = moves.at(i)->step();
      movementFinished &= finished.at(i);
    }
  }
  return movementFinished;
}
