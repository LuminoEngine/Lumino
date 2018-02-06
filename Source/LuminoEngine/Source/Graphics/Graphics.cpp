
#include "../Internal.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/Graphics.h>

#if defined(LN_OS_WIN32)
#include "Device/DirectX9/DX9GraphicsDevice.h"
#include "Device/OpenGL/WGLGraphicsDevice.h"
#endif

LN_NAMESPACE_BEGIN

//==============================================================================
// Graphics
//==============================================================================
#if 0
//------------------------------------------------------------------------------
void Graphics::Set2DRenderingMode(float minZ, float maxZ)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->Set2DRenderingMode(minZ, maxZ);
}
void Graphics::SetBrush(Brush* brush)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->SetBrush(brush);
}
void Graphics::SetOpacity(float opacity)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->SetOpacity(opacity);
}
void Graphics::MoveTo(const Vector3& point, const Color& color)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->MoveTo(point, color);
}
void Graphics::LineTo(const Vector3& point, const Color& color)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->LineTo(point, color);
}
void Graphics::BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const Color& color)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->BezierCurveTo(cp1, cp2, endPt, color);
}
void Graphics::ClosePath()
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->ClosePath();
}
//void Graphics::DrawPoint(const Vector3& point, const Color& color)
//{
//	GraphicsManager::getInstance()->GetGraphicsContext()->DrawPoint(point, color);
//}
void Graphics::DrawTriangle(const Vector3& p1, const Color& p1Color, const Vector3& p2, const Color& p2Color, const Vector3& p3, const Color& p3Color)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->DrawTriangle(p1, p1Color, p2, p2Color, p3, p3Color);
}
void Graphics::DrawRectangle(const RectF& rect)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->DrawRectangle(rect);
}
void Graphics::DrawEllipse(const Vector3& center, const Vector2& radius)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->DrawEllipse(center, radius);

}
void Graphics::DrawTexture(const RectF& rect, Texture* texture, const RectI& srcRect, const Color& color)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->DrawTexture(rect, texture, srcRect, color);
}
void Graphics::DrawText(const StringRef& text, const Point& position)
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->DrawText(text, position);
}
void Graphics::Flush()
{
	GraphicsManager::GetInstance()->GetGraphicsContext()->Flush();
}
#endif

//------------------------------------------------------------------------------
void Graphics::changeDirectX9Device(void* id3d9device)
{
#if defined(LN_OS_WIN32)
	if (id3d9device == NULL)
	{
		detail::GraphicsManager::getInstance()->changeDevice(NULL);
	}
	else
	{
		Driver::DX9GraphicsDevice::ConfigData data;
		data.MainWindow = detail::GraphicsManager::getInstance()->getMainWindow();
		data.FileManager = detail::GraphicsManager::getInstance()->getFileManager();
		data.D3D9Device = (IDirect3DDevice9*)id3d9device;
		//data.BackbufferSize = configData.MainWindow->GetSize();	// TODO
		//data.EnableVSyncWait = false;			// TODO
		//data.EnableFPUPreserve = false;			// TODO
		auto* device = LN_NEW Driver::DX9GraphicsDevice();
		device->initialize(data);
		detail::GraphicsManager::getInstance()->changeDevice(device);
		device->release();
	}
    
#endif
}

//------------------------------------------------------------------------------
//RenderTargetTexturePtr GraphicsPlatformSupport::CreateRenderTargetFromD3D9Surface(void* surface)
//{
//	auto* device = dynamic_cast<Driver::DX9GraphicsDevice*>(detail::EngineDomain::getGraphicsManager()->getGraphicsDevice());
//	if (LN_REQUIRE(device != nullptr)) return nullptr;
//
//	auto* ptr = LN_NEW Driver::DX9BackBufferTexture(device);
//	ptr->reset((IDirect3DSurface9*)surface);
//
//}

LN_NAMESPACE_END
