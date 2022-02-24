#include "Button.h"

Button::Button(int pin) {
  this->pin = pin;
  init();
}

void Button::init(){
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH);
    buttonIsPressed = false;
    buttonWasPressed = false;
    buttonDown = false;
}

void Button::updateButton(){
    bool buttonState = digitalRead(pin);
    if (buttonState == LOW)
        buttonIsPressed = true;
    else
        buttonIsPressed = false;
  if(buttonIsPressed != buttonWasPressed && buttonIsPressed == HIGH){
    buttonDown = true;
  }
  else{
    buttonDown = false;
  }
  buttonWasPressed = buttonIsPressed;
}

bool Button::getButton(){
  return digitalRead(pin);
}

bool Button::getButtonDown(){
  return buttonDown;
}
