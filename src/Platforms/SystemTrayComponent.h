#ifndef CBA_SYSTRAYCMP_H
#define CBA_SYSTRAYCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Win32App.h"
#include <shellapi.h>

class SystemTrayComponent : public Component
{
public:
	friend class Component;

	// Static Variables //
	static const Status WM_TRAY_ICON;

	// Static Methods //

	// Class Variables //
	Status ID_TRAY_ICON = Status::registerState(_T("System Tray Icon ID"));
	Status trayIconID;
	NOTIFYICONIDENTIFIER identifier;

	// Class Methods //
	virtual ~SystemTrayComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //
	NOTIFYICONDATA notifyIconData;
	const tstring smallIconPath;
	const tstring tooltip;

	// Class Methods //
	SystemTrayComponent(const std::weak_ptr<IApp>& app, const tstring smallIconPath, const tstring tooltip);

	Status registerEvents();

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_SYSTRAYCMP_H