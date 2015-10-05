
#pragma once
#include "Common.h"
#include "GraphicsResourceObject.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
class RenderingCommandList;
class Texture;

/**
	@brief		スワップチェインのクラスです。
*/
class SwapChain
	: public GraphicsResourceObject
{
public:
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

LN_INTERNAL_ACCESS:
	SwapChain(GraphicsManager* manager, bool isDefault/*, const Size& mainWindowSize, Driver::ISwapChain* deviceSwapChain*/);
	void Initialize(/*const Size& backbufferSize*/);
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

	GraphicsManager*		m_manager;
	Driver::ISwapChain*		m_deviceObj;
	RenderingCommandList*	m_commandList;
	Threading::EventFlag	m_waiting;		///< コマンド実行していない
	Texture*				m_backColorBuffer;
	Texture*				m_backDepthBuffer;
	bool					m_isDefault;
};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
