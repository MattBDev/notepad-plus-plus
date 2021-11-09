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
#include <windows.h>

class Window
{
public:
	//! \name Constructors & Destructor
	//@{
	Window() = default;
	Window(const Window&) = delete;
	virtual ~Window() = default;
	//@}


	virtual void init(HINSTANCE hInst, HWND parent)
	{
		_hInst = hInst;
		_hParent = parent;
	}

	virtual void destroy() = 0;

	/**
	 * @brief Sets the window's show state.
	 *
	 * @param toShow if true it will activate the window and displays it in its current size and position.
	 */
	virtual void display(bool toShow = true) const
	{
		::ShowWindow(_hSelf, toShow ? SW_SHOW : SW_HIDE);
	}


	/**
	 * @brief Changes the position and dimensions of the specified window. For a top-level window, the position and dimensions are relative to the upper-left corner of the screen. For a child window, they are relative to the upper-left corner of the parent window's client area.
	 *
	 * @param rc
	 */
	virtual void reSizeTo(RECT& rc) // should NEVER be const !!!
	{
		::MoveWindow(_hSelf, rc.left, rc.top, rc.right, rc.bottom, TRUE);
		redraw();
	}


	virtual void reSizeToWH(RECT& rc) // should NEVER be const !!!
	{
		::MoveWindow(_hSelf, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, TRUE);
		redraw();
	}


	virtual void redraw(bool forceUpdate = false) const
	{
		::InvalidateRect(_hSelf, nullptr, TRUE);
		if (forceUpdate)
			::UpdateWindow(_hSelf);
	}

	/**
	 * @brief Retrieves the coordinates of a window's client area. The client coordinates
	 *  specify the upper-left and lower-right corners of the client area. Because client
	 *  coordinates are relative to the upper-left corner of a window's client area, the
	 *  coordinates of the upper-left corner are (0,0).
	 *
	 * @param rc A pointer to a RECT structure that receives the client coordinates.
	 *  The left and top members are zero. The right and bottom members contain the
	 *  width and height of the window.
	 */
	virtual void getClientRect(RECT& rc) const
	{
		::GetClientRect(_hSelf, &rc);
	}

	/**
	 * @brief Retrieves the dimensions of the bounding rectangle of the window.
	 *  The dimensions are given in screen coordinates that are relative to the upper-left
	 *  corner of the screen.
	 *
	 * @param rc A pointer to a RECT structure that receives the screen coordinates of the
	 *  upper-left and lower-right corners of the window.
	 */
	virtual void getWindowRect(RECT& rc) const
	{
		::GetWindowRect(_hSelf, &rc);
	}

	virtual int getWidth() const
	{
		RECT rc;
		::GetClientRect(_hSelf, &rc);
		return (rc.right - rc.left);
	}

	virtual int getHeight() const
	{
		RECT rc;
		::GetClientRect(_hSelf, &rc);
		if (::IsWindowVisible(_hSelf) == TRUE)
			return (rc.bottom - rc.top);
		return 0;
	}

	/**
	 * @brief Determines the visibility state of the window.
	 *
	 * @return If the window, its parent window, its parent's parent window,
	 *  and so forth, have the WS_VISIBLE style, the return value is true. Otherwise, the return
	 *  value is false. Because the return value specifies whether the window has the WS_VISIBLE style,
	 *  it may be true even if the window is totally obscured by other windows.
	 */
	virtual bool isVisible() const
	{
		return ::IsWindowVisible(_hSelf) != 0;
	}

	HWND getHSelf() const
	{
		return _hSelf;
	}

	/**
	* @brief Sets the keyboard focus to the window.
	*
	* @return If the function succeeds, the return value is the handle to the window that previously had the keyboard focus.
	*/
	HWND getHParent() const {
		return _hParent;
	}

	/**
	 * @brief Get the keyboard focus to the window.
	 *
	 */
	void getFocus() const {
		::SetFocus(_hSelf);
	}

	HINSTANCE getHinst() const
	{
		//assert(_hInst != 0);
		return _hInst;
	}


	Window& operator = (const Window&) = delete;


protected:
	HINSTANCE _hInst = nullptr;
	HWND _hParent = nullptr;
	HWND _hSelf = nullptr;
};