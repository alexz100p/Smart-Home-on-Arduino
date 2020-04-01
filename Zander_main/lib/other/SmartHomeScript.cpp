/*
	SmartHomeScript.cpp - Library
*/

#include "Arduino.h"
#include "SmartHomeScript.h"
 

SmartHomeScript::SmartHomeScript()
{
	//
}

void SmartHomeScript::set_time_now(int hours_now, int minutes_now)
{
  // (...)
}

String SmartHomeScript::get_script(int script_ID)
{
  _is_script_run = true;
  switch(script_ID){
    case _MORNING_SCRIPT_ID:  _script = _MORNING_SCRIPT;
      break;
    case _EVENING_SCRIPT_ID:  _script = _EVENING_SCRIPT;
      break;
    case _NIGHT_SCRIPT_ID:  _script = _NIGHT_SCRIPT;
      break;
    case _FIRE_SAFETY_SCRIPT_ID:  _script = _FIRE_SAFETY_SCRIPT;
      break;
    case _SAFETY_FROM_MISTERS_SCRIPT_ID:  _script = _SAFETY_FROM_MISTERS_SCRIPT;
      break;
    default: return "";
      break;
  }
  return _script;
}

String SmartHomeScript::get_script()
{
  return _script;
}

void SmartHomeScript::change_value_is_script_run()
{
  _is_script_run = !_is_script_run;
}

boolean SmartHomeScript::is_script_run()
{
  return _is_script_run;
}

