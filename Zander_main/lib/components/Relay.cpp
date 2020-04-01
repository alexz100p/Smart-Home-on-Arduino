/*
	Relay.cpp - Library
*/

#include "Arduino.h"
#include "Relay.h"


Relay::Relay()
{
	_component_ID = -1;
}

Relay::Relay(int component_ID)
{
  _component_ID = component_ID;
}

void Relay::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int Relay::get_component_ID()
{
  return _component_ID;
}

int Relay::get_component_ID_with_action()
{
  int _ID = _component_ID * 10;
  if(_is_component_on) _ID++;
  return _ID;
}

void Relay::set_configuration_pin(int pin_relay)
{
	_pin_relay = pin_relay;
  pinMode(_pin_relay, OUTPUT);
}

void Relay::component_on()
{
  digitalWrite(_pin_relay, HIGH);
  _is_component_on = true;
}

void Relay::component_off()
{
  digitalWrite(_pin_relay, LOW);
  _is_component_on = false;
}

void Relay::relay_control_simple()
{
  if(_is_component_on) component_off(); else component_on(); 
}

boolean Relay::is_component_on()
{
	return _is_component_on;
}
