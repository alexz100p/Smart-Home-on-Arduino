/*
	MotionSensor.cpp - Library
*/

#include "Arduino.h"
#include "MotionSensor.h"

MotionSensor::MotionSensor()
{
  _component_ID = -1;
}

MotionSensor::MotionSensor(int component_ID)
{
	_component_ID = component_ID;
}

MotionSensor::MotionSensor(int component_ID, int pin_sensor)
{
  _component_ID = component_ID;
  _pin_sensor = pin_sensor;
  pinMode(_pin_sensor, INPUT);
}

void MotionSensor::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int MotionSensor::get_component_ID()
{
  return _component_ID;
}

int MotionSensor::get_component_ID_with_action()
{
  int _ID = _component_ID * 10;
  if(is_detect_action()) _ID++;
  return _ID;
}

void MotionSensor::set_configuration_pin(int pin_sensor)
{
  _pin_sensor = pin_sensor;
  pinMode(_pin_sensor, INPUT);
}

void MotionSensor::set_configuration_delay(int delay_value)
{
  _delay_value = delay_value;
}

void MotionSensor::is_detect_action_checking()
{
  delay(_delay_value);
  if(digitalRead(_pin_sensor) == HIGH){
    _is_detect_action = true;
  } else { _is_detect_action = false; }
}

boolean MotionSensor::is_detect_action()
{
  return _is_detect_action;
}
