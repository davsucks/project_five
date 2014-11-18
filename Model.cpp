#include "Model.h"
#include "View.h"
#include "Sim_object.h"
#include "Structure.h"
#include "Agent.h"
#include "Agent_factory.h"
#include "Structure_factory.h"
#include "Geometry.h"
#include "Utility.h"
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

// error messages
const char* const no_structure {"Structure not found!"};
const char* const no_agent {"Agent not found!"};

// default Model's instance to nullptr
Model* Model::ptr = nullptr;

void Model::insert_Agent(Agent* agent)
{
	sim_objs.insert(pair<string, Sim_object*>(agent->get_name(), agent) );
	agent_objs.insert(pair<string, Agent*>(agent->get_name(), agent) );
}
void Model::insert_Structure(Structure* structure)
{
	sim_objs.insert(pair<string, Sim_object*>(structure->get_name(), structure) );
	structure_objs.insert(pair<string, Structure*>(structure->get_name(), structure));
}

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
}

// destroy all objects
Model::~Model()
{
	for(auto& i : sim_objs)
		delete i.second;
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
void Model::add_structure(Structure* new_structure)
{
	sim_objs.insert( pair<string, Sim_object*>(new_structure->get_name(), new_structure));
	structure_objs.insert( pair<string, Structure*>(new_structure->get_name(), new_structure));
	new_structure->broadcast_current_state();
}


// will throw Error("Structure not found!") if no structure of that name
Structure* Model::get_structure_ptr(const string& name) const
{
	// invariant: index into structure_objs with the first
	// two chars of name
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
void Model::add_agent(Agent* new_agent)
{
	sim_objs.insert( pair<string, Sim_object*>(new_agent->get_name(), new_agent));
	agent_objs.insert( pair<string, Agent*>(new_agent->get_name(), new_agent));
	new_agent->broadcast_current_state();
}
// will throw Error("Agent not found!") if no agent of that name
Agent* Model::get_agent_ptr(const string& name) const
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
	// collect all the Agents that are disappearing, remove them from containers
	// and delete them in alphabetical order
	for(auto agent_itr = agent_objs.begin(); agent_itr != agent_objs.end(); ) {
		if(agent_itr->second->is_disappearing()) {
			// remove from the proper containers and delete
			string key = agent_itr->second->get_name();
			auto temp = agent_itr++;
			sim_objs.erase(key);
			agent_objs.erase(key);
			delete temp->second;
		} else
			++agent_itr;
	}
}

/* View services */
// Attaching a View adds it to the container and causes it to be updated
// with all current objects'location (or other state information.
void Model::attach(View* view)
{
	views.push_back(view);
	for(auto& i : sim_objs)
		i.second->broadcast_current_state();
}
// Detach the View by discarding the supplied pointer from the container of Views
// - no updates sent to it thereafter.
void Model::detach(View* view)
{
	views.remove(view);
}
// notify the views about an object's location
void Model::notify_location(const string& name, Point location)
{
	for(View* view : views)
		view->update_location(name, location);
}
// notify the views that an object is now gone
void Model::notify_gone(const string& name)
{
	for(View* view : views)
		view->update_remove(name);
}
