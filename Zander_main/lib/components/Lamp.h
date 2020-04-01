/*
 * Lamp.h - Library
 */


#ifndef Lamp_h

#define Lamp_h


#include "Arduino.h"



class Lamp 

{
  
	public:

Lamp();
    
Lamp(int component_ID);

void set_component_ID(int component_ID);

int get_component_ID();

int get_component_ID_with_action();
    
void set_configuration_pin(int pin_lamp);
    
void set_configuration_lamp_control(boolean is_control, int bright_lamp, int signal_step_value);

void lamp_on();

void lamp_off();

void lamp_control_simple();

void increase_lamp_brightness();

void reduce_lamp_brightness();

int get_bright_value();
    
boolean is_lamp_on();
  

	private:
    
int _component_ID;

int _pin_lamp;

boolean _is_control = false;

int _bright_lamp = 255;

int _signal_step_value = 10;

int _max_bright_value = 255;

int _min_bright_value = 0;
    
boolean _is_lamp_on = false;

};


#endif
