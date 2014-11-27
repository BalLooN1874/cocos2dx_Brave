#pragma  once
#include "cocos2d.h"
#include <functional>
USING_NS_CC;

//×´Ì¬»úÀà
class CFSM : public cocos2d::Ref
{
public:
	//create FSM with a initial state name and optional callback function
	static CFSM* create(std::string& state, std::function<void()> onEnter = nullptr);

	CFSM(std::string& state, std::function<void()> onEnter = nullptr);

	//add state into FSM
	CFSM* addState(std::string& state, std::function<void()> onEnter = nullptr);

	//add Event into  FSM
	CFSM* addEvent(const std::string& eventName, const std::string& from, const std::string& to);

	//check if state is already in FSM
	bool isContainState(const std::string& stateName);

	//printf a list of states
	void printfState();

	//do the event 
	void doEvent(const std::string& eventName);

	//check if the event can change state
	bool canDoEvent(const std::string& eventName);

	//set the onEnter callback for a specified state
	void setOnEnter(const std::string& state, std::function<void()> onEnter);

	bool init();
private:
	//change state and run callback
	void changeToState(const std::string& state);

private:
	std::set<std::string> m_states;
	std::unordered_map<std::string, std::unordered_map<std::string, std::string> > m_events;
	std::unordered_map<std::string, std::function<void()> > m_onEnters;
	std::string  m_currentState;
	std::string  m_previousState;
};