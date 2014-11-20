#include "Agent_factory.h"
#include "Peasant.h"
#include "Soldier.h"
#include "Utility.h"
using namespace std;

// constants for error messages
const char* const unknown_type {"Trying to create agent of unknown type!"};

shared_ptr<Agent> create_agent(const string& name, const string& type, Point location)
{
	if (type == "Peasant") {
		return shared_ptr<Agent>(new Peasant(name, location));
	} else if (type == "Soldier") {
		return shared_ptr<Agent>(new Soldier(name, location));
	} else {
		throw Error(unknown_type);
	}
}
