
#pragma once
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/GeometryRenderer.h>
#include <Lumino/Physics/IDebugRenderer.h>
#include <Lumino/Scene/Common.h>

namespace Lumino
{
LN_NAMESPACE_SCENE_BEGIN

class DebugRenderer
	: public Physics::IDebugRenderer
{
public:
	DebugRenderer(GeometryRenderer* renderer)
		: m_renderer(renderer)
	{
	}

public:

	virtual void DrawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor)
	{
		m_renderer->DrawLine(from, to, ColorF(fromColor, 1.0f), ColorF(toColor, 1.0f));
	}

private:
	GeometryRenderer*	m_renderer;
};

LN_NAMESPACE_SCENE_END
} // namespace Lumino
