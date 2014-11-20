#include "Controller.h"
#include "Model.h"
#include "View.h"
#include "Structure.h"
#include "Utility.h"
#include "Structure_factory.h"
#include "Agent.h"
#include "Agent_factory.h"
#include <iostream>
#include <map>
#include <locale>
#include <algorithm>
#include <cassert>
using namespace std;

// types for the command map
// TODO: review proj3 feedback and see what kieras said about this
using CommandFunction = void (Controller::*)(shared_ptr<View>);
using Command_Map_t = map<string, CommandFunction>;

using AgentCommand = void (Controller::*)(shared_ptr<Agent>);
using Agent_Command_Map_t = map<string, AgentCommand>;


// string constants for error messages
const char* const expected_int {"Expected an integer!"};
const char* const expected_double {"Expected a double!"};
const char* const unrecognized_cmd {"Unrecognized command!"};
const char* const invalid_name {"Invalid name for new object!"};
const char* const dead_agent {"Agent is not alive!"};

Controller::Controller()
{}

void clear_and_skip_line()
{
	cin.clear();
	while (cin.get() != '\n');
}

void Controller::run()
{
	shared_ptr<View> view(new View);
	Model::get_Model()->attach(view);

	Command_Map_t command_map;
	Agent_Command_Map_t agent_command_map;

	// populate the commands
	// view:
	command_map["default"] = &Controller::default_fn;
	command_map["size"] = &Controller::size;
	command_map["zoom"] = &Controller::zoom;
	command_map["pan"] = &Controller::pan;

	// program-wide commands
	command_map["status"] = &Controller::status;
	command_map["show"] = &Controller::show;
	command_map["go"] = &Controller::go;
	command_map["build"] = &Controller::build;
	command_map["train"] = &Controller::train;

	// agent commands
	agent_command_map["move"] = &Controller::move;
	agent_command_map["work"] = &Controller::work;
	agent_command_map["attack"] = &Controller::attack;
	agent_command_map["stop"] = &Controller::stop;

	// command loop!
	string first_word;
	while(true) {
		try {
		cout << "\nTime " << Model::get_Model()->get_time() << ": Enter command: ";
		cin >> first_word;
		if (first_word == "quit") {
			// TODO: shouldn't need this, shared_ptrs yo
			// delete view;
			cout << "Done" << endl;
			return;
		}
		// test if word is name of an agent
		if (Model::get_Model()->is_agent_present(first_word)) {
			shared_ptr<Agent> agent = Model::get_Model()->get_agent_ptr(first_word);
			assert(agent->is_alive());
			string cmd_name;
			cin >> cmd_name;
			// find command from appropriate command map
			auto cmd_fn = agent_command_map.find(cmd_name);
			// test it was actually found
			if(cmd_fn == agent_command_map.end())
				throw Error(unrecognized_cmd);
			// cmd_fn is an iterator, so call its mapped value (function)
			auto mem = cmd_fn->second;
			(this->*mem)(agent);
		} else {
			// test to see if the command is in the map
			auto cmd_fn = command_map.find(first_word);
			// test if it was there
			if(cmd_fn == command_map.end())
				throw Error(unrecognized_cmd);
			auto mem = cmd_fn->second;
			(this->*mem)(view);			
		}
		}
		// end of try block
		catch(exception& e) {
			cout << e.what() << endl;
			clear_and_skip_line();
		} catch(...) {
			cout << "Unknown exception caught!" << endl;
			clear_and_skip_line();
		}
	}
}

// command functions by category
// view:
void Controller::default_fn(shared_ptr<View> view)
{
	view->set_defaults();
}

// checks that cin is valid, if not, throws an error containing message
void check_cin(string message)
{
	if (!cin) {
		throw Error(message);
	}
}
void Controller::size(shared_ptr<View> view)
{
	int size;
	cin >> size;
	check_cin(expected_int);
	view->set_size(size);
}

void Controller::zoom(shared_ptr<View> view)
{
	double scale;
	cin >> scale;
	check_cin(expected_double);
	view->set_scale(scale);
}

Point read_Point()
{
	double x, y;
	cin >> x;
	check_cin(expected_double);
	cin >> y;
	check_cin(expected_double);
	return Point(x, y);
}
void Controller::pan(shared_ptr<View> view)
{
	view->set_origin(read_Point());
}

// program-wide commands
void Controller::status(shared_ptr<View>)
{
	Model::get_Model()->describe();
}

void Controller::show(shared_ptr<View> view)
{
	view->draw();
}

void Controller::go(shared_ptr<View>)
{
	Model::get_Model()->update();
}

bool char_is_alnum(char c)
{
	return isalnum(c);
}
bool string_is_alnum(const string &str)
{
    return find_if_not(str.begin(), str.end(), char_is_alnum) == str.end();
}
// throws an error if the name is less than 2 characters,
// if a name was unable to be read to cin, or if the name
// isn't alphanumeric
void check_name(string name)
{
	const int min_chars_c {2};

	bool too_short = name.length() < min_chars_c;
	bool not_alnum = !string_is_alnum(name);
	if (too_short || not_alnum || Model::get_Model()->is_name_in_use(name))
		throw Error(invalid_name);
}
void Controller::build(shared_ptr<View>)
{
	string name, type;
	cin >> name;
	check_name(name);
	cin >> type;
	Point location = read_Point();
	shared_ptr<Structure> new_structure = create_structure(name, type, location);
	Model::get_Model()->add_structure(new_structure);
}

void Controller::train(shared_ptr<View>)
{
	string name, type;
	cin >> name;
	check_name(name);
	cin >> type;
	Point location = read_Point();
	shared_ptr<Agent> new_agent = create_agent(name, type, location);
	Model::get_Model()->add_agent(new_agent);
}

// agent commands
void Controller::move(shared_ptr<Agent> agent)
{
	Point location = read_Point();
	agent->move_to(location);
}

void Controller::work(shared_ptr<Agent> agent)
{
	string destination_str, source_str;
	cin >> source_str;
	shared_ptr<Structure> source = Model::get_Model()->get_structure_ptr(source_str);
	cin >> destination_str;
	shared_ptr<Structure> destination = Model::get_Model()->get_structure_ptr(destination_str);
	agent->start_working(source, destination);
}

void Controller::attack(shared_ptr<Agent> agent)
{
	string name;
	cin >> name;
	shared_ptr<Agent> victim = Model::get_Model()->get_agent_ptr(name);
	agent->start_attacking(victim);
}

void Controller::stop(shared_ptr<Agent> agent)
{
	agent->stop();
}
