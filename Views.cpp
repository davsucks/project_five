#include "View.h"
#include "Views.h"
#include "Utility.h"
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
using namespace std;

// ============================
// === GRAPH IMPLEMENTATION ===
// ============================

void Grid::print_grid(vector<vector<string>>& grid, const int& size, const double& scale, const Point& origin)
{
	// print the grid
	const int label_freq_c = 3;
	for (int y = size - 1; y >= 0; --y) {
		if (y % label_freq_c == 0) {
			// store the label
			// int(origin.x) + int(y*scale)
			double label = origin.y + (y * scale);
			// save old settings
			ios::fmtflags old_settings = cout.flags();
			int old_precision = cout.precision();
			// actually print the label
			cout.precision(0);
			cout << setw(4) << setprecision(0) << fixed << label << " ";
			// resore old settings
			cout.precision(old_precision);
			cout.flags(old_settings);
		} else {
			cout << "     ";
		}
		for (int x = 0; x < size; ++x) {
			cout << grid[y][x];
		}
		cout << endl;
	}
	// now print labels for the last row
	for (int x = 0; x < size; x += label_freq_c) {
		double label = (x * scale) + origin.x;
		// save old settings

		ios::fmtflags old_settings = cout.flags();
		int old_precision = cout.precision();
		// actually print the label
		cout << "  " << setw(4) << setprecision(0) << fixed << label;
		// restore old settings
		cout.precision(old_precision);
		cout.flags(old_settings);
	}
	cout << endl;
}

// Discard the saved information - drawing will show only a empty pattern
void Grid::clear()
{
	object_list.clear();
}

void Grid::update_location(const string& name, Point location)
{
	object_list[name] = location;
}

void Grid::update_remove(const string& name)
{
	object_list.erase(name);
}

// Calculate the cell subscripts corresponding to the supplied location parameter, 
// using the current size, scale, and origin of the display. 
// This function assumes that origin is a  member variable of type Point, 
// scale is a double value, and size is an integer for the number of rows/columns 
// currently being used for the grid.
// Return true if the location is within the grid, false if not

bool Grid::get_subscripts(int &ix, int &iy, Point location, const int& size, const double& scale, const Point& origin)
{
	// adjust with origin and scale
	Cartesian_vector subscripts = (location - origin) / scale;
	// truncate coordinates to integer after taking the floor
	// floor function will return the largest integer smaller than the supplied value
	// even for negative values, so -0.05 => -1., which will be outside the array.
	ix = int(floor(subscripts.delta_x));
	iy = int(floor(subscripts.delta_y));
	// if out of range, return false
	if ((ix < 0) || (ix >= size) || (iy < 0) || (iy >= size)) {
		return false;
		}
	else
		return true;
}

void Grid::populate_grid(vector<vector<string>>& grid, vector<string>& outside, const int &size, const double& scale, const Point& origin)
{
	int ix = 0;
	int iy = 0;
	for (auto& i : object_list) {
		ix = i.second.x;
		iy = i.second.y;
		if (!get_subscripts(ix, iy, i.second, size, scale, origin)) {
			// isn't present in the grid, need to add it to the outside container
			auto insert_itr = lower_bound(outside.begin(), outside.end(), i.first);
			outside.insert(insert_itr, i.first);
		} else {
			// is in the grid so add it!
			const int max_char_c = 2;
			// add the first to chars of the name if its the first name in the cell,
			// otherwise make it an asterik
			grid[iy][ix] = (grid[iy][ix] == ". " ? i.first.substr(0, max_char_c) : "* ");
		}
	}
}


// ============================
// === LOCAL IMPLEMENTATION ===
// ============================

static const int def_local_size_c {9};
static const double def_local_scale_c {2};

Local::Local(Point location, string name_)
:
name {name_},
size {def_local_size_c}, 
scale {def_local_scale_c},
origin(calculate_origin(location))
{ }

void Local::draw()
{
	// first create an empty grid
	// TODO: this is sloppy, bad design?
	vector< vector<string>> grid (size, vector<string>(size, ". "));
	vector<string> outside;
	// populate grid/outside with proper names and locations
	Grid::populate_grid(grid, outside, size, scale, origin);
	// then print everything
	cout << "Local view for: " << name << endl;
	Grid::print_grid(grid, size, scale, origin);
}

void Local::clear()
{
	name.clear();
	Grid::clear();
}

void Local::update_location(const string& name, Point location)
{
	// first update the location stored in the parents object list
	Grid::update_location(name, location);
	// now update our own origin
	origin = calculate_origin(location);
}

Point Local::calculate_origin(Point location)
{
	double x = origin.x - (size / 2.0) * scale;
	double y = origin.y - (size / 2.0) * scale;
	return Point(x, y);
}

// ==========================
// === MAP IMPLEMENTATION ===
// ==========================

// error message constants
const char* const map_too_big {"New map size is too big!"};
const char* const map_too_small {"New map size is too small!"};
const char* const positive_scale {"New map scale must be positive!"};

// constants
static const int def_map_size_c {25};
static const double def_map_scale_c {2.0};
static const double def_map_origin_x_c {-10.0};
static const double def_map_origin_y_c {-10.0};

// default constructor sets the default size, scale, and origin
Map::Map()
:
size {def_map_size_c},
scale {def_map_scale_c},
origin(def_map_origin_x_c, def_map_origin_y_c)
{ }

// prints out the current map
void Map::draw()
{
	// first create an empty grid
	vector< vector<string>> grid (size, vector<string>(size, ". "));
	vector<string> outside;
	// populate grid/outside with proper names and locations
	Grid::populate_grid(grid, outside, size, scale, origin);
	// then print everything
	print_grid(grid, outside);
}

// modify the display parameters
// if the size is out of bounds will throw Error("New map size is too big!")
// or Error("New map size is too small!")
void Map::set_size(int size_)
{
	const int max_size_c = 30;
	const int min_size_c = 6;
	
	if (size_ > max_size_c)
		throw Error(map_too_big);
	if (size_ <= min_size_c)
		throw Error(map_too_small);
	size = size_;
}

// If scale is not postive, will throw Error("New map scale must be positive!");
void Map::set_scale(double scale_)
{
	if (scale_ <= 0)
		throw Error(positive_scale);
	scale = scale_;
}

// any values are legal for the origin
void Map::set_origin(Point origin_)
{
	origin = origin_;
}

// set the parameters to the default values
void Map::set_defaults()
{
	size = def_map_size_c;
	scale = def_map_scale_c;
	origin = Point(def_map_origin_x_c, def_map_origin_y_c);
}

void Map::print_outliers(vector<string>& outside)
{
	ostringstream ss;
	copy(outside.begin(), outside.end() - 1, ostream_iterator<string>(ss, ", "));
	ss << outside.back();
	cout << ss.str();
	cout << " outside the map" << endl;
}

void Map::print_grid(vector<vector<string>>& grid, vector<string>& outside)
{
	cout << "Display size: " << size << ", scale: " << scale << ", origin: " << origin << endl;
	// print everyone who's outside of the grid
	if (outside.size())
		print_outliers(outside);

	Grid::print_grid(grid, size, scale, origin);
}

// =============================
// === VALUES IMPLEMENTATION ===
// =============================

// display information to the user
void Values::draw()
{
	cout << "--------------" << endl;
	for(auto& itr : stored_values)
		cout << itr.first << ": " << itr.second << endl;
}
// tell value to "forget" its data
void Values::clear()
{
	stored_values.clear();
}
// tell value to "forget" a specific name
void Values::update_remove(const std::string& name)
{
	stored_values.erase(name);
}

void Values::update_value(const std::string& name, double value)
{
	stored_values[name] = value;
}

// =============================
// === HEALTH IMPLEMENTATION ===
// ============================= 

// display information
void Health::draw()
{
	cout << "Current " << "Health" << ":" << endl;
}
// update the amount of health
void Health::update_health(const std::string& name, double health)
{
	Values::update_value(name, health);
}

// ==============================
// === AMOUNTS IMPLEMENTATION ===
// ==============================

// display information
void Amounts::draw()
{
	cout << "Current " << "Amounts" << ":" << endl;
}
// update the amount
void Amounts::update_amount(const std::string& name, double amount)
{
	Values::update_value(name, amount);
}
