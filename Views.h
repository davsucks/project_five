#ifndef VIEWS_H
#define VIEWS_H
/* *** View class ***
The View class encapsulates the data and functions needed to generate the map
display, and control its properties. It has a "memory" for the names and locations
of the to-be-plotted objects.

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
4. As needed, change the origin, scale, or displayed size of the map 
with the appropriate functions. Since the view "remembers" the previously updated
information, immediately calling the draw function will print out a map showing the previous objects
using the new settings.
*/
#include <string>
#include <map>
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
	// TODO: move these into a friend class?
	// TODO: that would probably couple everything too tightly
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
	void print_grid(std::vector<std::vector<std::string>>& grid, std::vector<std::string>& outside);
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

class Health : public Values {
public:
	// display information
	void draw() override;
	// update the amount of health
	void update_health(const std::string& name, double health) override;
};

class Amounts : public Values {
public:
	// display information
	void draw() override;
	// update the amount
	void update_amount(const std::string& name, double amount) override;

};

#endif
