/*
 * SmartHomeScript.h - Library
 */


#ifndef SmartHomeScript_h

#define SmartHomeScript_h


#include "Arduino.h"



class SmartHomeScript 

{
  
	public:
 
static const int _MORNING_SCRIPT_ID = 0;

static const int _EVENING_SCRIPT_ID = 1;

static const int _NIGHT_SCRIPT_ID = 2;

static const int _FIRE_SAFETY_SCRIPT_ID = 10;

static const int _SAFETY_FROM_MISTERS_SCRIPT_ID = 11;

SmartHomeScript();

void set_time_now(int hours_now, int minutes_now);

String get_script(int script_ID);

String get_script();

void change_value_is_script_run();

boolean is_script_run();
  

	private:
/*
 * Lamps                 1
 * Blinds                2
 * Relays                3
 * Heat Floor            4
 * Doors		 5
*/
String _MORNING_SCRIPT = "";

String _EVENING_SCRIPT = "";

String _NIGHT_SCRIPT = "";

String _FIRE_SAFETY_SCRIPT = "501 511 521 191 101 111 121 201 211 221 300 310 320 400 410 420";

String _SAFETY_FROM_MISTERS_SCRIPT = "";

int _MORNING_SCRIPT_RUN_AT_HOURS = 7;

int _DAY_SCRIPT_RUN_AT_HOURS = 12;

int _EVENING_SCRIPT_RUN_AT_HOURS = 19;

int _NIGHT_SCRIPT_RUN_AT_HOURS = 0;

String _script;

boolean _is_script_run = false;

boolean _is_scripts_on = true;

};


#endif
