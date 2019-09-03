#ifndef FAULT_TOLERANCE_DRONES_ARGOS_SWARMLOCATIONIMPL_H
#define FAULT_TOLERANCE_DRONES_ARGOS_SWARMLOCATIONIMPL_H

#include <vector>
#include <argos3/core/utility/math/vector3.h>
#include "SwarmManager.h"
#include "SwarmLocation.h"

class SwarmLocationImpl : public virtual SwarmLocation {

public:
  SwarmLocationImpl(int level, SwarmManager* manager, argos::CVector3 offset, SwarmLocation* parent): level(level), manager(manager), offset(offset), parent(parent){}
  argos::CVector3 getOffset() { return offset;}
  long getMaxChildrenSize();
  SwarmLocation *GetChildLocation(long index);
  SwarmLocation *GetParent();
  bool IsRoot();
  int getLevel() { return level; }

private:
  int level;
  SwarmManager* manager;
  argos::CVector3 offset;
  SwarmLocation* parent;
  std::vector<SwarmLocation*>* children = NULL;

  std::vector<SwarmLocation*> getChildren();
};


#endif //FAULT_TOLERANCE_DRONES_ARGOS_SWARMLOCATIONIMPL_H
