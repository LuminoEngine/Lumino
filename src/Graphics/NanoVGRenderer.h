
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Base/GeometryStructs.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Math/Matrix.h>
#include <Lumino/Graphics/Brush.h>

LN_NAMESPACE_BEGIN
namespace detail
{

class NanoVGCommandHelper
{
public:
};

class NanoVGRenderer
	: public RefObject
	, public detail::IRendererPloxy
{
public:
	NanoVGRenderer();
	virtual ~NanoVGRenderer();
	void Initialize(GraphicsManager* manager);

	virtual bool IsStandaloneShader() const { return true; }
	virtual void Flush() override {}
	virtual void OnActivated() override {}
	virtual void OnDeactivated() override { Flush(); }

	GraphicsManager*		m_manager;
};

} // namespace detail
LN_NAMESPACE_END
