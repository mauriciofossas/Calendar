#include "stdafx.h"
#include "DisplayableEvent.h"
#include <iostream>

using namespace std;

DisplayableEvent::DisplayableEvent(std::tm d, std::shared_ptr<DisplayableComponent> p, std::string name) : CalendarComponent(d, p) 
{
	this->dateInfo = d;
	this->name = name;
	int y = d.tm_year + BASEYEAR;
	int m = d.tm_mon + 1;
	int day = d.tm_mday;
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	int weekday = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + day) % 7;
	this->dateInfo.tm_wday = weekday;
}

void DisplayableEvent::display()
{
	cout << "\t\t\t\t";
	cout << daysoftheweek[this->dateInfo.tm_wday] << ", " << CalendarComponent::months[this->dateInfo.tm_mon] << ", " << this->dateInfo.tm_mday << ", " << this->dateInfo.tm_year + this->BASEYEAR << endl;
	cout << "\t\t\t\t";
	cout << this->name << " at " << this->dateInfo.tm_hour << ":" << this->dateInfo.tm_min << endl;
}


//Overloaded < operator to sort event for days.
bool operator<(shared_ptr<DisplayableComponent> event1, shared_ptr<DisplayableComponent> event2) {
	DisplayableEvent * e1 = dynamic_cast<DisplayableEvent *>(event1.get());
	DisplayableEvent * e2 = dynamic_cast<DisplayableEvent *>(event2.get());
	if (e1->dateInfo.tm_hour < e2->dateInfo.tm_hour) {
		return true;
	}
	else if (e1->dateInfo.tm_hour == e2->dateInfo.tm_hour) {
		if (e1->dateInfo.tm_min < e2->dateInfo.tm_min) { 
			return true;
		}
	}
	return false;
}
