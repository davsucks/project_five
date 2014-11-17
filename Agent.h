#ifndef AGENT_H
#define AGENT_H
/* 
Agents are a kind of Sim_object, and privately inherit from Moving_object.
Agents can be commanded to move to a destination. Agents have a health, which
is decreased when they take a hit. If the Agent's health > 0, it is alive.
If its heath <= 0, it starts dying, then on subsequent updates, 
it becomes dead, and finally disappearing.
*/
#include "Sim_object.h"
#include "Moving_object.h"
#include <string>

// forward declarations
struct Structure;
struct Point;

class Agent : public Sim_object, private Moving_object {
public:

	Agent(const std::string& name_, Point location_);
	~Agent();

	// return true if this agent is Alive or Disappearing
	bool is_alive() const
	{return health_state == Health_State_e::ALIVE;}
	bool is_disappearing() const
	{return health_state == Health_State_e::DISAPPEARING;}
	
	// return this Agent's location
	Point get_location() const override
	{return get_current_location();}

	// return true if this Agent is in motion
	bool is_moving() const
	{return is_currently_moving();}
	
	// tell this Agent to start moving to location destination_
	virtual void move_to(Point destination_);

	// tell this Agent to stop its activity
	virtual void stop();

	// Tell this Agent to accept a hit from an attack of a specified strength
	// The attacking Agent identifies itself with its this pointer.
	// A derived class can override this function.
	// The function lose_health is called to handle the effect of the attack.
	virtual void take_hit(int attack_strength, Agent *attacker_ptr);
	
	// update the moving state and Agent state of this object.
	void update() override;
	
	// output information about the current state
	void describe() const override;
	
	// ask Model to broadcast our current state to all Views
	void broadcast_current_state() override;

	/* Fat Interface for derived classes */
	// Throws exception that an Agent cannot work.
	virtual void start_working(Structure *, Structure *);

	// Throws exception that an Agent cannot attack.
	virtual void start_attacking(Agent *);

protected:
	

	// calculate loss of health due to hit.
	// if health decreases to zero or negative, Agent state becomes Dying, and any movement is stopped.
	void lose_health(int attack_strength);

private:

	enum class Health_State_e { ALIVE, DYING, DEAD, DISAPPEARING };
	Health_State_e health_state;

	int health;
};

#endif
