/*
Author: Jon Shidal
Purpose: create a CalendarInterface object and call run
*/

#include "stdafx.h"
#include "CalendarInterface.h"
#include <fstream>

using namespace std;

int main(int argc, char * argv[]) {
	CalendarInterface ci("incremental", "test", 4, std::ifstream());
	ci.run();

	//CalendarInterface ci("full", "test", 3,std::ifstream());
	//ci.run();
	return 0;
}