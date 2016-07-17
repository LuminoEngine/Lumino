
#pragma once
#include <Lumino/Graphics/Common.h>

LN_NAMESPACE_BEGIN

/**
	@brief
*/
class StaticMeshModel
	: public Object
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

private:
	StaticMeshModel();
	virtual ~StaticMeshModel();
	void Initialize(GraphicsManager* manager);

LN_INTERNAL_ACCESS:
	void Draw(GraphicsContext* g);

	RefPtr<VertexBuffer>	m_vertexBuffer;
	RefPtr<IndexBuffer>		m_indexBuffer;
};

LN_NAMESPACE_END
