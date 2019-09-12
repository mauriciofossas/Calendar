#pragma once

#include "CalendarComponent.h"
// forward declarations needed to avoid circular includes, used to declare friends only
class FullCalendarBuilder;
class CalendarInterface;
class IncrementalBuilder;

class Calendar : public CalendarComponent {
	// friends
	friend FullCalendarBuilder;
	friend CalendarInterface;
	friend IncrementalBuilder;
protected:
	std::string name;
	size_t yearsToHold;
	std::tm currentDate; // current date and time
						 // dateInfo is the start date and time

						 // Calendar contains years, so override accordingly
	virtual std::shared_ptr<DisplayableComponent> addComponent(std::shared_ptr<DisplayableComponent>) override;
public:
	// arguments: name of the calendar, length of the calendar in years
	Calendar(std::string n, size_t y);
	//A constructor for a calendar for the IncrementalBuilder
	Calendar(std::string n);
	// inherited methods
	virtual void display() override;
	//map for pairing events to their name
	std::multimap<std::string, std::shared_ptr<DisplayableComponent>> mapOfEvents;
};