#ifndef VK_TOPMOSTWINDOWCMP_H
#define VK_TOPMOSTWINDOWCMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Win32App.h"

class TopMostWindowComponent : public Component
{
public:
	friend class Component;

	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	~TopMostWindowComponent();

	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);
	Status onWindowZOrderChange(const IEventArgs& evtArgs);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	TopMostWindowComponent(const std::weak_ptr<IApp>& app);

	Status registerEvents();

private:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // VK_TOPMOSTWINDOWCMP_H