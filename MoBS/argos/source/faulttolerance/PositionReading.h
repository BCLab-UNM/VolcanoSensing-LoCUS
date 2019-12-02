#ifndef FAULT_TOLERANCE_DRONES_ARGOS_POSITIONREADING_H
#define FAULT_TOLERANCE_DRONES_ARGOS_POSITIONREADING_H


#include <argos3/core/utility/math/vector3.h>

class PositionReading {

public:
  PositionReading(argos::CVector3 location, double value): location(location), value(value){}

  argos::CVector3 getLocation() { return location;}
  double getValue() { return value;}

private:

  double value;
  argos::CVector3 location;

};


#endif //FAULT_TOLERANCE_DRONES_ARGOS_POSITIONREADING_H
