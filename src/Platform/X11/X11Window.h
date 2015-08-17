
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
	GLXFBConfig* GetGLXFBConfig() { return &m_fbConfig; }
	XVisualInfo* GetX11VisualInfo() { return m_visualInfo; }
	::Window GetX11WindowID() { return m_x11Window; }
	X11WindowManager* GetWindowManager() { return (X11WindowManager*)(m_windowManager); }

public:
	// override Window
	virtual bool IsActive() const { return true; }
	virtual const Size& GetSize() const { return m_clientSize; }
	virtual void SetFullScreenEnabled(bool enabled);
	virtual bool IsFullScreenEnabled() const { return false; }
	virtual void CaptureMouse();
	virtual void ReleaseMouseCapture();
	
private:
	void SetVisible(bool show);

private:
	String			m_titleText;		///< ウィンドウタイトルの文字列
	Size			m_clientSize;		///< クライアント領域の大きさ
	
	GLXFBConfig		m_fbConfig;
	XVisualInfo*	m_visualInfo;
	::Window		m_x11Window;
};

} // namespace Platform
} // namespace Lumino
