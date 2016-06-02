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

