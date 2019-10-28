#ifndef FAULT_TOLERANCE_DRONES_ARGOS_SWARMMANAGER_H
#define FAULT_TOLERANCE_DRONES_ARGOS_SWARMMANAGER_H

#include "SwarmLocation.h"
#include "ControllerBase.h"
#include <vector>
#include <map>

class SwarmManager {

public:
  SwarmManager(double rmin, double rmax);
  SwarmLocation *GetLocation(int index);
  std::vector<SwarmLocation *> *GetChildrenLocations(SwarmLocation *location);
  std::vector<ControllerBase*> GetChildren(ControllerBase *value);
  ControllerBase* GetParent(ControllerBase *child);
  void RemoveChild(ControllerBase *child);
  SwarmLocation* AddChild(ControllerBase *parent, ControllerBase *child);
  ControllerBase* GetValue(SwarmLocation* location);
  SwarmLocation* GetLocation(ControllerBase* controller);

  void UpdateLocation(ControllerBase *value, SwarmLocation *location);

  bool AreChildrenFull(ControllerBase *parent);

  bool IsRoot(ControllerBase *pController);

private:
  double rmin;
  double rmax;

  std::vector<int> levelCounts;
  std::vector<SwarmLocation*> locations;
  std::map<SwarmLocation*, std::vector<SwarmLocation*>*> children;
  std::map<ControllerBase*, SwarmLocation*> locationAssociation;

  void initalizeLevel(int level);
  SwarmLocation *calculateParent(int level, argos::CVector3 childOffset);
  void associate(SwarmLocation *pLocation, SwarmLocation *pSwarmLocation);
};


#endif //FAULT_TOLERANCE_DRONES_ARGOS_SWARMMANAGER_H
