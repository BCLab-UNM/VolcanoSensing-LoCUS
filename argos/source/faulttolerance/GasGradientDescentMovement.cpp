#include "GasGradientDescentMovement.h"
#include "SpokeSpiralSearch.h"

bool GasGradientDescentMovement::step() {
  PositionReading reading = controller->GetReading();

  CVector3 gradientStep;
  if (previousValue - reading.getValue() < 0) {
    // Continue in the current direction
    gradientStep = reading.getLocation() + previousVector.Normalize();
  } else {
    // Change direction randomly
    previousVector = CVector3(rand() % 2000 - 1000, rand() % 2000 - 1000, 0);
    gradientStep = reading.getLocation() + previousVector.Normalize();
    if(previousValue == 0 && reading.getValue() == 0) {
      zeroCount++;
    }
  }
  if(zeroCount > 4) {
    controller->AddMovement(new SpokeSpiralSearch(controller, radius));
  } else {
    controller->AddWaypoint(gradientStep);
    previousValue = reading.getValue();
    controller->AddMovement(this);
  }

  return true;
}
