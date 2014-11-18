#include "Agent.h"
#include "Geometry.h"
#include "Utility.h"
#include "Model.h"
#include <iostream>
using namespace std;

static const int initial_health_c {5};
static const int initial_speed_c {5};

Agent::Agent(const string& name_, Point location_)
:
Sim_object(name_),
moving_obj(location_, initial_speed_c),
health_state{Health_State_e::ALIVE},
health{initial_health_c}
{
	cout << "Agent " << name_ << " constructed" << endl;
}

Agent::~Agent()
{
	cout << "Agent " << get_name() << " destructed" << endl;
}

// tell this Agent to start moving to location destination_
void Agent::move_to(Point destination_)
{
	// tell the agent to move to the destination
	moving_obj.start_moving(destination_);
	// if moving output proper message and set the state accordingly
	if (moving_obj.is_currently_moving())
		cout << get_name() << ": I'm on the way" << endl;
	else
		cout << get_name() << ": I'm already there" << endl;
}

// tell this Agent to stop its activity
void Agent::stop()
{
	if (moving_obj.is_currently_moving()) {
		moving_obj.stop_moving();
		cout << get_name() << ": I'm stopped" << endl;
	}
}

// Tell this Agent to accept a hit from an attack of a specified strength
// The attacking Agent identifies itself with its this pointer.
// A derived class can override this function.
// The function lose_health is called to handle the effect of the attack.
void Agent::take_hit(int attack_strength, Agent *attacker_ptr)
{
	lose_health(attack_strength);
}

// update the moving state and Agent state of this object.
void Agent::update()
{
	switch(health_state) {
		case Health_State_e::ALIVE:
			if (moving_obj.is_currently_moving()) {
				if (moving_obj.update_location())
					cout << get_name() << ": I'm there!"<< endl;
				else
					cout << get_name() << ": step..." << endl;
				broadcast_current_state();
			}
			break;
		case Health_State_e::DYING:
			health_state = Health_State_e::DEAD;
			broadcast_current_state();
			break;
		case Health_State_e::DEAD:
			health_state = Health_State_e::DISAPPEARING;
			break;
		case Health_State_e::DISAPPEARING:
		default:
			break;
	}
	
}

// output information about the current state
void Agent::describe() const
{
	cout << get_name() << " at " << moving_obj.get_current_location() << endl;
	switch(health_state) {
		case Health_State_e::ALIVE:
			cout << "   Health is " << health << endl;
			if (moving_obj.is_currently_moving()) {
				cout << "   Moving at speed "<< moving_obj.get_current_speed() << " to " << moving_obj.get_current_destination() << endl;
			} else {
				cout << "   Stopped" << endl;
			}
			break;
		case Health_State_e::DYING:
			cout << "   Is dying" << endl;
			break;
		case Health_State_e::DEAD:
			cout << "   Is dead" << endl;
			break;
		case Health_State_e::DISAPPEARING:
			cout << "   Is disappearing" << endl; // not expected to be visible in this project
			break;
	}

}

// ask Model to broadcast our current state to all Views
void Agent::broadcast_current_state()
{
	switch(health_state) {
		case Health_State_e::ALIVE:
			Model::get_Model()->notify_location(get_name(), moving_obj.get_current_location());
			break;
		case Health_State_e::DYING:
		case Health_State_e::DEAD:
		case Health_State_e::DISAPPEARING:
		default:
			Model::get_Model()->notify_gone(get_name());
			break;
	}
}

/* Fat Interface for derived classes */
// Throws exception that an Agent cannot work.
void Agent::start_working(Structure *, Structure *)
{
	throw Error(get_name() + ": Sorry, I can't work!");
}

// Throws exception that an Agent cannot attack.
void Agent::start_attacking(Agent *)
{
	throw Error(get_name() + ": Sorry, I can't attack!");
}

// calculate loss of health due to hit.
// if health decreases to zero or negative, Agent state becomes Dying, and any movement is stopped.
void Agent::lose_health(int attack_strength)
{
	health -= attack_strength;
	if (health <= 0) {
		health_state = Health_State_e::DYING;
		moving_obj.stop_moving();
		cout << get_name() << ": Arrggh!" << endl;
	} else {
		cout << get_name() << ": Ouch!" << endl;
	}
}
