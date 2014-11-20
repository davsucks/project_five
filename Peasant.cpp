#include "Peasant.h"
#include "Structure.h"
#include "Utility.h"
#include <iostream>
using namespace std;

static const int max_food {35};

Peasant::Peasant(const string& name_, Point location_)
:
Agent(name_, location_),
working_state {Working_State_e::NOT_WORKING},
amount {0}
{
	cout << "Peasant " << name_ << " constructed" << endl;
}

Peasant::~Peasant()
{
	cout << "Peasant " << get_name() << " destructed" << endl;
}

// implement Peasant behavior
void Peasant::update()
{
	// first update agent's state
	Agent::update();
	// check if the agent's alive or not moving
	if (!Agent::is_alive() || working_state == Working_State_e::NOT_WORKING) {
		// do nothing
		return;
	}
	// update working state
	switch(working_state) {
		case Working_State_e::INBOUND: {
			if (!Agent::is_moving() && Agent::get_location() == source->get_location()) {
				// we have arrived at the source
				working_state = Working_State_e::COLLECTING;
			}
			break;
		}
		case Working_State_e::COLLECTING: {
			double request = max_food - amount;
			double received = source->withdraw(request);
			amount += received;

			if (received > 0.0) {
				cout << get_name() << ": Collected " << received << endl;
				working_state = Working_State_e::OUTBOUND;
				Agent::move_to(destination->get_location());
			} else {
				cout << get_name() << ": Waiting " << endl;
			}
			break;
		}
		case Working_State_e::OUTBOUND: {
			if (!Agent::is_moving() && Agent::get_location() == destination->get_location())
				working_state = Working_State_e::DEPOSITING;
			break;
		}
		case Working_State_e::DEPOSITING: {
			// deposit the amount we're carrying
			destination->deposit(amount);
			cout << get_name() << ": Deposited " << amount << endl;
			amount = 0;
			Agent::move_to(source->get_location());
			working_state = Working_State_e::INBOUND;
			break;
		}
		case Working_State_e::NOT_WORKING:
		default:
			break;
	}
}

void Peasant::stop_working()
{
	switch(working_state) {
		case Working_State_e::INBOUND:
		case Working_State_e::COLLECTING:
		case Working_State_e::OUTBOUND:
		case Working_State_e::DEPOSITING:
			cout << get_name() << ": I'm stopping work" << endl;
			working_state = Working_State_e::NOT_WORKING;
			// note: break intentionally left out
		case Working_State_e::NOT_WORKING:
			source.reset();
			destination.reset();
			break;
	}
}

// overridden to suspend working behavior
void Peasant::move_to(Point dest)
{
	if(working_state != Working_State_e::NOT_WORKING) {
		stop_working();
		working_state = Working_State_e::NOT_WORKING;
	}
	Agent::move_to(dest);
}

// stop moving and working
void Peasant::stop()
{
	Agent::stop();
	stop_working();
}

// starts the working process
// Throws an exception if the source is the same as the destination.
void Peasant::start_working(shared_ptr<Structure> source_, shared_ptr<Structure> destination_)
{
	Agent::stop();
	working_state = Working_State_e::NOT_WORKING;
	source.reset();
	destination.reset();
	// error check
	if (source_ == destination_)
		throw Error(get_name() + ": I can't move food to and from the same place!");

	// remember the source and destination
	source = source_;
	destination = destination_;

	if (amount == 0.0) {
		if (get_location() == source->get_location())
			working_state = Working_State_e::COLLECTING;
		else {
			Agent::move_to(source->get_location());
			working_state = Working_State_e::INBOUND;
		}
	} else {
		if (get_location() == destination->get_location())
			working_state = Working_State_e::DEPOSITING;
		else {
			Agent::move_to(destination->get_location());
			working_state = Working_State_e::OUTBOUND;
		}
	}
}

// output information about the current state
void Peasant::describe() const
{
	cout << "Peasant ";
	Agent::describe();
	cout << "   Carrying " << amount << endl;
	switch(working_state) {
		case Working_State_e::INBOUND:
			cout << "   Inbound to source " << source->get_name() << endl;
			break;
		case Working_State_e::OUTBOUND:
			cout << "   Outbound to destination " << destination->get_name() << endl;
			break;
		case Working_State_e::COLLECTING:
			cout << "   Collecting at source " << source->get_name() << endl;
			break;
		case Working_State_e::DEPOSITING:
			cout << "   Depositing at destination " << destination->get_name() << endl;
			break;
		case Working_State_e::NOT_WORKING:
		default:
			break;
	}
}