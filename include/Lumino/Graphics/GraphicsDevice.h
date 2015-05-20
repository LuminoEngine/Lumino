
#pragma once

namespace Lumino
{
namespace Graphics
{

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

} // namespace Graphics
} // namespace Lumino
