#include "FaultToleranceLoopFunctions.h"
#include "SpiriController.h"

void Gradient_loop_functions::Init(TConfigurationNode& node) {

  TConfigurationNode simNode  = GetNode(node, "simulation");

  GetNodeAttribute(simNode, "RMin", rmin);
  GetNodeAttribute(simNode, "RMax", rmax);
  GetNodeAttribute(simNode, "StopCoverageRadius", stopRadius);

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

    if(fullshells < level) {
      fullshells = level;
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
  simulationTime++;
  if(rootController->IsFinished()) {
    if(!constellation_setup) {
      LOG << "Setup:" << simulationTime << endl;
      cout << "Setup:" << simulationTime << endl;
      constellation_setup = true;
      cout << "Fullshells:" << fullshells << endl;
    }
    argos::CVector3 waypoint = buildArchimedesSpiralWaypoint(spiralIndex++, 2 * fullshells * rmax);
    rootController->AddRecursiveWaypoint(waypoint);
    waypoints.push_back(waypoint);
  }
}

argos::CVector3 Gradient_loop_functions::buildArchimedesSpiralWaypoint(int index, double radius) {
  double altitude = 0;

  // https://www.comsol.com/blogs/how-to-build-a-parameterized-archimedean-spiral-geometry/
  double a = 0;
  double b = radius / (2 * M_PI);
  double index_angle_offset = 0;//(1.0 / size) * (2 * M_PI);

  double angle = index * 2 * M_PI / points_per_rotation;
  double point_radius = a + (b * angle);
  double xoffset = point_radius * cos(angle + index_angle_offset); // in meters
  double yoffset = point_radius * sin(angle + index_angle_offset);

  return argos::CVector3(xoffset, yoffset, altitude);
}

void Gradient_loop_functions::Reset() {
  spiralIndex = 0;
  simulationTime = 0;

  for(int i = 0; i < 1000; i++) {
    for(int j = 0; j < 1000; j++) {
      coverage[i][j] = false;
    }
  }
}

bool Gradient_loop_functions::IsExperimentFinished() {
  bool surveyCovered = (2.0 * fullshells * rmax * spiralIndex / points_per_rotation) > stopRadius;
  return rootController->IsFinished() && surveyCovered;
}

void Gradient_loop_functions::PostExperiment() {
  LOG << "Time:" << simulationTime << endl;
  cout << "Time:" << simulationTime << endl;
}

REGISTER_LOOP_FUNCTIONS(Gradient_loop_functions, "Gradient_loop_functions")