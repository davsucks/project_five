#include "Town_Hall.h"
#include <iostream>
using namespace std;

Town_Hall::Town_Hall(const string& name_, Point location_)
:
Structure(name_, location_),
amount {0}
{
	cout << "Town_Hall " << name_ << " constructed" << endl;
}

Town_Hall::~Town_Hall()
{
	cout << "Town_Hall " << get_name() << " destructed" << endl;	
}

void Town_Hall::deposit(double deposit_amount)
{
	amount += deposit_amount;
}

double Town_Hall::withdraw(double amount_to_obtain)
{
	double amount_available = amount * .9;
	if (amount_available < 1.0)
		amount_to_obtain = 0;
	else if (amount_available < amount_to_obtain)
		amount_to_obtain = amount_available;

	amount -= amount_to_obtain;
	return amount_to_obtain;

}

void Town_Hall::describe() const
{
	cout << "Town_Hall ";
	Structure::describe();
	cout << "   Contains " << amount << endl;
}
