#include "stdafx.h"
#include "Calendar.h"
#include "DisplayableYear.h"
#include <iostream>

using namespace std;

// Calendar
Calendar::Calendar(string n, size_t y) : CalendarComponent(tm(), nullptr), name(n), yearsToHold(y) { // just initialize with a default tm for now.
	time_t now = time(0); // get the current time
	tm now_tm;
	localtime_s(&now_tm, &now); // create a struct tm(now_tm) from the current time
	currentDate = now_tm;    // set Calendar's date and time to now
	dateInfo = now_tm; // setup dateInfo to represent January 1st of the current year, start time of the calendar
	dateInfo.tm_sec = 0;
	dateInfo.tm_min = 0;
	dateInfo.tm_hour = 0;
	dateInfo.tm_mday = 1;
	dateInfo.tm_mon = 0;
	// calculate and set day of the week to that of January 1st of the current year. Very sloppy, I know...
	dateInfo.tm_wday = (now_tm.tm_wday + CalendarComponent::DAYSINAWEEK - (now_tm.tm_yday % CalendarComponent::DAYSINAWEEK)) % CalendarComponent::DAYSINAWEEK;
	dateInfo.tm_yday = 0;
	dateInfo.tm_isdst = 0;
	// intialize calendar to hold __ years
	for (size_t i = 0; i < yearsToHold; ++i) {
		children.push_back(nullptr);
	}
}
//Constructor to use with the incremental builder
Calendar::Calendar(string n) : CalendarComponent(tm(), nullptr), name(n) { // just initialize with a default tm for now.
	time_t now = time(0); // get the current time
	tm now_tm;
	localtime_s(&now_tm, &now); // create a struct tm(now_tm) from the current time
	currentDate = now_tm;    // set Calendar's date and time to now
	dateInfo = now_tm; // setup dateInfo to represent January 1st of the current year, start time of the calendar
	dateInfo.tm_sec = 0;
	dateInfo.tm_min = 0;
	dateInfo.tm_hour = 0;
	dateInfo.tm_mday = 1;
	dateInfo.tm_mon = 0;
	// calculate and set day of the week to that of January 1st of the current year. Very sloppy, I know...
	dateInfo.tm_wday = (now_tm.tm_wday + CalendarComponent::DAYSINAWEEK - (now_tm.tm_yday % CalendarComponent::DAYSINAWEEK)) % CalendarComponent::DAYSINAWEEK;
	dateInfo.tm_yday = 0;
	dateInfo.tm_isdst = 0;
	yearsToHold = mapOfChildren.size();
}

void Calendar::display() {
	cout << "Calendar: " << name << endl;
	//The following lines are for displaying a calendar if a FullBuilder was used.
	unsigned int index = 0;
	for (size_t i = 0; i < children.size(); ++i) { // forward request to all children
		cout << "Index: " << index << " ";
		index = index + 1;
		if (children[i] != nullptr) {
			DisplayableYear * year = dynamic_cast<DisplayableYear *>(children[i].get());
			cout << year->dateInfo.tm_year + this->BASEYEAR << endl;
		}
	}
	//The following lines are for displaying a calendar if a IncrementalBuilder was used
	auto iterator = mapOfChildren.begin();
	while (iterator != mapOfChildren.end()) {
		cout << "Index: " << iterator->first << " ";
		DisplayableYear * year = dynamic_cast<DisplayableYear *>(iterator->second.get());
		cout << year->dateInfo.tm_year + this->BASEYEAR << endl;
		iterator++;
	}
}

shared_ptr<DisplayableComponent> Calendar::addComponent(std::shared_ptr<DisplayableComponent> comp) {
	DisplayableYear* y = dynamic_cast<DisplayableYear *>(comp.get());
	if (y == nullptr) { // if the cast fails, return nullptr
		return nullptr;
	}
	unsigned int calendarYear = CalendarComponent::BASEYEAR + dateInfo.tm_year;
	unsigned int yearAdding = CalendarComponent::BASEYEAR + y->dateInfo.tm_year;
	int index = yearAdding - calendarYear; // which child?
	if (index >= 0 && index < children.size() && children[index] == nullptr) {
		children[index] = comp;
		return comp;
	}
	else {
		return nullptr;
	}
}