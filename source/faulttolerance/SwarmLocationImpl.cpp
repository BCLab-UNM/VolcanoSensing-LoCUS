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
