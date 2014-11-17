#ifndef STRUCTURE_H
#define STRUCTURE_H

/* A Structure is a Sim_object with a location and interface to derived types */
#include "Sim_object.h"
#include "Geometry.h"
#include <string>

class Structure : public Sim_object {
public:
	Structure(std::string _name, Point _location);

	virtual ~Structure() = 0;
		
	Point get_location() const
	{return location;}

	void update(){/*noop*/};

	// output information about the current state
	virtual void describe() const;

	// ask model to notify views of current state
    virtual void broadcast_current_state();
    
	// fat interface for derived types	
	virtual double withdraw(double amount_to_get)
	{return 0.0;}
	virtual void deposit(double amount_to_give)
	{}

private:
	Point location;
};

#endif
