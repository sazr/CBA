/*
Copyright (c) 2016 Sam Zielke-Ryner All rights reserved.

For job opportunities or to work together on projects please contact
myself via Github:   https://github.com/sazr

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. The source code, API or snippets cannot be used for commercial
purposes without written consent from the author.

THIS SOFTWARE IS PROVIDED ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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
	/*output(_T("Storing: %d, %s, Res: %d\n"), states.size(), stateMsg.c_str(), res.second);
	output(_T("Retrieval: [%d,%s]\n"), nextStateTmp, states[nextStateTmp].c_str());*/

	return (res.second) ? Status(++nextState) : Status(res.first->first);
}

// Function Implementation //
CStatus::CStatus(const STATE &state) : state(state)
{

}

const tstring CStatus::toString() const 
{
	//output(_T("FINDING: [%d,%s], %d\n"), state, states[state].c_str(), states.size());
	return (states.find(state) != states.end()) ? states[state] : _T("Undefined");
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
