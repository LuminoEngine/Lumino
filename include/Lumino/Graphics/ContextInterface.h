
#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

namespace detail
{
class RenderTargetTextureCache;

class IRendererPloxy// : public RefObject
{
public:
	virtual void Flush() = 0;
	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;
};

class HiLevelRendererCore : public RefObject
{
public:

	void ActivateFront(IRendererPloxy* renderer)
	{
		if (renderer != m_rendererFront)
		{
			if (m_rendererFront != nullptr)
			{
				m_rendererFront->Flush();
			}
			m_rendererFront = renderer;
		}
	}

private:
	IRendererPloxy*	m_rendererFront = nullptr;
};




class IContext
{
protected:
	IContext();
	virtual ~IContext();
	void Initialize(GraphicsManager* manager);

	virtual void OnStateFlushRequested() = 0;
	virtual void OnPrimitiveFlushRequested();

	friend class ShaderVariable;
	void OnStateChanging();
	void OnDrawing(detail::IRendererPloxy* rendererPloxy);

	friend class GraphicsManager;
	virtual void OnActivated() = 0;
	virtual void OnDeactivated();

LN_INTERNAL_ACCESS:
	virtual ShaderPass* GetShaderPass() const = 0;

private:
	void SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy);

	GraphicsManager*		m_manager;
	detail::IRendererPloxy*	m_activeRendererPloxy;
};

} // namespace detail


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

