#include "Structure_factory.h"
#include "Town_Hall.h"
#include "Farm.h"
#include "Utility.h"
#include "Geometry.h"
using namespace std;

const char* const unknown_type {"Trying to create structure of unknown type!"};

Structure* create_structure(const string& name, const string& type, Point location)
{
	if (type == "Farm") {
		// make Farm
		return new Farm(name, location);
	} else if (type == "Town_Hall") {
		return new Town_Hall(name, location);
		// make Town_Hall
	} else {
		throw Error(unknown_type);
	}
}


