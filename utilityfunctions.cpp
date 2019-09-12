#include "stdafx.h"
#include "utilityfunctions.h"
#include<time.h>
#include <iostream>
#include "CalendarComponent.h"

// this function advances a tm struct ahead by some number of days
tm addDays(tm current, int numDays) {
	// convert current tm to time_t (stores number of seconds since 1/1/1970)
	time_t t = mktime(&current);
	int secondsToAdd = numDays * 60 * 60 * 24;
	t += secondsToAdd;
	// convert back to a tm struct, stored in local time
	tm temp;
	localtime_s(&temp, &t);
	return temp;
}

int read_int(std::istream & c) {
	std::string test;
	bool pureNumeric = true;
	c >> test;
	for (unsigned int i = 0; i < test.length(); ++i) {
		if (!isdigit(test[i])) {
			pureNumeric = false;
		}
	}
	if (pureNumeric == false) {
		return -1;
	}
	else {
		return stoi(test);
	}
}