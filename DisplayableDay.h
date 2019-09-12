#pragma once
/*
Author: Jon Shidal
Purpose: declare the DisplayableDay class. This class may be a component of a calendar. It inherits from CalendarComponent and defines pure virtual functions
declared in DisplayableComponent
*/

#include "CalendarComponent.h"
// forward declarations needed to avoid circular includes, used to declare friends only
class FullCalendarBuilder;
class CalendarInterface;
class DisplayableMonth;
class DisplayableYear;
class IncrementalBuilder;

class DisplayableDay : public CalendarComponent {
	friend class DisplayableMonth;
	friend class DisplayableYear;
	friend FullCalendarBuilder;
	friend CalendarInterface;
	class IncrementalBuilder;
public:
	// 1st argument = start date/timeof the day, 2nd argument = its parent
	DisplayableDay(std::tm, std::shared_ptr<DisplayableComponent>);
	virtual void display() override;
	// currently a leaf class, so no need to override addComponent()
	virtual std::shared_ptr<DisplayableComponent> addComponent(std::shared_ptr<DisplayableComponent>) override;
};
