
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Animation/Common.h>
#include <Lumino/IO/Common.h>
#include <Lumino/Platform/Common.h>
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/Text/Font.h>

LN_NAMESPACE_BEGIN
class EngineDiagCore;
class FileManager;

LN_NAMESPACE_GRAPHICS_BEGIN
class RenderingThread;
class GraphicsContext;
class RenderingContext;
class DrawingContext;
class TextRenderer;
class BitmapTextRenderer;
class SwapChain;
namespace detail { class ContextInterface; }
namespace detail { class PhysicsManager; }


namespace detail {

class TextRendererCore;
class FontGlyphTextureCache;

// ShaderVariable からコミットするルートと、Material からコミットするルートがある。
// ShaderVariableCommitSerializeHelper は、その同じような処理をまとめたクラス。
// Material は ShaderVariable を参照するが、Material から ShaderVariable に値をセットしたくない。
// ShaderVariable はユーザーに見えるので、知らないうちに値が変わっている、ということが発生してしまう。
class ShaderVariableCommitSerializeHelper
{
public:
	ShaderVariableCommitSerializeHelper();

	void BeginSerialize();
	void WriteValue(Driver::IShaderVariable* targetVariable, const ShaderValue& value);
	void* GetSerializeData();
	size_t GetSerializeDataLength();

	void Deserialize(const void* data, size_t length);

private:
	MemoryStreamPtr			m_writerBuffer;
	RefPtr<BinaryWriter>	m_writer;
};


class IDeviceResetListener
{
public:
	virtual void OnLostDevice() = 0;
	virtual void OnResetDevice() = 0;

protected:
	virtual ~IDeviceResetListener() = default;
};

// グラフィックス機能の管理クラス
class GraphicsManager
	: public RefObject
{
public:

	struct ConfigData
	{
		GraphicsAPI					graphicsAPI = GraphicsAPI::DirectX9;				// レンダリングに使用する API の種類
		GraphicsRenderingType		renderingType = GraphicsRenderingType::Threaded;
		PlatformWindow*				mainWindow = nullptr;								// アプリケーションのメインウィンドウ
		SizeI						backBufferSize = SizeI(640, 480);					// バックバッファのサイズ
		AnimationManager*			animationManager = nullptr;
		FileManager*				fileManager = nullptr;								// FileManager
		PhysicsManager*				physicsManager = nullptr;
		bool						platformTextureLoading = false;						// 画像リソースの読み込みにプラットフォーム固有の機能を使用するか
		void*						D3D9Device = nullptr;								// 作成済みの IDirect3DDevice9 インターフェイス
		bool						fpuPreserveEnabled = false;
		EngineDiagCore*				diag = nullptr;
	};

	static GraphicsManager*	GetInstance(GraphicsManager* priority = nullptr);

public:
	GraphicsManager();
	~GraphicsManager();

	void Initialize(const ConfigData& configData);
	void Finalize();

	/** 現在のグラフィックスシステムが使用している API の種類を確認します。*/
	GraphicsAPI GetGraphicsAPI() const;

	/** 現在のグラフィックスシステムのレンダリング方法を確認します。*/
	GraphicsRenderingType GetRenderingType() const { return m_renderingType; }

	/** グラフィックスシステムのメイン Renderer を取得します。*/
	Details::Renderer* GetRenderer() const { return m_renderer; }

	/** メインの SwapChain を取得します。これは Create() で指定されたメインウィンドウへのスワップチェインです。*/
	SwapChain* GetMainSwapChain() { return m_mainSwapChain; }


	AnimationManager* GetAnimationManager() const { return m_animationManager; }

	/** 関連付けられている FileManager を取得します。*/
	FileManager* GetFileManager() const { return m_fileManager; }

	PlatformWindow* GetMainWindow() const { return m_mainWindow; }

	/** 関連付けられている FontManager を取得します。*/
	FontManager* GetFontManager() const { return m_fontManager; }
	
	RenderingContext* GetRenderingContext() const { return m_renderingContext; }
	DrawingContext* GetDrawingContext() const { return m_drawingContext; }\

	BitmapTextRenderer* GetBitmapTextRenderer() const { return m_bitmapTextRenderer; }

	void AddDeviceResetListener(IDeviceResetListener* listener) { m_deviceResetListenerList.Add(listener); }
	void RemoveDeviceResetListener(IDeviceResetListener* listener) { m_deviceResetListenerList.Remove(listener); }

	/// TODO: (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void PauseDevice();
	/// TODO: (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void ResumeDevice();

	void ChangeDevice(Driver::IGraphicsDevice* device);

	void SwitchActiveContext(ContextInterface* context);
	ContextInterface* GetActiveContext() const { return m_activeContext; }

public:
	void AddResourceObject(GraphicsResourceObject* obj) { m_resourceObjectList.Add(obj); }
	void RemoveResourceObject(GraphicsResourceObject* obj) { m_resourceObjectList.Remove(obj); }
	PhysicsManager* GetPhysicsManager() const { return m_physicsManager; }
	Driver::IGraphicsDevice* GetGraphicsDevice() { return m_graphicsDevice; }
	RenderingThread* GetRenderingThread() { return m_renderingThread; }
	bool IsPlatformTextureLoading() { return m_platformTextureLoading; }
	RenderingCommandList* GetPrimaryRenderingCommandList();
	TextRendererCore* GetTextRendererCore() { return m_textRendererCore; }
	Driver::ITexture* GetDummyDeviceTexture() { return m_dummyDeviceTexture; }
	const RefPtr<Texture2D>& GetDummyWhiteTexture() { return m_dymmyWhiteTexture; }
	VertexDeclaration* GetDefaultVertexDeclaration() const { return m_defaultVertexDeclaration; }
	ShaderVariableCommitSerializeHelper* GetShaderVariableCommitSerializeHelper() { return &m_shaderVariableCommitSerializeHelper; }

private:
	AnimationManager*				m_animationManager;
	FileManager*					m_fileManager;
	PhysicsManager*					m_physicsManager;
	PlatformWindow*					m_mainWindow;
	FontManager*					m_fontManager;
	GraphicsRenderingType			m_renderingType;
	Array<GraphicsResourceObject*>	m_resourceObjectList;
	Array<IDeviceResetListener*>	m_deviceResetListenerList;
	
	Driver::IGraphicsDevice*		m_graphicsDevice;
	SwapChain*						m_mainSwapChain;
	Details::Renderer*				m_renderer;
	RenderingThread*				m_renderingThread;

	ContextInterface*				m_activeContext;
	RenderingContext*				m_renderingContext;
	DrawingContext*					m_drawingContext;


	TextRendererCore*				m_textRendererCore;
	BitmapTextRenderer*				m_bitmapTextRenderer;
	ShaderVariableCommitSerializeHelper	m_shaderVariableCommitSerializeHelper;

	Driver::ITexture*				m_dummyDeviceTexture;
	RefPtr<Texture2D>				m_dymmyWhiteTexture;
	VertexDeclaration*				m_defaultVertexDeclaration;
	bool							m_platformTextureLoading;
};

} // namespace detail
LN_NAMESPACE_END
