#include "FaultToleranceQTUserFunctions.h"

Gradient_qt_user_functions::Gradient_qt_user_functions() :
    loopFunctions(static_cast<Gradient_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions()))
{}

Gradient_qt_user_functions::~Gradient_qt_user_functions() {

}

void Gradient_qt_user_functions::Init(TConfigurationNode &t_tree) {
  CQTOpenGLUserFunctions::Init(t_tree);

  RegisterUserFunction<Gradient_qt_user_functions, CFloorEntity>(&Gradient_qt_user_functions::DrawOnArena);
}

void Gradient_qt_user_functions::DrawOnArena(CFloorEntity& entity) {

}

REGISTER_QTOPENGL_USER_FUNCTIONS(Gradient_qt_user_functions, "Gradient_qt_user_functions")