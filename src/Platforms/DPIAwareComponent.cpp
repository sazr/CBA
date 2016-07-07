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
	output(_T("DPIAwareComponent::init\n"));
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