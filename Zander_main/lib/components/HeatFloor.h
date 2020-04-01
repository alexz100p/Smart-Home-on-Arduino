/*
 * HeatFloor.h - Library
 */


#ifndef HeatFloor_h

#define HeatFloor_h


#include "Arduino.h"



class HeatFloor 

{
  
	public:

int _STANDARD_TEMPERATURE_VALUE = 22;

int _LOW_TEMPERATURE_VALUE = 18;

int _MAX_SIGNAL_VALUE = 255;

int _MIN_SIGNAL_VALUE = 0;

HeatFloor();
    
HeatFloor(int component_ID);

void set_component_ID(int component_ID);

int get_component_ID();

int get_component_ID_with_action();
    
void set_configuration_pin(int pin_component);
    
void set_configuration_heat_floor_control(boolean is_control, int heat_floor_value, int signal_step_value);

void heat_floor_on();

void heat_floor_off();

void heating_control_simple();

void increase_heating();

void reduce_heating();

int get_signal_value();
    
boolean is_component_on();
  

	private:
    
int _component_ID;

int _pin_component;

boolean _is_control = false;

int _heat_floor_value = 255;

int _signal_step_value = 25;

int _max_signal_value = 255;

int _min_signal_value = 0;
    
boolean _is_component_on = false;

};


#endif
