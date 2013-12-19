// NCurses Display for Robots
// Josh Villbrandt
// Sep 21, 2009

#ifndef DISPLAY_H
#define DISPLAY_H

#include <curses.h>
#include <string>
#include <vector>
#include <sstream>

class Display
{
private:
	std::vector<std::string> names;
	int row, col;
	
public:
	Display(std::string title);
	~Display();
	
	void update(std::string name, double value);
	void log(std::string str);
};

#endif
