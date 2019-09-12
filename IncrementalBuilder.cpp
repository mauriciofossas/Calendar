#include "stdafx.h"
#include "IncrementalBuilder.h"
#include "CalendarComponent.h"
#include "DisplayableDay.h"
#include "DisplayableMonth.h"
#include "DisplayableYear.h"
#include "DisplayableEvent.h"
#include "utilityfunctions.h"
#include <map>

//added for testing
#include <iostream>

using namespace std;

//Class is used only to build the initial incremental calendar (with current year, month, and day)
//years/month/days are added to the calendar in either build event or getComponentByDate
std::shared_ptr<Calendar> IncrementalBuilder::buildCalendar(std::string name, size_t years)
{
	currentCalendar = make_shared<Calendar>(name);
	//First call to the builder (the calendar will have no children; will only build current year, month, and day)
	if(currentCalendar->mapOfChildren.size() == 0) {
		years = 0; //builds only the current year.
	}
	tm y = currentCalendar->dateInfo;
	currentCalendar->mapOfChildren.insert(make_pair(years, buildYear(y, currentCalendar)));
	currentCalendar->yearsToHold = currentCalendar->yearsToHold + 1;
	return currentCalendar;
}

std::shared_ptr<DisplayableComponent> IncrementalBuilder::buildEvent(std::shared_ptr<DisplayableComponent> cal, std::string name, std::tm when, int recurrEvery, int recurrFor)
{
	shared_ptr<DisplayableComponent> event;
	//Store these to avoid resetting to 0 when setting when to the dateInfo of the day.
	int hour = when.tm_hour;
	int minute = when.tm_min;
	for (int i = 0; i <= recurrFor; ++i) {
		shared_ptr<DisplayableComponent> dayComponent = getComponentByDate(cal, when, "day");
		DisplayableDay * day = dynamic_cast<DisplayableDay *>(dayComponent.get());
		when = day->dateInfo;
		when.tm_hour = hour;
		when.tm_min = minute;
		event = make_shared<DisplayableEvent>(when, dayComponent, name);
		day->addComponent(event);
		currentCalendar->mapOfEvents.insert(std::pair<std::string, std::shared_ptr<DisplayableComponent>>(name, event));
		when = addDays(when, recurrEvery);
	}
	return event;
}

std::shared_ptr<DisplayableComponent> IncrementalBuilder::getComponentByDate(std::shared_ptr<DisplayableComponent> cal, std::tm d, std::string granularity)
{
	int yearIndex = d.tm_year - currentCalendar->dateInfo.tm_year;
	shared_ptr<DisplayableComponent> year;

	bool yearExists = false;
	auto Yeariterator = this->currentCalendar->mapOfChildren.begin();
	//Checks to see if the year at the provided index exitsts.
	while (Yeariterator != this->currentCalendar->mapOfChildren.end()) {
		if (Yeariterator->first == yearIndex) {
			yearExists = true;
			break;
		}
		Yeariterator++;
	}
	if (granularity == "year") {
		//If the year already exists.
		if (yearExists == true) {
			Yeariterator = this->currentCalendar->mapOfChildren.begin();
			while (Yeariterator != this->currentCalendar->mapOfChildren.end()) {
				if (Yeariterator->first == yearIndex) {
					year = Yeariterator->second;
					break;
				}
				Yeariterator++;
			}
			return year;
		}
		//If the year we are trying to zoom into does not exist yet, we must create it.
		else {
			shared_ptr<DisplayableComponent> yearToAdd = buildYear(d, currentCalendar);
			pair<int, std::shared_ptr<DisplayableComponent>> pairToAdd = make_pair(yearIndex, yearToAdd);
			currentCalendar->mapOfChildren.insert(pairToAdd);
			currentCalendar->yearsToHold = currentCalendar->yearsToHold + 1;
			year = yearToAdd;
			return year;
		}
	}
	else if (granularity == "month") {
		//If the year already exists.
		if (yearExists == true) {
			Yeariterator = this->currentCalendar->mapOfChildren.begin();
			while (Yeariterator != this->currentCalendar->mapOfChildren.end()) {
				if (Yeariterator->first == yearIndex) {
					year = Yeariterator->second;
					break;
				}
				Yeariterator++;
			}
		}
		//If the year we are trying to zoom into does not exist yet, we must create it.
		else {
			shared_ptr<DisplayableComponent> yearToAdd = buildYear(d, currentCalendar);
			pair<int, std::shared_ptr<DisplayableComponent>> pairToAdd = make_pair(yearIndex, yearToAdd);
			currentCalendar->mapOfChildren.insert(pairToAdd);
			currentCalendar->yearsToHold = currentCalendar->yearsToHold + 1;
			year = yearToAdd;
		}
		shared_ptr<DisplayableComponent> month;
		int monthIndex = d.tm_mon;
		bool monthExists = false;
		auto Monthiterator = year->mapOfChildren.begin();
		//Checks to see if the month at the provided index exitsts.
		while (Monthiterator != year->mapOfChildren.end()) {
			if (Monthiterator->first == monthIndex) {
				monthExists = true;
				break;
			}
			Monthiterator++;
		}
		//if the month we are zooming into exists.
		if (monthExists == true) {
			Monthiterator = year->mapOfChildren.begin();
			while (Monthiterator != year->mapOfChildren.end()) {
				if (Monthiterator->first == monthIndex) {
					month = Monthiterator->second;
					break;
				}
				Monthiterator++;
			}
			return month;
		}
		//if the month does not exist, we must create it.
		else {
			shared_ptr<DisplayableComponent> monthToAdd = buildMonth(d, year);
			pair<int, std::shared_ptr<DisplayableComponent>> pairToAdd = make_pair(monthIndex, monthToAdd);
			year->mapOfChildren.insert(pairToAdd);
			month = monthToAdd;
			return month;
		}
	}
	else if (granularity == "day") {
		//If the year already exists.
		if (yearExists == true) {
			Yeariterator = this->currentCalendar->mapOfChildren.begin();
			while (Yeariterator != this->currentCalendar->mapOfChildren.end()) {
				if (Yeariterator->first == yearIndex) {
					year = Yeariterator->second;
					break;
				}
				Yeariterator++;
			}
		}
		//If the year we are trying to zoom into does not exist yet, we must create it.
		else {
			shared_ptr<DisplayableComponent> yearToAdd = buildYear(d, currentCalendar);
			pair<int, std::shared_ptr<DisplayableComponent>> pairToAdd = make_pair(yearIndex, yearToAdd);
			currentCalendar->mapOfChildren.insert(pairToAdd);
			currentCalendar->yearsToHold = currentCalendar->yearsToHold + 1;
			year = yearToAdd;
		}
		shared_ptr<DisplayableComponent> month;
		int monthIndex = d.tm_mon;
		bool monthExists = false;
		auto Monthiterator = year->mapOfChildren.begin();
		//Checks to see if the month at the provided index exitsts.
		while (Monthiterator != year->mapOfChildren.end()) {
			if (Monthiterator->first == monthIndex) {
				monthExists = true;
				break;
			}
			Monthiterator++;
		}
		//if the month we are zooming into exists.
		if (monthExists == true) {
			Monthiterator = year->mapOfChildren.begin();
			while (Monthiterator != year->mapOfChildren.end()) {
				if (Monthiterator->first == monthIndex) {
					month = Monthiterator->second;
					break;
				}
				Monthiterator++;
			}
		}
		//if the month does not exist, we must create it.
		else {
			shared_ptr<DisplayableComponent> monthToAdd = buildMonth(d, year);
			pair<int, std::shared_ptr<DisplayableComponent>> pairToAdd = make_pair(monthIndex, monthToAdd);
			year->mapOfChildren.insert(pairToAdd);
			month = monthToAdd;
		}
		//To find the day
		shared_ptr<DisplayableComponent> day;
		int dayIndex = d.tm_mday;
		bool dayExists = false;
		auto dayIterator = month->mapOfChildren.begin();
		//Checks to see if the day at the provided index exitsts.
		while (dayIterator != month->mapOfChildren.end()) {
			if (dayIterator->first == dayIndex) {
				dayExists = true;
				break;
			}
			dayIterator++;
		}
		//if the month we are zooming into exists.
		if (dayExists == true) {
			dayIterator = month->mapOfChildren.begin();
			while (dayIterator != month->mapOfChildren.end()) {
				if (dayIterator->first == dayIndex) {
					day = dayIterator->second;
					break;
				}
				dayIterator++;
			}
			return day;
		}
		//if the day does not exist, we must create it.
		else {
			shared_ptr<DisplayableComponent> dayToAdd = buildDay(d, month);
			pair<int, std::shared_ptr<DisplayableComponent>> pairToAdd = make_pair(dayIndex, dayToAdd);
			month->mapOfChildren.insert(pairToAdd);
			day = dayToAdd;
			return day;
		}
	}
	//Should never reach this point; granularity is checked in the interface to be day, month or year.
	else {
		return nullptr;
	}
}

std::shared_ptr<DisplayableComponent> IncrementalBuilder::buildYear(std::tm d, std::shared_ptr<DisplayableComponent> p)
{
	shared_ptr<DisplayableComponent> y = make_shared<DisplayableYear>(d, p, false, "incremental");
	//Will build the initial month (only for initially build, when the calendar only has one year and the
	//year has 0 months).
	if (currentCalendar->mapOfChildren.size() == 0 && y->mapOfChildren.size() == 0) {
		shared_ptr<DisplayableComponent> monthToAdd = buildMonth(d, y);
		pair<int, std::shared_ptr<DisplayableComponent>> pairToAdd = make_pair(d.tm_mon, monthToAdd);
		y->mapOfChildren.insert(pairToAdd);
	}
	return y;
}

std::shared_ptr<DisplayableComponent> IncrementalBuilder::buildDay(std::tm d, std::shared_ptr<DisplayableComponent> p)
{
	shared_ptr<DisplayableComponent> day = make_shared<DisplayableDay>(d, p);
	return day;
}

std::shared_ptr<DisplayableComponent> IncrementalBuilder::buildMonth(std::tm d, std::shared_ptr<DisplayableComponent> p)
{
	int index = d.tm_mon;
	shared_ptr<DisplayableComponent> m = make_shared<DisplayableMonth>(d, p, CalendarComponent::months[index]);
	//Will only construct the first day (if the calendar only has one year, the year only has one month, and
	// the month has no days.
	if (m->mapOfChildren.size() == 0 && this->currentCalendar->mapOfChildren.size() == 0 && p->mapOfChildren.size() == 0) {
		d.tm_wday = (d.tm_wday + 1) % CalendarComponent::DAYSINAWEEK;
		shared_ptr<DisplayableComponent> dayToAdd = buildDay(d, m);
		pair<int, std::shared_ptr<DisplayableComponent>> pairToAdd = make_pair(d.tm_mon + 1, dayToAdd);
		m->mapOfChildren.insert(pairToAdd);
	}
	return m;
}

