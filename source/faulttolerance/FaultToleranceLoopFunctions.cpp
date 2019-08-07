#include "FaultToleranceLoopFunctions.h"
#include "FaultToleranceController.h"
#include "SpiriController.h"

void Gradient_loop_functions::Init(TConfigurationNode& node) {


  CSpace::TMapPerType& spiris = GetSpace().GetEntitiesByType("spiri");

  int i = 0;
  for (auto &spiri : spiris) {
    CSpiriEntity& spiriEntity = *any_cast<CSpiriEntity*>(spiri.second);
    auto & controller = (Spiri_controller&)(spiriEntity.GetControllableEntity().GetController());

    controller.SetIndex(i++);
  }
}

void Gradient_loop_functions::Destroy() {
}

void Gradient_loop_functions::Reset() {

}

bool Gradient_loop_functions::IsExperimentFinished() {
  bool finished = true;

  CSpace::TMapPerType& footbots = GetSpace().GetEntitiesByType("spiri");

  for(CSpace::TMapPerType::iterator it = footbots.begin(); it != footbots.end(); it++) {
    CSpiriEntity& spiri = *any_cast<CSpiriEntity*>(it->second);
    Spiri_controller& c = (Spiri_controller&)(spiri.GetControllableEntity().GetController());

    finished &= c.IsFinished();
  }

  return finished;
}

REGISTER_LOOP_FUNCTIONS(Gradient_loop_functions, "Gradient_loop_functions")