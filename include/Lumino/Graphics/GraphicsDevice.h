
#pragma once

namespace Lumino
{
namespace Graphics
{
#if 0

struct GraphicsDeviceConfigData
{
	GraphicsAPI			API;
	Platform::Window*	MainWindow;

	int		OpenGLMajorVersion;
	int		OpenGLMinorVersion;
};

/**
	@brief		TODO: Manager に統合したほうが良いかも
*/
class GraphicsDevice
	: public RefObject
{
public:
public:
	GraphicsDevice(const GraphicsDeviceConfigData& configData);
	~GraphicsDevice();

public:
	Device::IGraphicsDevice* GetDeviceObject() { return m_deviceObj; }

private:
	Device::IGraphicsDevice*	m_deviceObj;
	
};
#endif

} // namespace Graphics
} // namespace Lumino
