#include "Soldier.h"
#include "Geometry.h"
#include "Utility.h"
#include <iostream>
using namespace std;

static const int def_strength_c {2};
static const int def_range_c {2};

Soldier::Soldier(const std::string& name_, Point location_)
:
Agent(name_, location_),
attack_state {Attack_State_e::NOT_ATTACKING},
attack_strength {def_strength_c},
attack_range {def_range_c},
target {nullptr}
{
	cout << "Soldier " << name_ << " constructed" << endl;
}

Soldier::~Soldier()
{
	cout << "Soldier " << get_name() << " destructed" << endl;
}

// update implements Soldier behavior
void Soldier::update()
{
	Agent::update();
	if (!Agent::is_alive() || attack_state == Attack_State_e::NOT_ATTACKING)
		return;
	// else we're attacking
	if (!target->is_alive()) {
		cout << get_name() << ": Target is dead" << endl;
		attack_state = Attack_State_e::NOT_ATTACKING;
		return;
	}
	if (cartesian_distance(get_location(), target->get_location()) > attack_range) {
		cout << get_name() << ": Target is now out of range" << endl;
		attack_state = Attack_State_e::NOT_ATTACKING;
		target = nullptr;
		return;
	}
	cout << get_name() << ": Clang!" << endl;
	target->take_hit(attack_strength, this);
	if (!target->is_alive()) {
		cout << get_name() << ": I triumph!" << endl;
		attack_state = Attack_State_e::NOT_ATTACKING;
		target = nullptr;
	}
}

// Make this Soldier start attacking the target Agent.
// Throws an exception if the target is the same as this Agent,
// is out of range, or is not alive.
void Soldier::start_attacking(Agent* target_ptr)
{
	if (target_ptr == this)
		throw Error(get_name() + ": I cannot attack myself!");

	if (!target_ptr->is_alive())
		throw Error(get_name() + ": Target is not alive!");

	if (cartesian_distance (get_location(), target_ptr->get_location()) > attack_range)
		throw Error(get_name() + ": Target is out of range!");

	target = target_ptr;
	cout << get_name() << ": I'm attacking!" << endl;
	attack_state = Attack_State_e::ATTACKING;
}

// Overrides Agent's take_hit to counterattack when attacked.
void Soldier::take_hit(int attack_strength, Agent* attacker_ptr)
{
	Agent::lose_health(attack_strength);
	switch(attack_state) {
		case Attack_State_e::ATTACKING:
			if (!Agent::is_alive()) {
				attack_state = Attack_State_e::NOT_ATTACKING;
				target = nullptr;
			}
			break;
		case Attack_State_e::NOT_ATTACKING:
			if (Agent::is_alive() && attacker_ptr->is_alive()) {
				attack_state = Attack_State_e::ATTACKING;
				target = attacker_ptr;
				// TODO: erase this?
				cout << get_name() << ": I'm attacking!" << endl;
			}
	}
}

// Overrides Agent's stop to print a message
void Soldier::stop()
{
	cout << get_name() << ": Don't bother me" << endl;
}

// output information about the current state
void Soldier::describe() const
{
	cout << "Soldier ";
	Agent::describe();
	if (attack_state == Attack_State_e::ATTACKING)
		cout << "   Attacking " << target->get_name() << endl;
	else
		cout << "   Not attacking" << endl;
}