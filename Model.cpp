#include "Model.h"
#include "View.h"
#include "Sim_object.h"
#include "Structure.h"
#include "Agent.h"
#include "Agent_factory.h"
#include "Structure_factory.h"
#include "Geometry.h"
#include "Utility.h"
#include <iostream>
#include <algorithm>
using namespace std;

// error messages
const char* const no_structure {"Structure not found!"};
const char* const no_agent {"Agent not found!"};

// default Model's instance to nullptr
Model* Model::ptr = nullptr;

void Model::insert_Agent(shared_ptr<Agent> agent)
{
	sim_objs.insert(make_pair(agent->get_name(), agent));
	agent_objs.insert(make_pair(agent->get_name(), agent));
}
void Model::insert_Structure(shared_ptr<Structure> structure)
{
	sim_objs.insert(make_pair(structure->get_name(), structure));
	structure_objs.insert(make_pair(structure->get_name(), structure));
}

// should be one of the only uses of a "raw" pointer in the program
Model* Model::get_Model()
{
	if (!ptr)
		ptr = new Model();
	return ptr;
}

Model::Model()
{
	insert_Structure(create_structure("Rivendale", "Farm", Point(10., 10.)));
	insert_Structure(create_structure("Sunnybrook", "Farm", Point(0., 30.)));
	insert_Structure(create_structure("Shire", "Town_Hall", Point(20., 20.)));
	insert_Structure(create_structure("Paduca", "Town_Hall", Point(30., 30.)));
	
	insert_Agent(create_agent("Pippin", "Peasant", Point(5., 10.)));
	insert_Agent(create_agent("Merry", "Peasant", Point(0., 25.)));
	insert_Agent(create_agent("Zug", "Soldier", Point(20., 30.)));
	insert_Agent(create_agent("Bug", "Soldier", Point(15., 20.)));
	insert_Agent(create_agent("Iriel", "Archer", Point(20., 38.)));
}

// destroy all objects
Model::~Model()
{
	for(auto& i : sim_objs)
		i.second.reset();
	for(auto& i : views)
		i.second.reset();
}

// is name already in use for either agent or structure?
// either the identical name, or identical in first two characters counts as in-use
bool Model::is_name_in_use(const string& name) const
{
	return sim_objs.find(name) != sim_objs.end();
}

// is there a structure with this name?
bool Model::is_structure_present(const string& name) const
{
	return structure_objs.find(name) != structure_objs.end();
}
// add a new structure; assumes none with the same name
void Model::add_structure(shared_ptr<Structure> new_structure)
{
	sim_objs.insert(make_pair(new_structure->get_name(), new_structure));
	structure_objs.insert(make_pair(new_structure->get_name(), new_structure));
	new_structure->broadcast_current_state();
}


// will throw Error("Structure not found!") if no structure of that name
shared_ptr<Structure> Model::get_structure_ptr(const string& name) const
{
	auto structure_itr = structure_objs.find(name);
	if (structure_itr == structure_objs.end())
		throw Error(no_structure);
		
	return structure_itr->second;
}

// is there an agent with this name?
bool Model::is_agent_present(const string& name) const
{
	return agent_objs.find(name) != agent_objs.end();
}
// add a new agent; assumes none with the same name
void Model::add_agent(shared_ptr<Agent> new_agent)
{
	sim_objs.insert( make_pair(new_agent->get_name(), new_agent));
	agent_objs.insert( make_pair(new_agent->get_name(), new_agent));
	new_agent->broadcast_current_state();
}
// will throw Error("Agent not found!") if no agent of that name
shared_ptr<Agent> Model::get_agent_ptr(const string& name) const
{
	auto agent_itr = agent_objs.find(name);
	if(agent_itr == agent_objs.end())
		throw Error(no_agent);
	else
		return agent_itr->second;
}

// tell all objects to describe themselves to the console
void Model::describe() const
{
	for(auto& i : sim_objs)
		i.second->describe();
}
// increment the time, and tell all objects to update themselves
void Model::update()
{
	// add one to the time
	++time;
	// update all the Sim_objects alphabetically
	for(auto& i : sim_objs)
		i.second->update();
}

void Model::remove_agent(shared_ptr<Agent> agent)
{
	// remove from sim objs and agents
	sim_objs.erase(agent->get_name());
	agent_objs.erase(agent->get_name());
}

/* View services */
// Attaching a View adds it to the container and causes it to be updated
// with all current objects'location (or other state information.
void Model::attach(string view_name, shared_ptr<View> view)
{
	views.push_back(make_pair(view_name, view));
	for(auto& i : sim_objs)
		i.second->broadcast_current_state();
}
// Detach the View by discarding the supplied pointer from the container of Views
// - no updates sent to it thereafter.
void Model::detach(string view_name)
{
	for(auto itr = views.begin(); itr != views.end(); ++itr) {
		if (itr->first == view_name) {
			views.erase(itr);
			return;
		}
	}
}
// notify the views about an object's location
void Model::notify_location(const string& name, Point location)
{
	for(auto& i : views)
		i.second->update_location(name, location);
}
// notify views about an objects amounts
void Model::notify_amount(const string& name, double amount)
{
	for(auto& i : views)
		i.second->update_amount(name, amount);
}
// notify the views about an objects health
void Model::notify_health(const string& name, double health)
{
	for(auto & i : views)
		i.second->update_health(name, health);
}

// notify the views that an object is now gone
void Model::notify_gone(const string& name)
{
	for(auto& i : views)
		i.second->update_remove(name);
}

void Model::draw_all_views()
{
	for(auto& i : views)
		i.second->draw();
}

shared_ptr<View> Model::get_view(const string& view_name)
{
	for(auto& i : views) {
		if(i.first == view_name)
			return i.second;
	}
	return shared_ptr<View>();
}

// function object to compare distances between two Agents and the saved agent
class CompSimObjDistance {
public:
	CompSimObjDistance(shared_ptr<Agent> current_agent_)
	: current_agent(current_agent_)
	{}

	bool operator() (pair<string, shared_ptr<Sim_object> > lhs, pair<string, shared_ptr<Sim_object> > rhs)
	{
		if (current_agent == lhs.second) {
			// don't want to attack ourselves
			return false;
		} else if (current_agent == rhs.second) {
			return true;
		}
		// calculate the distance between the current location and two different points
		double lhs_distance = cartesian_distance(current_agent->get_location(), lhs.second->get_location());
		double rhs_distance = cartesian_distance(current_agent->get_location(), rhs.second->get_location());
		return lhs_distance < rhs_distance;
	}
private:
	shared_ptr<Agent> current_agent;
};

// returns a shared_ptr to the closest agent to location
shared_ptr<Agent> Model::get_closest_agent(shared_ptr<Agent> current_agent)
{
	// in case of a tie, min_element will return the first of the two objects
	// luckily, agent_objs and structure_objs are kept in alphabetical order
	return min_element(agent_objs.begin(), agent_objs.end(), CompSimObjDistance(current_agent))->second;
}

// returns a shared_ptr to the closes structure to location
shared_ptr<Structure> Model::get_closest_structure(shared_ptr<Agent> current_agent)
{
	return min_element(structure_objs.begin(), structure_objs.end(), CompSimObjDistance(current_agent))->second;
}