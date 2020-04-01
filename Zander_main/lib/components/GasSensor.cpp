/*
  GasSensor.cpp - Library
*/

#include "Arduino.h"
#include "GasSensor.h"


GasSensor::GasSensor()
{
  _component_ID = -1;
}

GasSensor::GasSensor(int component_ID)
{
  _component_ID = component_ID;
}

void GasSensor::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int GasSensor::get_component_ID()
{
  return _component_ID;
}

int GasSensor::get_component_ID_with_action()
{
  int _ID = _component_ID * 10;
  if(_is_detect_action) _ID++;
  return _ID;
}

void GasSensor::set_configuration_pin(int pin_sensor)
{
  _pin_sensor = pin_sensor;
  pinMode(_pin_sensor, INPUT);
}

void GasSensor::set_configuration_threshold_signal(int threshold_signal)
{
    _threshold_signal = threshold_signal;
}

int GasSensor::get_threshold_value()
{
  return _threshold_signal;
}

void GasSensor::run_measure()
{
  _signal_value = analogRead(_pin_sensor);
  if(_signal_value >= _threshold_signal) _is_detect_action = true; else _is_detect_action = false;
}

int GasSensor::get_signal_value()
{
  return _signal_value;
}

boolean GasSensor::is_detect_action()
{
  return _is_detect_action;
}
