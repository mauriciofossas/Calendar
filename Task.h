// Header file for the Task class
#pragma once
#include "DisplayableComponent.h"

class Task : public DisplayableComponent {
public:
	// Constructs task out of its three instance variables: the name, time, and whether it has been completed
	Task(std::string, std::tm, bool);
	void display();
	void setComplete();
	std::tm time;
private:
	std::string t;
	bool isComp;
};