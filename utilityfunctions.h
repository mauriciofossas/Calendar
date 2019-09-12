#pragma once

#include<ctime>
#include <istream>
#include <string>
/*
Author: Jon Shidal
Purpose: declare functions for dealing with tm structs, particularly for advancing the date/time represented by a particular number of days.
*/

tm addDays(tm current, int numDays);

//added for reading in an int for creating an event
int read_int(std::istream &);