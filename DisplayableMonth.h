#pragma once
/*
Author: Jon Shidal
Purpose: declare the DisplayableMonth class. This class may be a component of a calendar. It inherits from CalendarComponent and defines pure virtual functions
declared in DisplayableComponent
*/

#include "CalendarComponent.h"
// forward declarations needed to avoid circular includes, used to declare friends only
class FullCalendarBuilder;
class CalendarInterface;
class DisplayableYear;
class IncrementalBuilder;

class DisplayableMonth : public CalendarComponent {
	friend class DisplayableYear;
	friend FullCalendarBuilder;
	friend CalendarInterface;
	friend IncrementalBuilder;
public:
	// arguments = date/time info, its parent, name of the month, days in the month
	DisplayableMonth(std::tm, std::shared_ptr<DisplayableComponent>, std::string monthname, unsigned int numdays);
	//For incrementalBuilder
	DisplayableMonth(std::tm, std::shared_ptr<DisplayableComponent>, std::string monthname);
	virtual void display() override;
protected:
	std::string name;
	unsigned int numberOfDays;
	// Month contains days, so it is a composite object. override addComponent accordingly
	virtual std::shared_ptr<DisplayableComponent> addComponent(std::shared_ptr<DisplayableComponent>) override;
};