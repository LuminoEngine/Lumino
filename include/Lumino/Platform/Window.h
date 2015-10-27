
#pragma once

#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Size.h>
#include "EventListener.h"

LN_NAMESPACE_BEGIN
namespace Platform
{
class PlatformManager;

/**
	@brief	プラットフォーム固有のウィンドウシステムのウィンドウを抽象化します。
*/
class Window
	: public RefObject
{
public:

	/**
		@brief		ウィンドウを作成します。
		@param[in]	title		: ウィンドウタイトル
		@param[in]	size		: クライアント領域のピクセルサイズ
		@param[in]	resizable	: 可変ウィンドウとして作成するかどうか
	*/
	static Window* Create(const String& title, const Size& clientSize, bool resizable, PlatformManager* manager);

public:

	/**
		@brief		クライアント領域のサイズを取得します。
	*/
	virtual const Size& GetSize() const = 0;
	
	/**
		@brief		フルスクリーンの有効フラグを設定します。
		@details	enabled に true を設定するとフルスクリーン、false を設定するとウィンドウモードになります。
					この関数はウィンドウスタイルを変更して最大化するだけです。
					ユーザー定義ウィンドウの場合はなにもしません。
	*/
	virtual void SetFullScreenEnabled(bool enabled) = 0;

	/**
		@brief		フルスクリーン状態かを判定します。
	*/
	virtual bool IsFullScreenEnabled() const = 0;

	/**
		@brief		ウィンドウがアクティブかどうかを示す値を取得します。
	*/
	virtual bool IsActive() const = 0;

	/**
		@brief		マウスカーソルがクライアント領域上にある場合に表示するかを設定します。
	*/
	virtual void SetCursorVisible(bool visible) = 0;

	/**
		@brief		このウィンドウにマウスキャプチャを設定します。
	*/
	virtual void CaptureMouse() = 0;

	/**
		@brief		このウィンドウからマウスキャプチャを削除します。
	*/
	virtual void ReleaseMouseCapture() = 0;

	/**
		@brief		このウィンドウにイベントリスナーをアタッチします。(priority が大きいものが先に処理される。必ずデタッチすること)
	*/
	virtual void AttachEventListener(IEventListener* listener, int priority) = 0;

	/**
		@brief		このウィンドウからイベントリスナーをデタッチします。
	*/
	virtual void DetachEventListener(IEventListener* listener) = 0;

protected:
	Window();
	virtual ~Window();
};

#if 0
/**
	@brief	ウィンドウシステムのウィンドウを表すクラスです。
*/
class Window
	: public RefObject
{
public:
	/**
		@brief		ウィンドウを作成します。
		@param[in]	title		: ウィンドウタイトル
		@param[in]	size		: クライアント領域のピクセルサイズ
		@param[in]	resizable	: 可変ウィンドウとして作成するかどうか
	*/
	Window(const String& title, const Size& clientSize, bool resizable);
	virtual ~Window();

public:

	/**
		@brief		クライアント領域のサイズを取得します。
	*/
	const Size& GetSize() const;
	
	/**
		@brief		フルスクリーンの有効フラグを設定します。
		@details	enabled に true を設定するとフルスクリーン、false を設定するとウィンドウモードになります。
					この関数はウィンドウスタイルを変更して最大化するだけです。
					ユーザー定義ウィンドウの場合はなにもしません。
	*/
	void SetFullScreenEnabled(bool enabled);

	/**
		@brief		フルスクリーン状態かを判定します。
	*/
	bool IsFullScreenEnabled() const;

	/**
		@brief		ウィンドウがアクティブかどうかを示す値を取得します。
	*/
	bool IsActive() const;

	/**
		@brief		このウィンドウにマウスキャプチャを設定します。
	*/
	void CaptureMouse();

	/**
		@brief		このウィンドウからマウスキャプチャを削除します。
	*/
	void ReleaseMouseCapture();

	/**
		@brief		このウィンドウにイベントリスナーをアタッチします。(priority が大きいものが先に処理される。必ずデタッチすること)
	*/
	void AttachEventListener(IEventListener* listener, int priority);

	/**
		@brief		このウィンドウからイベントリスナーをデタッチします。
	*/
	void DetachEventListener(IEventListener* listener);

private:
	friend class Application;
	Window(NativeWindow* nativeWindow);
	NativeWindow*	m_nativeWindow;
};
#endif

} // namespace Platform
LN_NAMESPACE_END



