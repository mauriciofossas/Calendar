#include "stdafx.h"
#include "CalendarInterface.h"
#include "FullCalendarBuilder.h"
#include"utilityfunctions.h"
#include<iostream>
#include "DisplayableEvent.h"
#include <map>
#include <fstream>
#include <ostream>
#include "IncrementalBuilder.h"
#include "sstream"
#include <string>


using namespace std;

CalendarInterface::CalendarInterface(std::string builderType, std::string calendarName, size_t years,ifstream & ifs) : builder(nullptr), cal(nullptr), currentDisplay(nullptr) {
	if (builderType == "full") {
		builder = make_shared<FullCalendarBuilder>();
		cal = builder->buildCalendar(calendarName, years);
		currentDisplay = cal;
		levelOfCurrentDisplay = 0;
		typeOfBuilder = builderType;
		if (!tdlIsSet) {
			tdl = shared_ptr<ToDoList>(new ToDoList());
			tdlIsSet = true;
		}
	}
	if (builderType == "incremental") {
		builder = make_shared<IncrementalBuilder>();
		cal = builder->buildCalendar(calendarName, years);
		currentDisplay = cal;
		levelOfCurrentDisplay = 0;
		typeOfBuilder = builderType;
		if (!tdlIsSet) {
			tdl = shared_ptr<ToDoList>(new ToDoList());
			tdlIsSet = true;
		}
		
	}
	if (builderType == "saved" || builderType == "savedIncremental") {
		if (builderType == "savedIncremental") {
			builder = make_shared<IncrementalBuilder>();
		}
		else {
			builder = make_shared<FullCalendarBuilder>();
		}
		cal = builder->buildCalendar(calendarName, years);
		levelOfCurrentDisplay = 0;
		currentDisplay = cal;

		std::string eventName = "";
		std::string line;
		std::tm finalWhen;
		try {
			while (getline(ifs, line)) {
				if (line != "-") {
					break;
				}
				getline(ifs, eventName);
				getline(ifs, line);
				finalWhen.tm_year = stoi(line)-(cal->BASEYEAR);
				getline(ifs, line);
				finalWhen.tm_mon = stoi(line);
				getline(ifs, line);
				finalWhen.tm_mday = stoi(line);
				getline(ifs, line);
				finalWhen.tm_hour = stoi(line);
				getline(ifs, line);
				finalWhen.tm_min = stoi(line);
				getline(ifs, line);
				finalWhen.tm_sec = stoi(line);
				cout << "attempted to build event" << endl;
				builder->buildEvent(this->cal, eventName, finalWhen, 0, 0);
			}
		}
		catch (...) {
			throw -1;
		}
		//here we should add saved events
	}
}

void CalendarInterface::run() {
	// run until the user quits
	while (1) {
		// display the current display of the calendar
		currentDisplay->display();
		// display options to the user and respond to user input accordingly
		if (currentDisplay == tdl)
		{
			cout << "Add item: ai. Mark complete: mc. View calendar: vc." << endl;
		}
		else
		{
			cout << "Zoom out: out. Zoom in: in. Add event: ae. Jump to a specific date: jump.";
			cout << " Find an event by name: find. Save the current calendar to a file: save. Restore a calendar";
			cout << " from a file: restore. View ToDoList: tdl. Merge a calendar from a file: merge. Quit: q." << endl;
			if (levelOfCurrentDisplay == 4) {
				cout << "Edit an event: edit, Delete an event: delete" << endl;
			}
		}
		string in;
		cin >> in;
		if (in == "in") {
			if (typeOfBuilder == "incremental") {
				cout << "Index?" << endl;
				int index;
				index = read_int(cin);
				while (index < 0) {
					cout << "Please provide a valid index." << endl;
					index = read_int(cin);
				}
				shared_ptr<DisplayableComponent> temporary = currentDisplay;
				zoomIn(index);
				if (temporary == currentDisplay) {
				}
				else {
					levelOfCurrentDisplay = levelOfCurrentDisplay + 1;
				}
				
			}
			else {
				if (currentDisplay->children.size() > 0) {
					cout << "index? (0-" << currentDisplay->children.size() - 1 << ")" << endl;
					int index;
					index = read_int(cin);
					while (index < 0) {
						cout << "Please provide a valid index." << endl;
						index = read_int(cin);
					}
					shared_ptr<DisplayableComponent> temporary = currentDisplay;
					zoomIn(index);
					levelOfCurrentDisplay = levelOfCurrentDisplay + 1;
				}
				else {
					cout << "Nothing to zoom in on" << endl;
				}
			}
		}
		else if (in == "out") {
			zoomOut();
		}
		//For addition of an event.
		else if (in == "ae") {
			std::string name = "";
			string stringInput;
			unsigned int tmInput;
			std::tm finalWhen;
			cout << "Name the event." << endl;
			cin.ignore();
			getline(cin, name);
			cout << "In what year will the event take place?" << endl;
			if (typeOfBuilder == "incremental") {
				int year = read_int(cin);
				finalWhen.tm_year = year - CalendarComponent::BASEYEAR;
			}
			else {
				finalWhen.tm_year = promptForYear(cin) - CalendarComponent::BASEYEAR;
			}
			cout << "In what month (0-11) will the event take place?" << endl;
			finalWhen.tm_mon = promptForMonth(cin);
			cout << "On what day (0-" << CalendarComponent::days[finalWhen.tm_mon] - 1 << ") will the event take place?" << endl;
			finalWhen.tm_mday = promptForDay(cin, finalWhen.tm_mon) + 1;
			cout << "At what hour (0-23) will the event take place?" << endl;
			tmInput = read_int(cin);
			while (tmInput > CalendarComponent::HOURSINADAY - 1 || tmInput < 0) {
				cout << "Please provide a valid hour (0-23)." << endl;
				tmInput = read_int(cin);
			}
			finalWhen.tm_hour = tmInput;
			cout << "At what minute (0-59) will the event take place?" << endl;
			tmInput = read_int(cin);
			while (tmInput > CalendarComponent::MINUTESINANHOUR - 1 || tmInput < 0) {
				cout << "Please provide a valid minute(0-59).";
				tmInput = read_int(cin);
			}
			finalWhen.tm_min = tmInput;
			cout << "How many times will this event recur?" << endl;
			int rec;
			rec = read_int(cin);
			while (rec < 0) {
				cout << "Please provide valid (numeric) input." << endl;
				rec = read_int(cin);
			}
			cout << "How frequently will it occur?" << endl;
			int freq;
			freq = read_int(cin);
			while (freq < 0) {
				cout << "Please provide valid (numeric) input." << endl;
				freq = read_int(cin);
			}
			builder->buildEvent(this->cal, name, finalWhen, freq, rec);
		}
		//To jump to a specific year, month or day
		else if (in == "jump") {
			cout << "Provide the granularity of the date you would like to jump to (e.g. if you would like";
			cout << " to jump to a day provide \"day\", to a month provide \"month\" or to a year, provide";
			cout << " \"year\"." << endl;
			string granularity;
			cin >> granularity;
			while ((granularity != "year") && (granularity != "month") && (granularity != "day")) {
				cout << "Please provide a valid granularity (day, month or year)." << endl;
				cin >> granularity;
			}
			tm toJump;
			cin.ignore();
			if (granularity == "year") {
				if (typeOfBuilder == "incremental") {
					int year;
					cout << "What year would you like to jump to." << endl;
					year = read_int(cin);
					toJump.tm_year = year - Calendar::BASEYEAR;
					levelOfCurrentDisplay = 1;
				}
				else {
					cout << "What year would you like to jump to." << endl;
					toJump.tm_year = promptForYear(cin) - Calendar::BASEYEAR;
					levelOfCurrentDisplay = 1;
				}
			}
			if (granularity == "month") {
				if (typeOfBuilder == "incremental") {
					int year;
					cout << "What year would you like to jump to." << endl;
					year = read_int(cin);
					toJump.tm_year = year - Calendar::BASEYEAR;
					levelOfCurrentDisplay = 2;
				}
				else {
					cout << "What year would you like to jump to." << endl;
					toJump.tm_year = promptForYear(cin) - Calendar::BASEYEAR;
				}
				cout << "What month (0-11) would you like to jump to." << endl;
				toJump.tm_mon = promptForMonth(cin);
				levelOfCurrentDisplay = 2;
			}
			if (granularity == "day") {
				if (typeOfBuilder == "incremental") {
					int year;
					cout << "What year would you like to jump to." << endl;
					year = read_int(cin);
					toJump.tm_year = year - Calendar::BASEYEAR;
					levelOfCurrentDisplay = 3;
				}
				else {
					cout << "What year would you like to jump to." << endl;
					toJump.tm_year = promptForYear(cin) - Calendar::BASEYEAR;
				}
				cout << "What month (0-11) would you like to jump to." << endl;
				toJump.tm_mon = promptForMonth(cin);
				cout << "What day (0-" << CalendarComponent::days[toJump.tm_mon] - 1 << ") would you like to jump to." << endl;
				toJump.tm_mday = promptForDay(cin, toJump.tm_mon) + 1;
				levelOfCurrentDisplay = 3;
			}
			currentDisplay = builder->getComponentByDate(this->cal, toJump, granularity);
		}
		//To find a specific event
		else if (in == "find") {
			string name;
			cout << "What event do you wish to search for?" << endl;
			cin >> name;
			auto iterator = this->cal->mapOfEvents.equal_range(name);
			if (iterator.first == iterator.second) {
				cout << "Event not found." << endl;
			}
			else {
				cout << "Events with that name have been found at the following dates." << endl;
				int index = 0;
				map<int, std::shared_ptr<DisplayableComponent>> mapToFindIndex;
				for (auto i = iterator.first; i != iterator.second; ++i) {
					DisplayableEvent * de = dynamic_cast<DisplayableEvent *>(i->second.get());
					cout << "Index: " << index << " " << de->daysoftheweek[de->dateInfo.tm_wday] << ", " << CalendarComponent::months[de->dateInfo.tm_mon];
					cout << ", " << de->dateInfo.tm_mday << ", " << de->dateInfo.tm_year + de->BASEYEAR;
					cout << ". At " << de->dateInfo.tm_hour << ":" << de->dateInfo.tm_min << endl;
					std::pair<int, std::shared_ptr<DisplayableComponent>> p(index, i->second);
					mapToFindIndex.insert(p);
					index = index + 1;
				}
				cout << "Please choose what event to display based on its associated index." << endl;
				unsigned int chosenEvent = read_int(cin);
				while (chosenEvent > mapToFindIndex.size() - 1 || chosenEvent < 0) {
					cout << "Provide a valid index associated with an event." << endl;
					chosenEvent = read_int(cin);
				}
				auto mapIterator = mapToFindIndex.begin();
				while (mapIterator->first != chosenEvent) {
					mapIterator++;
				}
				
				currentDisplay = mapIterator->second;
				levelOfCurrentDisplay = 4;
				
				
			}
		}
		else if (in == "edit"&&levelOfCurrentDisplay==4) {
			bool repeat = false;
			int indexNum = 0;
			while (repeat) {
				try {
					cout << "select and indice to edit the event" << endl;
					string index;
					cin >> index;
					int indexNum = stoi(index);
					if (indexNum >= currentDisplay->children.size()) {
						throw -1;
					} 
				}
				catch (...) {
					repeat == true;
				}
			}
			DisplayableEvent* de = dynamic_cast<DisplayableEvent*>(currentDisplay.get());
			cout << "please enter year, month, day, hour, minute, name" << endl;
			string thingToEdit;
			cin >> thingToEdit;
			if (thingToEdit == "year") {
				string yearInput;
				while (true) {
				cout << "enter a year after this current date" << endl;
				cin >> yearInput;
					try {
						int year = stoi(yearInput);
						if (year < de->BASEYEAR) {
							throw -1;
						}
						de->dateInfo.tm_year = year-(de->BASEYEAR);
						int m = de->dateInfo.tm_mon;
						int day = de->dateInfo.tm_mday;
						int y = de->dateInfo.tm_year;
						static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
						y -= m < 3;
						int weekday = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + day) % 7;
						de->dateInfo.tm_wday = weekday;
						break;
					}
					catch (...) {
						continue;
					}
				}
				
			}
			else if (thingToEdit == "month") {
				string monthInput;
				while (true) {
					cout << "enter a month betwene 0-11" << endl;
					cin >> monthInput;
			
					try {
						int month = stoi(monthInput);
						if (month > 11 || month < 0) {
							continue;
						}
						de->dateInfo.tm_mon= month;
						int day = de->dateInfo.tm_yday;
						int m = de->dateInfo.tm_mon;
						int y = de->dateInfo.tm_year;
						static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
						y -= m < 3;
						int weekday = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + day) % 7;
						de->dateInfo.tm_wday = weekday;
						break;
					}
					catch (...) {
						continue;
					}
				}
			}
			else if (thingToEdit == "day") {
				string dayInput;
				while (true) {
					cout << "enter a day betwene 1-31" << endl;
				cin >> dayInput;
				
					try {
						int day = stoi(dayInput);
						if (day < 1 || day>31) {
							continue;
						}
						de->dateInfo.tm_mday = day;
						int m = de->dateInfo.tm_mon;
						int y = de->dateInfo.tm_year;
						static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
						y -= m < 3;
						int weekday = (y + y / 4 - y / 100 + y / 400 + t[m - 1] + day) % 7;
						de->dateInfo.tm_wday = weekday;
						break;
					}
					catch (...) {
						continue;
					}
				}
			}
			else if (thingToEdit == "hour") {
				string hourInput;
				while (true) {
					cout << "enter a day betwene 0-23" << endl;;
				cin >> hourInput;
				
					try {
						int hour = stoi(hourInput);
						if (hour > 23 || hour < 0) {
							continue;
						}
						de->dateInfo.tm_hour = hour;
						break;
					}
					catch (...) {
						continue;
					}
				}
			}
			else if (thingToEdit == "minute") {
				string minuteInput;
				while (true) {
					cout << "enter a minute betwene 0-59" << endl;
				cin >> minuteInput;
					try {
						int min=stoi(minuteInput);
						if (min > 59 || min < 0) {
							continue;
						}
						de->dateInfo.tm_min = min;
						break;
					}
					catch (...) {
						continue;
					}
				}
			}
			else if (thingToEdit == "name") {
				string nameInput;
				cout << "enter a new name" << endl;
				cin >> nameInput;
				de->name = nameInput;
				continue;
			}
			else {
				cout << "non - valid input" << endl;
				continue;
			}
			builder->buildEvent(this->cal, de->name, de->dateInfo, 0, 0);
			weak_ptr<DisplayableComponent> parent2 = (de->getParent());
			shared_ptr<DisplayableComponent> parent = parent2.lock();
			vector<std::shared_ptr<DisplayableComponent> > v = (parent->children);
			for (auto i = 0;i < v.size();i++) {
				if (dynamic_cast<DisplayableEvent*>(v.at(i).get()) == de) {
					//parent->removeComponent(i);
					//v.at(i) = nullptr;
					v.erase(v.begin() + i);
					currentDisplay = parent;
					levelOfCurrentDisplay--;
				}
			}
			parent->children = v;
			map<string, shared_ptr<DisplayableComponent>>::iterator iter2;
			for (map<string,shared_ptr<DisplayableComponent>>::iterator iter = cal->mapOfEvents.begin();iter != cal->mapOfEvents.end();iter++) {
				if (dynamic_cast<DisplayableEvent*>((iter->second.get())) == de) {
					iter2 = iter;
				}
			}
			cal->mapOfEvents.erase(iter2);
		
			
			
		}
		else if (in == "delete"&&levelOfCurrentDisplay == 4) {
			DisplayableEvent* de = dynamic_cast<DisplayableEvent*>(currentDisplay.get());
			weak_ptr<DisplayableComponent> parent2 = (de->getParent());
			shared_ptr<DisplayableComponent> parent = parent2.lock();
			vector<std::shared_ptr<DisplayableComponent> > v = (parent->children);
			for (auto i = 0;i < v.size();i++) {
				if (dynamic_cast<DisplayableEvent*>(v.at(i).get()) == de) {
					//parent->removeComponent(i);
					//v.at(i) = nullptr;
					v.erase(v.begin() + i);
					currentDisplay = parent;
					levelOfCurrentDisplay--;
				}
			}
			parent->children = v;

			map<string, shared_ptr<DisplayableComponent>>::iterator iter2;
			for (map<string, shared_ptr<DisplayableComponent>>::iterator iter = cal->mapOfEvents.begin();iter != cal->mapOfEvents.end();iter++) {
				if (dynamic_cast<DisplayableEvent*>((iter->second.get())) == de) {
					iter2 = iter;
				}
			}
			cal->mapOfEvents.erase(iter2);
			
		}
		/*
		//syle of calendar display. This is what the file will look like. 

			calendar name(my cal)
			years(5)
			-
			name(my event)
			year(2019)
			month(10)
			day(5)
			hour(10)
			minute(50)
			second(5)
			-
			name(my event)
			year(2019)
			month(10)
			day(5)
			hour(10)
			minute(50)
			second(5)
			-
			...
			//note all events will recur 0 times
			*/
		else if (in == "save") {
			string NameOfFile;
			cout << "Please provide the name of the file you wish to save your calendar to." << endl;
			cin >> NameOfFile;
			ofstream myfile;
			try {
				myfile.open(NameOfFile);
				myfile << this->cal->name;
				myfile << "\n";
				myfile << this->cal->yearsToHold;
				myfile << "\n";
				for (pair<string, shared_ptr<DisplayableComponent>> iter : this->cal->mapOfEvents) {
					DisplayableEvent* dispEventIter = dynamic_cast<DisplayableEvent *>(iter.second.get());
					myfile << "-";
					myfile << "\n";
					myfile<<dispEventIter->name;
					myfile << "\n";
					myfile << (dispEventIter->dateInfo.tm_year)+(cal->BASEYEAR);
					myfile << "\n";
					myfile << dispEventIter->dateInfo.tm_mon;
					myfile << "\n";
					myfile << dispEventIter->dateInfo.tm_mday;
					myfile << "\n";
					myfile << dispEventIter->dateInfo.tm_hour;
					myfile << "\n";
					myfile << dispEventIter->dateInfo.tm_min;
					myfile << "\n";
					myfile << dispEventIter->dateInfo.tm_sec;
					myfile << "\n";

				}
				//loop over events to add them
				myfile.close();
			}
			catch (...) {
				cout << "could not write to the given file" << endl;
			}
			
		}
		else if (in == "restore") {
			string NameOfFile;
			cout << "Please provide the name of the file you wish to restore your calendar from." << endl;
			cin >> NameOfFile;
			ifstream FileStream;
			FileStream.open(NameOfFile, ifstream::in);
			if (!FileStream.is_open()) {
				cout << "Could not open the provided file." << endl;
			}
			else {
				string name;
				int yearsToHold;
				string yearString;
				try {

					getline(FileStream, name);
					getline(FileStream, yearString);
					yearsToHold = stoi(yearString);
					typeOfBuilder;
					CalendarInterface ci("incremental", "test", 4, std::ifstream());
					if (typeOfBuilder == "incremental") {
						CalendarInterface temp("savedIncremental", name, yearsToHold, FileStream);
						ci = temp;
					}
					else {
						CalendarInterface temp("saved", name, yearsToHold, FileStream);
						ci = temp;
					}
					this->cal = ci.cal;
					this->builder = ci.builder;
					this->currentDisplay = ci.currentDisplay;
					levelOfCurrentDisplay = 0;
				}
				catch(...){
					cout << "Could not read calendar from file." << endl;
				}
				//FROM HERE WE SHOULD READ IN FROM THE FILE INTO THE NAME AND YEARSTOHOLD AND FORWARD TO THE
				//CALENDARINTERFACE CONSTRUCTOR.
			}
		}
		else if (in == "tdl")
		{
			currentDisplay = tdl;
			// tdl->display();
		}
		// Adds an element to the TODO list by taking in data from user
		else if (in == "ai")
		{
			std::string name = "";
			string stringInput;
			unsigned int tmInput;
			std::tm finalWhen;
			cout << "Name the task." << endl;
			cin.ignore();
			getline(cin, name);
			cout << "In what year is the task due?" << endl;
			if (typeOfBuilder == "incremental") {
				int year = read_int(cin);
				finalWhen.tm_year = year - CalendarComponent::BASEYEAR;
			}
			else {
				finalWhen.tm_year = promptForYear(cin) - CalendarComponent::BASEYEAR;
			}
			cout << "In what month (0-11) is the task due?" << endl;
			finalWhen.tm_mon = promptForMonth(cin);
			cout << "On what day (0-" << CalendarComponent::days[finalWhen.tm_mon] - 1 << ") is the task due?" << endl;
			finalWhen.tm_mday = promptForDay(cin, finalWhen.tm_mon) + 1;
			cout << "At what hour (0-23) is the task due?" << endl;
			tmInput = read_int(cin);
			while (tmInput > CalendarComponent::HOURSINADAY - 1 || tmInput < 0) {
				cout << "Please provide a valid hour (0-23)." << endl;
				tmInput = read_int(cin);
			}
			finalWhen.tm_hour = tmInput;
			cout << "At what minute (0-59) is the task due?" << endl;
			tmInput = read_int(cin);
			while (tmInput > CalendarComponent::MINUTESINANHOUR - 1 || tmInput < 0) {
				cout << "Please provide a valid minute(0-59).";
				tmInput = read_int(cin);
			}
			finalWhen.tm_min = tmInput;
			tdl->addComponent(name, finalWhen, false);
		}
		// Asks which element the user would like to mark complete, and marks it complete
		else if (in == "mc")
		{
			int highestIndex = tdl->children.size() - 1;
			cout << "Select the component that you would like to mark complete, ranging from 0 to " << highestIndex << "." << " " 
				<< endl;
			string input;
			int index;
			getline(cin, input);
			stringstream stream(input);
			stream >> index;
			while (index < 0 || index > highestIndex)
			{
				cout << "Please provide valid index (0-" << highestIndex << ")" << endl;;
				getline(cin, input);
				stringstream stream(input);
				stream >> index;
			}
			Task * tsk = dynamic_cast<Task *>(tdl->children[index].get());
			tsk->setComplete();
		}
		// switches mode from the TODO list to the calendar
		else if (in == "vc")
		{
			currentDisplay = cal;
			levelOfCurrentDisplay = 0;
		}
		else if (in == "merge") {
			string NameOfFile;
			cout << "Please provide the name of the file you wish to restore your calendar from." << endl;
			cin >> NameOfFile;
			ifstream FileStream;
			FileStream.open(NameOfFile, ifstream::in);
			if (!FileStream.is_open()) {
				cout << "Could not open the provided file." << endl;
			}
			else {
				string name;
				int yearsToHold;
				string yearString;
				try {

					getline(FileStream, name);
					getline(FileStream, yearString);
					yearsToHold = stoi(yearString);
					merge(name, FileStream);
				}
				catch (...) {
					cout << "Could not read calendar from file." << endl;
				}
				//FROM HERE WE SHOULD READ IN FROM THE FILE INTO THE NAME AND YEARSTOHOLD AND FORWARD TO THE
				//CALENDARINTERFACE CONSTRUCTOR.
			}
		}
		else if (in == "q") {
			break;
		}
	} 
}

void CalendarInterface::zoomIn(unsigned int index) {
	if (typeOfBuilder == "incremental") {
		auto iter = currentDisplay->mapOfChildren.begin();
		bool zoomExists = false;
		while (iter != currentDisplay->mapOfChildren.end()) {
			if (iter->first == index) {
				zoomExists = true;
				currentDisplay = iter->second;
				break;
			}
			iter++;
		}
		if (zoomExists == false) {
			if (levelOfCurrentDisplay == 0) { //It is currently displaying a calendar
				CalendarComponent * calendar = dynamic_cast<CalendarComponent*>(currentDisplay.get());
				std::tm temporary = calendar->dateInfo;
				temporary.tm_year = calendar->dateInfo.tm_year + index;
				if (temporary.tm_year < 0) {
					cout << "The input provided was broken" << endl;
				}
				else {
					currentDisplay = builder->getComponentByDate(this->cal, temporary, "year");
				}

			}
			else if (levelOfCurrentDisplay == 1) { //Is currently displaying a year
				if (index < 0 || index>11) {
					cout << "A year must have 0-11 months, the index provided is not valid." << endl;
					levelOfCurrentDisplay = 1;
				}
				else {
					CalendarComponent * year = dynamic_cast<CalendarComponent*>(currentDisplay.get());
					year->dateInfo.tm_mon = index;
					currentDisplay = builder->getComponentByDate(this->cal, year->dateInfo, "month");
				}
			}
			else if (levelOfCurrentDisplay == 2) { //Is currently displaying a month
				CalendarComponent * month = dynamic_cast<CalendarComponent*>(currentDisplay.get());
				int daysOfCurrentMonth = Calendar::days[month->dateInfo.tm_mon];
				if (index > daysOfCurrentMonth || index < 1) {
					cout << "Not a valid index to zoom into, the current month has " << daysOfCurrentMonth;
					cout << " days." << endl;
					levelOfCurrentDisplay = 2;
				}
				else {
					month->dateInfo.tm_mday = index;
					currentDisplay = builder->getComponentByDate(this->cal, month->dateInfo, "day");
				}
			}
			else if (levelOfCurrentDisplay == 3) { //is a day
				shared_ptr<DisplayableComponent> temp = currentDisplay->getChild(index);
				// if the child exists, update the current display to the child. Otherwise, the current view remains the same
				if (temp != nullptr) {
					currentDisplay = temp;
				}
			}
			else {
				cout << "The index provided was not valid or there is nothing to zoom in on." << endl;
			}
		}
	}
	else {
		shared_ptr<DisplayableComponent> temp = currentDisplay->getChild(index);
		// if the child exists, update the current display to the child. Otherwise, the current view remains the same
		if (temp != nullptr) {
			currentDisplay = temp;
		}
	}
}

void CalendarInterface::zoomOut() {
	// if the current display has a parent, update current display to the parent
	if (currentDisplay->getParent().lock() != nullptr) {
		// create a shared_ptr from a weak_ptr, contributing to the objects reference count
		currentDisplay = currentDisplay->getParent().lock();
		levelOfCurrentDisplay = levelOfCurrentDisplay - 1;
	}
}

//Functions to prompt user for year, month, and day
int CalendarInterface::promptForYear(std::istream & c) {
	unsigned int year;
	year = read_int(c);
	while (year < (this->cal->currentDate.tm_year + CalendarComponent::BASEYEAR) || year >(cal->yearsToHold + (this->cal->currentDate.tm_year + CalendarComponent::BASEYEAR)) - 1) {
		cout << "Please provide a valid year, an event must take place in the current or a future ";
		cout << "year." << endl;
		year = read_int(c);
	}
	return year;
}
int CalendarInterface::promptForMonth(std::istream & c) {
	unsigned int month;
	month = read_int(c);
	while (month > (CalendarComponent::MONTHS - 1) || month < 0) {
		cout << "Please provide a valid month (0-11)." << endl;
		month = read_int(c);
	}
	return month;
}
int CalendarInterface::promptForDay(std::istream & c, unsigned int month) {
	unsigned int day;
	day = read_int(c);
	while (day < 0 || ((day) > CalendarComponent::days[month] - 1)) { 
		cout << "Please provide a valid day (0-" << CalendarComponent::days[month] - 1;
		cout << ")." << endl;
		day = read_int(c);
	}
	return day;
}

void CalendarInterface::merge(string calName, std::ifstream & ifs) {
		std::string eventName = "";
		std::string line;
		std::tm finalWhen;
		for (std::pair<string, shared_ptr<DisplayableComponent>> iter : this->cal->mapOfEvents) {
			DisplayableEvent* de = dynamic_cast<DisplayableEvent*>(iter.second.get());
			de->name = (this->cal->name) + ":: " + (de->name);
		}
		try {
			while (getline(ifs, line)) {
				if (line != "-") {
					break;
				}
				getline(ifs, eventName);
				eventName = calName + ":: " + eventName;
				getline(ifs, line);
				finalWhen.tm_year = stoi(line)-(cal->BASEYEAR);
				getline(ifs, line);
				finalWhen.tm_mon = stoi(line);
				getline(ifs, line);
				finalWhen.tm_mday = stoi(line);
				getline(ifs, line);
				finalWhen.tm_hour = stoi(line);
				getline(ifs, line);
				finalWhen.tm_min = stoi(line);
				getline(ifs, line);
				finalWhen.tm_sec = stoi(line);
				cout << "attempted to build event" << endl;
				cout << eventName << endl;
				this->builder->buildEvent(this->cal, eventName, finalWhen, 0, 0);
			}
		}
		catch (...) {
			throw - 1;
		}
		//here we should add saved events
	
}

