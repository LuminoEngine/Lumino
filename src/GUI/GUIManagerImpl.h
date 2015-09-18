

#pragma once
#include <map>
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Documents/DocumentsManager.h>
#include <Lumino/GUI/CursorImage.h>
#include <Lumino/GUI/Common.h>
#include <Lumino/GUI/RoutedEventArgs.h>

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
namespace Internal { class GUIPainter; }
	

/**
	@brief	
	@note	このクラスはイベント引数の頻繁な new を避けるために使用する。
			例えば MouseMove イベントは最悪毎フレームのように発生する可能性があり、new は大きなオーバーヘッドになる。
			それなら union を利用したイベント引数構造体を使うのもひとつの手かもしれない。
			
			しかし、本ライブラリは C# や Ruby 等、他の言語へのバインディングを想定している。
			当然 GUI モジュールも公開され、拡張されることが前提。
			C# 側で作成したユーザーコントロールが MouseEventArgs を使用することは想定しなければならない。

			union や struct にした場合、これはこれで言語別に余計なオーバーヘッドや合わせこみの実装が増えることになる。
			例えば C# でBは値渡しのままでよいのか、ref をつけるのか。Ruby で struct は使えないので結局 new する羽目になるがいいのか。

			Pool しておけば、若干直感的ではなくなるが、バインダ側の new も抑えることができる。

			後々イベントの種類が増えてきたときは拡張性のため、イベント名をキーにして Create するような仕組みが必要になるかもしれない。
*/
class EventArgsPool
{
public:
	EventArgsPool();
	~EventArgsPool();

	MouseEventArgs* CreateMouseEventArgs(MouseButton button, int wheel, float x, float y, int clickCount);	// TODO: やめる
	KeyEventArgs* CreateKeyEventArgs(Key keyCode, bool isAlt, bool isShift, bool isControl);	// TODO: やめる

	template<class TEventArgs, typename ...TArgs>
	TEventArgs* Create(TArgs... args)
	{
		TEventArgs* e = static_cast<TEventArgs* >(Find(TEventArgs::GetClassTypeInfo()));
		if (e == NULL) {
			e = LN_NEW TEventArgs(args...);
			Register(e);
		}
		else {
			e->~TEventArgs();
			new (e)TEventArgs(args...);
		}
		e->Handled = false;
		e->AddRef();
		return e;
	}

private:
	typedef Array<RoutedEventArgs*>					EventArgsList;
	typedef SortedArray<TypeInfo*, EventArgsList*>	EventArgsMap;
	EventArgsMap	m_pool;

	RoutedEventArgs* Find(TypeInfo* typeId)
	{
		EventArgsList* list;
		if (m_pool.TryGetValue(typeId, &list))
		{
			for (auto e : (*list))
			{
				if (e->GetRefCount() == 1) {	// このリストからしか参照されていなければ返す
					return e;
				}
			}
		}
		return NULL;
	}

	void Register(RoutedEventArgs* e)
	{
		EventArgsList* list;
		if (!m_pool.TryGetValue(GetTypeInfo(e), &list))
		{
			list = LN_NEW EventArgsList();
			m_pool.Add(GetTypeInfo(e), list);
		}
		list->Add(e);
	}


	//Array<MouseEventArgs*>	m_mouseEventArgsPool;





	template<class T>
	T FindFreeObject(const Array<T>& pool)
	{
		LN_FOREACH(T a, pool)
		{
			if (a->GetRefCount() == 1) {
				return a;
			}
		}
		return NULL;
	}

	Array<MouseEventArgs*>	m_mouseEventArgsPool;
	Array<KeyEventArgs*>	m_keyEventArgsPool;
};



/**
	@brief		GUI 機能の管理クラスです。
*/
class GUIManagerImpl
	: public RefObject
{
public:
	static const float DefaultouseButtonClickTimeout;

	static GUIManagerImpl* Instance;

public:
	struct ConfigData
	{
		GraphicsManager*		GraphicsManager;
		Platform::Window*				MainWindow;
		Documents::DocumentsManager*	DocumentsManager;

		ConfigData()
			: GraphicsManager(NULL)
			, MainWindow(NULL)
			, DocumentsManager(NULL)
		{}
	};

public:
	GUIManagerImpl();
	virtual ~GUIManagerImpl();

public:
	void Initialize(const ConfigData& configData);
	void Finalize();
	GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	Documents::DocumentsManager* GetDocumentsManager() const { return m_documentsManager; }

	void AddContextOnMainWindow(GUIContext* context) { m_mainWindowContextList.Add(context); }
	void RemoveContextOnMainWindow(GUIContext* context) { m_mainWindowContextList.Remove(context); }
	void UpdateLayoutOnMainWindow(const Size& viewPixelSize);
	void RenderOnMainWindow();

private:
	Array<GUIContext*>	m_mainWindowContextList;












public:

	RootFrame* GetDefaultRootFrame() const { return m_defaultRootFrame; }

	// TODO: context
	Platform::Window* GetNativeWindow() const { return m_mainWindow; }

	void RegisterFactory(const String& typeFullName, ObjectFactory factory);	// とりあえず内部用。言語バインダのことは後で。
	CoreObject* CreateObject(const String& typeFullName);

	bool InjectMouseMove(float clientX, float clientY);
	bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY);
	bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY);
	bool InjectMouseWheel(int delta, float clientX, float clientY);
	bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl);
	bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl);
	bool InjectChar(TCHAR ch);
	//bool InjectChar(int ch);
	void InjectElapsedTime(float elapsedTime);

	double GetTime() const { return m_time; }

	//void SetViewPixelSize(const Size& viewPixelSize) { m_viewPixelSize = viewPixelSize; }	// TODO: GUIContext
	void Render();	// TODO: GUIContext

public:	// internal
	ResourceDictionary* GetDefaultTheme() { return m_defaultTheme; }
	CombinedLocalResource* GetRootCombinedResource() { return m_rootCombinedResource; }
	EventArgsPool* GetEventArgsPool() { return &m_eventArgsPool; }

	void SetFocusElement(UIElement* element);	// TODO: GUIContext

	void CaptureMouse(UIElement* element);
	void ReleaseMouseCapture(UIElement* element);

private:
	bool UpdateMouseHover(const PointF& mousePos);	// ハンドリングしたら true を返す
	void BuildDefaultTheme();


private:	// internal
	void AddAnimationClock(AnimationClock* clock);
	void RemoveAnimationClock(AnimationClock* clock);

	//static char KeyToChar(Key keyCode);

private:
	typedef std::map<String, ObjectFactory>	ObjectFactoryMap;
	typedef std::pair<String, ObjectFactory>	ObjectFactoryPair;

	RefPtr<GraphicsManager>	m_graphicsManager;
	RefPtr<Documents::DocumentsManager>	m_documentsManager;
	Internal::GUIPainter*				m_painter;

	EventArgsPool						m_eventArgsPool;
	RootFrame*							m_defaultRootFrame;
	ResourceDictionary*					m_defaultTheme;
	CombinedLocalResource*				m_rootCombinedResource;
	UIElement*							m_focusElement;
	UIElement*							m_mouseHoverElement;		///< 現在マウス位置にある UIElement
	UIElement*							m_capturedElement;			///< 現在キャプチャ中の UIElement
	RefPtr<Platform::Window>			m_mainWindow;		// TODO: Context
	Array<AnimationClock*>				m_activeAnimationClockList;	///< TODO: インデックススタックを使ったリストにしたい

	ObjectFactoryMap					m_objectFactoryMap;

	double								m_time;	// TODO: context

	// GUIContext
	struct MouseClickTracker
	{
		double		LastTime;
		int			ClickCount;
		UIElement*	HoverElement;
	};

	MouseClickTracker m_mouseClickTrackers[MouseButton::TERMINATOR];

	RefPtr<Texture>	m_defaultSkinTexture;
	RefPtr<CursorImage>			m_cursorImageTable[1];
	RefPtr<CursorImage>			m_currentCursorImage;
	PointF						m_mousePosition;
	float						m_mouseButtonClickTimeout;
	float						m_cursorAnimationTime;
	Size						m_viewPixelSize;


	friend class GUIHelper;
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
