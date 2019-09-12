// Header for the ToDoList class
// Includes default constructor and methods needed in this class
#pragma once
#include "DisplayableComponent.h"

class ToDoList : public DisplayableComponent {
public:
	ToDoList();
	void addComponent(std::string, std::tm, bool);
	void display();
};