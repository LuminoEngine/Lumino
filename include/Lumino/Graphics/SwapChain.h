
#pragma once
#include "Common.h"
#include "GraphicsResourceObject.h"

LN_NAMESPACE_BEGIN
class PlatformWindow;

LN_NAMESPACE_GRAPHICS_BEGIN

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
	RenderTarget* GetBackBuffer() { return m_backColorBuffer; }
	
	/**
		@brief	バックバッファの深度バッファを取得します。
	*/
	DepthBuffer* GetBackBufferDepth() { return m_backDepthBuffer; }

	/**
		@brief	バックバッファのサイズを変更します。
		TODO: 公開しない。サイズ変更は Viewport から。
	*/
	void Resize(const SizeI& newSize);

	/**
		@brief	バックバッファのレンダリング結果をフロントバッファに転送します。
	*/
	void Present();

LN_INTERNAL_ACCESS:
	SwapChain();
	void InitializeDefault(GraphicsManager* manager);
	void InitializeSub(GraphicsManager* manager, PlatformWindow* window);
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

//private:
	void PostInitialize();

	Driver::ISwapChain*			m_deviceObj;
	RenderingCommandList*		m_commandList;
	ConditionFlag				m_waiting;		// コマンド実行していない
	RenderTarget*				m_backColorBuffer;
	DepthBuffer*				m_backDepthBuffer;
	bool						m_isDefault;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
