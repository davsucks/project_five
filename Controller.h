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

	// create View object, run the program by acccepting user commands, then destroy View object
	void run();

private:

	// command functions by category
	// view:
	void default_fn(std::shared_ptr<View>);
	void size(std::shared_ptr<View>);
	void zoom(std::shared_ptr<View>);
	void pan(std::shared_ptr<View>);

	// program-wide commands
	void status(std::shared_ptr<View>);
	void show(std::shared_ptr<View>);
	void go(std::shared_ptr<View>);
	void build(std::shared_ptr<View>);
	void train(std::shared_ptr<View>);

	// agent commands
	void move(std::shared_ptr<Agent>);
	void work(std::shared_ptr<Agent>);
	void attack(std::shared_ptr<Agent>);
	void stop(std::shared_ptr<Agent>);
};

#endif