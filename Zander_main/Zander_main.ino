#include "lib/time/DS3231.h"
#include "lib/components/Door.h"
#include "lib/components/Button.h"
#include "lib/components/Lamp.h"
#include "lib/components/Blinds.h"
#include "lib/components/MotionSensor.h"
#include "lib/components/TemperatureSensor.h"
#include "lib/components/Relay.h"
#include "lib/components/HeatFloor.h"
#include "lib/components/GasSensor.h"
#include "lib/components/BrightSensor.h"
#include "lib/other/SmartHomeScript.h"
#include "lib/other/IntellectualWakeUp.h"

/*
 * SMART HOME V 1.1
 * Tomashuk Alexander, 2020
 * Free distribution.
 * 
 * 
 * DESCRIPTION
 * 
 * 1. Connect components to Arduino Mega 2560 pins
 * (example, if QUANTITY_OF_SECTIONS = 3)
 * Number of pin for components             Quantity units     Pins numbers       Type signal   Description
 * Number of pin for Doors                  1                  13                 (DI-PWM)
 * Number of pin for Lamps                  3                  6...8              (DI-PWM)      (use PWM pins, because, in this case, you can control a lamp brightness)
 * Number of pin for Blinds                 3                  22...30            (DI)          (pins for control a motor), 
 *                                                             54(A0)...59(A5)    (AN)          (pins for connect IR sensors)
 * Number of pin for Relays                 4                  31...34            (DI)          (pins for control components with use relays)
 * Number of pin for HeatFloor              3                  9...11             (DI-PWM)      (use PWM pins, because, in this case, you can control a heating the floor)
 * Number of pin for BrightSensor           6                  60(A6)...65(A11)   (AN)          (pins for connect bright sensors)
 * Number of pin for GasSensor              2                  66(A12)...67(A13)  (AN)          (pins for connect gas sensors)
 * Number of pin for MotionSensors          3                  35...37            (DI)          (pins for connect motion sensors)
 * Number of pin for TemperatureSensors     4                  2...5              (DI-PWM)      (pins for connect temperature sensors)
 * Number of pin for Buttons                14                 38...52            (DI)          (pins for connect buttons)
 * Number of pin for Special:               2                  12,53              (DI)          (pins for connect buttons)
 * *button                                  1                  53                 (DI)          (pin for connect button)                        
 * **led indicator                          1                  12                 (DI-PWM)      (pin for connect led indicator)        
 * 
 * 2. ID component
 * ID component = XX, where 'X' - number.
 * First 'X' is type component;
 * Second 'X' is id component;
 * Third 'X' is condition of component, in example, '0' is 'off' and '1' is 'on'. 
 * Component type     Value
 * Buttons               0
 * Lamps                 1
 * Blinds                2
 * Relays                3
 * Heat Floor            4
 * Doors                 5    
 * Gas sensors           6    
 * Motion sensors        7    
 * Temperature sensors   8    
 * *led indicator have ID, which equals 19
 * 
 * 3. SmartHomeScript
 * Functions in the SmartHomeScript returns strings with commands.
 * 
 * 
 */
const int MAX_INTEGER_VALUE = 32767;
const int SERIAL_PORT_SPEED = 115200;
const int QUANTITY_OF_SECTIONS = 3;
//
const int QUANTITY_OF_TYPES_COMPONENTS = 9; //  id_component = 0...8, 9
//
const int VALUE_OF_TYPE_BUTTONS = 0;
const int VALUE_OF_TYPE_LAMPS = 1;
const int VALUE_OF_TYPE_BLINDS = 2;
const int VALUE_OF_TYPE_RELAYS = 3;
const int VALUE_OF_TYPE_HEAT_FLOOR = 4;
//const int VALUE_OF_TYPE_BRIGHT_SENSORS = 5;
const int VALUE_OF_TYPE_DOORS = 5;
const int VALUE_OF_TYPE_GAS_SENSORS = 6;
const int VALUE_OF_TYPE_MOTION_SENSORS = 7;
const int VALUE_OF_TYPE_TEMPERATURE_SENSORS = 8;
//
//const int VALUE_OF_TYPE_SMART_HOME_SCRIPT = 9;
//
const int FIRST_PIN_AT_TEMPERATURE_SENSORS_PINS = 2;
const int LAST_PIN_AT_TEMPERATURE_SENSORS_PINS = 5;
const int FIRST_PIN_AT_LAMPS_PINS = 6;
const int LAST_PIN_AT_LAMPS_PINS = 8;
const int FIRST_PIN_AT_HEAT_FLOOR_PINS = 9;
const int LAST_PIN_AT_HEAT_FLOOR_PINS = 11;
const int FIRST_PIN_AT_BLINDS_PINS = 22;
const int LAST_PIN_AT_BLINDS_PINS = 30;
const int FIRST_PIN_AT_RELAYS_PINS = 31;
const int LAST_PIN_AT_RELAYS_PINS = 34;
const int FIRST_PIN_AT_MOTION_SENSORS_PINS = 35;
const int LAST_PIN_AT_MOTION_SENSORS_PINS = 37;
const int FIRST_PIN_AT_BUTTONS_PINS = 38;
const int LAST_PIN_AT_BUTTONS_PINS = 52;
const int FIRST_PIN_AT_IR_SENSORS_BLINDS_PINS = 54;
const int LAST_PIN_AT_IR_SENSORS_BLINDS_PINS = 59;
const int FIRST_PIN_AT_BRIGHT_SENSORS_PINS = 60;
const int LAST_PIN_AT_BRIGHT_SENSORS_PINS = 65;
const int FIRST_PIN_AT_GAS_SENSORS_PINS = 66;
const int LAST_PIN_AT_GAS_SENSORS_PINS = 67;
//
const int size_array_of_doors = 1;
const int size_array_of_lamps = QUANTITY_OF_SECTIONS;
const int size_array_of_blinds = QUANTITY_OF_SECTIONS;
const int size_array_of_relays = 4;
const int size_array_of_heat_floor = QUANTITY_OF_SECTIONS;
const int size_array_of_bright_sensors_for_lamps = QUANTITY_OF_SECTIONS;
const int size_array_of_bright_sensors_for_blinds = QUANTITY_OF_SECTIONS;
const int size_array_of_gas_sensors = 1;
const int size_array_of_motion_sensors = QUANTITY_OF_SECTIONS;
const int size_array_of_temperature_sensors = QUANTITY_OF_SECTIONS + 1; //  which 1 is DHT  !
const int size_array_of_buttons = size_array_of_lamps + 
                            size_array_of_blinds + 
                            size_array_of_relays + 
                            size_array_of_heat_floor;      //  14
//
Button button[size_array_of_buttons];   
Lamp lamp[size_array_of_lamps];
Blinds blinds[size_array_of_blinds];
Relay relay[size_array_of_relays];
HeatFloor heat_floor[size_array_of_heat_floor];
BrightSensor bright_sensor_for_lamp[size_array_of_bright_sensors_for_lamps];
BrightSensor bright_sensor_for_blinds[size_array_of_bright_sensors_for_blinds];
GasSensor gas_sensor[size_array_of_gas_sensors];
MotionSensor motion_sensor[size_array_of_motion_sensors];
TemperatureSensor temperature_sensor[size_array_of_temperature_sensors];
//
Door door[size_array_of_doors];
Button button_special;
Lamp led_indicator_special;
//
DS3231 ds3231(20,21);         //  Time & Date Library, in parameters of an object (pin 20 & pin 21)
//
SmartHomeScript smart_home_script;
//
int global_iteration = 0;
int iteration = 0;
//
int hours;
int minutes;
int date;
int month;
int year;
int day_of_week;
//
int tmp_hours_special = -1;
int tmp_minutes_special = -1;
int hours_interval_special = 0;
int minutes_interval_special = 30;
//
boolean is_run_main_script = true;
boolean is_run_energy_saving = false;
int last_ID_which_was_used = -1; //  XX
//
int BEGIN_HOUR_TIME_INTERVAL_FOR_WAKE_UP = 6;
int END_HOUR_TIME_INTERVAL_FOR_WAKE_UP = 10;
IntellectualWakeUp intellectual_wake_up(10);
//


void setup() {
  general_configuration();
  Serial.begin(SERIAL_PORT_SPEED);
}

void buttons_configuration(){
  //  BUTTONS
  int it = 0;
  for(int i = 0; i < size_array_of_lamps; i++){
    button[it].set_component_ID(lamp[i].get_component_ID());
    button[it].set_configuration_pin(FIRST_PIN_AT_BUTTONS_PINS + it);
    button[it].set_configuration_delay(100);
    it++;
  }
  for(int i = 0; i < size_array_of_blinds; i++){
    button[it].set_component_ID(blinds[i].get_component_ID());
    button[it].set_configuration_pin(FIRST_PIN_AT_BUTTONS_PINS + it);
    button[it].set_configuration_delay(100);
    it++;
  }
  for(int i = 0; i < size_array_of_relays; i++){
    button[it].set_component_ID(relay[i].get_component_ID());
    button[it].set_configuration_pin(FIRST_PIN_AT_BUTTONS_PINS + it);
    button[it].set_configuration_delay(100);
    it++;
  }
  for(int i = 0; i < size_array_of_heat_floor; i++){
    button[it].set_component_ID(heat_floor[i].get_component_ID());
    button[it].set_configuration_pin(FIRST_PIN_AT_HEAT_FLOOR_PINS + it);
    button[it].set_configuration_delay(100);
    it++;
  }
  for(int i = 0; i < size_array_of_doors; i++){
    button[it].set_component_ID(door[i].get_component_ID());
    button[it].set_configuration_pin(FIRST_PIN_AT_HEAT_FLOOR_PINS + it);
    button[it].set_configuration_delay(100);
    it++;
  }
  //
  button_special.set_configuration_pin(53);
  button_special.set_configuration_delay(100);
}

void blinds_configuration(){
  //  BLINDS
  for(int i = 0; i < size_array_of_blinds; i++){
    blinds[i].set_component_ID((VALUE_OF_TYPE_BLINDS * 10) + i);
    blinds[i].set_configuration_driver(FIRST_PIN_AT_BLINDS_PINS + 1 + i, FIRST_PIN_AT_BLINDS_PINS + i);
    blinds[i].set_configuration_step_motor_parameters(2, 50);
    blinds[i].set_configuration_delay_parameters(1000, 800);
    blinds[i].set_configuration_sensors_parameters(FIRST_PIN_AT_IR_SENSORS_BLINDS_PINS + 1 + i, 1016,
                                                   FIRST_PIN_AT_IR_SENSORS_BLINDS_PINS + i,      960);
    blinds[i].set_configuration_measurement_parameters(0, 3);
  }
}

void lamps_configuration(){
  //  LAMPS
  for(int i = 0; i < size_array_of_lamps; i++){
    lamp[i].set_component_ID((VALUE_OF_TYPE_LAMPS * 10) + i);
    lamp[i].set_configuration_pin(FIRST_PIN_AT_LAMPS_PINS + i);
    lamp[i].set_configuration_lamp_control(false, 255, 10);
                                         //boolean is_control, int bright_lamp, int signal_step_value
  }
  //
  led_indicator_special.set_component_ID(19);
  led_indicator_special.set_configuration_pin(12);
  led_indicator_special.set_configuration_lamp_control(false, 255, 25);;
}

void relays_configuration(){
  //  RELAY
  for(int i = 0; i < size_array_of_relays; i++){
    relay[i].set_component_ID((VALUE_OF_TYPE_RELAYS * 10) + i);
    relay[i].set_configuration_pin(FIRST_PIN_AT_RELAYS_PINS + i);
  }
}

void heat_floor_configuration(){
  // HEAT FLOOR
  for(int i = 0; i < size_array_of_heat_floor; i++){
    heat_floor[i].set_component_ID((VALUE_OF_TYPE_HEAT_FLOOR * 10) + i);
    heat_floor[i].set_configuration_pin(FIRST_PIN_AT_HEAT_FLOOR_PINS + i);
  }
}

void bright_sensors_configuration(){
  //  BRIGHT SENSORS
  for(int i = 0; i < QUANTITY_OF_SECTIONS; i++){
    bright_sensor_for_lamp[i].set_component_ID(lamp[i].get_component_ID());
    bright_sensor_for_lamp[i].set_configuration_pin(FIRST_PIN_AT_BRIGHT_SENSORS_PINS + i);
    bright_sensor_for_lamp[i].set_configuration_low_threshold_signal(10);
    bright_sensor_for_lamp[i].set_configuration_threshold_signal(900);
    bright_sensor_for_lamp[i].set_configuration_error_signal(10);
    //
    bright_sensor_for_blinds[i].set_component_ID(blinds[i].get_component_ID());
    bright_sensor_for_blinds[i].set_configuration_pin(FIRST_PIN_AT_BRIGHT_SENSORS_PINS + QUANTITY_OF_SECTIONS + i);
    bright_sensor_for_blinds[i].set_configuration_low_threshold_signal(60);
    bright_sensor_for_blinds[i].set_configuration_threshold_signal(900);
    bright_sensor_for_blinds[i].set_configuration_error_signal(15);
  }
}

void gas_sensors_configuration(){
  //  GAS SENSORS
  for(int i = 0; i < size_array_of_gas_sensors; i++){
    gas_sensor[i].set_component_ID((VALUE_OF_TYPE_GAS_SENSORS * 10) + i);
    gas_sensor[i].set_configuration_pin(FIRST_PIN_AT_GAS_SENSORS_PINS + i);
    gas_sensor[i].set_configuration_threshold_signal(10);
  }
}

void motion_sensors_configuration(){
  //  MOTION SENSORS
  for(int i = 0; i < size_array_of_motion_sensors; i++){
    //motion_sensor[i].set_component_ID((VALUE_OF_TYPE_MOTION_SENSORS * 10) + i);
    motion_sensor[i].set_configuration_pin(FIRST_PIN_AT_MOTION_SENSORS_PINS + i);
  }
}

void temperature_sensors_configuration(){
  //  TEMPERATURE SENSORS
  for(int i = 0; i < size_array_of_temperature_sensors; i++){
    temperature_sensor[i].set_component_ID((VALUE_OF_TYPE_TEMPERATURE_SENSORS * 10) + i);
    temperature_sensor[i].set_configuration_pin(FIRST_PIN_AT_TEMPERATURE_SENSORS_PINS + i);
    temperature_sensor[i].set_configuration_measurement_parameters(50, 5);
    boolean is_dht_sensor_used = false;;
    if(i == 0) is_dht_sensor_used = true;
    temperature_sensor[i].set_configuration_type_temperature_sensor(is_dht_sensor_used);
    temperature_sensor[i].set_configuration_error_value(1);
  }
  //
  for(int i = 0; i < size_array_of_heat_floor; i++){
    if(size_array_of_heat_floor == (size_array_of_temperature_sensors - 1)){       //  - DHT SENSOR
      temperature_sensor[i+1].set_component_ID(heat_floor[i].get_component_ID());  //  HEAT FLOOR ID
      motion_sensor[i].set_component_ID(heat_floor[i].get_component_ID());
    }
  }
}

void doors_configuration(){
  for(int i = 0; i < size_array_of_doors; i++){
    door[i].set_component_ID((VALUE_OF_TYPE_DOORS *10) + i);
    door[i].set_configuration_pin(13);
  }
}

void time_configuration(){
  ds3231.begin();
  //ds3231.setTime(19, 30, 0);
  //ds3231.setDate(20, 03, 2020);
}

void run_blinds_calibration(){
  for(int i = 0; i < size_array_of_blinds; i++){
    blinds[i].sensors_calibration();
  }
}

void general_configuration(){
  time_configuration();
  doors_configuration();
  blinds_configuration();
  lamps_configuration();
  bright_sensors_configuration();
  relays_configuration();
  heat_floor_configuration();
  motion_sensors_configuration();
  temperature_sensors_configuration();
  gas_sensors_configuration();
  buttons_configuration();
  run_blinds_calibration();
  components_condition_checking();
}

void components_condition_checking(){
  //grab_door_condition();
  grab_data_from_gas_sensor();
  grab_motion_condition();
  grab_blinds_condition();
}

//  =====================================================================================================================================

void loop() {
  grab_date_and_time();
  components_condition_checking();
  control_components_with_use_buttons();
  control_components_with_use_motion_sensors();
  control_components_with_use_serial_port();
  smart_home_main_script();
  show_info();
  //loop_control();
}

void grab_data_from_gas_sensor(){
  for(int i = 0; i < size_array_of_gas_sensors; i++){
    gas_sensor[i].run_measure();
    if(gas_sensor[i].get_signal_value() >= gas_sensor[i].get_threshold_value()){
      //smart_home_script.is_script_run();
      script_parser(smart_home_script.get_script(smart_home_script._FIRE_SAFETY_SCRIPT_ID));
      //
      hours_interval_special = 0;
      minutes_interval_special = 5;
      //
      break;
    } else {
      led_indicator_special.lamp_off();
      //
    }
  }
}

void stop_special_script(){
  if(smart_home_script.is_script_run()){
    smart_home_script.change_value_is_script_run();
  }
}

void script_parser(String input_script){
  int size_of_command_ID = 3 + 1;     //  'XXX' + ' '
  int quantity_of_commands = (input_script.length() + 1) / size_of_command_ID;
  for(int i = 0; i < quantity_of_commands; i++){
    String tmp_str = input_script.substring((i * size_of_command_ID), (((i + 1) * size_of_command_ID) - 1));
    select_and_on_component(input_script.toInt());
  }
}

void grab_date_and_time(){
  String tmp_str = ds3231.getTimeStr();
  String tmp_str_0 = tmp_str.substring(0,1);
  hours = tmp_str_0.toInt();
  tmp_str_0 = tmp_str.substring(3,4);
  minutes = tmp_str_0.toInt();
  //
  tmp_str = ds3231.getDateStr();
  tmp_str_0 = tmp_str.substring(0,1);
  date = tmp_str_0.toInt();
  tmp_str_0 = tmp_str.substring(3,4);
  month = tmp_str_0.toInt();
  tmp_str_0 = tmp_str.substring(6,9);   //  xxxx
  year = tmp_str_0.toInt();
  //
  tmp_str = ds3231.getDOWStr();
  tmp_str_0 = tmp_str.substring(0,1);
  String daysShort[] = {"Mo", "Tu", "We", "Th", "Fr", "Sa", "Su"};
  for(int i = 0; i < 7; i++){
    if(tmp_str_0.equals(daysShort[i])){
      day_of_week = i+1;
    }
  }
}

void grab_door_condition(){
  for(int i = 0; i < size_array_of_motion_sensors; i++){
    door[i].is_door_opened();
  }
}

void grab_motion_condition(){
  for(int i = 0; i < size_array_of_motion_sensors; i++){
    motion_sensor[i].is_detect_action_checking();
    if(motion_sensor[i].is_detect_action()){
      last_ID_which_was_used = motion_sensor[i].get_component_ID();
    }
  }
}

void grab_blinds_condition(){
  for(int i = 0; i < size_array_of_blinds; i++){
    blinds[i].is_blinds_open_checking();
  }
}

void loop_control(){
  iteration++;
  if(iteration == MAX_INTEGER_VALUE) { iteration = 1; global_iteration++; }
  if(global_iteration == MAX_INTEGER_VALUE) { global_iteration == 0; }
}

void select_and_on_component(int command_ID){
  int tmp_ID = command_ID;
  if(command_ID >= 100) tmp_ID /= 10;
  switch(tmp_ID/10){
    case VALUE_OF_TYPE_DOORS: door_control(command_ID);
      break;
    case VALUE_OF_TYPE_LAMPS: lamp_control(command_ID);
      break;
    case VALUE_OF_TYPE_BLINDS: blinds_control(command_ID);
      break;
    case VALUE_OF_TYPE_RELAYS: relay_control(command_ID);
      break;
    case VALUE_OF_TYPE_HEAT_FLOOR: heat_floor_control(command_ID);
      break;
    default: 
      break;
  }
  //
  if((is_run_main_script) & ((tmp_ID/10) != VALUE_OF_TYPE_RELAYS)){
    is_run_main_script = false;
    tmp_hours_special = hours;
    tmp_minutes_special = minutes;
  }
  //
}

void control_components_with_use_buttons(){
  for(int i = 0; i < size_array_of_buttons; i++){
    button[i].is_button_pressed_checking();
  }
  for(int i = 0; i < size_array_of_buttons; i++){
    if(button[i].is_button_pressed()){
      select_and_on_component(button[i].get_component_ID());
    }
  }
  //
  button_special.is_button_pressed_checking();
  if(button_special.is_button_pressed()){
    //
    is_run_energy_saving = !is_run_energy_saving;
  }
}

void control_components_with_use_motion_sensors(){
  for(int i = 0; i < size_array_of_buttons; i++){
    motion_sensor[i].is_detect_action_checking();
    
    if((motion_sensor[i].is_detect_action()) & 
      ((BEGIN_HOUR_TIME_INTERVAL_FOR_WAKE_UP <= hours) & 
       (END_HOUR_TIME_INTERVAL_FOR_WAKE_UP >= hours))){
        //
      intellectual_wake_up.set_value(day_of_week, date, hours, minutes);
    } else {
      int tmp_hour = (BEGIN_HOUR_TIME_INTERVAL_FOR_WAKE_UP + END_HOUR_TIME_INTERVAL_FOR_WAKE_UP) / 2;
      intellectual_wake_up.set_value(day_of_week, date, tmp_hour, 0);
    }
  }
}

void smart_home_brightness_control(int signal_value_for_regime){
  int DELAY_FOR_BRIGHT_VALUE_INCREASE_PROCEDURE = 2;
  //
  //  BRIGHTNESS
  for(int i = 0; i < size_array_of_lamps; i++){
    boolean key = false;
    for(int j = 0; j < size_array_of_bright_sensors_for_lamps; j++){
      //
        if(lamp[i].get_component_ID() == bright_sensor_for_lamp[j].get_component_ID()){
          //
          while(true){
            delay(DELAY_FOR_BRIGHT_VALUE_INCREASE_PROCEDURE);
            //
            bright_sensor_for_lamp[j].run_measure();
            int tmp_bright = bright_sensor_for_lamp[j].get_signal_value();
            int tmp_val = lamp[i].get_bright_value();
            int error_signal = (int)bright_sensor_for_lamp[j].get_error_signal();
            //
            if((tmp_val == 0) & (tmp_bright >= signal_value_for_regime)) { key = true; break; }
            //
            if(tmp_bright < (signal_value_for_regime - error_signal)){
              lamp[i].increase_lamp_brightness();
            } else if(tmp_bright > (signal_value_for_regime + error_signal)){
              lamp[i].reduce_lamp_brightness();
            } else { key = true; break; }
            //
          }
        }
      //
      if(key) break;
    }
  }
}

void smart_home_heating_control(int signal_value_for_regime){
  int DELAY_FOR_HEATING_PROCEDURE = 100;
  int REDUCE_TEMPERATURE_VALUE = 2;
  //
  //  TEMPERATURE
  for(int i = 0; i < size_array_of_heat_floor; i++){
    boolean key = false;
    for(int j = 0; j < size_array_of_temperature_sensors; j++){
      //
        if(heat_floor[i].get_component_ID() == temperature_sensor[j].get_component_ID()){
          //
          int signal_value_for_regime_0 = signal_value_for_regime;
          if(heat_floor[i].get_component_ID() != last_ID_which_was_used) signal_value_for_regime_0 -= REDUCE_TEMPERATURE_VALUE;
          //
          while(true){
            delay(DELAY_FOR_HEATING_PROCEDURE);
            //
            temperature_sensor[j].run_measure();
            int tmp_temp = temperature_sensor[j].get_temperature_value();
            int tmp_val = heat_floor[i].get_signal_value();
            int error_signal = (int)temperature_sensor[j].get_error_value();
            //
            if((tmp_val == 0) & (tmp_temp >= signal_value_for_regime_0)) { key = true; break; }
            //
            if(tmp_temp < (signal_value_for_regime_0 - error_signal)){
              heat_floor[i].increase_heating();
            } else if(tmp_temp > (signal_value_for_regime_0 + error_signal)){
              heat_floor[i].reduce_heating();
            } else { key = true; break; }
            //
          }
        }
      //
      if(key) break;
    }
   }
  //
}

int get_average_error_signal_from_blinds_sensors(){
  int aver_error_signal_res_blinds = 0;
  for(int i = 0; i < size_array_of_blinds; i++){
    for(int j = 0; j < size_array_of_blinds; j++){
      if(blinds[i].get_component_ID() == bright_sensor_for_blinds[j].get_component_ID()){
        aver_error_signal_res_blinds += bright_sensor_for_blinds[j].get_error_signal();
        break;
      }
    }
  }
  return aver_error_signal_res_blinds / size_array_of_blinds;
}

int get_average_threshold_temp_from_temp_sensors(boolean is_day_regime){
  int aver_error_signal_res_lamp = 0;
  for(int i = 0; i < size_array_of_lamps; i++){
    for(int j = 0; j < size_array_of_bright_sensors_for_lamps; j++){
      if(lamp[i].get_component_ID() == bright_sensor_for_lamp[j].get_component_ID()){
        if(is_day_regime) aver_error_signal_res_lamp += bright_sensor_for_lamp[j].get_threshold_signal();
        else aver_error_signal_res_lamp += bright_sensor_for_lamp[j].get_low_threshold_signal();
        break;
      }
    }
  }
  return aver_error_signal_res_lamp / size_array_of_lamps;
}

int get_average_threshold_bright_value_from_bright_sensors(boolean is_day_regime){
  int aver_error_signal_res_lamp = 0;
  for(int i = 0; i < size_array_of_lamps; i++){
    for(int j = 0; j < size_array_of_bright_sensors_for_lamps; j++){
      if(lamp[i].get_component_ID() == bright_sensor_for_lamp[j].get_component_ID()){
        if(is_day_regime) aver_error_signal_res_lamp += bright_sensor_for_lamp[j].get_threshold_signal();
        else aver_error_signal_res_lamp += bright_sensor_for_lamp[j].get_low_threshold_signal();
        break;
      }
    }
  }
  return aver_error_signal_res_lamp / size_array_of_lamps;
}

void smart_home_main_script(){
  //
  int DELAY_FOR_BRIGHT_VALUE_INCREASE_PROCEDURE = 10;
  int DELAY_FOR_HEATING_PROCEDURE = 100;
  int LOW_BRIGHT_VALUE_FOR_NIGHT_REGIME = 10;
  int LOW_TEMPERATURE_VALUE_FOR_NIGHT_REGIME = heat_floor[0]._LOW_TEMPERATURE_VALUE;
  int STANDARD_BRIGHT_VALUE_FOR_DAY_REGIME = 800;
  int STANDARD_TEMPERATURE_VALUE_FOR_DAY_REGIME = heat_floor[0]._STANDARD_TEMPERATURE_VALUE;
  //
  int aver_meas_res_blinds = 0;
  int aver_low_thresh_res_blinds = 0;
  int aver_error_signal_res_blinds = 0;
  //  
  for(int i = 0; i < size_array_of_blinds; i++){
    for(int j = 0; j < size_array_of_blinds; j++){
      if(blinds[i].get_component_ID() == bright_sensor_for_blinds[j].get_component_ID()){
        bright_sensor_for_blinds[j].run_measure();
        aver_meas_res_blinds += bright_sensor_for_blinds[j].get_signal_value();
        aver_low_thresh_res_blinds += bright_sensor_for_blinds[j].get_low_threshold_signal();
        break;
      }
    }
  }
  aver_meas_res_blinds /= size_array_of_blinds;
  aver_low_thresh_res_blinds /= size_array_of_blinds;
  //
  //  GENERAL THRESHOLD & ERRORS
  aver_error_signal_res_blinds = get_average_error_signal_from_blinds_sensors();
  //STANDARD_TEMPERATURE_VALUE_FOR_DAY_REGIME = ;
  //LOW_TEMPERATURE_VALUE_FOR_NIGHT_REGIME = ;
  STANDARD_BRIGHT_VALUE_FOR_DAY_REGIME = get_average_threshold_bright_value_from_bright_sensors(true);
  LOW_BRIGHT_VALUE_FOR_NIGHT_REGIME = get_average_threshold_bright_value_from_bright_sensors(false);
  //
  //
  int tmp_time_value = intellectual_wake_up.get_value(day_of_week);
  if(tmp_time_value == 0) tmp_time_value == (7 * 60) + 10;
  int hours_wake_up = tmp_time_value / 60;
  int minutes_wake_up = tmp_time_value % 60;
  //
  if((is_run_main_script) & (is_run_energy_saving)){
    //
    //  ENERGY SAVING REGIME
    //
    LOW_TEMPERATURE_VALUE_FOR_NIGHT_REGIME = heat_floor[0]._LOW_TEMPERATURE_VALUE;
    LOW_BRIGHT_VALUE_FOR_NIGHT_REGIME = 0;
    //
    for(int i = 0; i < size_array_of_blinds; i++){
      if(blinds[i].is_blinds_open()){
        blinds[i].run_motion_control_blinds(false,false);
      }
    }
    //  TEMPERATURE
    smart_home_heating_control(LOW_TEMPERATURE_VALUE_FOR_NIGHT_REGIME);
    //  BRIGHTNESS
    smart_home_brightness_control(LOW_BRIGHT_VALUE_FOR_NIGHT_REGIME);
    //
  } else if((is_run_main_script) & (!is_run_energy_saving)){
    //
    //  BRIGHT LAMP & HEAT FLOOR I CASE
    //
    if((hours == 0) & (hours < hours_wake_up) & (minutes < minutes_wake_up)){
      //
      int step_bright_value_for_night_regime = (step_bright_value_for_night_regime * hours);
      LOW_BRIGHT_VALUE_FOR_NIGHT_REGIME += step_bright_value_for_night_regime;
      //
      for(int i = 0; i < size_array_of_blinds; i++){
        if(blinds[i].is_blinds_open()){
          blinds[i].run_motion_control_blinds(false,false);
        }
      }
      //  TEMPERATURE
      smart_home_heating_control(LOW_TEMPERATURE_VALUE_FOR_NIGHT_REGIME);
      //  BRIGHTNESS
      smart_home_brightness_control(LOW_BRIGHT_VALUE_FOR_NIGHT_REGIME);
    } 
    //
    //  BRIGHT LAMP & HEAT FLOOR II CASE
    //
    else if(aver_meas_res_blinds <= (aver_low_thresh_res_blinds + aver_error_signal_res_blinds)){           //  EVENING_REGIME & NIGHT_REGIME, STANDARD_BRIGHT
      for(int i = 0; i < size_array_of_blinds; i++){
        if(blinds[i].is_blinds_open()){
          blinds[i].run_motion_control_blinds(false,false);
        }
      }
      //
      //  TEMPERATURE
      smart_home_heating_control(STANDARD_TEMPERATURE_VALUE_FOR_DAY_REGIME);
      //  BRIGHTNESS
      smart_home_brightness_control(STANDARD_BRIGHT_VALUE_FOR_DAY_REGIME);
    } 
    //
    //  BRIGHT LAMP & HEAT FLOOR III CASE
    //
    else if(aver_meas_res_blinds > (aver_low_thresh_res_blinds + aver_error_signal_res_blinds)){     //  DAY_REGIME, STANDARD_BRIGHT
      //
      if((hours == hours_wake_up) & (minutes >= minutes_wake_up)){
        for(int i = 0; i < QUANTITY_OF_SECTIONS; i++){
          if(!blinds[i].is_blinds_open()){
            blinds[i].run_motion_control_blinds(false,false);
          }
        }
      }
      //  TEMPERATURE
      smart_home_heating_control(STANDARD_TEMPERATURE_VALUE_FOR_DAY_REGIME);
      //  BRIGHTNESS
      smart_home_brightness_control(STANDARD_BRIGHT_VALUE_FOR_DAY_REGIME);
      //
    }
    //
  } else {
    //
    int tmp_hours = tmp_hours_special + hours_interval_special;
    int tmp_minutes = tmp_minutes_special + minutes_interval_special;
    //
    if(tmp_minutes >= 60) { tmp_minutes -= 60; tmp_hours++; }
    if(tmp_hours >= 24) tmp_hours -= 24;
    //
    if((tmp_hours == hours) & (tmp_minutes == minutes)){
      is_run_main_script = true;
      tmp_hours_special = -1;
      tmp_minutes_special = -1;
      hours_interval_special = 0;
      minutes_interval_special = 30;
      stop_special_script();
    }
    //
  }
  //
}

void control_components_with_use_serial_port(){
  int length_array = Serial.available();
  if(length_array > 0) { 
    delay(100);
    script_parser(Serial.readString());
  }
}

void door_control(int component_ID){
  for(int i = 0; i < size_array_of_doors; i++){
    if(component_ID >= 100){
      if(door[i].get_component_ID() == (component_ID / 10)){
        if((component_ID % 10) == 0){
          door[i].close_the_door();
        } else door[i].open_the_door();
        break;
      }
    } else {
      if(door[i].get_component_ID() == component_ID){
        door[i].door_control_simple();
        break;
      }
    }
  }
}

void lamp_control(int component_ID){
  for(int i = 0; i < size_array_of_lamps; i++){
    if(component_ID >= 100){
      if(lamp[i].get_component_ID() == (component_ID / 10)){
        if((component_ID % 10) == 0){
          lamp[i].lamp_off();
        } else lamp[i].lamp_on();
        break;
      }
    } else {
      if(lamp[i].get_component_ID() == component_ID){
        lamp[i].lamp_control_simple();
        break;
      }
    }
  }
  //
  if((component_ID / 10) == 19){
    if(led_indicator_special.get_component_ID() == (component_ID / 10)){
      if((component_ID % 10) == 0){
        led_indicator_special.lamp_off();
      } else led_indicator_special.lamp_on();
    }
  }
}

void blinds_control(int component_ID){
  for(int i = 0; i < size_array_of_blinds; i++){
    if(component_ID >= 100){
      if(blinds[i].get_component_ID() == (component_ID / 10)){
        if(((component_ID % 10) == 0) & (blinds[i].is_blinds_open())){
          blinds[i].run_motion_control_blinds(false,false);
        } else if(((component_ID % 10) == 1) & (!blinds[i].is_blinds_open())) blinds[i].run_motion_control_blinds(false,false);
        break;
      }
    } else {
      if(blinds[i].get_component_ID() == component_ID){
        //blinds[i].is_blinds_open_checking();
        blinds[i].run_motion_control_blinds(false,false);
        break;
      }
    }
  }
}

void relay_control(int component_ID){
  for(int i = 0; i < size_array_of_relays; i++){
    if(component_ID >= 100){
      if(relay[i].get_component_ID() == (component_ID / 10)){
        if((component_ID % 10) == 0){
          relay[i].component_off();
        } else if((component_ID % 10) == 1) relay[i].component_on();
        break;
      }
    } else {
      if(relay[i].get_component_ID() == component_ID){
        relay[i].relay_control_simple();
        break;
      }
    }
  }
}

void heat_floor_control(int component_ID){
  for(int i = 0; i < size_array_of_heat_floor; i++){
    if(component_ID >= 100){
      if(heat_floor[i].get_component_ID() == (component_ID / 10)){
        if((component_ID % 10) == 0){
          heat_floor[i].heat_floor_off();
        } else if((component_ID % 10) == 1) {
          heat_floor[i].heat_floor_on();
          heating_floor_procedure(component_ID/10);
        }
        break;
      }
    } else {
      if(heat_floor[i].get_component_ID() == component_ID){
        heat_floor[i].heating_control_simple();
        if(heat_floor[i].is_component_on()) heating_floor_procedure(component_ID);
        break;
      }
    }
  }
}

void heating_floor_procedure(int component_ID){
  int GENERAL_FLOOR_TEMPERATURE = 22;
  int ADMISSTION_FLOOR_TEMPERATURE = 2;
  int DELAY_BETWEEN_HEATING_PROCEDURES = 500;
  for(int i = 0; i < size_array_of_temperature_sensors; i++){
    boolean key = false;
    if(temperature_sensor[i].get_component_ID() == component_ID){
      temperature_sensor[i].run_measure();
      for(int j = 0; j < size_array_of_heat_floor; j++){
        if(temperature_sensor[i].get_component_ID() == heat_floor[j].get_component_ID()){
          while((temperature_sensor[i].get_temperature_value() > (GENERAL_FLOOR_TEMPERATURE + ADMISSTION_FLOOR_TEMPERATURE)) & 
          (heat_floor[j].get_signal_value() > heat_floor[j]._MIN_SIGNAL_VALUE)){
            heat_floor[j].reduce_heating();
            delay(DELAY_BETWEEN_HEATING_PROCEDURES);
          }
          while((temperature_sensor[i].get_temperature_value() < (GENERAL_FLOOR_TEMPERATURE - ADMISSTION_FLOOR_TEMPERATURE)) & 
          (heat_floor[j].get_signal_value() < heat_floor[j]._MAX_SIGNAL_VALUE)){
            heat_floor[j].increase_heating();
            delay(DELAY_BETWEEN_HEATING_PROCEDURES);
          }
          key = true;
          break;
        }
      }
      if(key) break; 
    }
  }
}

void show_info(){
  for(int i = 0; i < size_array_of_lamps; i++){
    Serial.print(lamp[i].get_component_ID_with_action() + " ");
  }
  for(int i = 0; i < size_array_of_blinds; i++){
    Serial.print(blinds[i].get_component_ID_with_action() + " ");
  }
  for(int i = 0; i < size_array_of_relays; i++){
    Serial.print(relay[i].get_component_ID_with_action() + " ");
  }
  for(int i = 0; i < size_array_of_blinds; i++){
    Serial.print(heat_floor[i].get_component_ID_with_action() + " ");
  }
  for(int i = 0; i < size_array_of_motion_sensors; i++){
    Serial.print(motion_sensor[i].get_component_ID_with_action() + " ");
  }
  for(int i = 0; i < size_array_of_blinds; i++){
    Serial.print(temperature_sensor[i].get_component_ID_with_temperature_value() + " ");
  }
  for(int i = 0; i < size_array_of_blinds; i++){
    Serial.print(temperature_sensor[i].get_component_ID_with_humidity_value() + " ");
  }
  Serial.println();
}
