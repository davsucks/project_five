#ifndef CONTROLLER_H
#define CONTROLLER_H
/* Controller
This class is responsible for controlling the Model and View according to interactions
with the user.
*/

#include <memory>
#include <map>

// forward declarations
struct View;
struct Agent;

class Controller {
public:
	
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

	// view factory
	std::shared_ptr<View> create_view(const std::string& name);
	// queries views_in_use to see if the view is currently not
	// open and throws the error with the given message
	void check_if_not_open(const std::string& name, const std::string& error_msg);
	// an std::map to easily know which types of views are being used
	std::map<std::string, bool> views_in_use;
};

#endif