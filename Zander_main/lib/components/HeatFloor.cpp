/*
	HeatFloor.cpp - Library
*/

#include "Arduino.h"
#include "HeatFloor.h"


HeatFloor::HeatFloor()
{
	_component_ID = -1;
}

HeatFloor::HeatFloor(int component_ID)
{
  _component_ID = component_ID;
}

void HeatFloor::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int HeatFloor::get_component_ID()
{
  return _component_ID;
}

int HeatFloor::get_component_ID_with_action()
{
  int _ID = _component_ID * 10;
  if(_is_component_on) _ID++;
  return _ID;
}

void HeatFloor::set_configuration_pin(int pin_component)
{
	_pin_component = pin_component;
  pinMode(_pin_component, OUTPUT);
}

void HeatFloor::set_configuration_heat_floor_control(boolean is_control, int heat_floor_value, int signal_step_value)
{
    _is_control = is_control;
    _heat_floor_value = heat_floor_value;
    _signal_step_value = signal_step_value;
}

void HeatFloor::heat_floor_on(){
  if(!_is_control){
      digitalWrite(_pin_component, HIGH);
      _is_component_on = true;
    } else {
      _heat_floor_value = _MAX_SIGNAL_VALUE;
      analogWrite(_pin_component, _MAX_SIGNAL_VALUE);
      _is_component_on = true;
    }
}

void HeatFloor::heat_floor_off(){
  if(!_is_control){
      digitalWrite(_pin_component, LOW);
      _is_component_on = false;
    } else {
      _heat_floor_value = _MIN_SIGNAL_VALUE; 
      analogWrite(_pin_component, _MIN_SIGNAL_VALUE);
      _is_component_on = false;
    }
}

void HeatFloor::heating_control_simple(){
  if(!_is_component_on) heat_floor_on(); else heat_floor_off();
}

void HeatFloor::increase_heating()
{
  if(_is_control){
    _heat_floor_value += _signal_step_value;
    if(_heat_floor_value >= _MAX_SIGNAL_VALUE){ _heat_floor_value = _MAX_SIGNAL_VALUE; }
    if(_heat_floor_value > _MIN_SIGNAL_VALUE){ _is_component_on = true; }
    analogWrite(_pin_component, _heat_floor_value);
  }
}

void HeatFloor::reduce_heating()
{
  if(_is_control){
    _heat_floor_value -= _signal_step_value;
    if(_heat_floor_value <= _MIN_SIGNAL_VALUE){ _heat_floor_value = _MIN_SIGNAL_VALUE; }
    if(_heat_floor_value == _MIN_SIGNAL_VALUE){ _is_component_on = false; }
    analogWrite(_pin_component, _heat_floor_value);
  }
}

int HeatFloor::get_signal_value()
{
  return _heat_floor_value;
}

boolean HeatFloor::is_component_on()
{
	return _is_component_on;
}
