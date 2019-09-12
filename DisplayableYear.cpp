#include "stdafx.h"
#include "DisplayableYear.h"
#include <iostream>
#include "DisplayableMonth.h"
//included to be able to display the month as desired
#include "DisplayableDay.h"
#include "DisplayableEvent.h"

using namespace std;

DisplayableYear::DisplayableYear(std::tm d, std::shared_ptr<DisplayableComponent> p, bool l) : CalendarComponent(d, p), leap(l) {
	for (size_t i = 0; i < CalendarComponent::MONTHS; ++i) {
		children.push_back(nullptr);
	}
}

//For incrementalBuilder
DisplayableYear::DisplayableYear(std::tm d, std::shared_ptr<DisplayableComponent> p, bool l, std::string incr) : CalendarComponent(d, p), leap(l) {

}


void DisplayableYear::display() {
	cout << "\tYear " << dateInfo.tm_year + CalendarComponent::BASEYEAR << ":" << endl;
	//The following is for displaying a year when using a FullCalendarBuilder.
	unsigned int index = 0;
	//Iterate through each month
	for (size_t i = 0; i < children.size(); ++i) {
		cout << "\t\tIndex: " << index << " ";
		index = index + 1;
		//Checks that the child is not nullptr
		if (children[i] != nullptr) {
			DisplayableMonth * month = dynamic_cast<DisplayableMonth *>(children[i].get());
			cout << this->months[month->dateInfo.tm_mon] << endl;
			//Iterate through each day
			for (size_t j = 0; j < month->children.size(); ++j) {
				DisplayableDay * day = dynamic_cast<DisplayableDay*>(month->children[j].get());
				//Iterate through each event in a given day
				for (size_t m = 0; m < day->children.size(); ++m) {
					DisplayableEvent * event = dynamic_cast<DisplayableEvent*>(day->children[m].get());
					int hour = 0;
					if (event->dateInfo.tm_hour < 10) {
						hour = 1;
					}
					int minute = 0;
					if (event->dateInfo.tm_min < 10) {
						minute = 1;
					}
					cout << "\t\t\t" << month->dateInfo.tm_mon + 1 << "/" << event->dateInfo.tm_mday << "/";
					cout << this->dateInfo.tm_year + this->BASEYEAR << " " << string(hour,'0') << event->dateInfo.tm_hour;
					cout << ":" << string(minute,'0') << event->dateInfo.tm_min << " " << event->name << endl;
				}
			}
		}
	}
	//The following is for displaying a year using a IncrementalBuilder
	auto Monthiterator = mapOfChildren.begin();
	//Iterates through all months in our year
	while (Monthiterator != mapOfChildren.end()) {
		cout << "\t\tIndex: " << Monthiterator->first << " ";
		DisplayableMonth * month = dynamic_cast<DisplayableMonth *>(Monthiterator->second.get());
		cout << this->months[month->dateInfo.tm_mon] << endl;
		//For each month, we want to iterate through the days
		auto DayIterator = month->mapOfChildren.begin();
		while (DayIterator != month->mapOfChildren.end()) {
			DisplayableDay * day = dynamic_cast<DisplayableDay *>(DayIterator->second.get());
			//Iterate through each event in a given day
			for (size_t m = 0; m < day->children.size(); ++m) {
				DisplayableEvent * event = dynamic_cast<DisplayableEvent*>(day->children[m].get());
				int hour = 0;
				if (event->dateInfo.tm_hour < 10) {
					hour = 1;
				}
				int minute = 0;
				if (event->dateInfo.tm_min < 10) {
					minute = 1;
				}
				cout << "\t\t\t" << month->dateInfo.tm_mon + 1 << "/" << event->dateInfo.tm_mday << "/";
				cout << this->dateInfo.tm_year + this->BASEYEAR << " " << string(hour, '0') << event->dateInfo.tm_hour;
				cout << ":" << string(minute, '0') << event->dateInfo.tm_min << " " << event->name << endl;
			}
			DayIterator++;
		}
		Monthiterator++;
	}
}

shared_ptr<DisplayableComponent> DisplayableYear::addComponent(shared_ptr<DisplayableComponent> comp) {
	// try to dynamically cast comp to a pointer to a DisplayableMonth
	DisplayableMonth * m = dynamic_cast<DisplayableMonth *>(comp.get());
	if (m == nullptr) { // if the cast fails, return nullptr
		return nullptr;
	}
	// otherwise, add the month to the correct location
	int monthOfYear = m->dateInfo.tm_mon;
	if (children[monthOfYear] == nullptr) { // month does not already exist
		children[monthOfYear] = comp;
		return comp;
	}
	else {  // month already exist, return existing month
		return children[monthOfYear];
	}
}
