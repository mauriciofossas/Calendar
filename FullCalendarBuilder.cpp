/*
Author: Jon Shidal
Purpose: Define full calendar builder class, responsible for building and managing the structure of the calendar
*/
#include "stdafx.h"
#include "FullCalendarBuilder.h"
#include "CalendarComponent.h"
#include "DisplayableDay.h"
#include "DisplayableMonth.h"
#include "DisplayableYear.h"
#include "DisplayableEvent.h"
#include "utilityfunctions.h"
#include <map>
//Included for testing
#include <iostream>

using namespace std;

shared_ptr<Calendar> FullCalendarBuilder::buildCalendar(string name, size_t years) {
	currentCalendar = make_shared<Calendar>(name, years);
	// construct each year in a recursive way, add each year as a child of the calendar
	for (unsigned int i = 0; i < years; ++i) {
		tm y = currentCalendar->dateInfo;
		y.tm_year += i;
		y.tm_wday = (y.tm_wday + CalendarComponent::DAYSINAYEAR * i) % CalendarComponent::DAYSINAWEEK; // calculate day of the week for first day of the year
		currentCalendar->addComponent(buildYear(y, currentCalendar));
	}
	return currentCalendar;
}

// you may decide to define this.
shared_ptr<DisplayableComponent> FullCalendarBuilder::buildEvent(shared_ptr<DisplayableComponent> cal, string name, tm when, int recurrEvery, int recurrFor) {
	//Create a new event for every time it recurs (not just one event)
	shared_ptr<DisplayableComponent> event;
	//Store these to avoid resetting to 0 when setting when to the dateInfo of the day.
	int hour = when.tm_hour;
	int minute = when.tm_min;
	for (int i = 0; i <= recurrFor; ++i) {
		int index = when.tm_year - currentCalendar->dateInfo.tm_year;
		//If the event goes out of range of the calendar
		if (cal->getChild(index) == nullptr) {
			break;
		}
		shared_ptr<DisplayableComponent> year = cal->getChild(index);
		shared_ptr<DisplayableComponent> month = year->getChild(when.tm_mon);
		shared_ptr<DisplayableComponent> day = month->getChild(when.tm_mday - 1);
		DisplayableDay* d = dynamic_cast<DisplayableDay *>(day.get());
		//Sets when to the date info of the current day because addDays requires all fields to be filled.
		when = d->dateInfo;
		when.tm_hour = hour;
		when.tm_min = minute;
		event = make_shared<DisplayableEvent>(when, day, name);
		day->addComponent(event);
		currentCalendar->mapOfEvents.insert(std::pair<std::string, std::shared_ptr<DisplayableComponent>>(name, event));
		when = addDays(when, recurrEvery);
	}
	return event;
}

// you may decide to define this.
shared_ptr<DisplayableComponent> FullCalendarBuilder::getComponentByDate(shared_ptr<DisplayableComponent> cal, tm d, string granularity) {
	int index = d.tm_year - currentCalendar->dateInfo.tm_year;
	shared_ptr<DisplayableComponent> year = cal->getChild(index);
	
	if (granularity == "year") {
		DisplayableYear* y = dynamic_cast<DisplayableYear *>(year.get());
		return year;
	}
	else if (granularity == "month") {
		shared_ptr<DisplayableComponent> month = year->getChild(d.tm_mon);
		DisplayableMonth* y = dynamic_cast<DisplayableMonth *>(month.get());
		return month;
	}
	else if (granularity == "day") {
		shared_ptr<DisplayableComponent> month = year->getChild(d.tm_mon);
		shared_ptr<DisplayableComponent> day = month->getChild(d.tm_mday - 1);
		DisplayableDay* d = dynamic_cast<DisplayableDay *>(day.get());
		return day;
	}
	//Should never reach this point; granularity is checked in the interface to be day, month or year.
	else {
		return nullptr;
	}
}

shared_ptr<DisplayableComponent> FullCalendarBuilder::buildDay(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	shared_ptr<DisplayableComponent> day = make_shared<DisplayableDay>(d, p);
	return day;
}

shared_ptr<DisplayableComponent> FullCalendarBuilder::buildMonth(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	int index = d.tm_mon;
	shared_ptr<DisplayableComponent> m = make_shared<DisplayableMonth>(d, p, CalendarComponent::months[index], CalendarComponent::days[index]);
	for (int i = 0; i < CalendarComponent::days[index]; ++i) { // for each day in the month
		m->addComponent(buildDay(d, m)); // construct day and add as a child of the month
		++(d.tm_mday); // increment day of the month
		d.tm_wday = (d.tm_wday + 1) % CalendarComponent::DAYSINAWEEK; // increment weekday, reset to 0 if needed
	}
	return m;
}
shared_ptr<DisplayableComponent> FullCalendarBuilder::buildYear(std::tm d, std::shared_ptr<DisplayableComponent> p) {
	shared_ptr<DisplayableComponent> y = make_shared<DisplayableYear>(d, p, false);
	// construct each month and add it as a child of the year
	for (unsigned int i = 0; i < CalendarComponent::MONTHS; ++i) {
		d.tm_mon = i;
		y->addComponent(buildMonth(d, y));
		// set week day of first day of the next month
		d.tm_wday = (d.tm_wday + CalendarComponent::days[i]) % CalendarComponent::DAYSINAWEEK;
	}
	return y;
}