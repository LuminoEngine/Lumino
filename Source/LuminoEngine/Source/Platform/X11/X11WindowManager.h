
#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlocale.h>
#include <X11/keysym.h>
#include "../WindowManagerBase.h"
#include "X11Window.h"

namespace Lumino
{
namespace Platform
{

class X11WindowManager
	: public WindowManagerBase
{
public:
	X11WindowManager();
	virtual ~X11WindowManager();

public:
	// override Application
	virtual void CreateMainWindow(const WindowCreationSettings& settings);
	virtual Window* getMainWindow() { return m_mainWindow; }
	virtual Window* CreateSubWindow(const WindowCreationSettings& settings);
	virtual void DoEvents();
	virtual void Finalize();
	
public:
	Display* GetX11Display() { return m_x11Display; }
	int GetX11DefaultScreen() { return m_x11DefaultScreen; }
	::Window GetX11RootWindow() { return m_x11RootWindow; }
	XContext GetX11Context() { return m_x11Context; }

private:
	X11Window* FindX11Window(::Window x11Window);

private:
	RefPtr<X11Window>	m_mainWindow;
	Display*			m_x11Display;
	int					m_x11DefaultScreen;
	::Window			m_x11RootWindow;
	XContext			m_x11Context;
	
public:
	Atom				m_atom_WM_DELETE_WINDOW;
};

} // namespace Platform
} // namespace Lumino
