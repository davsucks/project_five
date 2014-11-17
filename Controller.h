#ifndef CONTROLLER_H
#define CONTROLLER_H
/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

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
	void default_fn(View*);
	void size(View*);
	void zoom(View*);
	void pan(View*);

	// program-wide commands
	void status(View*);
	void show(View*);
	void go(View*);
	void build(View*);
	void train(View*);

	// agent commands
	void move(Agent*);
	void work(Agent*);
	void attack(Agent*);
	void stop(Agent*);
};

#endif