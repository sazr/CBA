#include "DPIAwareComponent.h"

// Class Property Implementation //
DPIScaleHelper DPIAwareComponent::scaleHelper;
bool DPIAwareComponent::initialised = false;

// Static Function Implementation //
int DPIAwareComponent::scaleUnits(int n)
{
	if (!initialised || scaleHelper.GetScaleFactor() == UINT_MAX)
		return -1;

	return scaleHelper.ScaleValue(n);
}

Status DPIAwareComponent::scaleRect(RECT& rect)
{
	if (!initialised || scaleHelper.GetScaleFactor() == UINT_MAX)
		return S_UNDEFINED_ERROR;

	scaleHelper.ScaleRectangle(&rect);
	return S_SUCCESS;
}

// Function Implementation //
DPIAwareComponent::DPIAwareComponent(const std::weak_ptr<IApp>& app) : Component(app)
{
	if (!initialised) {
		registerEvents();
		initialised = true;
	}
}

DPIAwareComponent::~DPIAwareComponent()
{

}

Status DPIAwareComponent::init(const IEventArgs& evtArgs)
{
	HRESULT res = SetProcessDpiAwareness(PROCESS_SYSTEM_DPI_AWARE);

	UINT dpiX = 0, dpiY = 0;
	POINT point{ 1, 1 };
	HMONITOR hMonitor = MonitorFromPoint(point, MONITOR_DEFAULTTONEAREST);
	HRESULT hr = GetDpiForMonitor(hMonitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);

	scaleHelper.SetScaleFactor(dpiX);

	return S_SUCCESS;
}

Status DPIAwareComponent::terminate(const IEventArgs& evtArgs)
{

	return S_SUCCESS;
}

Status DPIAwareComponent::registerEvents()
{
	registerEvent(Win32App::WM_CUSTOM_PRE_CREATE.state, &DPIAwareComponent::init);
	return S_SUCCESS;
}

// DPIScaleHelper Function Implementation //
DPIScaleHelper::DPIScaleHelper()
{
	m_nScaleFactor = UINT_MAX;
}

UINT DPIScaleHelper::GetScaleFactor()
{
	return m_nScaleFactor;
}

void DPIScaleHelper::SetScaleFactor(__in UINT iDPI)
{
	m_nScaleFactor = MulDiv(iDPI, 100, 96);
}

int DPIScaleHelper::ScaleValue(int value)
{
	return MulDiv(value, m_nScaleFactor, 100);
}

void DPIScaleHelper::ScaleRectangle(__inout RECT *pRectangle)
{
	// Post: Scale rectangle from raw pixels to relative pixels.
	
	pRectangle->left = ScaleValue(pRectangle->left);
	pRectangle->right = ScaleValue(pRectangle->right);
	pRectangle->top = ScaleValue(pRectangle->top);
	pRectangle->bottom = ScaleValue(pRectangle->bottom);
}

void DPIScaleHelper::ScalePoint(__inout POINT *pPoint)
{
	// Post: Scale Point from raw pixels to relative pixels.

	pPoint->x = ScaleValue(pPoint->x);
	pPoint->y = ScaleValue(pPoint->y);
}

HFONT DPIScaleHelper::CreateScaledFont(int nFontHeight)
{
	int nScaledFontHeight = ScaleValue(nFontHeight);
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfQuality = CLEARTYPE_QUALITY;
	lf.lfHeight = -nScaledFontHeight;
	auto hFont = CreateFontIndirect(&lf);
	return hFont;
}