#ifndef GRADIENT_QT_USER_FUNCTIONS_H
#define GRADIENT_QT_USER_FUNCTIONS_H

#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_user_functions.h>
#include <argos3/plugins/robots/foot-bot/simulator/footbot_entity.h>
#include <argos3/core/simulator/entity/floor_entity.h>
#include <source/faulttolerance/FaultToleranceLoopFunctions.h>
#include "ColorUtil.h"

using namespace argos;
using namespace std;

class Gradient_controller;
class Gradient_loop_functions;

class Gradient_qt_user_functions : public CQTOpenGLUserFunctions {

public:

  Gradient_qt_user_functions();
  ~Gradient_qt_user_functions();

  void Init(TConfigurationNode &t_tree) override;

  void DrawOnArena(CFloorEntity& entity);

private:
  Gradient_loop_functions& loopFunctions;

  CColor readings[2000][400];
};

#endif /* GRADIENT_QT_USER_FUNCTIONS_H */