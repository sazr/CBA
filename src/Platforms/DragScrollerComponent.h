#ifndef CBA_DPIAWARECMP_H
#define CBA_DPIAWARECMP_H

#include "../CBA.h"
#include "../Component.h"
#include "Win32App.h"
#include <ShellScalingAPI.h>
#pragma comment(lib, "Shcore.lib")

class DPIScaleHelper
{
public:
	DPIScaleHelper();

	UINT GetScaleFactor();
	void SetScaleFactor(__in UINT iDPI);
	int ScaleValue(int value);
	void ScaleRectangle(__inout RECT *pRectangle);
	void ScalePoint(__inout POINT *pPoint);
	HFONT CreateScaledFont(int nFontHeight);

private:
	UINT m_nScaleFactor;
};

class DPIAwareComponent : public Component
{
public:
	friend class Component;

	// Static Variables //

	// Static Methods //

	// Class Variables //
	
	// Class Methods //
	virtual ~DPIAwareComponent();

	Status preInit(const IEventArgs& evtArgs);
	Status init(const IEventArgs& evtArgs);
	Status terminate(const IEventArgs& evtArgs);

	static int scaleUnits(int n);
	static Status scaleRect(RECT& rect);

protected:
	// Static Variables //

	// Static Methods //

	// Class Variables //

	// Class Methods //
	DPIAwareComponent(const std::weak_ptr<IApp>& app);

	Status registerEvents();
	
private:
	// Static Variables //
	static DPIScaleHelper scaleHelper;
	static bool initialised;

	// Static Methods //

	// Class Variables //

	// Class Methods //

};

#endif // CBA_DPIAWARECMP_H