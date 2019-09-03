#ifndef GRADIENT_ARGOS_SWARMLOCATION_H
#define GRADIENT_ARGOS_SWARMLOCATION_H

#include "argos3/core/utility/math/vector3.h"

class SwarmLocation {

public:
  virtual long getMaxChildrenSize() = 0;

  virtual argos::CVector3 getOffset() = 0;

  virtual SwarmLocation *GetChildLocation(long i) = 0;

  virtual SwarmLocation *GetParent() = 0;

  virtual bool IsRoot() = 0;

  virtual int getLevel() = 0;
};

#endif //GRADIENT_ARGOS_SWARMLOCATION_H
