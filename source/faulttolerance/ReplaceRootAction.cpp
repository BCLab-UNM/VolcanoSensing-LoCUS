#include "ReplaceRootAction.h"

bool ReplaceRootAction::step() {
  loopFunctions->SetRootController(heir);
  return true;
}
