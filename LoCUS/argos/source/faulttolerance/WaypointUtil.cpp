#include "WaypointUtil.h"

std::vector<Movement*> WaypointUtil::buildDDSAWaypoints(
        argos::CCI_PositioningSensor* compassSensor,
        argos::CCI_DifferentialSteeringActuator* motorActuator,
        int size, int index, int loops, double radius) {

  std::vector<Movement*> waypoints;

  for(int loop = 0; loop < loops; loop++) {
    for (int corner = 0; corner < 4; corner++) {
      double xoffset = 0;
      double yoffset = 0;

      if (loop == 0 && corner == 0) {
        //Starting locations
        xoffset = 0;
        yoffset = index + 1;
      } else {
        xoffset = 1 + index + (loop * size);
        yoffset = xoffset;
        if (corner == 0) {
          xoffset = -(1 + index + ((loop - 1) * size));
        }
        else if (corner == 3) {
          xoffset = -xoffset;
        }
        if (corner == 2 || corner == 3) {
          yoffset = -yoffset;
        }
      }
      MoveTo* waypoint = new MoveTo(compassSensor, motorActuator);
      waypoint->init(argos::CVector2(xoffset * radius, yoffset * radius));

      waypoints.push_back(waypoint);
    }
  }

  return waypoints;
}

std::vector<Movement*> WaypointUtil::buildLawnmowerWaypoints(
        argos::CCI_PositioningSensor* compassSensor,
        argos::CCI_DifferentialSteeringActuator* motorActuator,
        int size, int index, int cycle_count, double radius, int width, int height) {

  std::vector<Movement*> waypoints;

  for(int cycles = 0; cycles < cycle_count; cycles++) {
    for (int column = 0; column < 2; column++) {
      for(int row = 0; row < 2; row++) {
        MoveTo* waypoint = new MoveTo(compassSensor, motorActuator);



        waypoint->init(argos::CVector2(
                ((1.0 * height / size) * (index + 1) - (((2 * cycles) + row) * (radius / 2))) - height/2.1,
                (-1 + (2 * column)) * width/2.1));

        waypoints.push_back(waypoint);
      }
    }
  }

  return waypoints;
}
