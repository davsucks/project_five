#include "View.h"
#include "Geometry.h"
using namespace std;

// provide a fat interface for derived classes

// Save the supplied name and location for future use in a draw() call
// If the name is already present, the new location replaces the previous one.
void View::update_location(const string& name, Point location)
{ /* provide an empty definition for base classes to override */ }
// Save the supplied name and amount
void View::update_amount(const string& name, double amount_)
{ /* provide an empty definition for base classes to override */ }
// Save the supplied name and health
void View::update_health(const string& name, double health_)
{ /* provide an empty definition for base classes to override */ }
void View::update_remove(const string& name)
{ /* provide an empty definition for base classes to override */ }

void View::set_size(int size_)
{ /* provide an empty definition for base classes to override */ }
// If scale is not postive, will throw Error("New map scale must be positive!");
void View::set_scale(double scale_)
{ /* provide an empty definition for base classes to override */ }
// set the parameters to the default values
void View::set_defaults()
{ /* provide an empty definition for base classes to override */ }
// set the origin
void View::set_origin(Point origin_)
{ /* provide an empty definition for base classes to override */ }