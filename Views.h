#ifndef VIEWS_H
#define VIEWS_H
/* *** View hierarchy ***
The View hierarchy is implemented with 7 classes total, a View base class,
which provides the interface for all the derived classes, and is
located in View.h and View.cpp
In this file are the other 6 classes that make up the majority of the
class hierarchy.
There are two views that are derived directly from View: Grid and Values.
Grid represents the top down grid-based view, its responsibilities include
keeping track of the sim_objs which are abstracted away to just std::pairs of
strings and points. In addition it provides the functionality to its derived
classes to populate and display its data to users.

Derived from Grid are Map and Local.
Both Map and Local are responsible for encapsulating data about their
origins, size and scale. For Map, this data is subject to change at the hands
of the user, but for Local, this information is either updated automatically or
not able to be changed at all.

Map	is the original view from project 4, it contains functionality to display
an area of land that can be adjusted by the user. It therefore overloads the setter
functions and the draw function

Local is the view that is tied to a specific agent it therefore updates it's
origin every time the agent moves and doesn't expose any of the setter functionality
to its user.

On the other side of the view hierarchy we have the Values class, from which
are derived both Health and Amounts view.
Values encapsulates data regarding sim_objects in our simulation, abstracted
away to just pairs of strings and doubles. When asked to draw, it will output a
nicely readable list of its data.

Derived from Values are Health and Amounts

Health stores information regarding the health of specific agents.

Amounts stores information regarding the amount of food that agents or structures
currently have on hand.


Usage: 
1. Call the update_location function with the name and position of each object
to be plotted. If the object is not already in the View's memory, it will be added
along with its location. If it is already present, its location will be set to the 
supplied location. If a single object changes location, its location can be separately
updated with a call to update_location. 
2. Call the update_remove function with the name of any object that should no longer
be plotted. This must be done *after* any call to update_location that 
has the same object name since update_location will add any object name supplied.
3. Call the draw function to print out the map.
*/
#include <map>
#include <string>
#include <vector>
#include "Geometry.h"
#include "View.h"

// Grid is an abstract base class to provide an implementation for derived classes
// also manages the list of objects that are currently running around
// and contains functionality to print since that functionality is common
// to all grid based classes
class Grid : public View {
public:
	// tells the view to "forget" all the information it has
	void clear() override;
	// Save the supplied name and location for future use in a draw() call
	// If the name is already present, the new location replaces the previous one.
	void update_location(const std::string& name, Point location) override;
	// Remove the name and its location; no error if the name is not present.
	void update_remove(const std::string& name) override;

protected:
	// since populating and printing the grid is shared functionality
	// they're provided here as protected to avoid duplicating code
	// between Local and Map
	bool get_subscripts(int &ix, int &iy, Point location, const int& size, const double& scale, const Point& origin);
	// populates grid with outliers in a serparate vector
	void populate_grid(std::vector<std::vector<std::string>>& grid, std::vector<std::string>& outside, const int& size, const double& scale, const Point& origin);
	// print the grid and just the grid
	void print_grid(std::vector<std::vector<std::string>>& grid, const int& size, const double& scale, const Point& origin);

private:
	std::map<std::string, Point> object_list;	
};

// local and map, two of the grid views are responsible
// for maintaining their size, scales, and origins, and 
// well as printing their data
class Local : public Grid {
public:
	// default constructor sets default size, scale, and origin
	Local(Point origin_, std::string name);
	
	// prints out the current map
	void draw() override;
	// tells the view to "forget" all the information it has
	// TODO: not sure if I need this...
	void clear() override;
	// update the origin to match the objects new location
	void update_location(const std::string& name, Point location) override;

private:
	// given the location of the current object, calculates the correct
	// poisition of the origin relative to that location
	Point calculate_origin(Point location);
	// name of the person who is being displayed
	std::string name;

	int size;
	double scale;
	Point origin;
};

class Map : public Grid {
public:
	// default constructor sets the default size, scale, and origin
	Map(); 
	
	// prints out the current map
	void draw() override;
	
	// modify the display parameters
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
	void set_size(int size_) override;
	void set_scale(double scale_) override;
	void set_origin(Point origin_) override;
	void set_defaults() override;

private:
	void print_grid(std::vector< std::vector<std::string> >& grid, std::vector<std::string>& outside);
	void print_outliers(std::vector<std::string>& outside);

	int size;
	double scale;
	Point origin;
}; 

// Values is responsible for maintaining an std::map of names to values (doubles)
// it is also responsible for neatly printing its contents to the user
class Values : public View {
public:
	// display information to the user
	void draw() override;
	// tell value to "forget" its data
	void clear() override;
	// tell value to "forget" a specific name
	void update_remove(const std::string& name) override;

protected:
	void update_value(const std::string& name, double value);

private:
	std::map<std::string, double> stored_values;
};

// Health keeps track of the health of agents in the simulation
class Health : public Values {
public:
	// display information
	void draw() override;
	// update the amount of health
	void update_health(const std::string& name, double health) override;
};

// Amounts keeps track of the amount of food that is being carried by an agent,
// or stored in a structure
class Amounts : public Values {
public:
	// display information
	void draw() override;
	// update the amount
	void update_amount(const std::string& name, double amount) override;
};

#endif
