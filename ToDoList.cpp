// Source file for the ToDoList class

#include "stdafx.h"
#include "ToDoList.h"
#include "Task.h"
#include <iostream>
#include <string>
#include <vector>
#include "DisplayableComponent.h"

using namespace std;

ToDoList::ToDoList(){};

// Adds Task to the ToDoList
void ToDoList::addComponent(string str, tm time, bool compl)
{
	Task* temp = new Task(str, time, compl);
	shared_ptr<Task> sp = make_shared<Task>(*temp);
	children.push_back(sp);
}

//Determines whether one element is less than the other
bool lessThan(shared_ptr<DisplayableComponent> task1, shared_ptr<DisplayableComponent> task2) {
	Task * t1 = dynamic_cast<Task *>(task1.get());
	Task * t2 = dynamic_cast<Task *>(task2.get());
	if (t1->time.tm_year < t2->time.tm_year) {
		return true;
	}
	else if (t1->time.tm_year == t2->time.tm_year) {
		if (t1->time.tm_mon < t2->time.tm_mon) {
			return true;
		}
		else if (t1->time.tm_mon == t2->time.tm_mon)
		{
			if (t1->time.tm_mday < t2->time.tm_mday) {
				return true;
			}
			else if (t1->time.tm_mday == t2->time.tm_mday)
			{
				if (t1->time.tm_hour < t2->time.tm_hour)
				{
					return true;
				}
				else if (t1->time.tm_hour == t2->time.tm_hour)
				{
					if (t1->time.tm_min < t2->time.tm_min)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
}

// Insertion sort algorithm
void insertionSort(vector<shared_ptr<DisplayableComponent>> &v)
{
	for (unsigned int i = 1; i < v.size(); i++)
	{
		unsigned int j = i - 1;
		while (lessThan(v[i], v[j]) && j > 0)
		{
			shared_ptr<DisplayableComponent> temp = v[i];
			v[i] = v[j];
			v[j] = temp;
			j--;
		}
		if (lessThan(v[i], v[j]))
		{
			shared_ptr<DisplayableComponent> temp = v[i];
			v[i] = v[j];
			v[j] = temp;
			j--;
		}
	}
}

// Displays the sorted vector
void ToDoList::display()
{
	cout << "TODO LIST" << endl;
	for (unsigned int i = 0; i < children.size(); i++)
	{
		if (children.size() > 1)
		{
			insertionSort(children);
		}
		children[i]->display();
	}
}