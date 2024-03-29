#ifndef AGENT_H
#define AGENT_H
/* 
Agents are a kind of Sim_object, and privately inherit from Moving_object.
Agents can be commanded to move to a destination. Agents have a health, which
is decreased when they take a hit. If the Agent's health > 0, it is alive.
If its heath <= 0, it starts dying, then on subsequent updates, 
it becomes dead, and finally disappearing.
*/
#include "Moving_object.h"
#include "Sim_object.h"

#include <memory>
#include <string>

// forward declarations
struct Structure;
struct Point;

class Agent : public Sim_object, public std::enable_shared_from_this<Agent> {
public:

	Agent(const std::string& name_, Point location_);

	// return true if this agent is Alive
	bool is_alive() const
	{return health_state == Health_State_e::ALIVE;}
	// return this Agent's location
	Point get_location() const override
	{return moving_obj.get_current_location();}

	// return true if this Agent is in motion
	bool is_moving() const
	{return moving_obj.is_currently_moving();}
	
	// tell this Agent to start moving to location destination_
	virtual void move_to(Point destination_);

	// tell this Agent to stop its activity
	virtual void stop();

	// Tell this Agent to accept a hit from an attack of a specified strength
	// The attacking Agent identifies itself with its this pointer.
	// A derived class can override this function.
	// The function lose_health is called to handle the effect of the attack.
	virtual void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr);
	
	// update the moving state and Agent state of this object.
	void update() override;
	
	// output information about the current state
	void describe() const override;
	
	// ask Model to broadcast our current state to all Views
	void broadcast_current_state() override;

	/* Fat Interface for derived classes */
	// Throws exception that an Agent cannot work.
	virtual void start_working(std::shared_ptr<Structure>, std::shared_ptr<Structure>);

	// Throws exception that an Agent cannot attack.
	virtual void start_attacking(std::shared_ptr<Agent>);

protected:
	
	// calculate loss of health due to hit.
	// if health decreases to zero or negative, Agent state becomes Dead, and any movement is stopped.
	void lose_health(int attack_strength);

private:
	
	// Agent how "has-a" moving object
	Moving_object moving_obj;
	// Emun to indicate if the agent is alive or dead
	enum class Health_State_e { ALIVE, DEAD };
	Health_State_e health_state;
	// represents the amount of health an agent has
	int health;
};

#endif
