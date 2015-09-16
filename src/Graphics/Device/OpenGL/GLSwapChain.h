
#pragma once 

#include "../DeviceInterface.h"
#include "GLTexture.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Device
{
class GLRenderer;

/// OpenGL 用の ISwapChain の実装
class GLSwapChain
	: public ISwapChain
{
public:
	GLSwapChain();
	virtual ~GLSwapChain();

public:

	void Create();

	//virtual void MakeCurrentContext() = 0;

public:
	virtual ITexture* GetBackBuffer() { LN_THROW(0, NotImplementedException); return NULL; }
	virtual void OnLostDevice();
	virtual void OnResetDevice();

protected:
	void InternalPresent(ITexture* colorBuffer, GLRenderer* renderer/*GLuint rendererVAO*/);

private:
	GLuint	m_shaderProgram;
	GLint	m_positionLoc;
	GLint	m_texCoordLoc;
	GLint	m_textureLoc;
	GLuint	m_vertexBuffer;
	GLuint	m_vertexArray;
};

} // namespace Device
LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
