#include "Structure.h"
#include "Model.h"
#include <iostream>
using namespace std;

Structure::Structure(std::string name_, Point location_)
:
Sim_object(name_),
location(location_.x, location_.y)
{ }

Structure::~Structure()
{ }

void Structure::describe() const
{
	cout << Sim_object::get_name() << " at " << location << endl;
}

void Structure::broadcast_current_state()
{
	Model::get_Model()->notify_location(get_name(), location);
}
