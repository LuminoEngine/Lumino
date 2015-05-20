
#pragma once

#include <Lumino/Graphics/GeometryRenderer.h>
#include "../Physics/IDebugRenderer.h"

namespace Lumino
{
namespace Scene
{

class DebugRenderer
	: public Physics::IDebugRenderer
{
public:
	DebugRenderer(Graphics::GeometryRenderer* renderer)
		: m_renderer(renderer)
	{
	}

public:

	virtual void DrawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor)
	{
		m_renderer->DrawLine(from, to, Graphics::ColorF(fromColor, 1.0f), Graphics::ColorF(toColor, 1.0f));
	}

private:
	Graphics::GeometryRenderer*	m_renderer;
};

} // namespace Scene
} // namespace Lumino
