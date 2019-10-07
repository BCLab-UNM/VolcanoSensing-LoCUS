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

  if(loopFunctions.GetWaypoints().size() > 1) {
    size_t unStart = 0;
    size_t unEnd = 1;
    while(unEnd < loopFunctions.GetWaypoints().size()) {
      DrawRay(CRay3(loopFunctions.GetWaypoints()[unEnd],
                    loopFunctions.GetWaypoints()[unStart]), CColor::RED, 3.0f);
      ++unStart;
      ++unEnd;
    }
  }

//  for(int i = 0; i < 1000; i++) {
//    for(int j = 0; j < 1000; j++) {
//      if(loopFunctions.coverage[i][j]) {
//        CVector3 nest_3d(i - 500, j - 500, 0.1);
//        DrawBox(nest_3d, CQuaternion(), CVector3(1, 1, 0.001), CColor::BLUE);
//      }
//    }
//  }
}

REGISTER_QTOPENGL_USER_FUNCTIONS(Gradient_qt_user_functions, "Gradient_qt_user_functions")