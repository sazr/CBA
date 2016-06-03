#include "Status.h"

// Class Property Implementation //
STATE CStatus::nextState								= 50000;
std::unordered_map<STATE, tstring> CStatus::states;
Status S_SUCCESS										= CStatus::registerState(_T("Success"));
Status S_UNDEFINED_ERROR								= CStatus::registerState(_T("Undefined Error"));
Status S_INVALID_PTR									= CStatus::registerState(_T("Expired Pointer"));

// Static Function Implementation //
Status CStatus::registerState(const tstring &stateMsg)
{
	// Post: 

	// Initialise here to avoid the "static initialisation order fiasco"
	static STATE nextState = 50000;
	static std::unordered_map<STATE, tstring> states;

	int nextStateTmp = nextState + 1;
	auto res = states.emplace(std::make_pair(nextStateTmp, stateMsg));

	return (res.second) ? Status(++nextState) : Status(res.first->first);
}

// Function Implementation //
CStatus::CStatus(const STATE &state) : state(state)
{

}

const tstring CStatus::toString() const 
{
	return (states.find(state) == states.end()) ? states[state] : _T("Undefined");
}

CStatus::operator tstring() const
{
	return toString();
}

CStatus::operator int() const
{
	return state;
}

bool CStatus::operator==(const CStatus& obj) const
{
	return state == obj.state;
}

bool CStatus::operator!=(const CStatus& obj) const
{
	return state != obj.state;
}

bool CStatus::operator!() const
{
	return *this != S_SUCCESS;
}

std::ostream& operator <<(std::ostream &stream, const CStatus& obj)
{
	return stream << obj.toString().c_str();
}
