// This file is part of Notepad++ project
// Copyright (C)2021 Don HO <don.h@free.fr>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// at your option any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#pragma once
#include "ColourPopupResource.h"
#include "resource.h"
#include "Window.h"
#include <windows.h>
#include <cstdlib>
#include <cstring>

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.system.h>
#include <winrt/windows.ui.xaml.hosting.h>
#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>
#include "winrt/Windows.UI.Xaml.Controls.h"
#include "winrt/Windows.UI.Xaml.Media.h"
#undef GetCurrentTime

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::Foundation::Numerics;

#define WM_PICKUP_COLOR  (COLOURPOPUP_USER + 1)
#define WM_PICKUP_CANCEL (COLOURPOPUP_USER + 2)


class ColourPopup : public Window
{
public:
	ColourPopup()
	{
		// The call to winrt::init_apartment initializes COM; by default, in a multithreaded apartment.
		winrt::init_apartment(apartment_type::single_threaded);

		// Initialize the XAML framework's core window for the current thread.
		WindowsXamlManager winxamlmanager = WindowsXamlManager::InitializeForCurrentThread();

		// This DesktopWindowXamlSource is the object that enables a non-UWP desktop application 
		// to host WinRT XAML controls in any UI element that is associated with a window handle (HWND).
		_desktopWindowXamlSource = winrt::Windows::UI::Xaml::Hosting::DesktopWindowXamlSource{};

	};
	explicit ColourPopup(COLORREF defaultColor) : _colour(defaultColor) {}
	virtual ~ColourPopup() {}

	bool isCreated() const
	{
		return (_hSelf != nullptr);
	}

	void create(int dialogID);

	void doDialog(POINT p)
	{
		if (!isCreated())
			create(IDD_COLOUR_POPUP);
		SetWindowPos(_hSelf, HWND_TOP, p.x, p.y, _rc.right - _rc.left, _rc.bottom - _rc.top, SWP_SHOWWINDOW);
	}

	virtual void destroy()
	{
		DestroyWindow(_hSelf);
	}

	void setColour(COLORREF c)
	{
		_colour = c;
	}

	COLORREF getSelColour() { return _colour; };

private :
	RECT _rc = {0};
	COLORREF _colour = RGB(0xFF, 0xFF, 0xFF);
	DesktopWindowXamlSource _desktopWindowXamlSource{ nullptr };

	static INT_PTR CALLBACK dlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK run_dlgProc(UINT message, WPARAM wParam, LPARAM lParam);
};
