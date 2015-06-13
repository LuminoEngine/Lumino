

#pragma once
#include <map>
#include <Lumino/Graphics/GraphicsManager.h>
#include "Common.h"
#include "EventArgs.h"

namespace Lumino
{
namespace GUI
{
	
/**
	@brief		GUI 機能の管理クラスです。
*/
class GUIManager
	: public RefObject
{
public:
	struct ConfigData
	{
		Graphics::GraphicsManager*	GraphicsManager;

		ConfigData()
			: GraphicsManager(NULL)
		{}
	};

public:
	GUIManager();
	virtual ~GUIManager();

public:
	void Initialize(const ConfigData& configData);
	void Finalize();
	Graphics::GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	RootPane* GetDefaultRootPane() const { return m_defaultRootPane; }

	void RegisterFactory(const String& typeFullName, ObjectFactory factory);	// とりあえず内部用。言語バインダのことは後で。
	CoreObject* CreateObject(const String& typeFullName);

	bool InjectMouseMove(float clientX, float clientY);
	bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY);
	bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY);
	bool InjectMouseWheel(int delta, float clientX, float clientY);
	bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl);
	bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl);
	//bool InjectChar(int ch);
	bool InjectElapsedTime(float elapsedTime);

public:	// internal
	ResourceDictionary* GetDefaultTheme() { return m_defaultTheme; }
	CombinedLocalResource* GetRootCombinedResource() { return m_rootCombinedResource; }

private:
	void UpdateMouseHover(const PointF& mousePos);
	void BuildDefaultTheme();

private:
	typedef std::map<String, ObjectFactory>	ObjectFactoryMap;
	typedef std::pair<String, ObjectFactory>	ObjectFactoryPair;

	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	EventArgsPool						m_eventArgsPool;
	RootPane*							m_defaultRootPane;
	ResourceDictionary*					m_defaultTheme;
	CombinedLocalResource*				m_rootCombinedResource;
	UIElement*							m_mouseHoverElement;		///< 現在マウス位置にある UIElement

	ObjectFactoryMap					m_objectFactoryMap;
};

} // namespace GUI
} // namespace Lumino
