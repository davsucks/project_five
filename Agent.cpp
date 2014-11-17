#include "Agent.h"
#include "Geometry.h"
#include "Utility.h"
#include "Model.h"
#include <iostream>
using namespace std;

static const int initial_health {5};
static const int initial_speed {5};

Agent::Agent(const string& name_, Point location_)
:
Sim_object(name_),
Moving_object(location_, initial_speed),
health_state{Health_State_e::ALIVE}
{
	// need to just set the health_state and initial_health
	health_state = Health_State_e::ALIVE;
	health = initial_health;
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
	start_moving(destination_);
	// if moving output proper message and set the state accordingly
	if (is_currently_moving())
		cout << get_name() << ": I'm on the way" << endl;
	else
		cout << get_name() << ": I'm already there" << endl;
}

// tell this Agent to stop its activity
void Agent::stop()
{
	if (is_currently_moving()) {
		stop_moving();
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
			if (is_currently_moving()) {
				if (update_location())
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
	cout << get_name() << " at " << get_current_location() << endl;
	switch(health_state) {
		case Health_State_e::ALIVE:
			cout << "   Health is " << health << endl;
			if (is_currently_moving()) {
				cout << "   Moving at speed "<< get_current_speed() << " to " << get_current_destination() << endl;
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
			g_Model_ptr->notify_location(get_name(), get_current_location());
			break;
		case Health_State_e::DYING:
		case Health_State_e::DEAD:
		case Health_State_e::DISAPPEARING:
		default:
			g_Model_ptr->notify_gone(get_name());
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
		stop_moving();
		cout << get_name() << ": Arrggh!" << endl;
	} else {
		cout << get_name() << ": Ouch!" << endl;
	}
}
