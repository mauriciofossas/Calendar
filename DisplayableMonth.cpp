#include "stdafx.h"
#include "DisplayableMonth.h"
#include "DisplayableDay.h"
#include <iostream>

using namespace std;

DisplayableMonth::DisplayableMonth(std::tm d, std::shared_ptr<DisplayableComponent> p, string monthname, unsigned int numdays) : CalendarComponent(d, p), name(monthname), numberOfDays(numdays) {
	// initialize children vector, one spot for each day
	for (size_t i = 0; i < numberOfDays; ++i) {
		children.push_back(nullptr);
	}
}

//For incremental builder
DisplayableMonth::DisplayableMonth(std::tm d, std::shared_ptr<DisplayableComponent> p, string monthname) : CalendarComponent(d, p), name(monthname) {

}

void DisplayableMonth::display() {
	cout << "\t\t" << name << ":" << endl;
	for (unsigned int i = 0; i < this->DAYSINAWEEK; ++i) {
		cout << this->daysoftheweek[i] + string(this->longestStringDay - daysoftheweek[i].length(), ' ') << " ";
	}
	cout << "" << endl;

	int currDay = 0; //aka sunday
	
	for (size_t j = 0;j < children.size();j++) {
		if (children[j] != nullptr) {
			DisplayableDay * day = dynamic_cast<DisplayableDay *>(children[j].get());

			if (day->dateInfo.tm_wday == currDay) {
				int length = (day->dateInfo.tm_mday);
				int slength = 0;
				while (length != 0) {
					length = length / 10;
					slength++;
				}
				int numEvents = day->children.size();
				cout << day->dateInfo.tm_mday;
				cout << string(day->children.size(), '*');
				cout << string(this->longestStringDay - slength + 1 - numEvents, ' ');
				currDay++;
				currDay = currDay % 7;
				if (currDay == 0) {
					cout << endl;
					cout << " " << endl;
				}
			}
			else {

				
				cout << string(this->longestStringDay+1, ' ');
				j--;
				currDay++;
				currDay = currDay % 7;
				if (currDay == 0) {
					cout << endl;
				}
			}
		}
	}
	if (mapOfChildren.size() > 0) {
		//Used to find the starting day of the given month
		std::tm time = { 0, 0, 0, dateInfo.tm_mday, dateInfo.tm_mon, dateInfo.tm_year};
		time_t epoch = mktime(&time);
		this->dateInfo.tm_wday = time.tm_wday;
		int startingMonthDay = this->dateInfo.tm_wday;
		//Loop for printing out the initial spaces.
		for (int i = 0; i < startingMonthDay; ++i) {
			cout << string(this->longestStringDay + 1, ' ');
		}
		auto iter = mapOfChildren.begin();
		//Iterates thorugh every day of the month
		for (int i = 1; i < this->days[this->dateInfo.tm_mon] + 1; ++i) {
			//The index for that day exists in the map
			if (iter == mapOfChildren.end()) {

			}
			//If the day exists, we will display it
			else if (i == iter->first) {
				DisplayableDay * day = dynamic_cast<DisplayableDay *>(iter->second.get());
				int numEvents = day->children.size();
				if (i < 10) {
					cout << i << string(numEvents, '*') << string(this->longestStringDay - numEvents, ' ');
				}
				else {
					cout << i << string(numEvents, '*') << string(this->longestStringDay - 1 - numEvents, ' ');
				}
				iter++;
			}
			//Will just print out spaces
			else {
				cout << string(this->longestStringDay + 1, ' ');
			}
			//End line if we finsh a week
			if ((i+startingMonthDay) % 7 == 0) {
				cout << "" << endl;
				cout << "" << endl;
			}
		}
	}
	cout << " ";
	cout << endl;
}
	

shared_ptr<DisplayableComponent> DisplayableMonth::addComponent(shared_ptr<DisplayableComponent> comp) {
	// try to dynamically cast comp to a pointer to a DisplayableDay, will fail if the DisplayableComponent is not a day
	DisplayableDay* d = dynamic_cast<DisplayableDay *>(comp.get()); // can only cast regular pointers, not shared_ptrs
	if (d == nullptr) { // the cast failed
		return nullptr;
	}
	// otherwise, add the day to the correct location
	int dayOfMonth = d->dateInfo.tm_mday - 1;
	if (children[dayOfMonth] == nullptr) { // day does not already exist
		children[dayOfMonth] = comp;
		return comp;
	}
	else {  // day already exist, return existing day
		return children[dayOfMonth];
	}
}