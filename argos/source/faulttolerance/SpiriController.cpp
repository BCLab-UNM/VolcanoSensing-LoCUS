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
    CVector3 position = compassSensor->GetReading().Position;

    Gradient_loop_functions &loopFunctions = static_cast<Gradient_loop_functions &>(CSimulator::GetInstance().GetLoopFunctions());
    double valueAtPosition = loopFunctions.getPlume().getValue(position.GetX() * 10 + 500, position.GetY() * 10);
    if (isnan(valueAtPosition)) {
      valueAtPosition = 0;
    }

    if (valueAtPosition > 0) {
      cout << "Found value of " << valueAtPosition << endl;
      movement->reset();
      MoveToPosition *stopMovement = new MoveToPosition(positionActuator, compassSensor);
      stopMovement->init(position);
      movement->add(stopMovement);

      CVector3 normalizedDirection = position.Normalize();

      movement->add(new GasGradientDescentMovement(this, &loopFunctions, valueAtPosition, normalizedDirection));

      stopped = true;
    }
  }

  finished = movement->step();
}


void Spiri_controller::Reset() {
  finished = false;
  movement->reset();
}

bool Spiri_controller::IsFinished() {
  return finished;
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


bool Spiri_controller::failureDetected() {
  return failed;
}

std::vector<PositionReading> Spiri_controller::getReadings() {
  std::vector<PositionReading> readings;

  /*CVector3 position = compassSensor->GetReading().Position;

  Gradient_loop_functions& loopFunctions = static_cast<Gradient_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions());
  double valueAtPosition = loopFunctions.getPlume().getValue(position.GetX() * 10 + 500, position.GetY() * 10);
  if(isnan(valueAtPosition)) {
    valueAtPosition = 0;
  }
  PositionReading reading(location->getOffset(), valueAtPosition);

  readings.push_back(reading);

  for(ControllerBase* base : swarmManager->GetChildren(this)) {
    for(PositionReading childReading : dynamic_cast<Spiri_controller*>(base)->getReadings()) {
      readings.push_back(childReading);
    }
  }*/

  return readings;
}

REGISTER_CONTROLLER(Spiri_controller, "Spiri_controller")