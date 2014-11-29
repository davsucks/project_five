#ifndef WARRIORS_H
#define WARRIORS_H

/*
Warrior is an abstract base class that defines the interface that is
inherited by Soldier and Archer
The warrior class is responsible for data regarding its attack, including
attack strength, range, noise and target. In addition Warrior keeps track
of its attack state and offers a protected method for derived classes to 
reset that attack state, settings its target to nullptr and assigning
its state accordingly
*/
#include "Agent.h"
#include <memory>
#include <string>

class Warrior : public Agent {
public:
	
	Warrior(const std::string& name_, Point location_, int attack_strength, int attack_range, const char* attack_noise);
	~Warrior();
	
	// update implements Soldier behavior
	void update() override;
	
	// Make this Soldier start attacking the target Agent.
	// Throws an exception if the target is the same as this Agent,
	// is out of range, or is not alive.
	void start_attacking(std::shared_ptr<Agent> target_ptr) override;
	
	// Overrides Agent's take_hit to counterattack when attacked.
	void take_hit(int attack_strength, std::shared_ptr<Agent> attacker_ptr) override;
	
	// Overrides Agent's stop to print a message
	void stop() override;

	// output information about the current state
	void describe() const override;

	// returns true if the agent is currently attacking
	bool is_attacking()
	{return attack_state == Attack_State_e::ATTACKING;}

protected:

	// will set the warriors state to not attacking and forget its target
	void reset_attacking();
	// will set the warriors target, output that he's attacking, and set the state accordingly
	void attack(std::shared_ptr<Agent> target_ptr);

private:

	enum class Attack_State_e { ATTACKING, NOT_ATTACKING };
	Attack_State_e attack_state;
	
	int attack_strength;
	int attack_range;
	std::weak_ptr<Agent> target;
	std::string* attack_noise;
};

/*
Soldier is a type of warrior, it is fairly stupid.
It doesn't listen to you if you tell him to stop, and will continue to attack
its target until either he dies or his target goes out of range
*/
class Soldier : public Warrior {
public: 
	Soldier(const std::string& name_, Point location_);
	// loses health, and attacks its aggressor
	void take_hit(int attack_strength, std::shared_ptr<Agent> attack_ptr) override;
	// outputs soldier and then calls warrior's describe
	void describe() const override;
};

/*
Archer is another type of warrior, and its more cowardly and aggressive than its 
Soldier bretheren. At every turn, he looks around for the closest agent and
attacks him or her if in range. When attacked, he runs to the closest structure for safety.
*/
class Archer : public Warrior {
public:
	Archer(const std::string& name_, Point location_);
	// unless he's attacking, will find the closest agent and attack him if in range
	void update() override;
	// runs to the closest structure if its in range
	void take_hit(int attack_strength, std::shared_ptr<Agent> attack_ptr) override;
	// outputs archer and then calls warriors describe
	void describe() const override;
};

#endif
