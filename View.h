#ifndef VIEW_H
#define VIEW_H

#include <string>

class Point;

class View {
public:
	// the interface for both Grid and Value views

	// displays the views information to the user
	virtual void draw() = 0;
	// tells the view to "forget" all the information it has
	virtual void clear() = 0;
	// Save the supplied name and location for future use in a draw() call
	// If the name is already present, the new location replaces the previous one.
	virtual void update_location(const std::string& name, Point location);
	// Save the supplied name and amount
	virtual void update_amount(const std::string& name, double amount_);
	// Save the supplied name and health
	virtual void update_health(const std::string& name, double health_);
	// Remove the name and its location; no error if the name is not present.
	virtual void update_remove(const std::string& name);
	// modify the display parameters
	// if the size is out of bounds will throw Error("New map size is too big!")
	// or Error("New map size is too small!")
	virtual void set_size(int size_);
	// If scale is not postive, will throw Error("New map scale must be positive!");
	virtual void set_scale(double scale_);
	// set the parameters to the default values
	virtual void set_defaults();
	// set the origin
	virtual void set_origin(Point origin_);
};

#endif