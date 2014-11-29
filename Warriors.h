#ifndef WARRIORS_H
#define WARRIORS_H

/*
A Soldier is an Agent that has attack and defense behaviors. It can be commanded
to start attacking another Agent and will continue the attack as long as 
it is alive and the target is alive and in range. If attacked, the Soldier will
start attacking its attacker.
*/
#include <memory>
#include <string>
#include "Agent.h"

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

	bool is_attacking()
	{return attack_state == Attack_State_e::ATTACKING;}

protected:
	// these functions
	void reset_attacking();

	void attack(std::shared_ptr<Agent> target_ptr);

private:
	enum class Attack_State_e { ATTACKING, NOT_ATTACKING };
	Attack_State_e attack_state;
	
	int attack_strength;
	int attack_range;
	std::weak_ptr<Agent> target;
	std::string* attack_noise;
};

class Soldier : public Warrior {
public: 
	Soldier(const std::string& name_, Point location_);

	void take_hit(int attack_strength, std::shared_ptr<Agent> attack_ptr) override;

	void describe() const override;
};

class Archer : public Warrior {
public:
	Archer(const std::string& name_, Point location_);

	void update() override;

	void take_hit(int attack_strength, std::shared_ptr<Agent> attack_ptr) override;

	void describe() const override;
};

#endif
