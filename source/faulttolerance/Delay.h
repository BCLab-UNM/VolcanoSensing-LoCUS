//
// Created by john on 3/21/19.
//

#ifndef GRADIENT_ARGOS_DELAY_H
#define GRADIENT_ARGOS_DELAY_H


#include "Movement.h"

class Delay : public virtual Movement {

public:
  Delay(int delay_count) : delay_count(delay_count) {};

  bool step();

private:
  int count = 0;
  int delay_count;
};


#endif //GRADIENT_ARGOS_DELAY_H
