#include "timer.h"

void BurstTimer::update(float d) {
  dt += d;
  if (dt > duration) {
    timeUp = true;
    if (repeat) dt = 0;
  } else {
    timeUp = false;
  }
}

bool BurstTimer::tick() {
  return timeUp;
}

void BurstTimer::reset() {
  dt = 0;
}