#include "FaultToleranceQTUserFunctions.h"

Gradient_qt_user_functions::Gradient_qt_user_functions() :
    loopFunctions(static_cast<Gradient_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions()))
{}

Gradient_qt_user_functions::~Gradient_qt_user_functions() {

}

void Gradient_qt_user_functions::Init(TConfigurationNode &t_tree) {
  CQTOpenGLUserFunctions::Init(t_tree);

  RegisterUserFunction<Gradient_qt_user_functions, CFloorEntity>(&Gradient_qt_user_functions::DrawOnArena);

  SimplePlume plume = loopFunctions.getPlume();

  for(int i = 0; i < plume.getXSize(); i++) {
    for(int j = 0; j < plume.getYSize(); j++) {
      // Map the values into Blue to Red hue rainbow
      readings[i][j] = ColorUtil::HSVtoRGB((1 - (plume.getValue(i + plume.getXOffset(), j + plume.getYOffset() - 200))) / 1.4, 1, 1);
    }
  }
}

void Gradient_qt_user_functions::DrawOnArena(CFloorEntity& entity) {

  if(loopFunctions.GetWaypoints().size() > 1) {
    size_t unStart = 0;
    size_t unEnd = 1;
    while(unEnd < loopFunctions.GetWaypoints().size()) {
      DrawRay(CRay3(loopFunctions.GetWaypoints()[unEnd],
                    loopFunctions.GetWaypoints()[unStart]), CColor::RED, 2.0f);
      ++unStart;
      ++unEnd;
    }
  }

  SimplePlume plume = loopFunctions.getPlume();

  for(int i = 0; i < plume.getXSize(); i++) {
    for(int j = 0; j < plume.getYSize(); j++) {
      if(plume.getValue(i + plume.getXOffset(),j + plume.getYOffset() - 200) > 0.005) {
        CVector3 nest_3d(((i + plume.getXOffset()) / 10.0), ((j + plume.getYOffset() - 200) / 10.0), 0.1);
        DrawBox(nest_3d, CQuaternion(), CVector3(0.1, 0.1, 0.001), readings[i][j]);
      }
    }
  }

//  for(int i = 0; i < plume.getXSize(); i++) {
//    CVector3 nest_3d((i + plume.getXOffset()) / 10.0, (plume.getYOffset() - 200) / 10.0, 0.1);
//    DrawBox(nest_3d, CQuaternion(), CVector3(0.1, 0.1, 0.001), CColor::BLACK);
//    CVector3 nest_3d3((i + plume.getXOffset()) / 10.0, (plume.getYSize() + plume.getYOffset() - 200) / 10.0, 0.1);
//    DrawBox(nest_3d3, CQuaternion(), CVector3(0.1, 0.1, 0.001), CColor::BLACK);
//  }
//
//
//  for(int j = 0; j < plume.getYSize(); j++) {
//    CVector3 nest_3d(plume.getXOffset() / 10.0 , (j + plume.getYOffset() - 200)  / 10.0, 0.1);
//    DrawBox(nest_3d, CQuaternion(), CVector3(0.1, 0.1, 0.001), CColor::BLACK);
//    CVector3 nest_3d3((plume.getXSize() + plume.getXOffset()) / 10.0, (j + plume.getYOffset() - 200) / 10.0, 0.1);
//    DrawBox(nest_3d3, CQuaternion(), CVector3(0.1, 0.1, 0.001), CColor::BLACK);
//  }
}

REGISTER_QTOPENGL_USER_FUNCTIONS(Gradient_qt_user_functions, "Gradient_qt_user_functions")