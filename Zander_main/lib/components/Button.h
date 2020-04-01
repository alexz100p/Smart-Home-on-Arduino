/*
 * Button.h - Library
 */


#ifndef Button_h

#define Button_h


#include "Arduino.h"



class Button 

{
  
	public:

Button();
    
Button(int component_ID);

Button(int component_ID, int pin_button);

void set_component_ID(int component_ID);

int get_component_ID();

int get_component_ID_with_action();
    
void set_configuration_pin(int pin_button);

void set_configuration_delay(int delay_value);

void is_button_pressed_checking();
    
boolean is_button_pressed();
  

	private:
    
int _component_ID;

int _pin_button;

int _delay_value = 100;
    
boolean _is_button_pressed = false;
  
};


#endif
