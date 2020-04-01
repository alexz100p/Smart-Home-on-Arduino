/*
 * GasSensor.h - Library
 */


#ifndef GasSensor_h

#define GasSensor_h


#include "Arduino.h"



class GasSensor 

{
  
 public:

GasSensor();
    
GasSensor(int component_ID);

void set_component_ID(int component_ID);

int get_component_ID();

int get_component_ID_with_action();
    
void set_configuration_pin(int pin_sensor);
    
void set_configuration_threshold_signal(int threshold_signal);

int get_threshold_value();

void run_measure();

int get_signal_value();

boolean is_detect_action();
  

  private:
    
int _component_ID;

int _pin_sensor;

int _threshold_signal = 500;

int _signal_value = 0;
    
boolean _is_detect_action = false;

};


#endif
