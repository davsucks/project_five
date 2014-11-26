#include "Warriors.h"
#include "Structure.h"
#include "Geometry.h"
#include "Utility.h"
#include "Model.h"
#include <iostream>
using namespace std;


Warrior::Warrior(const std::string& name_, Point location_, int attack_strength_, int attack_range_)
:
Agent(name_, location_),
attack_state {Attack_State_e::NOT_ATTACKING},
attack_strength {attack_strength_},
attack_range {attack_range_}
{ }

// update implements Warrior behavior
void Warrior::update()
{
	Agent::update();
	if (!Agent::is_alive() || attack_state == Attack_State_e::NOT_ATTACKING)
		return;
	
	shared_ptr<Agent> shared_target = target.lock();
	if (target.expired() || !shared_target->is_alive())  {
		cout << get_name() << ": Target is dead" << endl;
		attack_state = Attack_State_e::NOT_ATTACKING;
		target.reset();
		return;
	}
	// else we're attacking
	if (cartesian_distance(get_location(), shared_target->get_location()) > attack_range) {
		cout << get_name() << ": Target is now out of range" << endl;
		attack_state = Attack_State_e::NOT_ATTACKING;
		target.reset();
		return;
	}
	cout << get_name() << ": " << get_noise() << endl;
	shared_target->take_hit(attack_strength, shared_from_this());
	if (!shared_target || !shared_target->is_alive()) {
		cout << get_name() << ": I triumph!" << endl;
		attack_state = Attack_State_e::NOT_ATTACKING;
		target.reset();
	}
}

// Make this Warrior start attacking the target Agent.
// Throws an exception if the target is the same as this Agent,
// is out of range, or is not alive.
void Warrior::start_attacking(shared_ptr<Agent> target_ptr)
{
	if (target_ptr.get() == this)
		throw Error(get_name() + ": I cannot attack myself!");

	if (!target_ptr->is_alive())
		throw Error(get_name() + ": Target is not alive!");

	if (cartesian_distance (get_location(), target_ptr->get_location()) > attack_range)
		throw Error(get_name() + ": Target is out of range!");

	attack(target_ptr);
}

void Warrior::attack(shared_ptr<Agent> target_ptr)
{
	target = target_ptr;
	cout << get_name() << ": I'm attacking!" << endl;
	attack_state = Attack_State_e::ATTACKING;
}

// Overrides Agent's take_hit to counterattack when attacked.
void Warrior::take_hit(int attack_strength, shared_ptr<Agent> attacker_ptr)
{
	Agent::lose_health(attack_strength);
}

// Overrides Agent's stop to print a message
void Warrior::stop()
{
	cout << get_name() << ": Don't bother me" << endl;
}

// output information about the current state
void Warrior::describe() const
{
	Agent::describe();
	if (attack_state == Attack_State_e::ATTACKING) {
		if (target.expired())
			cout << "   Attacking dead target" << endl;
		else
			cout << "   Attacking " << target.lock()->get_name() << endl;
	}
	else
		cout << "   Not attacking" << endl;
}

void Warrior::reset_attacking()
{
	attack_state = Attack_State_e::NOT_ATTACKING;
	target.reset();
}

// ==============================
// === SOLDIER IMPLEMENTATION ===
// ==============================

static const int def_soldier_strength_c {2};
static const int def_soldier_range_c {2};
static const char* const clang_c {"Clang!"};

Soldier::Soldier(const string& name_, Point location_)
: Warrior(name_, location_, def_soldier_strength_c, def_soldier_range_c)
{

}

void Soldier::take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr)
{
	// lose health
	Warrior::take_hit(attack_strength, attacker_ptr);
	// retaliate if in range
	if (Warrior::is_attacking()) {
		if (!Agent::is_alive()) {
			reset_attacking();
		}
	} else {
		if (Agent::is_alive() && attacker_ptr->is_alive()) {
			attack(attacker_ptr);
		}
	}
}

void Soldier::describe() const
{
	cout << "Soldier ";
	Warrior::describe();
}

const char* Soldier::get_noise() const
{
	return clang_c;
}

// =============================
// === ARCHER IMPLEMENTATION ===
// =============================

static const int def_archer_strength_c {1};
static const int def_archer_range_c {6};
static const char* const twang_c {"Twang!"};

Archer::Archer(const std::string& name_, Point location_)
:
Warrior(name_, location_, def_archer_strength_c, def_archer_range_c)
{}

void Archer::update()
{
	Warrior::update();
	if (!Warrior::is_attacking()) {
		// find next target
		shared_ptr<Agent> target = Model::get_Model()->get_closest_agent(shared_from_this());
		if(cartesian_distance(get_location(), target->get_location()) <= def_archer_range_c) {
			// new target is within range, attack!
			start_attacking(target);
		}
	}
}

void Archer::take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr)
{
	// lose health
	Warrior::take_hit(attack_strength, attacker_ptr);
	if (!is_alive())
		return;
	// find the closest structure and run there
	shared_ptr<Structure> closest_bld = Model::get_Model()->get_closest_structure(shared_from_this());
	cout << get_name() << ": I'm going to run away to " << closest_bld->get_name() << endl;
	move_to(closest_bld->get_location());
}

void Archer::describe() const
{
	cout << "Archer ";
	Warrior::describe();
}

const char* Archer::get_noise() const
{
	return twang_c;
}