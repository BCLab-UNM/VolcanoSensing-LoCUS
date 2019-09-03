#include "MoveTo.h"


void MoveTo::SetWheelSpeedsFromVector(const argos::CVector2& c_heading) {

  const float speed = 10;

  const argos::CRadians SoftTurnOnAngleThreshold = ToRadians(argos::CDegrees(70));
  const argos::CRadians HardTurnOnAngleThreshold = ToRadians(argos::CDegrees(90));
  const argos::CRadians NoTurnAngleThreshold = ToRadians(argos::CDegrees(10));

  argos::Real fSpeed1, fSpeed2;
  argos::CRadians cHeadingAngle = c_heading.Angle().SignedNormalize();
  if(Abs(cHeadingAngle) < NoTurnAngleThreshold) {
    // Move straight ahead
    fSpeed1 = speed;
    fSpeed2 = speed;
  } else if(Abs(cHeadingAngle) < SoftTurnOnAngleThreshold) {
    // Soft Turn
    argos::Real fSpeedFactor = (HardTurnOnAngleThreshold - Abs(cHeadingAngle)) / (HardTurnOnAngleThreshold);
    fSpeed1 = speed - speed * (1.0 - fSpeedFactor);
    fSpeed2 = speed + speed * (1.0 - fSpeedFactor);
  } else  {
    // Hard Turn
    fSpeed1 = -speed;
    fSpeed2 =  speed;
  }

  if(cHeadingAngle < argos::CRadians::ZERO) {
    argos::Real temp = fSpeed2;
    fSpeed2  = fSpeed1;
    fSpeed1 = temp;
  }

  motorActuator->SetLinearVelocity(fSpeed1, fSpeed2);
}

bool MoveTo::step() {

  argos::CVector3 position3D = compassSensor->GetReading().Position;
  argos::CVector2 diff(target.GetX() - position3D.GetX(), target.GetY() - position3D.GetY());

  if(diff.Length() < 0.1) {
    motorActuator->SetLinearVelocity(0, 0);
    return true;
  } else {
    argos::CQuaternion orientation = compassSensor->GetReading().Orientation;
    argos::CRadians z_angle, y_angle, x_angle;
    orientation.ToEulerAngles(z_angle, y_angle, x_angle);

    diff.Rotate(-z_angle);

    SetWheelSpeedsFromVector(diff);
    return false;
  }
}

void MoveTo::init(argos::CVector2 target) {
  this->target = target;
}
