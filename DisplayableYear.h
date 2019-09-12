#pragma once
/*
Author: Jon Shidal
Purpose: declare the DisplayableYear class. This class may be a component of a calendar. It inherits from CalendarComponent and defines pure virtual functions
declared in DisplayableComponent
*/

#include "CalendarComponent.h"
// forward declarations needed to avoid circular includes, used to declare friends only
class FullCalendarBuilder;
class CalendarInterface;
class Calendar;
class IncrementalBuilder;

class DisplayableYear : public CalendarComponent {
	friend Calendar;
	friend FullCalendarBuilder;
	friend CalendarInterface;
	friend IncrementalBuilder;
public:
	// arguments: date/time info, parent, leap year or no?
	DisplayableYear(std::tm, std::shared_ptr<DisplayableComponent>, bool);
	DisplayableYear(std::tm, std::shared_ptr<DisplayableComponent>, bool, std::string);
	virtual void display() override;
protected:
	bool leap;
	// year contains months - override accordingly
	virtual std::shared_ptr<DisplayableComponent> addComponent(std::shared_ptr<DisplayableComponent>) override;
};