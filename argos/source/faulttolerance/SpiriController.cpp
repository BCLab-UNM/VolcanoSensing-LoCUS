#include "SpiriController.h"
#include "FaultToleranceLoopFunctions.h"
#include "GasGradientDescentMovement.h"

void Spiri_controller::Init(TConfigurationNode& node) {
  compassSensor = GetSensor<argos::CCI_PositioningSensor>("positioning");
  positionActuator = GetActuator<argos::CCI_QuadRotorPositionActuator>("quadrotor_position");
  movement = new MovementVector();
}

void Spiri_controller::ControlStep() {

  if(!stopped) {
    PositionReading reading = GetReading();

    if (reading.getValue() > 0) {
      movement->reset();
      MoveToPosition *stopMovement = new MoveToPosition(positionActuator, compassSensor);
      stopMovement->init(reading.getLocation());
      movement->add(stopMovement);

      CVector3 normalizedDirection = reading.getLocation().Normalize();

      Gradient_loop_functions &loopFunctions = static_cast<Gradient_loop_functions &>(CSimulator::GetInstance().GetLoopFunctions());
      movement->add(new GasGradientDescentMovement(this, &loopFunctions, reading.getValue(), normalizedDirection));

      stopped = true;
    }
  }

  movement->step();
}


void Spiri_controller::Reset() {
  movement->reset();
}

void Spiri_controller::AddWaypoint(CVector3 waypoint) {
  MoveToPosition* moveTo = new MoveToPosition(positionActuator, compassSensor);
  moveTo->init(waypoint);

  movement->add(moveTo);
}

void Spiri_controller::AddMovement(Movement *move) {
  movement->add(move);
}

void Spiri_controller::fail() {
  failed = true;
}

PositionReading Spiri_controller::GetReading() {
  CVector3 position = compassSensor->GetReading().Position;

  Gradient_loop_functions& loopFunctions = static_cast<Gradient_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions());
  double valueAtPosition = loopFunctions.getPlume().getValue(position.GetX() * 10 + 500, position.GetY() * 10);
  if(isnan(valueAtPosition)) {
    valueAtPosition = 0;
  }
  return PositionReading(position, valueAtPosition);
}

REGISTER_CONTROLLER(Spiri_controller, "Spiri_controller")