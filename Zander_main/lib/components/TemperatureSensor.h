/*
 * TemperatureSensor.h - Library
 */


#ifndef TemperatureSensor_h

#define TemperatureSensor_h



#include "Arduino.h"
#include "DHT.h"

class TemperatureSensor 

{
  
	public:

TemperatureSensor();

TemperatureSensor(int component_ID);

TemperatureSensor(int component_ID, int pin_sensor);

void set_component_ID(int component_ID);

int get_component_ID();

//int get_component_ID_with_action();

int get_component_ID_with_temperature_value();

int get_component_ID_with_humidity_value();

int calculate_component_ID_with_action(float val_action);

void set_configuration_pin(int pin_sensor);

void set_configuration_type_temperature_sensor(boolean is_DHT_type);

boolean is_DHT_sensor_type();

void set_configuration_measurement_parameters(int delay_value, int iteration_quantity);

void set_configuration_threshold_value(int threshold_value);

int get_threshold_value();

void set_configuration_error_value(int error_value);

int get_error_value();

void run_measure();

float get_temperature_value();

float get_humidity_value();

float get_average_value_from_measured_signals(boolean is_temperature_measuring);
  

	private:
    
int _component_ID;

int _pin_sensor;

boolean _is_DHT_type = true;
    
float _temperature_value = 0.0;

float _humidity_value = 0.0;

int _measurement_delay_value = 10;

int _measurement_iteration_quantity = 3;

int _error_value = 1;

};


#endif
