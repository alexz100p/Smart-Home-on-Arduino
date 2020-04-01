/*
	Button.cpp - Library
*/

#include "Arduino.h"
#include "Button.h"

Button::Button()
{
  _component_ID = 0;
}

Button::Button(int component_ID)
{
	_component_ID = component_ID;
}

Button::Button(int component_ID, int pin_button)
{
  _component_ID = component_ID;
  _pin_button = pin_button;
  pinMode(_pin_button, OUTPUT);
}

void Button::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int Button::get_component_ID()
{
  return _component_ID;
}

int Button::get_component_ID_with_action()
{
  int _ID = _component_ID * 10;
  if(_is_button_pressed) _ID++;
  return _ID;
}

void Button::set_configuration_pin(int pin_button)
{
  _pin_button = pin_button;
  pinMode(_pin_button, INPUT);
}

void Button::set_configuration_delay(int delay_value)
{
  _delay_value = delay_value;
}

void Button::is_button_pressed_checking()
{
  delay(_delay_value);
  if(digitalRead(_pin_button) == HIGH){
    _is_button_pressed = true;
  } else {_is_button_pressed = false;}
}

boolean Button::is_button_pressed()
{
	return _is_button_pressed;
}
