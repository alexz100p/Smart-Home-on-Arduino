/*
 * Door.h - Library
 */


#ifndef Door_h

#define Door_h


#include "Arduino.h"



class Door 

{
  
	public:
    
Door();

Door(int component_ID);

void set_component_ID(int component_ID);

int get_component_ID();

int get_component_ID_with_action();
    
void set_configuration_pin(int pin_door);

void close_the_door();

void open_the_door();

void door_control_simple();
    
boolean is_door_opened();
  

	private:
    
int _component_ID;

int _pin_door;
    
boolean _is_door_opened = false;

};


#endif
