// Source file for the Task class
#include "stdafx.h"
#include "Task.h"
#include <iostream>
#include <string>

using namespace std;

Task::Task(string str, tm myTime, bool complete) : t(str), time(myTime), isComp(complete) {
};

// Displays the name of the task, followed by a properly formed date and time (assembled below)
void Task::display()
{
	int yr = time.tm_year + 1900;
	int mo = time.tm_mon + 1;
	if (time.tm_hour < 10 && time.tm_min < 10)
	{
		cout << t << " " << mo << "/" << time.tm_mday << "/" << yr;
		cout << " " << 0 << time.tm_hour << ":" << 0 << time.tm_min;
	}
	else if (time.tm_hour < 10 && time.tm_min >= 10)
	{
		cout << t << " " << mo << "/" << time.tm_mday << "/" << yr;
		cout << " " << 0 << time.tm_hour << ":" << time.tm_min;
	}
	else if (time.tm_hour >= 10 && time.tm_min < 10)
	{
		cout << t << " " << mo << "/" << time.tm_mday << "/" << yr;
		cout << " " << time.tm_hour << ":" << 0 << time.tm_min;
	}
	else
	{
		cout << t << " " << mo << "/" << time.tm_mday << "/" << yr;
		cout << " " << time.tm_hour << ":" << time.tm_min;
	}
	if (isComp == true)
	{
		cout << " COMPLETE" << endl;
	}
	else
	{
		cout << " TODO" << endl;
	}
}

void Task::setComplete()
{
	isComp = true;
}
