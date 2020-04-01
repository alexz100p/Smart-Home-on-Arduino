/*
 * IntellectualWakeUp.h - Library
 */


#ifndef IntellectualWakeUp_h

#define IntellectualWakeUp_h


#include "Arduino.h"



class IntellectualWakeUp 

{
  
	public:
 
static const int _MONDAY = 1;

static const int _TUESDAY = 2;

static const int _WEDNESDAY = 3;

static const int _THURSDAY = 4;

static const int _FRIDAY = 5;

static const int _SATURDAY = 6;

static const int _SUNDAY = 7;

IntellectualWakeUp(int length_array);

void set_value(int number_of_day_of_week, int date, int hours, int minutes);

int get_value(int number_of_day_of_week);
  

	private:
/*
 * Lamps                 1
 * Blinds                2
 * Relays                3
 * Heat Floor            4
 * Doors		 5
*/
int _date;

int _arr_monday[];

int _arr_tuesday[];

int _arr_wednesday[];

int _arr_thursday[];

int _arr_friday[];

int _arr_saturday[];

int _arr_sunday[];

int _full_length;

int _current_full_length;

int _length_array;

};


#endif
