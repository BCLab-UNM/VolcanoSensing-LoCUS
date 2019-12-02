#ifndef FAULT_TOLERANCE_DRONES_ARGOS_PARALLELMOVEMENT_H
#define FAULT_TOLERANCE_DRONES_ARGOS_PARALLELMOVEMENT_H


#include "Movement.h"
#include <vector>
#include <iostream>

class ParallelMovement : public virtual Movement {

public:
  ParallelMovement(std::vector<Movement*> moves): moves(moves) {
    finished = std::vector<bool>(moves.size());

    for(int i = 0; i < moves.size(); i++) {
      finished.push_back(false);
    }
  }

  bool step();
private:
  std::vector<Movement*> moves;
  std::vector<bool> finished;
};


#endif //FAULT_TOLERANCE_DRONES_ARGOS_PARALLELMOVEMENT_H
