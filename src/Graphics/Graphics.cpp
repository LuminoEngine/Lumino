
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/DrawingContext.h>
#include <Lumino/Graphics/Graphics.h>

#if defined(LN_OS_WIN32)
#include "Device/DirectX9/DX9GraphicsDevice.h"
#include "Device/OpenGL/WGLGraphicsDevice.h"
#endif

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// Graphics
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Graphics::Set2DRenderingMode(float minZ, float maxZ)
{
	GraphicsManager::Instance->GetGraphicsContext()->Set2DRenderingMode(minZ, maxZ);
}
void Graphics::SetBrush(Brush* brush)
{
	GraphicsManager::Instance->GetGraphicsContext()->SetBrush(brush);
}
void Graphics::DrawRectangle(const RectF& rect, const ColorF& color)
{
	GraphicsManager::Instance->GetGraphicsContext()->DrawRectangle(rect, color);
}
void Graphics::Flush()
{
	GraphicsManager::Instance->GetGraphicsContext()->Flush();
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Graphics::ChangeDirectX9Device(void* id3d9device)
{
	if (id3d9device == NULL)
	{
		GraphicsManager::Instance->ChangeDevice(NULL);
	}
	else
	{
		Driver::DX9GraphicsDevice::ConfigData data;
		data.MainWindow = GraphicsManager::Instance->GetMainWindow();
		data.FileManager = GraphicsManager::Instance->GetFileManager();
		data.D3D9Device = (IDirect3DDevice9*)id3d9device;
		//data.BackbufferSize = configData.MainWindow->GetSize();	// TODO
		//data.EnableVSyncWait = false;			// TODO
		//data.EnableFPUPreserve = false;			// TODO
		auto* device = LN_NEW Driver::DX9GraphicsDevice();
		device->Initialize(data);
		GraphicsManager::Instance->ChangeDevice(device);
		device->Release();
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
