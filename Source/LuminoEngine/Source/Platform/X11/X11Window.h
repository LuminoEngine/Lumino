
#pragma once

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xlocale.h>
#include <X11/keysym.h>
#include <GL/glx.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/Size.h>
#include "../WindowBase.h"

namespace Lumino
{
namespace Platform
{
class X11WindowManager;

class X11Window
	: public WindowBase
{
public:
	/// 初期化データ
	struct SettingData
	{
		const TCHAR*	TitleText;		///< ウィンドウタイトルの文字列
		Size			ClientSize;		///< クライアント領域のサイズ
		bool			Fullscreen;		///< フルスクリーンモードで初期化する場合 true
		bool			Resizable;
	};

public:
	X11Window(X11WindowManager* windowManager, const SettingData& settingData);
	virtual ~X11Window();
	GLXFBConfig* getGLXFBConfig() { return &m_fbConfig; }
	XVisualInfo* getX11VisualInfo() { return m_visualInfo; }
	::Window getX11WindowID() { return m_x11Window; }
	X11WindowManager* getWindowManager() { return (X11WindowManager*)(m_windowManager); }

public:
	// override Window
	virtual bool isActive() const { return true; }
	virtual const Size& getSize() const { return m_clientSize; }
	virtual void setFullScreenEnabled(bool enabled);
	virtual bool isFullScreenEnabled() const { return false; }
	virtual void captureMouse();
	virtual void releaseMouseCapture();
	
private:
	void setVisible(bool show);

private:
	String			m_titleText;		///< ウィンドウタイトルの文字列
	Size			m_clientSize;		///< クライアント領域の大きさ
	
	GLXFBConfig		m_fbConfig;
	XVisualInfo*	m_visualInfo;
	::Window		m_x11Window;
};

} // namespace Platform
} // namespace Lumino
