/*
	BrightSensor.cpp - Library
*/

#include "Arduino.h"
#include "BrightSensor.h"


BrightSensor::BrightSensor()
{
	_component_ID = -1;
}

BrightSensor::BrightSensor(int component_ID)
{
  _component_ID = component_ID;
}

void BrightSensor::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int BrightSensor::get_component_ID()
{
  return _component_ID;
}

int BrightSensor::get_component_ID_with_action()
{
  int _ID = _component_ID * 10;
  if(_is_detect_action) _ID++;
  return _ID;
}

void BrightSensor::set_configuration_pin(int pin_sensor)
{
	_pin_sensor = pin_sensor;
  pinMode(_pin_sensor, INPUT);
}

void BrightSensor::set_configuration_threshold_signal(int threshold_signal)
{
  _threshold_signal = threshold_signal;
}

int BrightSensor::get_threshold_signal()
{
  return _threshold_signal;
}

void BrightSensor::set_configuration_low_threshold_signal(int low_threshold_signal)
{
  _low_threshold_signal = low_threshold_signal;
}

int BrightSensor::get_low_threshold_signal()
{
  return _low_threshold_signal;
}

void BrightSensor::set_configuration_error_signal(int error_signal)
{
  _error_signal = error_signal;
}

int BrightSensor::get_error_signal()
{
  return _error_signal;
}

void BrightSensor::run_measure()
{
  _signal_value = analogRead(_pin_sensor);
  if(_signal_value >= _threshold_signal) _is_detect_action = true; else _is_detect_action = false;
}

int BrightSensor::get_signal_value()
{
  return _signal_value;
}

boolean BrightSensor::is_detect_action()
{
	return _is_detect_action;
}
