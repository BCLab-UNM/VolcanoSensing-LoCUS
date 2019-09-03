#ifndef GRADIENT_ARGOS_CONTROLLERBASE_H
#define GRADIENT_ARGOS_CONTROLLERBASE_H


class ControllerBase {

public:
  virtual bool CanInsert() = 0;

  virtual int GetMinimumDepth() = 0;
};

#endif //GRADIENT_ARGOS_CONTROLLERBASE_H
