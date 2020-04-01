/*
	Door.cpp - Library
*/

#include "Arduino.h"
#include "Door.h"


Door::Door()
{
	_component_ID = -1;
}

Door::Door(int component_ID)
{
  _component_ID = component_ID;
}

void Door::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int Door::get_component_ID()
{
  return _component_ID;
}

int Door::get_component_ID_with_action()
{
  int _ID = _component_ID * 10;
  if(_is_door_opened) _ID++;
  return _ID;
}

void Door::set_configuration_pin(int pin_door)
{
	_pin_door = pin_door;
  pinMode(_pin_door, OUTPUT);
}

void Door::close_the_door()           //  Door is closed
{
  digitalWrite(_pin_door, LOW);
  _is_door_opened = false;
}

void Door::open_the_door()          //  Door is opened
{
  digitalWrite(_pin_door, HIGH);
  _is_door_opened = true;
}

void Door::door_control_simple()
{
  if(!_is_door_opened) open_the_door(); else close_the_door();
}

boolean Door::is_door_opened()
{
	return _is_door_opened;
}
