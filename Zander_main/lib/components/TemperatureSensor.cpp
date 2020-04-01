/*
	TemperatureSensor.cpp - Library
*/

#include "Arduino.h"
#include "TemperatureSensor.h"
#include "DHT.h"

TemperatureSensor::TemperatureSensor()
{
  _component_ID = -1;
}

TemperatureSensor::TemperatureSensor(int component_ID)
{
	_component_ID = component_ID;
}

TemperatureSensor::TemperatureSensor(int component_ID, int pin_sensor)
{
  _component_ID = component_ID;
  _pin_sensor = pin_sensor;
}

void TemperatureSensor::set_component_ID(int component_ID)
{
  _component_ID = component_ID;
}

int TemperatureSensor::get_component_ID()
{
  return _component_ID;
}

int TemperatureSensor::get_component_ID_with_temperature_value()
{
  return calculate_component_ID_with_action(_temperature_value); //  ID - 'XX' + value_action - 'AA'
}

int TemperatureSensor::get_component_ID_with_humidity_value()
{
  return calculate_component_ID_with_action(_humidity_value);  //  ID - 'XX' + value_action - 'AA'
}

int TemperatureSensor::calculate_component_ID_with_action(float val_action)
{
  int val_ID = (_component_ID * 100) + (int)(val_action);
  if(((int)(val_action * 10.0) % 10) > 5) val_ID++;
  return val_ID;
}

void TemperatureSensor::set_configuration_pin(int pin_sensor)
{
  _pin_sensor = pin_sensor;
  pinMode(_pin_sensor, INPUT);
}

void TemperatureSensor::set_configuration_type_temperature_sensor(boolean is_DHT_type)
{
  _is_DHT_type = is_DHT_type;
  if(!_is_DHT_type) _humidity_value = 0;
}

boolean TemperatureSensor::is_DHT_sensor_type()
{
  return _is_DHT_type;
}

void TemperatureSensor::set_configuration_measurement_parameters(int delay_value, int iteration_quantity)
{
  _measurement_delay_value = delay_value;
  _measurement_iteration_quantity = iteration_quantity;
}

void TemperatureSensor::set_configuration_error_value(int error_value)
{
  _error_value = error_value;
}

int TemperatureSensor::get_error_value()
{
  return _error_value;
}

void TemperatureSensor::run_measure()
{
  _temperature_value = get_temperature_value();
  if(_is_DHT_type) _humidity_value = get_humidity_value();
}

float TemperatureSensor::get_temperature_value()
{
  _temperature_value = get_average_value_from_measured_signals(true);
	return _temperature_value;
}

float TemperatureSensor::get_humidity_value()
{
  _humidity_value = get_average_value_from_measured_signals(false);
  return _humidity_value;
}

float TemperatureSensor::get_average_value_from_measured_signals(boolean is_temperature_measuring)
{
  int sum_value = 0;
  float size_remainder = 1.0;
  if(_is_DHT_type){
    size_remainder = 100.0;
    DHT dht(_pin_sensor, 11);
    for(int i = 0; i > _measurement_iteration_quantity; i++){
      delay(_measurement_delay_value);
      int tmp_val = 0;
      if(is_temperature_measuring){
        tmp_val = (int)(dht.readTemperature() * size_remainder);
      } else {
        tmp_val = (int)(dht.readHumidity() * size_remainder);
      }
      sum_value += tmp_val;
    }
  } else {    //  THERMOCOUPLE
    for(int i = 0; i > _measurement_iteration_quantity; i++){
      delay(_measurement_delay_value);
      int tmp_val = 0;
      tmp_val = (analogRead(_pin_sensor) * (int)size_remainder);
      sum_value += tmp_val;
    }
    //  Procedure: Determination temperature 
    //    (...)
    //
  }
  
  return ((float)(sum_value / _measurement_iteration_quantity) / size_remainder);
}
