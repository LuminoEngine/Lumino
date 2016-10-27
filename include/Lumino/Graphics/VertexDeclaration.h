
#pragma once
#include "Common.h"
#include "GraphicsResourceObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class VertexDeclaration;
using VertexDeclarationPtr = RefPtr<VertexDeclaration>;

/**
	@brief		
*/
class VertexDeclaration
	: public GraphicsResourceObject
{
public:

	/**
		@brief		
	*/
	static VertexDeclarationPtr Create();
	
public:

	void AddVertexElement(int streamIndex, VertexElementType type, VertexElementUsage usage, int usageIndex);


LN_INTERNAL_ACCESS:
	VertexDeclaration();
	virtual ~VertexDeclaration();
	void Initialize(detail::GraphicsManager* manager);
	void Initialize(detail::GraphicsManager* manager, const VertexElement* elements, int count);
	void TryUpdateResource();
	Driver::IVertexDeclaration* GetDeviceObject();

	// GraphicsResourceObject interface
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

private:
	RefPtr<Driver::IVertexDeclaration>	m_deviceObj;		// TODO: これは RefPtr にしたくない。Driver 系の include が必要になってしまう。
	List<VertexElement>					m_vertexElements;
	bool								m_modified;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
