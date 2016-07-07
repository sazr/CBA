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

#ifndef CBA_STATUS_H
#define CBA_STATUS_H

#include "CBA.h"

class CStatus;
typedef int STATE;
typedef std::unordered_map<STATE, tstring>::iterator StateIterator;
typedef const CStatus Status;

#ifndef S_SUCCESS
#define S_SUCCESS CStatus::CS_SUCCESS
#endif // S_SUCCESS

#ifndef S_UNDEFINED_ERROR
#define S_UNDEFINED_ERROR CStatus::CS_UNDEFINED_ERROR
#endif // S_UNDEFINED_ERROR

#ifndef S_INVALID_PTR
#define S_INVALID_PTR CStatus::CS_INVALID_PTR
#endif // S_INVALID_PTR


class CStatus
{
public:
	// Static Properties//
	static Status CS_SUCCESS;
	static Status CS_UNDEFINED_ERROR;
	static Status CS_INVALID_PTR;

	// Static Functions //
	static Status registerState(const tstring &stateMsg);

	// Properties //
	const STATE state;

	// Functions //
	explicit CStatus(const STATE &state);

	const tstring toString() const;
	operator tstring() const;
	operator int() const;
	bool operator==(const CStatus& obj) const;
	bool operator!=(const CStatus& obj) const;
	bool operator!() const;

	friend std::ostream& operator<<(std::ostream& stream, const CStatus& obj);

private:
	static std::unordered_map<STATE, tstring> states;
	static STATE nextState;

protected:

};


#endif // CBA_STATUS_H

