#ifndef MODEL_H
#define MODEL_H
/*
Model is part of a simplified Model-View-Controller pattern.
Model keeps track of the Sim_objects in our little world. It is the only
component that knows how many Structures and Agents there are, but it does not
know about any of their derived classes, nor which Agents are of what kind of Agent. 
It has facilities for looking up objects by name, and removing Agents.  When
created, it creates an initial group of Structures and Agents using the Structure_factory
and Agent_factory.
Finally, it keeps the system's time.

Controller tells Model what to do; Model in turn tells the objects what do, and
when asked to do so by an object, tells all the Views whenever anything changes that might be relevant.
Model also provides facilities for looking up objects given their name.

Notice how only the Standard Library headers need to be included - reduced coupling!

*/
#include <string>
#include <map>
#include <list>
#include <memory>
#include <utility>
// other forward declarations
struct Structure;
struct Agent;
struct View;
struct Point;
struct Sim_object;
 
class Model {
public:
	static Model* get_Model();

	// return the current time
	int get_time() {return time;}

	// is name already in use for either agent or structure?
    // return true if the name matches the name of an existing agent or structure
	bool is_name_in_use(const std::string& name) const;

	// is there a structure with this name?
	bool is_structure_present(const std::string& name) const;
	// add a new structure; assumes none with the same name
	void add_structure(std::shared_ptr<Structure>);
	// will throw Error("Structure not found!") if no structure of that name
	std::shared_ptr<Structure> get_structure_ptr(const std::string& name) const;

	// is there an agent with this name?
	bool is_agent_present(const std::string& name) const;
	// add a new agent; assumes none with the same name
	void add_agent(std::shared_ptr<Agent>);
	// will throw Error("Agent not found!") if no agent of that name
	std::shared_ptr<Agent> get_agent_ptr(const std::string& name) const;
	
	// tell all objects to describe themselves to the console
	void describe() const;
	// increment the time, and tell all objects to update themselves
	void update();

	// removes the agent from the appropriate containers
	void remove_agent(std::shared_ptr<Agent>);
	
	/* View services */
	// Attaching a View adds it to the container and causes it to be updated
    // with all current objects'location (or other state information.
	void attach(std::string view_name, std::shared_ptr<View>);
	// Detach the View by discarding the supplied pointer from the container of Views
    // - no updates sent to it thereafter.
	void detach(std::string view_name);
    // notify the views about an object's location
	void notify_location(const std::string& name, Point location);
	// notify the views about an object's amount
	void notify_amount(const std::string& name, double amount);
	// notify the views about an object's health
	void notify_health(const std::string& name, double health);
	// notify the views that an object is now gone
	void notify_gone(const std::string& name);
	// tells all the views in views to draw themselves
	void draw_all_views();
	// returns a shared pointer to the named view
	std::shared_ptr<View> get_view(const std::string& view_name);
	
private:
	// make Model a singleton by making the constructor private
	Model();
	~Model();

	static Model* ptr;

	int time;
	
	std::map<std::string, std::shared_ptr<Sim_object> > sim_objs;
	std::map<std::string, std::shared_ptr<Structure> > structure_objs;
	std::map<std::string, std::shared_ptr<Agent> > agent_objs;

	// used by models constructor
	void insert_Agent(std::shared_ptr<Agent>);
	void insert_Structure(std::shared_ptr<Structure>);

	// the list of views will store a pair corresponding to the views name, and the view
	std::list< std::pair<std::string, std::shared_ptr<View> > > views;

	// disallow copy/move construction or assignment
	Model(const Model&) = delete;
	Model& operator= (const Model&)  = delete;
	Model(Model&&) = delete;
	Model& operator= (Model&&) = delete;
};

#endif
