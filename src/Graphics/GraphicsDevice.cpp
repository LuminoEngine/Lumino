

#if 0

#include "../Internal.h"
#include "../../include/Lumino/Graphics/GraphicsDevice.h"
#if defined(LN_WIN32)
#include "Device/DirectX9/DX9GraphicsDevice.h"
#include "Device/OpenGL/WGLGraphicsDevice.h"
#elif defined(LN_X11)
#include "Device/OpenGL/GLXGraphicsDevice.h"
#endif

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// GraphicsDevice
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsDevice::GraphicsDevice(const GraphicsDeviceConfigData& configData)
	: m_deviceObj(NULL)
{
#if defined(LN_WIN32)
	if (configData.API == GraphicsAPI_DirectX9)
	{
		Device::DX9GraphicsDevice::ConfigData data;
		data.MainWindow = configData.MainWindow;
		data.FileManager = &FileManager::GetInstance();			// TODO
		data.BackbufferSize = configData.MainWindow->GetSize();	// TODO
		data.EnableVSyncWait = false;			// TODO
		data.EnableFPUPreserve = false;			// TODO
		m_deviceObj = LN_NEW Device::DX9GraphicsDevice();
		static_cast<Device::DX9GraphicsDevice*>(m_deviceObj)->Initialize(data);
	}
	else if(configData.API == GraphicsAPI_OpenGL)
	{
		m_deviceObj = LN_NEW Device::WGLGraphicsDevice();
		static_cast<Device::WGLGraphicsDevice*>(m_deviceObj)->Initialize(configData);
	}
	else {
		LN_THROW(0, ArgumentException);
	}


#elif defined(LN_X11)
	m_deviceObj = LN_NEW Device::GLXGraphicsDevice();
	static_cast<Device::GLXGraphicsDevice*>(m_deviceObj)->Initialize(configData);
#endif


}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GraphicsDevice::~GraphicsDevice()
{
	if (m_deviceObj != NULL)
	{
		m_deviceObj->Finalize();
		LN_SAFE_RELEASE(m_deviceObj);
	}
}


} // namespace Graphics
} // namespace Lumino

#endif