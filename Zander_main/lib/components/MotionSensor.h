/*
 * MotionSensor.h - Library
 */


#ifndef MotionSensor_h

#define MotionSensor_h


#include "Arduino.h"



class MotionSensor 

{
  
	public:

MotionSensor();
    
MotionSensor(int component_ID);

MotionSensor(int component_ID, int pin_sensor);

void set_component_ID(int component_ID);

int get_component_ID();

int get_component_ID_with_action();
    
void set_configuration_pin(int pin_sensor);

void set_configuration_delay(int delay_value);

void is_detect_action_checking();
    
boolean is_detect_action();
  

	private:
    
int _component_ID;

int _pin_sensor;

int _delay_value = 5;
    
boolean _is_detect_action = false;

};


#endif
