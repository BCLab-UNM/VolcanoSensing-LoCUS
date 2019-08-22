#include "SwarmManager.h"
#include "SwarmLocationImpl.h"

SwarmLocation *SwarmManager::GetLocation(int index) {
  int innerCount = 0;
  int level = 0;

  while (innerCount <= index) {
    if (levelCounts.size() <= level) {
      initalizeLevel(level);
    }
    innerCount += levelCounts.at(level);
    level++;
  }

  return locations.at(index);
}

void SwarmManager::initalizeLevel(int level) {
  int innerSwarmCount = 0;
  for(int i = 0; i < level + 1; i++) {
    if(levelCounts.size() == i) {

      double radius = 0;
      int levelchildrencount = 1;
      if(level != 0) {
        radius = level * rmax;
        double levelangle = asin(rmin / radius);
        levelchildrencount = int((M_PI * 2) / levelangle);
      }

      levelCounts.push_back(levelchildrencount);

      double shell_angle = (M_PI * 2) / levelchildrencount;

      for(int levelCounter = 0; levelCounter < levelchildrencount; levelCounter++) {

        double angle = levelCounter * shell_angle;
        double x = radius * cos(angle);
        double y = radius * sin(angle);

        argos::CVector3 offset = argos::CVector3(x, y, 10);
        SwarmLocation *parentPtr = calculateParent(level, offset);
        SwarmLocation *locationPtr = new SwarmLocationImpl(level, this, offset, parentPtr);
        associate(parentPtr, locationPtr);
        locations.push_back(locationPtr);
      }
    }
    innerSwarmCount += levelCounts.at(i);
  }
}

SwarmLocation *SwarmManager::calculateParent(int level, argos::CVector3 childOffset) {
  SwarmLocation *closest = NULL;
  double closestDistance = 1000000;
  for(long i = 0; i < locations.size(); i++) {
    auto &controller = locations.at(i);
    if(controller->getLevel() == level - 1) {

      argos::CVector3 parentOffset = controller->getOffset();

      double relativeDistance = (parentOffset - childOffset).Length();
      if(relativeDistance < closestDistance) {
        closestDistance = relativeDistance;
        closest = controller;
      }
    }
  }
  return closest;
}

std::vector<SwarmLocation *> *SwarmManager::GetChildrenLocations(SwarmLocation *location) {
  for(int i = 0; i < locations.size(); i++) {
    initalizeLevel(location->getLevel() + 1);
    if(!children.count(location)) {
      children.insert(std::make_pair(location, new std::vector<SwarmLocation*>()));
    }
    return children.at(location);
  }
}

std::vector<ControllerBase*> SwarmManager::GetChildren(ControllerBase* parent) {
  std::vector<ControllerBase*> childrenValues;

  SwarmLocation* parentLocation = locationAssociation.at(parent);
  std::vector<SwarmLocation*>* childrenLocations = GetChildrenLocations(parentLocation);
  for(SwarmLocation* location : *childrenLocations) {
    ControllerBase* childValue = GetValue(location);
    if(childValue != NULL) {
      childrenValues.push_back(childValue);
    }
  }

  return childrenValues;
};

void SwarmManager::associate(SwarmLocation *parent, SwarmLocation *child) {
  if(!children.count(parent)) {
    children.insert(std::make_pair(parent, new std::vector<SwarmLocation*>()));
  }
  children.at(parent)->push_back(child);
}

ControllerBase* SwarmManager::GetParent(ControllerBase* child) {
  SwarmLocation* childLocation = locationAssociation.at(child);
  SwarmLocation* parentLocation = childLocation->GetParent();

  return GetValue(parentLocation);
}

ControllerBase* SwarmManager::GetValue(SwarmLocation* location) {
  for(auto iter = locationAssociation.begin(); iter != locationAssociation.end(); ++iter) {
    if(iter->second == location) {
      return iter->first;
    }
  }

  return NULL;
}

void SwarmManager::RemoveChild(ControllerBase *child) {
  locationAssociation.erase(child);
}

SwarmLocation* SwarmManager::AddChild(ControllerBase *parent, ControllerBase *child) {
  SwarmLocation *parentLocation = locationAssociation.at(parent);

  std::vector<SwarmLocation *> *childrenLocations = GetChildrenLocations(parentLocation);

  for(SwarmLocation* childLocation : *childrenLocations) {
    if(GetValue(childLocation) == NULL) {
      locationAssociation.insert(std::make_pair(child, childLocation));
      return childLocation;
    }
  }
  return NULL;
}

void SwarmManager::UpdateLocation(ControllerBase *value, SwarmLocation *location) {
  std::map<ControllerBase*, SwarmLocation*>::iterator iter = locationAssociation.find(value);

  if(iter == locationAssociation.end()) {
    locationAssociation.insert(std::make_pair(value, location));
  } else {
    iter->second = location;
  }

}

SwarmManager::SwarmManager(double rmin, double rmax) : rmin(rmin), rmax(rmax) {}
