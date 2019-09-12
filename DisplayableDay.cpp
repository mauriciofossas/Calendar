#include "stdafx.h"
#include "DisplayableDay.h"
#include <iostream>
#include <algorithm>
#include "DisplayableEvent.h"

using namespace std;

DisplayableDay::DisplayableDay(std::tm d, std::shared_ptr<DisplayableComponent> p) : CalendarComponent(d, p) { 
	//Date info updated for the IncrementalBuilder
	this->dateInfo.tm_sec = 0;
	int y = d.tm_year + BASEYEAR;
	int m = d.tm_mon + 1;
	int day = d.tm_mday;
	static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
	y -= m < 3;
	int weekday = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + day) % 7;
	this->dateInfo.tm_wday = weekday;
	this->dateInfo.tm_isdst = 0;

	std::tm a = { 0, 0, 0, 1, 0, dateInfo.tm_year }; //January 1st of current year
	std::tm b = { dateInfo.tm_sec, dateInfo.tm_min, dateInfo.tm_hour, dateInfo.tm_mday, dateInfo.tm_mon, dateInfo.tm_year }; //current day
	std::time_t base = std::mktime(&a);
	std::time_t current = std::mktime(&b);
	int difference = 0;
	if (base != (std::time_t)(-1) && current != (std::time_t)(-1))
	{
		difference = std::difftime(current, base) / (60 * 60 * 24);
	}
	this->dateInfo.tm_yday = difference;
}

void DisplayableDay::display() {
	cout << "\t\t\t" << daysoftheweek[dateInfo.tm_wday] << " ";
	cout << dateInfo.tm_mon + 1 << "/" << dateInfo.tm_mday << "/" << dateInfo.tm_year + CalendarComponent::BASEYEAR << endl;
	for (size_t i = 0; i < children.size(); ++i) {
		DisplayableEvent * event = dynamic_cast<DisplayableEvent *>(children[i].get());
		if (event->dateInfo.tm_hour < 10) {
			cout << "\t\t\t\t" << "0" << event->dateInfo.tm_hour;
		}
		else {
			cout << "\t\t\t\t" << event->dateInfo.tm_hour;
		}
		cout << ":";
		if (event->dateInfo.tm_min < 10) {
			cout << "0" << event->dateInfo.tm_min;
		}
		else {
			cout << event->dateInfo.tm_min;
		}
		cout << " " << event->name << endl;
	}
}

shared_ptr<DisplayableComponent> DisplayableDay::addComponent(shared_ptr<DisplayableComponent> comp) {
	children.push_back(comp);
	std::sort(children.begin(), children.end());
	return comp;
}