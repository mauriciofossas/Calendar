#pragma once

#include "CalendarBuilder.h"

class IncrementalBuilder : public CalendarBuilder {
public:
	virtual std::shared_ptr<Calendar> buildCalendar(std::string name, size_t years) override;
	virtual std::shared_ptr<DisplayableComponent> buildEvent(std::shared_ptr<DisplayableComponent>, std::string name, std::tm when, int recurrEvery, int recurrFor) override;
	virtual std::shared_ptr<DisplayableComponent> getComponentByDate(std::shared_ptr<DisplayableComponent> cal, std::tm d, std::string granularity) override;
protected:
	virtual std::shared_ptr<DisplayableComponent> buildYear(std::tm d, std::shared_ptr<DisplayableComponent> p) override;
	virtual std::shared_ptr<DisplayableComponent> buildDay(std::tm d, std::shared_ptr<DisplayableComponent> p) override;
	virtual std::shared_ptr<DisplayableComponent> buildMonth(std::tm d, std::shared_ptr<DisplayableComponent> p) override;
};