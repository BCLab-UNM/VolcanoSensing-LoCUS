#include "SpokeSpiralSearch.h"

bool SpokeSpiralSearch::step() {

  controller->AddWaypoint(CVector3(0, 0, 10));
  CVector3 randomWaypoint = CVector3(rand() % 200 - 100, rand() % 200 - 100, 10);
  controller->AddWaypoint(randomWaypoint);
  for(int i = 0; i < 100; i++) {
    CVector3 waypoint = randomWaypoint + buildArchimedesSpiralWaypoint(i, 1);
    controller->AddWaypoint(waypoint);
  }

  controller->AddWaypoint(randomWaypoint);
  controller->AddWaypoint(CVector3(0, 0, 10));
  controller->AddMovement(new SpokeSpiralSearch(controller));

  return true;
}

argos::CVector3 SpokeSpiralSearch::buildArchimedesSpiralWaypoint(int index, double radius) {
  double altitude = 0;

  // https://www.comsol.com/blogs/how-to-build-a-parameterized-archimedean-spiral-geometry/
  double a = 0;
  double b = radius / (2 * M_PI);
  double index_angle_offset = 0;//(1.0 / size) * (2 * M_PI);

  double angle = index * 2 * M_PI / 10;
  double point_radius = a + (b * angle);
  double xoffset = point_radius * cos(angle + index_angle_offset); // in meters
  double yoffset = point_radius * sin(angle + index_angle_offset);

  return argos::CVector3(xoffset, yoffset, altitude);
}
