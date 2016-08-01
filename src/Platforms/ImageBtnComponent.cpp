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

#include "ImageBtnComponent.h"

// Class Property Implementation //
const tstring ImageBtnComponent::PROP_CENTRE_BMP = _T("PROP_CENTRE_BMP");
const tstring ImageBtnComponent::PROP_UP_BMP = _T("PROP_UP_BMP");
const tstring ImageBtnComponent::PROP_DOWN_BMP = _T("PROP_DOWN_BMP");
const tstring ImageBtnComponent::PROP_HOVER_BMP = _T("PROP_HOVER_BMP");

// Static Function Implementation //


// Function Implementation //
ImageBtnComponent::ImageBtnComponent(const std::weak_ptr<IApp>& app) : Component(app)
{
	registerEvents();
}

ImageBtnComponent::~ImageBtnComponent()
{

}

Status ImageBtnComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	return S_SUCCESS;
}

Status ImageBtnComponent::terminate(const IEventArgs& evtArgs)
{
	
	return S_SUCCESS;
}

Status ImageBtnComponent::registerEvents()
{
	//registerEvent(WM_CREATE, &ImageBtnComponent::init);
	//registerEvent(WM_CLOSE, &ImageBtnComponent::terminate);
	return S_SUCCESS;
}

Status ImageBtnComponent::registerBtnImages(HWND hwnd, HBITMAP upBmp, HBITMAP downBmp, HBITMAP hoverBmp)
{
	if (!SetProp(hwnd, PROP_UP_BMP.c_str(), (HANDLE)upBmp))
		return S_UNDEFINED_ERROR;
	if (!SetProp(hwnd, PROP_DOWN_BMP.c_str(), (HANDLE)downBmp))
		return S_UNDEFINED_ERROR;
	if (!SetProp(hwnd, PROP_HOVER_BMP.c_str(), (HANDLE)hoverBmp))
		return S_UNDEFINED_ERROR;

	registerEventLambda<ImageBtnComponent>(DispatchWindowComponent::translateMessage(hwnd, WM_DRAWITEM), [hwnd](const IEventArgs& evtArgs)->Status {
		
		const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
		
		RECT clientRect;
		LPDRAWITEMSTRUCT lps = static_cast<LPDRAWITEMSTRUCT>((LPDRAWITEMSTRUCT)args.lParam);
		//HWND hwnd = GetDlgItem(args.hwnd, args.wParam);
		HDC hdcMem = CreateCompatibleDC(lps->hDC);
		HBITMAP bm, centreBm;
		GetClientRect(hwnd, &clientRect);

		if (lps->itemAction == ODA_SELECT && lps->itemState & ODS_SELECTED) {
			bm = (HBITMAP)GetProp(hwnd, PROP_DOWN_BMP.c_str());
		}
		else if (lps->itemAction == ODA_FOCUS && lps->itemState & ODS_FOCUS) {
			bm = (HBITMAP)GetProp(hwnd, PROP_HOVER_BMP.c_str());
		}
		else {
			bm = (HBITMAP)GetProp(hwnd, PROP_UP_BMP.c_str());
		}

		centreBm = (HBITMAP)GetProp(hwnd, PROP_CENTRE_BMP.c_str());
		SelectObject(hdcMem, bm);

		StretchBlt(
			lps->hDC,
			lps->rcItem.left, // x upper left
			lps->rcItem.top, // y upper left
			lps->rcItem.right - lps->rcItem.left,
			lps->rcItem.bottom - lps->rcItem.top,
			hdcMem, // source device context
			0, 0, // x and y upper left
			clientRect.right - clientRect.left, // source bitmap width
			clientRect.bottom - clientRect.top, // source bitmap height
			SRCCOPY); // raster operation

		SelectObject(hdcMem, centreBm);

		StretchBlt(
			lps->hDC,
			lps->rcItem.left, // x upper left
			lps->rcItem.top, // y upper left
			lps->rcItem.right - lps->rcItem.left,
			lps->rcItem.bottom - lps->rcItem.top,
			hdcMem, // source device context
			0, 0, // x and y upper left
			clientRect.right - clientRect.left, // source bitmap width
			clientRect.bottom - clientRect.top, // source bitmap height
			SRCCOPY); // raster operation
		
		DeleteDC(hdcMem);

		return S_SUCCESS;
	});

	registerEventLambda<ImageBtnComponent>(DispatchWindowComponent::translateMessage(hwnd, WM_NCDESTROY), [hwnd](const IEventArgs& evtArgs)->Status {

		RemoveProp(hwnd, ImageBtnComponent::PROP_UP_BMP.c_str());
		RemoveProp(hwnd, ImageBtnComponent::PROP_DOWN_BMP.c_str());
		RemoveProp(hwnd, ImageBtnComponent::PROP_HOVER_BMP.c_str());

		return S_SUCCESS;
	});

	return S_SUCCESS;
}
