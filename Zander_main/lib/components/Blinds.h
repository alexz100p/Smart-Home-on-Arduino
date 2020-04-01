/*
 * Blinds.h - Library
 */


#ifndef Blinds_h

#define Blinds_h


#include "Arduino.h"


class Blinds 

{
  
	public:

Blinds();
    
Blinds(int component_ID);

void set_component_ID(int component_ID);

int get_component_ID();

int get_component_ID_with_action();
    
void set_configuration_driver(int pin_stp, int pin_dir);

void set_configuration_driver(int pin_enable, int pin_stp, int pin_dir);
    
void set_configuration_speed_parameters(int speed_max_value, int speed_min_value);

void set_configuration_step_motor_parameters(int quantity_steps_at_interval_at_standard_motion, int quantity_steps_at_deluxe_motion);

void set_configuration_delay_parameters(int delay_at_interval_at_standard_motion, int delay_at_deluxe_motion);

void set_configuration_sensors_parameters(int top_sensor_pin, int threshold_top_sensor, int bottom_sensor_pin, int threshold_bottom_sensor);

void set_configuration_sensors_parameters(int bottom_sensor_pin, int threshold_bottom_sensor);

void set_configuration_measurement_parameters(int delay_value, int iteration_quantity);
    
void run_motion_control_blinds(boolean calculate_steps, boolean progressive_algorythm);

void run_motor(int quantity_steps, int delay_value);

int get_value_from_measured_signal(int pin_sensor);

int get_average_value_from_measured_signals(int pin_sensor);

int get_average_value_from_values(int array_values[], int measurement_iteration_quantity);

int get_error_value_from_values(int array_values[], int measurement_iteration_quantity, int average_value_from_array_values);

void is_blinds_open_checking();
    
boolean is_blinds_open();

boolean checking_level_signal(int pin_sensor);

boolean is_blinds_open_for_calibration();

void sensors_calibration();
  

	private:
    
int _component_ID;

int _pin_enable_motor_driver;

int _pin_dir_motor_driver;

int _pin_step_motor_driver;

int _quantity_steps_at_interval_at_standard_motion = 5;
    
int _quantity_steps_at_deluxe_motion = 30;

int _delay_at_interval_at_standard_motion = 1000;

int _delay_at_deluxe_motion = 800;
    
int _speed_max_value;
    
int _speed_min_value;
    
boolean _is_blinds_open;

int _top_sensor_pin;

int _threshold_top_sensor;

int _error_top_sensor;

int _bottom_sensor_pin; 

int _threshold_bottom_sensor;

int _error_bottom_sensor;

int _measurement_delay_value = 0;

int _measurement_iteration_quantity = 5;

int _quantity_steps_for_full_motion = 0;

boolean _is_top_sensor_used = true;

};


#endif
