/*
	IntellectualWakeUp.cpp - Library
*/

#include "Arduino.h"
#include "IntellectualWakeUp.h"
 

IntellectualWakeUp::IntellectualWakeUp(int length_array)
{
  _length_array = length_array;
  _full_length = length_array * 7;
  _current_full_length = 0;
  _date = -1;
  //
	_arr_monday[length_array];
  _arr_tuesday[length_array];
  _arr_wednesday[length_array];
  _arr_thursday[length_array];
  _arr_friday[length_array];
  _arr_saturday[length_array];
  _arr_sunday[length_array];
}

void IntellectualWakeUp::set_value(int number_of_day_of_week, int date, int hours, int minutes)
{
  if(date != _date){
    int val = (hours * 60) + minutes;
    if((_current_full_length + 1) == _full_length){
      for(int i = 1; i < _length_array; i++){
        _arr_monday[0] += _arr_monday[i];
        _arr_tuesday[0] += _arr_tuesday[i];
        _arr_wednesday[0] += _arr_wednesday[i];
        _arr_thursday[0] += _arr_thursday[i];
        _arr_friday[0] += _arr_friday[i];
        _arr_saturday[0] += _arr_saturday[i];
        _arr_sunday[0] += _arr_sunday[i];
        //
        _arr_monday[i] = 0;
        _arr_tuesday[i] = 0;
        _arr_wednesday[i] = 0;
        _arr_thursday[i] = 0;
        _arr_friday[i] = 0;
        _arr_saturday[i] = 0;
        _arr_sunday[i] = 0;
      }
      _arr_monday[0] /= _length_array;
      _arr_tuesday[0] /= _length_array;
      _arr_wednesday[0] /= _length_array;
      _arr_thursday[0] /= _length_array;
      _arr_friday[0] /= _length_array;
      _arr_saturday[0] /= _length_array;
      _arr_sunday[0] /= _length_array;
      _current_full_length = 7;
    }
    //
    _date = date;
    _current_full_length++;
    int it = _current_full_length / 7;
    //
    switch(number_of_day_of_week){
      case _MONDAY: _arr_monday[it] = val;
        break;
      case _TUESDAY: _arr_tuesday[it] = val;
        break;
      case _WEDNESDAY: _arr_wednesday[it] = val;
        break;
      case _THURSDAY: _arr_thursday[it] = val;
        break;
      case _FRIDAY: _arr_friday[it] = val;
        break;
      case _SATURDAY: _arr_saturday[it] = val;
        break;
      case _SUNDAY: _arr_sunday[it] = val;
        break;
      default:
        break;
    }
  }
  //
}

int IntellectualWakeUp::get_value(int number_of_day_of_week)
{
  int tmp_res;
  int it = _current_full_length / 7;
  if(_current_full_length >= 7){
    for(int i = 0; i < it; i++){
      switch(number_of_day_of_week){
        case _MONDAY: tmp_res += _arr_monday[i];
          break;
        case _TUESDAY: tmp_res += _arr_tuesday[i];
          break;
        case _WEDNESDAY: tmp_res += _arr_wednesday[i];
          break;
        case _THURSDAY: tmp_res += _arr_thursday[i];
          break;
        case _FRIDAY: tmp_res += _arr_friday[i];
          break;
        case _SATURDAY: tmp_res += _arr_saturday[i];
          break;
        case _SUNDAY: tmp_res += _arr_sunday[i];
          break;
        default:
          break;
      }
    }
  }  
  return tmp_res / it;
}

