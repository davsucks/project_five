#include "Sim_object.h"
#include <string>
#include <iostream>
using namespace std;

Sim_object::Sim_object(const string& name_)
:
name(name_)
{
	cout << "Sim_object " << name << " constructed" << endl;
}

Sim_object::~Sim_object()
{
	cout << "Sim_object " << name << " destructed" << endl;
}
