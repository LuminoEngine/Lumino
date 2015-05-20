
#pragma once

#include "Common.h"

namespace Lumino
{
namespace Graphics
{
class GraphicsManager;
class RenderingCommandList;
class Texture;

/**
	@brief		スワップチェインのクラスです。
*/
class SwapChain
	: public RefObject
{
public:
	SwapChain(Platform::Window* targetWindow);
	SwapChain(GraphicsManager* manager, Platform::Window* targetWindow);
	virtual ~SwapChain();

public:

	/**
		@brief	バックバッファのレンダリングターゲットを取得します。
	*/
	Texture* GetBackBuffer() { return m_backColorBuffer; }
	
	/**
		@brief	バックバッファの深度バッファを取得します。
	*/
	Texture* GetBackBufferDepth() { return m_backDepthBuffer; }

	/**
		@brief	バックバッファのサイズを変更します。
	*/
	void Resize(const Size& newSize);

	/**
		@brief	バックバッファのレンダリング結果をフロントバッファに転送します。
	*/
	void Present();

private:
	friend class GraphicsManager;
	friend class Renderer;
	friend class PresentCommand;
	SwapChain(GraphicsManager* manager, const Size& mainWindowSize);
	void Initialize(const Size& backbufferSize);

	GraphicsManager*		m_manager;
	Device::ISwapChain*		m_deviceObj;
	RenderingCommandList*	m_commandList;
	Threading::EventFlag	m_waiting;		///< コマンド実行していない
	Texture*				m_backColorBuffer;
	Texture*				m_backDepthBuffer;
};

//typedef RefPtr < SwapChain > SwapChainPtr;
//class SwapChainPtr : public RefPtr < SwapChain >
//{
//	void New(Platform::Window* targetWindow) { LN_NEW SwapChain(targetWindow); }
//	void New(GraphicsManager* manager, Platform::Window* targetWindow) { LN_NEW SwapChain(manager, targetWindow); }
//};

} // namespace Graphics
} // namespace Lumino
