
#include "MovementVector.h"

void MovementVector::add(Movement* movement) {
  movements.push_back(movement);
}

bool MovementVector::step() {
  if (!movements.empty()) {
    Movement *movement = movements.front();
    bool finished = movement->step();

    if (finished) {
      movements.erase(movements.begin());
    }
    return false;
  }
  return true;
}

void MovementVector::reset() {
  movements.clear();
}

void MovementVector::addAll(std::vector<Movement*> movementVector) {
  for(Movement* movement : movementVector) {
    add(movement);
  }
}
