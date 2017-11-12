
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
class ArchiveManager;
class InternalContext;
class TextRendererCore;
class FontGlyphTextureCache;
class GlyphIconFontManager;
class ShapesRendererCommandListCache;
class NanoVGCommandListCache;



// ShaderVariable からコミットするルートと、CommonMaterial からコミットするルートがある。
// ShaderVariableCommitSerializeHelper は、その同じような処理をまとめたクラス。
// CommonMaterial は ShaderVariable を参照するが、CommonMaterial から ShaderVariable に値をセットしたくない。
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
	Ref<BinaryWriter>	m_writer;
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
		ArchiveManager*				archiveManager = nullptr;
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
	void dispose();

	/** 現在のグラフィックスシステムが使用している API の種類を確認します。*/
	GraphicsAPI getGraphicsAPI() const;

	/** 現在のグラフィックスシステムのレンダリング方法を確認します。*/
	GraphicsRenderingType getRenderingType() const { return m_renderingType; }

	/** グラフィックスシステムのメイン Renderer を取得します。*/
	detail::CoreGraphicsRenderFeature* getRenderer() const { return m_renderer; }

	/** メインの SwapChain を取得します。これは create() で指定されたメインウィンドウへのスワップチェインです。*/
	SwapChain* getMainSwapChain() { return m_mainSwapChain; }


	AnimationManager* getAnimationManager() const { return m_animationManager; }

	/** 関連付けられている FileManager を取得します。*/
	FileManager* getFileManager() const { return m_fileManager; }

	PlatformWindow* getMainWindow() const { return m_mainWindow; }

	FontManager* getFontManager() const { return m_fontManager; }
	const Ref<GlyphIconFontManager>& getGlyphIconFontManager() const { return m_glyphIconFontManager; }
	
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
	const Ref<ShapesRendererCommandListCache>& getShapesRendererCommandListCache() const { return m_shapesRendererCommandListCache; }
	const Ref<NanoVGCommandListCache>& getNanoVGCommandListCache() const { return m_nanoVGCommandListCache; }
	Driver::ITexture* getDummyDeviceTexture() { return m_dummyDeviceTexture; }
	const Ref<Texture2D>& getDummyBlackTexture() const { return m_dymmyBlackTexture; }
	const Ref<Texture2D>& getDummyWhiteTexture() const { return m_dymmyWhiteTexture; }
	VertexDeclaration* getDefaultVertexDeclaration() const { return m_defaultVertexDeclaration; }
	ShaderVariableCommitSerializeHelper* getShaderVariableCommitSerializeHelper() { return &m_shaderVariableCommitSerializeHelper; }
	InternalContext* getInternalContext() const;

	const std::string& getCommonShaderHeader() const { return m_commonShaderHeader; }
	const Ref<Shader>& getBuiltinShader(BuiltinShader shader) const;

	

private:
	void createGlobalObjects();

	AnimationManager*				m_animationManager;
	FileManager*					m_fileManager;
	PhysicsManager*					m_physicsManager;
	PlatformWindow*					m_mainWindow;
	FontManager*					m_fontManager;
	Ref<GlyphIconFontManager>		m_glyphIconFontManager;

	GraphicsRenderingType			m_renderingType;
	List<Ref<GraphicsResourceObject>>	m_resourceObjectList;
	List<IDeviceResetListener*>		m_deviceResetListenerList;
	
	Driver::IGraphicsDevice*		m_graphicsDevice;
	SwapChain*						m_mainSwapChain;
	detail::CoreGraphicsRenderFeature*	m_renderer;
	RenderingThread*				m_renderingThread;

	ContextInterface*				m_activeContext;

	Ref<InternalContext>			m_internalContext;
	std::string						m_commonShaderHeader;
	Ref<Shader>					m_builtinShaders[2];

	TextRendererCore*				m_textRendererCore;
	BitmapTextRenderer*				m_bitmapTextRenderer;
	ShaderVariableCommitSerializeHelper	m_shaderVariableCommitSerializeHelper;

	Ref<ShapesRendererCommandListCache>	m_shapesRendererCommandListCache;
	Ref<NanoVGCommandListCache>	m_nanoVGCommandListCache;

	Driver::ITexture*						m_dummyDeviceTexture;
	Ref<Texture2D>						m_dymmyBlackTexture;
	Ref<Texture2D>						m_dymmyWhiteTexture;
	List<Ref<SolidColorBrush>>			m_globalBrushes;

	VertexDeclaration*						m_defaultVertexDeclaration;
	bool									m_platformTextureLoading;
};

} // namespace detail
LN_NAMESPACE_END
