/*
 * Relay.h - Library
 */


#ifndef Relay_h

#define Relay_h


#include "Arduino.h"



class Relay 

{
  
	public:
    
Relay();

Relay(int component_ID);

void set_component_ID(int component_ID);

int get_component_ID();

int get_component_ID_with_action();
    
void set_configuration_pin(int pin_relay);

void component_on();

void component_off();

void relay_control_simple();
    
boolean is_component_on();
  

	private:
    
int _component_ID;

int _pin_relay;
    
boolean _is_component_on = false;

};


#endif
