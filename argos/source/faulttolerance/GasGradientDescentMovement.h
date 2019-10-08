#ifndef UNCOORDINATED_CD_ARGOS_GASGRADIENTDESCENTMOVEMENT_H
#define UNCOORDINATED_CD_ARGOS_GASGRADIENTDESCENTMOVEMENT_H

#include "SpiriController.h"
#include "FaultToleranceLoopFunctions.h"

class GasGradientDescentMovement : public virtual Movement {

public:
  GasGradientDescentMovement(Spiri_controller* controller,
          Gradient_loop_functions* loopFunctions,
          double previousValue,
          CVector3 previousVector):
          controller(controller),
          loopFunctions(loopFunctions),
          previousValue(previousValue),
          previousVector(previousVector) {}

  bool step();
private:
  Spiri_controller* controller;
  Gradient_loop_functions* loopFunctions;
  double previousValue;
  CVector3 previousVector;
  int zeroCount = 0;
};


#endif //UNCOORDINATED_CD_ARGOS_GASGRADIENTDESCENTMOVEMENT_H
