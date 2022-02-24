#ifndef MY_BUTTON_H
#define MY_BUTTON_H

#include <Arduino.h>

class Button{
  private:
  int pin;
  bool buttonIsPressed;
  bool buttonWasPressed;
  bool buttonDown;

  public:
  Button(int pin);
  void init();
  void updateButton();
  bool getButton();
  bool getButtonDown();
};
#endif
