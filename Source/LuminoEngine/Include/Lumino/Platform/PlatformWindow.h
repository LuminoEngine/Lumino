
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/SortedArray.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include "../Base/GeometryStructs.h"
#include "Common.h"
#include "EventListener.h"

LN_NAMESPACE_BEGIN

class DataObject
	: public Object
{
public:
	bool hasDropFiles() const { return !m_dropFiles.isEmpty(); }

	const List<String>& dropFiles() const { return m_dropFiles; }

	void setDropFiles(const List<String>& files) { m_dropFiles = files; }

LN_CONSTRUCT_ACCESS:
	DataObject();
	virtual ~DataObject();

private:
	List<String>	m_dropFiles;
};

/**
	@brief	プラットフォーム固有のウィンドウシステムのウィンドウを抽象化します。
*/
class PlatformWindow
	: public RefObject
{
public:

	/**
		@brief		ウィンドウを作成します。
		@param[in]	title		: ウィンドウタイトル
		@param[in]	size		: クライアント領域のピクセルサイズ
		@param[in]	resizable	: 可変ウィンドウとして作成するかどうか
	*/
	//static PlatformWindow* create(const String& title, const Size& clientSize, bool resizable, PlatformManager* manager);

public:

	/**
		@brief		クライアント領域のサイズを取得します。
	*/
	SizeI getSize() const { return m_clientSize; }

	virtual void setSize(const SizeI& size) = 0;

	virtual void setVisible(bool visible) = 0;

	virtual void setTitleText(const StringRef& title) = 0;
	virtual const String& getTitleText() const = 0;
	
	/**
		@brief		フルスクリーンの有効フラグを設定します。
		@details	enabled に true を設定するとフルスクリーン、false を設定するとウィンドウモードになります。
					この関数はウィンドウスタイルを変更して最大化するだけです。
					ユーザー定義ウィンドウの場合はなにもしません。
	*/
	virtual void setFullScreenEnabled(bool enabled) = 0;

	/**
		@brief		フルスクリーン状態かを判定します。
	*/
	virtual bool isFullScreenEnabled() const = 0;

	/**
		@brief		ウィンドウがアクティブかどうかを示す値を取得します。
	*/
	bool isActive() const { return m_isActive; }

	/**
		@brief		マウスカーソルがクライアント領域上にある場合に表示するかを設定します。
	*/
	virtual void setCursorVisible(bool visible);

	/**
		@brief		このウィンドウにイベントリスナーをアタッチします。(priority が大きいものが先に処理される。必ずデタッチすること)
	*/
	void attachEventListener(IEventListener* listener, int priority);

	/**
		@brief		このウィンドウからイベントリスナーをデタッチします。
	*/
	void detachEventListener(IEventListener* listener);

	bool sendPlatformEvent(const PlatformEventArgs& e);

	virtual void setAllowDragDrop(bool value);

protected:
	PlatformWindow(WindowManagerBase* windowManager);
	virtual ~PlatformWindow();
	void initialize(const SizeI& clientSize);
    
LN_INTERNAL_ACCESS:
	virtual void onPlatformEvent(const PlatformEventArgs& e);

	typedef List<std::pair<int, IEventListener*>>	EventListenerList;

	WindowManagerBase*		m_windowManager;
	EventListenerList		m_listenerEntryArray;

LN_INTERNAL_ACCESS:
	bool SendEventToAllListener(const PlatformEventArgs& e);

	detail::MouseCursorVisibility* m_mouseCursorVisibility;

private:
	SizeI	m_clientSize;
	bool	m_isActive;
};

LN_NAMESPACE_END



