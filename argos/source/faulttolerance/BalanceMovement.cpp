#include "BalanceMovement.h"

bool BalanceMovement::step() {
  cout << "Balancing" << endl;
  Spiri_controller* rootController = loopFunctions->getRootController();
  rootController->Balance();
  finished = true;
  return true;
}
