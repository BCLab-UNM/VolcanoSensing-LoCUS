#include "SwarmLocationImpl.h"

long SwarmLocationImpl::getMaxChildrenSize() {
  return getChildren().size();
}

SwarmLocation *SwarmLocationImpl::GetChildLocation(long index) {
  return getChildren().at(index);
}

std::vector<SwarmLocation*> SwarmLocationImpl::getChildren() {
  if(children == NULL) {
    children = manager->GetChildrenLocations(this);
  }
  return *children;
}

bool SwarmLocationImpl::IsRoot() {
  return parent == NULL;
}

SwarmLocation *SwarmLocationImpl::GetParent() {
  return parent;
}

argos::CVector3 SwarmLocationImpl::getOffset() {
  double x = radius * cos(angle + theta);
  double y = radius * sin(angle + theta);

  return argos::CVector3(x, y, 10);
}
