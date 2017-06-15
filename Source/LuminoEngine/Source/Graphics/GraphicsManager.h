
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
class GraphicsContext;
class TextRenderer;
class BitmapTextRenderer;
class SwapChain;
class SolidColorBrush;
namespace detail { class ContextInterface; }
namespace detail { class PhysicsManager; }


namespace detail {
class InternalContext;
class TextRendererCore;
class FontGlyphTextureCache;
class ShapesRendererCommandListCache;
class NanoVGCommandListCache;



// ShaderVariable からコミットするルートと、Material からコミットするルートがある。
// ShaderVariableCommitSerializeHelper は、その同じような処理をまとめたクラス。
// Material は ShaderVariable を参照するが、Material から ShaderVariable に値をセットしたくない。
// ShaderVariable はユーザーに見えるので、知らないうちに値が変わっている、ということが発生してしまう。
class ShaderVariableCommitSerializeHelper
{
public:
	ShaderVariableCommitSerializeHelper();

	void beginSerialize();
	void writeValue(Driver::IShaderVariable* targetVariable, const ShaderValue& value);
	void* getSerializeData();
	size_t getSerializeDataLength();

	void deserialize(const void* data, size_t length);

private:
	MemoryStreamPtr			m_writerBuffer;
	RefPtr<BinaryWriter>	m_writer;
};


class IDeviceResetListener
{
public:
	virtual void onLostDevice() = 0;
	virtual void onResetDevice() = 0;

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

	static GraphicsManager*	getInstance(GraphicsManager* priority = nullptr);

public:
	GraphicsManager();
	virtual ~GraphicsManager();

	void initialize(const ConfigData& configData);
	void Dispose();

	/** 現在のグラフィックスシステムが使用している API の種類を確認します。*/
	GraphicsAPI getGraphicsAPI() const;

	/** 現在のグラフィックスシステムのレンダリング方法を確認します。*/
	GraphicsRenderingType getRenderingType() const { return m_renderingType; }

	/** グラフィックスシステムのメイン Renderer を取得します。*/
	Details::Renderer* getRenderer() const { return m_renderer; }

	/** メインの SwapChain を取得します。これは create() で指定されたメインウィンドウへのスワップチェインです。*/
	SwapChain* getMainSwapChain() { return m_mainSwapChain; }


	AnimationManager* getAnimationManager() const { return m_animationManager; }

	/** 関連付けられている FileManager を取得します。*/
	FileManager* getFileManager() const { return m_fileManager; }

	PlatformWindow* getMainWindow() const { return m_mainWindow; }

	/** 関連付けられている FontManager を取得します。*/
	FontManager* getFontManager() const { return m_fontManager; }
	
	BitmapTextRenderer* getBitmapTextRenderer() const { return m_bitmapTextRenderer; }

	void addDeviceResetListener(IDeviceResetListener* listener) { m_deviceResetListenerList.add(listener); }
	void removeDeviceResetListener(IDeviceResetListener* listener) { m_deviceResetListenerList.remove(listener); }

	/// TODO: (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void pauseDevice();
	/// TODO: (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void resumeDevice();

	void changeDevice(Driver::IGraphicsDevice* device);

	void switchActiveContext(ContextInterface* context);
	ContextInterface* getActiveContext() const { return m_activeContext; }

public:
	void addResourceObject(GraphicsResourceObject* obj);
	void removeResourceObject(GraphicsResourceObject* obj);
	PhysicsManager* getPhysicsManager() const { return m_physicsManager; }
	Driver::IGraphicsDevice* getGraphicsDevice() { return m_graphicsDevice; }
	RenderingThread* getRenderingThread() { return m_renderingThread; }
	bool isPlatformTextureLoading() { return m_platformTextureLoading; }
	RenderingCommandList* getPrimaryRenderingCommandList();
	TextRendererCore* getTextRendererCore() { return m_textRendererCore; }
	const RefPtr<ShapesRendererCommandListCache>& getShapesRendererCommandListCache() const { return m_shapesRendererCommandListCache; }
	const RefPtr<NanoVGCommandListCache>& getNanoVGCommandListCache() const { return m_nanoVGCommandListCache; }
	Driver::ITexture* getDummyDeviceTexture() { return m_dummyDeviceTexture; }
	const RefPtr<Texture2D>& getDummyBlackTexture() const { return m_dymmyBlackTexture; }
	const RefPtr<Texture2D>& getDummyWhiteTexture() const { return m_dymmyWhiteTexture; }
	VertexDeclaration* getDefaultVertexDeclaration() const { return m_defaultVertexDeclaration; }
	ShaderVariableCommitSerializeHelper* getShaderVariableCommitSerializeHelper() { return &m_shaderVariableCommitSerializeHelper; }
	InternalContext* getInternalContext() const;

	const StringA& getCommonShaderHeader() const { return m_commonShaderHeader; }
	const RefPtr<Shader>& getBuiltinShader(BuiltinShader shader) const;

	void presentSwapChain(SwapChain* swapChain);

private:
	void createGlobalObjects();

	AnimationManager*				m_animationManager;
	FileManager*					m_fileManager;
	PhysicsManager*					m_physicsManager;
	PlatformWindow*					m_mainWindow;
	FontManager*					m_fontManager;
	GraphicsRenderingType			m_renderingType;
	List<RefPtr<GraphicsResourceObject>>	m_resourceObjectList;
	List<IDeviceResetListener*>		m_deviceResetListenerList;
	
	Driver::IGraphicsDevice*		m_graphicsDevice;
	SwapChain*						m_mainSwapChain;
	Details::Renderer*				m_renderer;
	RenderingThread*				m_renderingThread;
	Mutex							m_lockPresentCommandList;

	ContextInterface*				m_activeContext;

	RefPtr<InternalContext>			m_internalContext;
	StringA							m_commonShaderHeader;
	RefPtr<Shader>					m_builtinShaders[2];

	TextRendererCore*				m_textRendererCore;
	BitmapTextRenderer*				m_bitmapTextRenderer;
	ShaderVariableCommitSerializeHelper	m_shaderVariableCommitSerializeHelper;

	RefPtr<ShapesRendererCommandListCache>	m_shapesRendererCommandListCache;
	RefPtr<NanoVGCommandListCache>	m_nanoVGCommandListCache;

	Driver::ITexture*						m_dummyDeviceTexture;
	RefPtr<Texture2D>						m_dymmyBlackTexture;
	RefPtr<Texture2D>						m_dymmyWhiteTexture;
	List<RefPtr<SolidColorBrush>>			m_globalBrushes;

	VertexDeclaration*						m_defaultVertexDeclaration;
	bool									m_platformTextureLoading;
};

} // namespace detail
LN_NAMESPACE_END
