#ifndef CONTROLLER_H
#define CONTROLLER_H
/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

#include <memory>

// forward declarations
struct View;
struct Agent;

class Controller {
public:	
	Controller();

	// run the program by acccepting user commands
	void run();

private:

	// command functions by category
	// view:
	void default_fn();
	void size();
	void zoom();
	void pan();
	// new p5 functionality
	void open();
	void close();

	// program-wide commands
	void status();
	void show();
	void go();
	void build();
	void train();

	// agent commands
	void move(std::shared_ptr<Agent>);
	void work(std::shared_ptr<Agent>);
	void attack(std::shared_ptr<Agent>);
	void stop(std::shared_ptr<Agent>);
};

#endif