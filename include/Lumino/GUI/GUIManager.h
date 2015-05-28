
#pragma once
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
	Graphics::GraphicsManager* GetGraphicsManager() const { return m_graphicsManager; }
	RootPane* CreateRootPane();

	bool InjectMouseMove(float clientX, float clientY);
	bool InjectMouseButtonDown(MouseButton button, float clientX, float clientY);
	bool InjectMouseButtonUp(MouseButton button, float clientX, float clientY);
	bool InjectMouseWheel(int delta, float clientX, float clientY);
	bool InjectKeyDown(Key keyCode, bool isAlt, bool isShift, bool isControl);
	bool InjectKeyUp(Key keyCode, bool isAlt, bool isShift, bool isControl);
	//bool InjectChar(int ch);
	bool InjectElapsedTime(float elapsedTime);

private:
	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
	EventArgsPool						m_eventArgsPool;
	RootPane*							m_rootPane;
};

} // namespace GUI
} // namespace Lumino
