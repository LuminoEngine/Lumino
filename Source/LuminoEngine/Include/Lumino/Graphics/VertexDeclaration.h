
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
	static VertexDeclarationPtr create();
	
public:

	void addVertexElement(int streamIndex, VertexElementType type, VertexElementUsage usage, int usageIndex);

protected:
	virtual void Dispose() override;

LN_INTERNAL_ACCESS:
	VertexDeclaration();
	virtual ~VertexDeclaration();
	void initialize(detail::GraphicsManager* manager);
	void initialize(detail::GraphicsManager* manager, const VertexElement* elements, int count);
	void tryUpdateResource();
	Driver::IVertexDeclaration* getDeviceObject();

	// GraphicsResourceObject interface
	virtual void onChangeDevice(Driver::IGraphicsDevice* device);

private:
	RefPtr<Driver::IVertexDeclaration>	m_deviceObj;		// TODO: これは RefPtr にしたくない。Driver 系の include が必要になってしまう。
	List<VertexElement>					m_vertexElements;
	bool								m_modified;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
