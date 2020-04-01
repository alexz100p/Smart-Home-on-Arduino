/*
	Lamp.cpp - Library
*/

#include "Arduino.h"
#include "Lamp.h"


Lamp::Lamp()
{
	_component_ID = -1;
}

Lamp::Lamp(int component_ID)
{
  _component_ID = component_ID;
}

void Lamp::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int Lamp::get_component_ID()
{
  return _component_ID;
}

int Lamp::get_component_ID_with_action()
{
  int _ID = _component_ID * 10;
  if(_is_lamp_on) _ID++;
  return _ID;
}

void Lamp::set_configuration_pin(int pin_lamp)
{
	_pin_lamp = pin_lamp;
  pinMode(_pin_lamp, OUTPUT);
}

void Lamp::set_configuration_lamp_control(boolean is_control, int bright_lamp, int signal_step_value)
{
    _is_control = is_control;
    _bright_lamp = bright_lamp;
    _signal_step_value = signal_step_value;
}

void Lamp::lamp_on(){
  if(!_is_control){
      digitalWrite(_pin_lamp, HIGH);
      _is_lamp_on = true;
    } else {
      _bright_lamp = _max_bright_value;
      analogWrite(_pin_lamp, _max_bright_value);
      _is_lamp_on = true;
    }
}

void Lamp::lamp_off(){
  if(!_is_control){
      digitalWrite(_pin_lamp, LOW);
      _is_lamp_on = false;
    } else {
      _bright_lamp = _min_bright_value; 
      analogWrite(_pin_lamp, _min_bright_value);
      _is_lamp_on = false;
    }
}

void Lamp::lamp_control_simple(){
  if(!_is_lamp_on) lamp_on(); else lamp_off();
}

void Lamp::increase_lamp_brightness()
{
  if(_is_control){
    _bright_lamp += _signal_step_value;
    if(_bright_lamp >= _max_bright_value){ _bright_lamp = _max_bright_value; _is_lamp_on = true; }
    analogWrite(_pin_lamp, _bright_lamp);
  }
}

void Lamp::reduce_lamp_brightness()
{
  if(_is_control){
    _bright_lamp -= _signal_step_value;
    if(_bright_lamp <= _min_bright_value){ _bright_lamp = _min_bright_value; _is_lamp_on = false; }
    analogWrite(_pin_lamp, _bright_lamp);
  }
}

int Lamp::get_bright_value()
{
  return _bright_lamp;
}

boolean Lamp::is_lamp_on()
{
	return _is_lamp_on;
}
