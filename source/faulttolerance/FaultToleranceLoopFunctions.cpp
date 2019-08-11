#include "FaultToleranceLoopFunctions.h"
#include "SpiriController.h"

void Gradient_loop_functions::Init(TConfigurationNode& node) {

  for(int i = 0; i < 1000; i++) {
    for(int j = 0; j < 1000; j++) {
      coverage[i][j] = false;
    }
  }

  CSpace::TMapPerType& spiris = GetSpace().GetEntitiesByType("spiri");

  int i = 0;
  for (auto &spiri : spiris) {
    CSpiriEntity& spiriEntity = *any_cast<CSpiriEntity*>(spiri.second);
    auto & controller = (Spiri_controller&)(spiriEntity.GetControllableEntity().GetController());

    if(i == 0) {
      rootController = &controller;
    }

    controllers.push_back(&controller);

    double rmax = 2;
    double rmin = 1;

    int level = 0;
    int innerswarmcount = 0;
    int levelchildrencount = 0;
    double radius = 0;
    while (innerswarmcount < i) {
      level = level + 1;
      radius = ((level + 1) * rmax);
      double levelangle = 2 * asin(rmin / radius);
      levelchildrencount = int((M_PI * 2) / levelangle);
      innerswarmcount = innerswarmcount + levelchildrencount;
    }

    double shell_angle = 0;
    if(levelchildrencount > 0) {
      shell_angle = (M_PI * 2) / levelchildrencount;
      radius = radius - rmax;
    }

    double angle = (i - (innerswarmcount - levelchildrencount)) * shell_angle;
    double x = radius * cos(angle);
    double y = radius * sin(angle);

    argos::CVector3 offset = argos::CVector3(x, y, 10);

    controller.Setup(i, level, offset, &controllers);

    i++;
  }

  for (auto &controller : controllers) {
    if(controller->GetLevel() > 0) {
      int parentId = calculateParent(controller->GetLevel(), controller->GetOffset());
      controller->SetParent(parentId);
      controllers.at(parentId)->AddChild(controller->GetIdentifier());
    }
  }
}

int Gradient_loop_functions::calculateParent(int level, argos::CVector3 childOffset) {
  int closest = 0;
  double closestDistance = 1000000;
  for(long i = 0; i < controllers.size(); i++) {
    auto &controller = controllers.at(i);

    argos::CVector3 parentOffset = controller->GetOffset();

    double relativeDistance = (parentOffset - childOffset).Length();
    if(controller->GetLevel() == level - 1 && relativeDistance < closestDistance) {
      closestDistance = relativeDistance;
      closest = i;
    }
  }
  return closest;
}

void Gradient_loop_functions::PostStep() {
  if(rootController->IsFinished()) {
    argos::CVector3 waypoint = buildArchimedesSpiralWaypoint(spiralIndex++, 15, 20, 1);
    rootController->AddRecursiveWaypoint(waypoint);
    waypoints.push_back(waypoint);
  }
}

argos::CVector3 Gradient_loop_functions::buildArchimedesSpiralWaypoint(int index, int size, int loops, double radius) {
  double altitude = 0;

  // https://www.comsol.com/blogs/how-to-build-a-parameterized-archimedean-spiral-geometry/
  double a = radius;
  double b = (radius * size * size) / (2 * M_PI * loops);
  double index_angle_offset = 0;//(1.0 / size) * (2 * M_PI);
  int points_per_rotation = 100;

  double angle = index * 2 * M_PI / points_per_rotation;
  double point_radius = a + (b * angle);
  double xoffset = point_radius * cos(angle + index_angle_offset); // in meters
  double yoffset = point_radius * sin(angle + index_angle_offset);

  return argos::CVector3(xoffset, yoffset, altitude);
}

void Gradient_loop_functions::Reset() {
  spiralIndex = 0;
}

bool Gradient_loop_functions::IsExperimentFinished() {
  return rootController->IsFinished() && spiralIndex >= 5 * 100;
}

REGISTER_LOOP_FUNCTIONS(Gradient_loop_functions, "Gradient_loop_functions")