/*
	Blinds.cpp - Library
*/

#include "Arduino.h"
#include "Blinds.h"
#include "Stepper.h"


Blinds::Blinds()
{
  _component_ID =-1;
}

Blinds::Blinds(int component_ID)
{
	_component_ID = component_ID;
}

void Blinds::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int Blinds::get_component_ID()
{
  return _component_ID;
}

int Blinds::get_component_ID_with_action()
{
  int _ID = _component_ID * 10;
  if(is_blinds_open()) _ID++;
  return _ID;
}

void Blinds::set_configuration_driver(int pin_stp, int pin_dir)
{
  _pin_enable_motor_driver = -1;
  _pin_step_motor_driver = pin_stp;
  _pin_dir_motor_driver = pin_dir;
  pinMode(_pin_step_motor_driver, OUTPUT);
  pinMode(_pin_dir_motor_driver, OUTPUT);
}

void Blinds::set_configuration_driver(int pin_enable, int pin_stp, int pin_dir)
{
  _pin_enable_motor_driver = pin_enable;
	_pin_step_motor_driver = pin_stp;
	_pin_dir_motor_driver = pin_dir;
  pinMode(_pin_enable_motor_driver, OUTPUT);
	pinMode(_pin_step_motor_driver, OUTPUT);
  pinMode(_pin_dir_motor_driver, OUTPUT);
}

void Blinds::set_configuration_speed_parameters(int speed_max_value, int speed_min_value)
{
	//
}

void Blinds::set_configuration_step_motor_parameters(int quantity_steps_at_interval_at_standard_motion, int quantity_steps_at_deluxe_motion)
{
  _quantity_steps_at_interval_at_standard_motion = quantity_steps_at_interval_at_standard_motion;
  _quantity_steps_at_deluxe_motion = quantity_steps_at_deluxe_motion;
}

void Blinds::set_configuration_delay_parameters(int delay_at_interval_at_standard_motion, int delay_at_deluxe_motion)
{
  _delay_at_interval_at_standard_motion = delay_at_interval_at_standard_motion;
  _delay_at_interval_at_standard_motion = delay_at_deluxe_motion;
}

void Blinds::set_configuration_sensors_parameters(int top_sensor_pin, int threshold_top_sensor, int bottom_sensor_pin, int threshold_bottom_sensor)
{
  _top_sensor_pin = top_sensor_pin;
  _threshold_top_sensor = threshold_top_sensor;
  _bottom_sensor_pin = bottom_sensor_pin;
  _threshold_bottom_sensor = threshold_bottom_sensor;
  _is_top_sensor_used = true;
}

void Blinds::set_configuration_sensors_parameters(int bottom_sensor_pin, int threshold_bottom_sensor)
{
  _top_sensor_pin = -1;
  _threshold_top_sensor = -1;
  _bottom_sensor_pin = bottom_sensor_pin;
  _threshold_bottom_sensor = threshold_bottom_sensor;
  _is_top_sensor_used = false;
}

void Blinds::set_configuration_measurement_parameters(int delay_value, int iteration_quantity)
{
  _measurement_delay_value = delay_value;
  _measurement_iteration_quantity = iteration_quantity;
}

void Blinds::run_motion_control_blinds(boolean calculate_steps, boolean progressive_algorythm)
{
  int size_arr_meas_res = 3;
  int arr_meas_res[size_arr_meas_res];
  int res_progressive_algorythm = 0;
  int quantity_steps_at_deluxe_motion = _quantity_steps_at_deluxe_motion;
  int quantity_full_steps = 0;
  //
  if(_is_blinds_open){                               //  IF BLINDS IS OPEN
    if(progressive_algorythm){
      for(int i = 0; i < size_arr_meas_res; i++){
        arr_meas_res[i] = get_average_value_from_measured_signals(_bottom_sensor_pin);
      }
    }
    digitalWrite(_pin_dir_motor_driver, HIGH);
    while(true){
      if(!progressive_algorythm){
        if(get_average_value_from_measured_signals(_bottom_sensor_pin) > _threshold_bottom_sensor){     
          run_motor(_quantity_steps_at_interval_at_standard_motion, _delay_at_interval_at_standard_motion);
          if(calculate_steps) quantity_full_steps += _quantity_steps_at_interval_at_standard_motion;
        } else break;
      } else {
        for(int i = 1; i < size_arr_meas_res; i++) { arr_meas_res[i] = arr_meas_res[i-1]; }
        arr_meas_res[0] = get_average_value_from_measured_signals(_bottom_sensor_pin);
        res_progressive_algorythm = 0;
        for(int i = 0; i < size_arr_meas_res; i++) {
          if(arr_meas_res[i] < (_threshold_bottom_sensor - _error_bottom_sensor)){ res_progressive_algorythm++; }
        }
        if(res_progressive_algorythm == size_arr_meas_res){ 
          break;
        } else {
          run_motor(_quantity_steps_at_interval_at_standard_motion, _delay_at_interval_at_standard_motion);
          if(calculate_steps) quantity_full_steps += _quantity_steps_at_interval_at_standard_motion;
        }
      }
    }
      if(progressive_algorythm){ quantity_steps_at_deluxe_motion = _quantity_steps_at_deluxe_motion - 
                                                     (size_arr_meas_res * _quantity_steps_at_interval_at_standard_motion); }
      run_motor(quantity_steps_at_deluxe_motion, _delay_at_deluxe_motion);
      if(calculate_steps) { quantity_full_steps += _quantity_steps_at_deluxe_motion; 
                            _quantity_steps_for_full_motion = quantity_full_steps; }
      _is_blinds_open = false;                              //  BLINDS IS CLOSE
  } else if(!_is_blinds_open){                      //  IF BLINDS IS CLOSE
    digitalWrite(_pin_dir_motor_driver, LOW);
    if(!_is_top_sensor_used){
      run_motor(_quantity_steps_for_full_motion, _delay_at_interval_at_standard_motion);
    } else {
      while(true){
        if(!progressive_algorythm){
          if(get_average_value_from_measured_signals(_top_sensor_pin) < _threshold_top_sensor){
            run_motor(_quantity_steps_at_interval_at_standard_motion, _delay_at_interval_at_standard_motion);
          } else break;
        } else {
          for(int i = 1; i < size_arr_meas_res; i++) { arr_meas_res[i] = arr_meas_res[i-1]; }
          arr_meas_res[0] = get_average_value_from_measured_signals(_top_sensor_pin);
          res_progressive_algorythm = 0;
          for(int i = 0; i < size_arr_meas_res; i++) {
            if(arr_meas_res[i] > (_threshold_top_sensor - _error_top_sensor)){
              res_progressive_algorythm++;
            }
          }
          if(res_progressive_algorythm == size_arr_meas_res){
            break;
          } else {
            run_motor(_quantity_steps_at_interval_at_standard_motion, _delay_at_interval_at_standard_motion);
            //if(calculate_steps) quantity_full_steps += _quantity_steps_at_interval_at_standard_motion;
          }
        }
      }
      if(progressive_algorythm){ quantity_steps_at_deluxe_motion = _quantity_steps_at_deluxe_motion - 
                                                   (size_arr_meas_res * _quantity_steps_at_interval_at_standard_motion); }
      run_motor(quantity_steps_at_deluxe_motion, _delay_at_deluxe_motion);
    }
    _is_blinds_open = true;                               //  BLINDS IS OPEN
  }
}

void Blinds::run_motor(int quantity_steps, int delay_value)
{
  for(int i = 0; i < quantity_steps; i++){
    digitalWrite(_pin_step_motor_driver, HIGH);
    delayMicroseconds(delay_value);
    digitalWrite(_pin_step_motor_driver, LOW);
    delayMicroseconds(delay_value);
   }
}

int Blinds::get_value_from_measured_signal(int pin_sensor)
{
  return analogRead(pin_sensor);
}

int Blinds::get_average_value_from_measured_signals(int pin_sensor)
{
  int sum_value = 0;
  for(int i = 0; i < _measurement_iteration_quantity; i++){
    int tmp_value = 0;
    while(true){
      delay(_measurement_delay_value);
      tmp_value = analogRead(pin_sensor);
      if(tmp_value != 0) break;
    }
    sum_value += tmp_value;
  }
  return sum_value / _measurement_iteration_quantity;
}

int Blinds::get_average_value_from_values(int array_values[], int measurement_iteration_quantity)
{
  int sum_value = 0;
  for(int i = 0; i < measurement_iteration_quantity; i++){ sum_value += array_values[i]; }
  return sum_value / measurement_iteration_quantity;
}

int Blinds::get_error_value_from_values(int array_values[], int measurement_iteration_quantity, int average_value_from_array_values)
{
  double sum_value_in_double = 0.0;
  for(int i = 0; i < measurement_iteration_quantity; i++){
    sum_value_in_double += pow((float)(array_values[i] - average_value_from_array_values), (float)2);
  }
  return sqrt(sum_value_in_double / (double)measurement_iteration_quantity);
}

void Blinds::is_blinds_open_checking()
{
  if((get_average_value_from_measured_signals(_bottom_sensor_pin) > _threshold_bottom_sensor) &
  (get_average_value_from_measured_signals(_bottom_sensor_pin) < _threshold_top_sensor)){
    _is_blinds_open = true;
  } else _is_blinds_open = false;
}

boolean Blinds::is_blinds_open()
{
	return _is_blinds_open;
}

boolean Blinds::checking_level_signal(int pin_sensor)
{
  int MAX_VALUE_SIGNAL = 1023;
  int THRESHOLD_SIGNAL = 1000;
  int QUANTITY_MEASUREMENT_PROCEDURE = 50;
  int QUANTITY_MEASUREMENT_PROCEDURE_FOR_ACTION = QUANTITY_MEASUREMENT_PROCEDURE / 5;
  int DELAY_BETWEEN_MEASUREMENTS = 5;
  int quantity_signals_with_high_level_signal = 0;
  for(int i = 0; i < QUANTITY_MEASUREMENT_PROCEDURE; i++){
    delay(DELAY_BETWEEN_MEASUREMENTS);
    if(analogRead(pin_sensor) >= THRESHOLD_SIGNAL){ quantity_signals_with_high_level_signal++; }
    if(quantity_signals_with_high_level_signal > QUANTITY_MEASUREMENT_PROCEDURE_FOR_ACTION) { return true; }
  }
  return false;
}

boolean Blinds::is_blinds_open_for_calibration()
{
  boolean is_blinds_open = false;
  int QUANTITY_STEPS_MOTOR_AT_INTERVAL = 100;
  int DELAY_FROM_BLINDS_MOTION = 1000;
  boolean is_high_level_signal_in_bottom_sensor = checking_level_signal(_bottom_sensor_pin);
  boolean is_high_level_signal_in_top_sensor = checking_level_signal(_top_sensor_pin);
  if((is_high_level_signal_in_bottom_sensor) & (is_high_level_signal_in_top_sensor)) 
  { is_blinds_open = true; _is_blinds_open = true; } else {
    digitalWrite(_pin_dir_motor_driver, LOW);
    run_motor(QUANTITY_STEPS_MOTOR_AT_INTERVAL, DELAY_FROM_BLINDS_MOTION);
    _is_blinds_open = false;
  }
  return is_blinds_open;
}

void Blinds::sensors_calibration()
{
  int DELAY_BETWEEN_SETS = 100;
  int DELAY_BETWEEN_MEASUREMENTS = 10;
  int DELAY_FROM_BLINDS_MOTION = 1000;
  //
  int average_high_signal_in_bottom_sensor = 0;
  //int average_middle_signal_in_bottom_sensor = 0;
  int average_low_signal_in_bottom_sensor = 0;
  //
  int average_high_signal_in_top_sensor = 0;
  //int average_middle_signal_in_top_sensor = 0;
  int average_low_signal_in_top_sensor = 0; 
  //
  int error_high_signal_in_bottom_sensor = 0;
  //int error_middle_signal_in_bottom_sensor = 0;
  int error_low_signal_in_bottom_sensor = 0;
  //
  int error_high_signal_in_top_sensor = 0;
  //int error_middle_signal_in_top_sensor = 0;
  int error_low_signal_in_top_sensor = 0;
  //
  boolean is_blinds_open = false;
  while(!is_blinds_open){
    delay(DELAY_BETWEEN_SETS);
    is_blinds_open = is_blinds_open_for_calibration();
  }
  //
  delay(DELAY_BETWEEN_SETS);
  //
  //  MEASURING PROCEDURE
  //
  //  I
  //
  int QUANTITY_MEASUREMENT_PROCEDURE = 30;
  int array_values[QUANTITY_MEASUREMENT_PROCEDURE];   // QUANTITY_MEASUREMENT_PROCEDURE <= 32 !
  //
  for(int i = 0; i < QUANTITY_MEASUREMENT_PROCEDURE; i++){
    delay(DELAY_BETWEEN_MEASUREMENTS);
    array_values[i] = analogRead(_top_sensor_pin);
  }
  //  (...)
  average_high_signal_in_top_sensor = get_average_value_from_values(array_values, 
                                                                    QUANTITY_MEASUREMENT_PROCEDURE);
  error_high_signal_in_top_sensor = get_error_value_from_values(array_values, 
                                                                QUANTITY_MEASUREMENT_PROCEDURE, 
                                                                average_high_signal_in_top_sensor);
  //
  for(int i = 0; i < QUANTITY_MEASUREMENT_PROCEDURE; i++){
    delay(DELAY_BETWEEN_MEASUREMENTS);
    array_values[i] = analogRead(_bottom_sensor_pin);
  }
  //  (...)
  average_high_signal_in_bottom_sensor = get_average_value_from_values(array_values, 
                                                                       QUANTITY_MEASUREMENT_PROCEDURE);
  error_high_signal_in_bottom_sensor = get_error_value_from_values(array_values, 
                                                                   QUANTITY_MEASUREMENT_PROCEDURE, 
                                                                   average_high_signal_in_bottom_sensor);
  //
  run_motion_control_blinds(true,false);
  //
  delay(DELAY_BETWEEN_SETS);
  //
  //  II
  //
  for(int i = 0; i < QUANTITY_MEASUREMENT_PROCEDURE; i++){
    delay(DELAY_BETWEEN_MEASUREMENTS);
    array_values[i] = analogRead(_top_sensor_pin);
  }
  //  (...)
  average_low_signal_in_top_sensor = get_average_value_from_values(array_values, 
                                                                   QUANTITY_MEASUREMENT_PROCEDURE);
  error_low_signal_in_top_sensor = get_error_value_from_values(array_values, 
                                                               QUANTITY_MEASUREMENT_PROCEDURE, 
                                                               average_high_signal_in_top_sensor);
  //
  for(int i = 0; i < QUANTITY_MEASUREMENT_PROCEDURE; i++){
    delay(DELAY_BETWEEN_MEASUREMENTS);
    array_values[i] = analogRead(_bottom_sensor_pin);
  }
  //  (...)
  average_low_signal_in_bottom_sensor = get_average_value_from_values(array_values, 
                                                                      QUANTITY_MEASUREMENT_PROCEDURE);
  error_low_signal_in_bottom_sensor = get_error_value_from_values(array_values, 
                                                                  QUANTITY_MEASUREMENT_PROCEDURE, 
                                                                  average_high_signal_in_bottom_sensor);
  //
  //  ==========================================================
  //
  //int half_steps_from_full_steps = _quantity_steps_for_full_motion / 2;
  //digitalWrite(_pin_dir_motor_driver, LOW);
  //run_motor(half_steps_from_full_steps, DELAY_FROM_BLINDS_MOTION);
  //
  // delay(DELAY_BETWEEN_SETS);
  //
  //  III
  //
  //  (...)
  //
  //  Write results
  _threshold_top_sensor = average_high_signal_in_bottom_sensor;
  _threshold_bottom_sensor = average_high_signal_in_bottom_sensor;
  _error_top_sensor = error_high_signal_in_bottom_sensor;
  _error_bottom_sensor = error_high_signal_in_bottom_sensor;
  //
  _is_top_sensor_used = false;
  //
}
