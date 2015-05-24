
#pragma once
#include <Lumino/Graphics/GraphicsManager.h>
#include "Common.h"

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
	Workbench* CreateWorkbench();

private:
	RefPtr<Graphics::GraphicsManager>	m_graphicsManager;
};

} // namespace GUI
} // namespace Lumino
