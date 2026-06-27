#ifndef OVERRIDE_TOUCH_H
#define OVERRIDE_TOUCH_H

enum TouchEvent {
  NO_PRESS,
  SHORT_PRESS,
  LONG_PRESS
};

void overrideTouchInit();
TouchEvent readOverrideTouch();

#endif
