#ifndef SPIRI_CONTROLLER_H_
#define SPIRI_CONTROLLER_H_

#include <argos3/core/control_interface/ci_controller.h>
#include <argos3/core/utility/logging/argos_log.h>
#include <argos3/core/utility/math/rng.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/simulator/loop_functions.h>
#include <argos3/plugins/robots/generic/control_interface/ci_quadrotor_position_actuator.h>
#include <argos3/plugins/robots/generic/control_interface/ci_positioning_sensor.h>
#include <stdlib.h>
#include "MovementVector.h"
#include "MoveToPosition.h"
#include "Delay.h"
#include <math.h>

using namespace argos;
using namespace std;

class Spiri_controller : public CCI_Controller, public CLoopFunctions {

public:
  void Init(TConfigurationNode& node);
  void ControlStep();
  bool IsFinished();
  void Reset();
  void Setup(int id, int level, argos::CVector3 offset, std::vector<Spiri_controller*>* controllers);
  void SetParent(int parentId);
  void AddChild(int childId);
  int GetLevel() { return level;}
  argos::CVector3 GetOffset() { return offset;}
  int GetIdentifier() { return id;}
  int SetupHeir();
  void replaceChild(int toReplace, int replacement);
  void fail() ;

  vector<int> GetChildren() { return children; }

  void AddRecursiveWaypoint(CVector3 waypoint);
  void AddMovement(Movement *move);
  MoveToPosition* CreateOffsetMovement(CVector3 waypoint);

  int NO_HEIR = -1;
  int heir = NO_HEIR;
  int parentHeir = NO_HEIR;
  int id;
  int parentId = NO_HEIR;
  int replaceId = NO_HEIR;
  bool failed = false;
  std::vector<int> children;
  std::vector<int> childHeir;
  argos::CVector3 offset;

  int GetMinimumHeight();

  void replace(Spiri_controller *pController);

  bool failureDetected();

  void removeChild(int i);

  Movement *CreateMovement(CVector3 vector3);

private:

  bool finished = false;
  MovementVector* movement;
  argos::CCI_PositioningSensor* compassSensor;
  argos::CCI_QuadRotorPositionActuator* positionActuator;
  std::vector<Spiri_controller*>* controllers;
  int level;
  void setupPosition() ;
  void addCoverage(CVector3 waypoint);
  int GetHeir();
  int GetSuccessor();
  int GetPredecessor();
  int leftmost();
  int rightmost();


};

#endif /* SPIRI_CONTROLLER_H_ */