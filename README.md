# Calendar

The following project was done in collaboration with: Matthew Schwartz and Reid Parisi, with starter code provided by Dr. Jonathan Shidal


// answer the following questions
1. The following point talks about adding events to the calendar. It discusses separation of concerns and the classes responsible for the user interface of the calendar.
The class responsible for the user interface calendar is the CalendarInterface class, which has 3 member
variables, which are all shared_ptr, one of type calendar (called cal), one of type CalendarBuilder (called 
builder), and one of type DisplayableComponent (called currentDisplay). It has 3 public functions: a constructor
(which initializes it's 3 member variables to nullptr) which takes in 3 strings; it reads in the first string and
if it == "full" it forwards the request to initialize the cal member variable to a buildCalendar function, and 
it then sets the member variable, currentDisplay, equal to cal. It has 3 void functions, zoomIn, zoomOut, and 
run.

The construction of the calendar is done by the CalendarBuilder class and its derived class, FullCalendarBuilder.
The representation is managed by the DisplyableComponent and all of its derived classes (CalendarComponent,
DisplayableYear, DisplayableMonth, and DisplayableDay).

The user should only be allowed to interact with the interface; the constructor of CalendarInterface takes 3
paramenters: the type of builder being used, the name of the calendar, and the years to hold. The run() function
displays the calendar and shows available options for the user, and when inputed forwards requests to the builder
classes (e.g. to build events), other CalendarInterface classes (such as ZoomIn or ZoomOut which in term 
forward requests to displayable classes), or other displayable classes.

We created a class called DisplayableEvent which inherits publicly from CalendarComponent; we also changed
the addComponent function for the DisplayableDay (because it is no longer a leaf) to hold all the Events
as children. An event has all the member variables that a CalendarComponent has since it inherits publicly from
it, but we also added a name member variable to store the name of the event. So, the constructor of an event takes
in 3 parameters: a tm to represent when the event will take place, a shared_ptr<DisplayableComponent> which will
be its parent, and a string to represent the name of the event. The constructor forwards the date and parent
variables to CalendarComponent and it makes the name of the event = name provided to the constructor. 



2. Describes how the calendar was made interactive. We describe how we implemented event searches, which objects in the project understand the structure of the calendar, and which object is responsible for jumping to a specific object in the calendar.
We stored events in a multimap which maps the event name (key) to the event, and since there can be multiple
events with the same name that is why we store it in a multimap. The multimap of events is declared in the
Calendar.h file because this class stores global member variables for our calendar construction and the
calendar is accessible from the CalendarInterface. When the user inputs the name of an event to look for
we create an iterator and display all events to the user with an index; we map these events to the index (unique
index) so the user can choose which event to display based on its index.


The builder understands the structure of the calendar, so to jump to a specific date we prompt the user for some
granularity (year, month or day) and then prompt for the respective year month and day they wish to jump to and
se the current display of the calendar (delcared in the CalendarInterface class) and forward the request to the
builder to getComponentByDate which will get the respective year, month, and/or day prompted in by the user.
The FullCalendarBuilder has access to all DisplayableComponents and hence can find a specific DisplayableComponent
object (weather this be a Displayable Day, Month or Year).



I designed saving and restoring by saving the calendar in a specific format that can them be interpretted as
a set of events, a calendar name, and the years to hold. The format is as follows:

		//style of calendar display. This is what the file will look like. 

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
			
This format can then be understood when restoring a calendar by itterating and adding events to the calendar. 
All events information listed above, the number of years in the calendar and the name of the calendar
needs to be stored. 


3. We talk about the display; how particular components of the calendar should be displayed relative to the current object being displayed and how this was implemented.
We changed the display() functions of all CalendarComponents. The way it was given to us it would forward all
requests to its children, so the entire calendar would be displayed. We changed this so it would only display 
the specifications in the instructions by getting certain paramenters from the current displayed object and its
children or childrens children.



Did you make any changes to the interfaces provided in the starter code? If so, how easy was it to 
refactor the code to match this change? 
We added global options to the user menu (such as adding an event by typing "ae") and, in the ToDoList section of the lab, 
edited the interface so that you could transfer between options for the calendar and for the ToDoList. Due to the fact that the 
starter code was based on if statements, this was pretty easy: we just added more if statements for the new options. 
Aside from these changes, we did not make any other changes to the starter code. Hence, we did not have to refactor the code 
to match changes.




How does your implementation maintain flexibility and extensibility of the project? Compare this with other designs you considered.
Our implementation maintains flexibility because by utilizing the interface, it is easy to add and delete options 
that we need or don't need. Our use of the builder method (through incremental builder) helps us construct objects in small steps, 
thus allowing us to adapt the construction of our calendar. Furthermore, our use of the strategy pattern helps make our code 
extensibile because it allows us to choose which algorithms we want to use at certain parts of the code. As a result, we can 
easily add other choices of algorithm to extend the functionality of our code. We did not, however, use the decorator pattern , which 
would have also allowed us to add new aspects to our objects and thus in a sense "decorate" them. Nonetheless, our program was 
quite flexible and extensible using the interface and design patterns that we did.





3.1: Merging calendars
Describe the benefits of the Decorator pattern? If you used the decorator pattern, how was it applied to 
support merging calendars?

I did not implement the decorator pattern. I iterate through the calendar to be merged and add the events to the
calendar using the format of stored calendar. Then the two calendars are merged into one calendar with access
to all the events. The decorator pattern has the advantage of being able to treat all elements the same but
some forward events to the decorator for additional functionality. I also add the name of the calendar that
the events are being merged from onto the name of the event so that the event can be identified by calendar.






If you chose not to use the decorator pattern, how does your design compare with it when 
considering flexibility and extensibility of the design? 

My design is flexible and extensible as you can always add an event from another calendar and tell where it comes
from. My pattern allows for you to retain knoledge of the set of calendars if you say merge then store then restore,
which a decorator pattern might not depending on how it was stored. However, the loss with this is that because
the name is changed to search for the new event you must know the full name that would be displayed. 





3.2 The following describes the implementation, structure, and behavior of our TODO list.
Describe your implementation? How does it support easy extensibility of the project in the future?
How is the singleton pattern used to enforce only a sigle TODO list instance?

We essentially have two "modes" - one mode that displays the calendar options, and one that displays the TODO list options, 
and in order to create the ToDoList I use a ToDoList class with a subclass of Task, where all the Task objects are items on 
the ToDoList. This allows us to make our implementation extensible - as we can easily add new options to our interface. 
In addition, in order to sort elements, we created a lessThan boolean function to compare the dates and times of two tasks, and then 
an insertionSort function that sorts the tasks accordingly, before displaying them.
The singleton pattern is enforced because it is a private member variable of our interaface, and will only be created
once per calendar. The user has no access and can not create a ToDoList; ToDoLists are automatically instantiated
with the creation of a calendar and the user cannot access it.





3.3 The following describes the structure and tests for the incremental builder calendar.
What challenges did you run into while implementing this? Were any changes to the builder interface required? 
How does using the builder pattern contribute to easily being able to change how an object(calendar in this case)
is represented?
The biggest challenge was figuring out how to store/represent the children of the CalendarComponents and all the 
modifications that would go into changing how the children are stored; for the full builder they are represented 
in vectors but if I wanted to make an IncrementalBuilder and build very far appart components if I kept the
representation as a vector then I would have to initialize and push in a bunch of nullptr which would partially
defeat the purpose of saving space and memory. I store the children of components when the calendar is build
incrementally in a map, which maps the indeces to the displayable component (e.g. the year 2019 would be mapped
in the child map of the calendar with a key of 0, 2060 would be mapped with a key of 41, and so on and so forth
with the children of years and months; note: the children of days, events were kept as children in vectors).
I then adapted all the functions in the fullCalendarBuilder to adapt to map children (e.g. the jump to a specific
date checks if the key for a year exists, if it doesn't it creates a year and stores it in the mapOfChildren of
the calendar, and so on and so forth). 
I made no changes to the builder interface since it defined all the necessary functions for building a calendar
and looking for dates.
The builder pattern helped me because I simply had to redifine the functions that were declared and the creation
of the objects was dealt with elsewhere (the builder was in charge of connecting the larger more complex object
that is the calendar).
Changes were done to the calendar interface (to the ZoomIn function) to make sure that we could zoom
into a date that did not exist yet if the incremental builder was being used. For this I created a member variable
of the interface which knows if we used a Full or Incremental builder. I also added a member variable to know
what current display is (what level its at, e.g. Calendar, Year, etc.) This was added to know if it is possible to
zoom into a specific index given a current display (e.g. you could only zoom into 0-11 if the current display
is a year). The variable was initialized to 0 when the calendar is built, set to 4 if find event ran succesfully
set to 1 if they try to jump to a specific date and the granularity is year, to 2 if its month and to 3 if its day
it is incremented if you zoom in successfully and deducted if you zoom out successfully.
Tests for the incremental builder:
Given the following main:
	CalendarInterface ci("incremental", "test", 4, std::ifstream());
	ci.run();
Initially it displayed as following
	Calendar: test
	Index: 0 2019
	Zoom out: out. Zoom in: in. Add event: ae. Jump to a specific date: jump. 
	Find an event by name: find. Save the current calendar to a file: save. 
	Restore a calendar from a file: restore. Merge a calendar from a file: merge. Quit: q.
which is what we want (only 2019 was created), and when zooming in to the following:
Year:
	in
	Index?
	0
			Year 2019:
					Index: 0 January
	Zoom out: out. Zoom in: in. Add event: ae. Jump to a specific date: jump. 
	Find an event by name: find. Save the current calendar to a file: save. 
	Restore a calendar from a file: restore. Merge a calendar from a file: merge. Quit: q.
which shows us that only January was created
Month:
	in
	Index?
	0
					January:
	Sunday    Monday    Tuesday   Wednesday Thursday  Friday    Saturday
						1



	Zoom out: out. Zoom in: in. Add event: ae. Jump to a specific date: jump. 
	Find an event by name: find. Save the current calendar to a file: save. 
	Restore a calendar from a file: restore. Merge a calendar from a file: merge. Quit: q.
which shows us that only the first day was constructed.
Day:
	Index?
	1
							Tuesday 1/1/2019
	Zoom out: out. Zoom in: in. Add event: ae. Jump to a specific date: jump. 
	Find an event by name: find. Save the current calendar to a file: save. 
	Restore a calendar from a file: restore. Merge a calendar from a file: merge. Quit: q.
which shows us the day with no events in it.
In that same calendar when trying to jump into a specific date (March 3rd, 3000) the follwoing was produced:
When displaying the Calendar:
	Calendar: test
	Index: 0 2019
	Index: 981 3000
When displaying the year (at index 981):
        Year 3000:
                Index: 2 March
which shows that only march was created for that year.
When displaying the month (at index 2):
                March:
Sunday    Monday    Tuesday   Wednesday Thursday  Friday    Saturday
                              3



which shows us that only the 3rd was created.
And for the day: 
	Monday 3/3/3000
which shows it has no events.
Adding events works in the same way as jumping to an event. I created an event starting on January 1st 2020
and recurring every 10 days 5 times, and the display for that month (January) looks as follows:
                January:
Sunday    Monday    Tuesday   Wednesday Thursday  Friday    Saturday
                              1*
                                                            11*

                    21*
                                                  31*
Which shows us that only the necessary days which held events were created.


GENERAL TESTS FOR THE FIRST PART OF THE LAB (E.G. ADDING EVENTS, CHANGING THE DISPLAY, GOING TO A SPECIFIC
DATE, SAVING AND RESTORING FROM A FILE; NOTE: USING THE FULL CALENDAR BUILDER):
Givent the following main (after adding events, being able to jump to specific dates and changing the dispalys)
	CalendarInterface ci("full", "test", 3,std::ifstream());
	ci.run();
The first produced output was:
	Calendar: test
	Index: 0 2019
	Index: 1 2020
	Index: 2 2021
	Zoom out: out. Zoom in: in. Add event: ae. Jump to a specific date: jump. Find an event by name: find. 
	Save the current calendar to a file: save. Restore a calendar from a file: restore.
Which is what we want as described by the instructions.
We then added an event called "Hello" which started on Jan 1st, 2019 at 06:10 and recurred 5 times, every 10 days.
We then zoomed into the year (2019) and the following output was produced:
        Year 2019:
                Index: 0 January
                        1/1/2019 06:10 Hello
                        1/11/2019 06:10 Hello
                        1/21/2019 06:10 Hello
                        1/31/2019 06:10 Hello
                Index: 1 February
                        2/10/2019 06:10 Hello
                        2/20/2019 06:10 Hello
                Index: 2 March
                Index: 3 April
                Index: 4 May
                Index: 5 June
                Index: 6 July
                Index: 7 August
                Index: 8 September
                Index: 9 October
                Index: 10 November
                Index: 11 December
Which is the desired output as described by the instructions.
If we zoomed into january the following was produced:
                January:
Sunday    Monday    Tuesday   Wednesday Thursday  Friday    Saturday
                    1*        2         3         4         5

6         7         8         9         10        11*       12

13        14        15        16        17        18        19

20        21*       22        23        24        25        26

27        28        29        30        31*
Which is the desired output as stated in the instructions.
We then added 2 events on January 1st, one called LaterHello at 20:05 and one called EarlierHello at 02:00 and
when zooming into the 1st the following was displayed:
                        Tuesday 1/1/2019
                                02:00 EarlierHello
                                06:10 Hello
                                20:05 HelloLater
which is what we wanted (with the events sorted).

Here are errors we encountered:
-We had the same signature for two different overrides of the less than operator, causing an error. To resolve this error, we made 
one of the overrides into a lessThan method and then did a manual sort (although there may have been an easier fix).
-We frequently encountered errors by accidentally forgetting to add "std" before strings and shared pointers. We easily fixed these 
errors by adding std.
-We accidentally set currentDisplay to null_ptr, leading to errors. We fixed this by making changes so that this would never occur.
-We had vector out of bounds errors in writing the insertionSort() method. We resolved this method by modifying our while loop.

We broke up the work as follows:
Mauricio:
-Worked on the interface for the calendar
-Worked on the display for the calendar
-Created the add event functionality
-Did the incremental builder
-Overrode the "less than" operator
-Performed tests and touched up final details
Reid:
-Worked on the display for the calendar
-Created the edit/delete event functionality
-Created the save/restore functions
-Created the merge functionality
Matthew:
-Worked on the interface functionality
-Did the TODO list functionality
-Worked on overriding the "less than" operator
