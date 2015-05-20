
#include "../Internal.h"
#include "MME/MMEShader.h"
#include "SceneGraphManager.h"
#include "ViewPane.h"

namespace Lumino
{
namespace Scene
{

//=============================================================================
// LayerList
//=============================================================================
LN_REF_OBJECT_LIST_IMPL(LayerList, Layer);

//=============================================================================
// ViewPane
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ViewPane::ViewPane(SceneGraphManager* manager)
	: m_manager(NULL)
{
	LN_REFOBJ_SET(m_manager, manager);
	memset(&m_leftMouseState, 0, sizeof(m_leftMouseState));
	memset(&m_rightMouseState, 0, sizeof(m_rightMouseState));
	memset(&m_middleMouseState, 0, sizeof(m_middleMouseState));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ViewPane::~ViewPane()
{
	LN_SAFE_RELEASE(m_manager);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ViewPane::Render(Graphics::Texture* renderTarget)
{
	SizeF viewSize((float)renderTarget->GetSize().Width, (float)renderTarget->GetSize().Height);

	// 全てのシェーダの Scene 単位データの更新
	MMESceneParams sceneParams;
	sceneParams.Time = (float)m_manager->GetTime();
	sceneParams.ElapsedTime = m_manager->GetElapsedTime();
	sceneParams.MousePosition.Set((2.0f * ((float)m_mousePosition.X) / viewSize.Width) - 1.0f, (2.0f * ((float)m_mousePosition.Y) / viewSize.Height) - 1.0f, 0, 0);
	m_leftMouseState.ToVector4(viewSize, &sceneParams.LeftMouseDown);
	m_rightMouseState.ToVector4(viewSize, &sceneParams.MiddleMouseDown);
	m_middleMouseState.ToVector4(viewSize, &sceneParams.RightMouseDown);
	LN_FOREACH(MMEShader* shader, *m_manager->GetShaderList()) {
		shader->UpdateSceneParams(sceneParams, m_manager);
	}
	
	// 各レイヤーのレンダリングを実行する
	int count = m_layerList.GetCount();
	for (int i = 0; i < count; i++) {
		m_layerList.GetAt(i)->PreRender(viewSize);
	}
	for (int i = 0; i < count; i++) {
		m_layerList.GetAt(i)->Render();
	}
	for (int i = count - 1; i >= 0; i--) {	// post は逆順
		m_layerList.GetAt(i)->PostRender();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ViewPane::InjectMouseMove(int x, int y)
{
	m_mousePosition.Set(x, y);
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ViewPane::InjectMouseButtonDown(Platform::MouseButton button)
{
	switch (button)
	{
	case Lumino::Platform::MouseButton_Left:
		m_leftMouseState.Position = m_mousePosition;
		m_leftMouseState.Time = static_cast<float>(m_manager->GetTime());
		m_leftMouseState.IsDown = true;
		return true;
	case Lumino::Platform::MouseButton_Right:
		m_rightMouseState.Position = m_mousePosition;
		m_rightMouseState.Time = static_cast<float>(m_manager->GetTime());
		m_rightMouseState.IsDown = true;
		return true;
	case Lumino::Platform::MouseButton_Middle:
		m_middleMouseState.Position = m_mousePosition;
		m_middleMouseState.Time = static_cast<float>(m_manager->GetTime());
		m_middleMouseState.IsDown = true;
		return true;
	default:
		break;
	}
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool ViewPane::InjectMouseButtonUp(Platform::MouseButton button)
{
	switch (button)
	{
	case Lumino::Platform::MouseButton_Left:
		m_leftMouseState.IsDown = false;
		return true;
	case Lumino::Platform::MouseButton_Right:
		m_leftMouseState.IsDown = false;
		return true;
	case Lumino::Platform::MouseButton_Middle:
		m_leftMouseState.IsDown = false;
		return true;
	default:
		break;
	}
	return false;
}

} // namespace Scene
} // namespace Lumino
