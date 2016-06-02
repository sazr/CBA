#ifndef CBA_WinUtilityComponentCMP_H
#define CBA_WinUtilityComponentCMP_H

#include "../CBA.h"
#include <Psapi.h>
#pragma comment(lib, "Psapi")

class WinUtilityComponent : public Component
{
public:
	friend class Component;

	// Static Variables //


	// Static Methods //


	// Class Variables //


	// Class Methods //
	virtual ~WinUtilityComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	Status getProcessFilePath(DWORD processId, tstring& filePath);
	Status getINISectionNames(const tstring& absINIPath, std::vector<tstring>& sectionNames);
	Status getINISectionKeyValues(const tstring& absINIPath, const tstring& sectionName, std::vector<tstring>& keys, std::vector<tstring>& values);
	Status getClientRect(RECT& clientRect);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	WinUtilityComponent(const std::weak_ptr<IApp>& app);

	Status registerEvents();
	Status onDisplayChange(const IEventArgs& evtArgs);

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	RECT clientArea;

	// Class Methods //

};

#endif // CBA_WinUtilityComponentCMP_H