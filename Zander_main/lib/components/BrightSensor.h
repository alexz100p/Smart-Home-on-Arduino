/*
 * BrightSensor.h - Library
 */


#ifndef BrightSensor_h

#define BrightSensor_h


#include "Arduino.h"



class BrightSensor 

{
  
	public:

BrightSensor();
    
BrightSensor(int component_ID);

void set_component_ID(int component_ID);

int get_component_ID();

int get_component_ID_with_action();
    
void set_configuration_pin(int pin_sensor);
    
void set_configuration_threshold_signal(int threshold_signal);

int get_threshold_signal();

void set_configuration_low_threshold_signal(int low_threshold_signal);

int get_low_threshold_signal();

void set_configuration_error_signal(int error_signal);

int get_error_signal();

void run_measure();

int get_signal_value();

boolean is_detect_action();
  

	private:
    
int _component_ID;

int _pin_sensor;

int _low_threshold_signal = 50;

int _threshold_signal = 800;

int _error_signal = 10;

int _signal_value = 0;
    
boolean _is_detect_action = false;

};


#endif
