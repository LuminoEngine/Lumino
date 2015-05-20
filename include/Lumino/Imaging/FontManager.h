
#pragma once

namespace Lumino
{
namespace Imaging
{

/**
	@brief		フォントの管理クラス
*/
class FontManager
	: public RefObject
{
public:
	static FontManager* Create(FileManager* fileManager);

protected:
	FontManager();
	~FontManager();

//public:
//
//
//	GraphicsDevice* GetGraphicsDevice() { return m_graphicsDevice; }
//	Renderer* GetRenderer() { return m_renderer; }
//	SwapChain* GetMainSwapChain() { return m_mainSwapChain; }
//
//private:
//	FileManager* GetFileManager() { return m_fileManager; }
//
//private:
//	friend class SwapChain;
//	friend class Renderer;
//	friend class Texture;
//
//	FileManager*			m_fileManager;
//	
//	RefPtr<GraphicsDevice>	m_graphicsDevice;
//	RefPtr<SwapChain>		m_mainSwapChain;
//
//	Renderer*				m_renderer;
//	RenderingThread*		m_renderingThread;
};

} // namespace Imaging
} // namespace Lumino
