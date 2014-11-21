#include "Farm.h"
#include <iostream>
using namespace std;

// define constant values
static const int def_amount_c {50};
static const int def_prod_rate_c {2};

Farm::Farm(const string& name_, Point location_)
: 
Structure(name_, location_),
amount {def_amount_c},
production_rate {def_prod_rate_c}
{
	cout << "Farm " << name_ << " constructed" << endl;
}

Farm::~Farm()
{
	cout << "Farm " << get_name() << " destructed" << endl;
}

double Farm::withdraw(double amount_to_get)
{
	if (amount_to_get > amount)
		amount_to_get = amount;

	amount -= amount_to_get;
	return amount_to_get;
}

void Farm::update()
{
	amount += production_rate;
	cout << "Farm " << get_name() << " now has " << amount << endl;
}

void Farm::describe() const
{
	cout << "Farm ";
	Structure::describe();
	cout << "   Food available: " << amount << endl;
}
