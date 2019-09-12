#pragma once

#include "CalendarComponent.h"
using namespace std;

class FullCalendarBuilder;
class CalendarInterface;
class DisplayableDay;
class DisplayableMonth;
class DisplayableYear;
class IncrementalBuilder;

class DisplayableEvent : public CalendarComponent{
	friend class DisplayableDay;
	friend class DisplayableMonth;
	friend class DisplayableYear;
	friend FullCalendarBuilder;
	friend CalendarInterface;
	friend IncrementalBuilder;
	friend bool operator<(shared_ptr<DisplayableComponent>, shared_ptr<DisplayableComponent>);
public:
	DisplayableEvent(std::tm, std::shared_ptr<DisplayableComponent>, std::string);
	virtual void display() override;
	std::string name;
};

bool operator<(shared_ptr<DisplayableComponent>, shared_ptr<DisplayableComponent>);
