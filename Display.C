// NCurses Display for Robots
// Josh Villbrandt
// Sep 21, 2009

#include "Display.H"

Display::Display(std::string title)
{
	initscr();
	//cbreak();
	//keypad(stdscr, TRUE);
	getmaxyx(stdscr, row, col);
	
	mvprintw(0, (col-title.size())/2, "%s", title.c_str());
}

Display::~Display()
{
	endwin();
}

void Display::update(std::string name, double value) {
	names.push_back(name);
	std::vector<std::string>::iterator iteratorOfName = lower_bound(names.begin(), names.end(), name);
	int indexOfName = iteratorOfName - names.begin();
	
	int row = 2 + indexOfName;
	std::ostringstream lineStream;
	lineStream << name << ": " << value;
	std::string line = lineStream.str();
	
	mvprintw(row, 0, "%s", line.c_str());
	clrtoeol();
}

void Display::log(std::string str) {
	// should use multiple NCurses windows for this
}
