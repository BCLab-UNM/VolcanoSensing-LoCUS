#ifndef GRADIENT_ARGOS_MOVEMENTVECTOR_H
#define GRADIENT_ARGOS_MOVEMENTVECTOR_H

#include <vector>
#include "Movement.h"

class MovementVector : public virtual Movement {

public:

  void add(Movement* movement);

  bool step();

  void reset();

  void addAll(std::vector<Movement*> vector);

private:
  std::vector<Movement*> movements;
};


#endif //GRADIENT_ARGOS_MOVEMENTVECTOR_H
