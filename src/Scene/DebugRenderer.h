
#pragma once
#include <Lumino/Graphics/Color.h>
//#include <Lumino/Graphics/DrawingContext.h>
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Physics/IDebugRenderer.h>
#include <Lumino/Scene/Common.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

class DebugRenderer
	: public Physics::IDebugRenderer
{
public:
	DebugRenderer(GraphicsContext* context)
		: m_context(context)
	{
	}

public:

	virtual void DrawLine(const Vector3& from, const Vector3& to, const Vector3& fromColor, const Vector3& toColor)
	{
		m_context->DrawLinePrimitive(from, Color(fromColor, 1.0f), to, Color(toColor, 1.0f));
	}

private:
	GraphicsContext*		m_context;
};

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END
