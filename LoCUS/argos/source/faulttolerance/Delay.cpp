#include "Delay.h"

bool Delay::step() {
  count++;

  return count > delay_count;
}
