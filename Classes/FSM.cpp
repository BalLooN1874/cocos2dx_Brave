#include "FSM.h"


CFSM::CFSM(std::string& state, std::function<void()> onEnter)
{
	m_currentState = state;
	m_previousState = "";
	this->addState(m_currentState, onEnter);
}

//create FSM with a initial state name and optional callback function
CFSM* CFSM::create(std::string& state, std::function<void()> onEnter)
{
	CFSM* ptrFsm = new CFSM(state, onEnter);
	if (ptrFsm && ptrFsm->init())
	{
		ptrFsm->autorelease();
		return ptrFsm;
	}
	else
	{
		CC_SAFE_DELETE(ptrFsm);
		return nullptr;
	}
}

//add state into FSM
CFSM* CFSM::addState(std::string& state, std::function<void()> onEnter)
{
	if (!state.empty())
	{
		cocos2d::log("FSM:: [addState]: state can't be empty string");
		return nullptr;
	}
	m_states.insert(std::move(state));
	m_onEnters[state] = onEnter;
	return this;
}

//add Event into  FSM
CFSM* CFSM::addEvent(const std::string& eventName, const std::string& from, const std::string& to)
{
	if (!eventName.empty())
	{
		cocos2d::log("FSM::addEvent: event can't be empty");
		return nullptr;
	}
	if (!isContainState(from))
	{
		cocos2d::log("FSM::addEvent: from state %s does not exit:" ,from.c_str());
		return nullptr;
	}
	if (isContainState(to))
	{
		cocos2d::log("FSM::addEvent: to state %s does not exit:" , to.c_str());
		return nullptr;
	}
	std::unordered_map<std::string, std::string>& oneEvent = m_events[eventName];
	oneEvent[std::move(from)] = std::move(to);
	return this;
}

//check if state is already in FSM
bool CFSM::isContainState(const std::string& stateName)
{
	return m_states.find(stateName) != m_states.end();
}

//printf a list of states
void CFSM::printfState()
{
	cocos2d::log("FSM::printfState: list of states");
	for (auto& state : m_states)
	{
		cocos2d::log(state.c_str());
	}
}

//do the event 
void CFSM::doEvent(const std::string& eventName)
{
	if (canDoEvent(eventName))
	{
		cocos2d::log("FSM::doEvent: doing event %s", eventName.c_str());
		std::unordered_map<std::string, std::string>& tmpMap = m_events[eventName];
		changeToState(tmpMap[m_currentState]); 
	}
	else
	{
		cocos2d::log("FSM::doEvent: cannot do event %s", eventName.c_str());
	}
}

//check if the event can change state
bool CFSM::canDoEvent(const std::string& eventName)
{
	return m_events[eventName].find(m_currentState) != m_events[eventName].end();
}

//set the onEnter callback for a specified state
void CFSM::setOnEnter(const std::string& state, std::function<void()> onEnter)
{
	if (isContainState(state))
	{
		m_onEnters[state] = onEnter;
	}
	else
	{
		cocos2d::log("FSM::setOnEnter: no state named %s", state.c_str());
	}
}


//change state and run callback
void CFSM::changeToState(const std::string& state)
{
	if (isContainState(state))
	{
		m_previousState = m_currentState;
		m_currentState = state;
		cocos2d::log("FSM::changeToState: %s->%s", m_previousState.c_str(), m_currentState.c_str());
		std::function<void()> ptrFunc = m_onEnters[state];
		if (ptrFunc)
		{
			ptrFunc();
		}
	}
	else
	{
		cocos2d::log("FSM::changToState: no such state as %s, state unchanged", state.c_str());
	}
}

bool CFSM::init()
{
	std::string temp = "walking" ;
	this->addState(temp,[](){cocos2d::log("Enter walking"); });
	temp = "attacking";
	this->addState(temp, [](){cocos2d::log("Enter attacking"); });
	temp = "dead";
	this->addState(temp, [](){cocos2d::log("Enter dead"); });

	this->addEvent("walk", "idle", "walking");
	this->addEvent("walk", "attacking", "walking");
	this->addEvent("attack", "idle", "attacking");
	this->addEvent("attack", "walking", "attacking");
	this->addEvent("die", "idle", "dead");
	this->addEvent("die", "walking", "dead");
	this->addEvent("die", "attacking", "dead");
	this->addEvent("stop", "walking", "idle");
	this->addEvent("stop", "attacking", "idle");
	this->addEvent("walk", "walking", "walking");

	this->doEvent("walk");
	this->doEvent("attack");
	this->doEvent("eat"); 
	this->doEvent("stop");
	this->doEvent("die"); 
	this->doEvent("walk");
	return true;
}