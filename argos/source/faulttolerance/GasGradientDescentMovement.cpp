#include "GasGradientDescentMovement.h"
#include "SpokeSpiralSearch.h"

bool GasGradientDescentMovement::step() {
  CVector3 position = controller->GetCompassSensor()->GetReading().Position;
  double valueAtPosition = loopFunctions->getPlume().getValue(position.GetX() * 10 + 500, position.GetY() * 10);
  if (isnan(valueAtPosition)) {
    valueAtPosition = 0;
  }

  CVector3 gradientStep;
  if (previousValue - valueAtPosition < 0) {
    // Continue in the current direction
    gradientStep = position + previousVector.Normalize();
  } else {
    // Change direction randomly
    previousVector = CVector3(rand() % 2000 - 1000, rand() % 2000 - 1000, 0);
    gradientStep = position + previousVector.Normalize();
    if(previousValue == 0 && valueAtPosition == 0) {
      zeroCount++;
    }
  }
  if(zeroCount > 4) {
    cout << "Too many zeros" << endl;
    controller->AddMovement(new SpokeSpiralSearch(controller));
  } else {
    controller->AddWaypoint(gradientStep);
    previousValue = valueAtPosition;
    controller->AddMovement(this);
  }

  return true;
}
