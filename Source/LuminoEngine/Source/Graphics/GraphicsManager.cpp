
#include "../Internal.h"
#include <Lumino/Base/Hash.h>
#include <Lumino/Graphics/Text/Font.h>

#if defined(LN_OS_WIN32)
#include "Device/DirectX9/DX9GraphicsDevice.h"
#include "Device/OpenGL/WGLGraphicsDevice.h"
#elif defined(LN_OS_MAC)
#include "Device/OpenGL/NSGLGraphicsDevice.h"
#elif defined(LN_X11)
#include "Device/OpenGL/GLXGraphicsDevice.h"
#endif

#include <Lumino/Graphics/BitmapPainter.h>
#include "GraphicsManager.h"
#include <Lumino/Graphics/Graphics.h>
#include "Text/FreeTypeFont.h"
#include "RendererImpl.h"
#include "Text/FontGlyphTextureCache.h"
#include "RenderingThread.h"
#include "../Rendering/ShapesRenderFeature.h"
#include "../Rendering/NanoVGRenderFeature.h"
#include "Text/FontManager.h"
#include "Text/TextRenderer.h"
#include "Text/BitmapTextRenderer.h"
#include <Lumino/Graphics/Viewport.h>
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Rendering/Rendering.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// GraphicsResourceObject
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(GraphicsResourceObject, Object);

//------------------------------------------------------------------------------
GraphicsResourceObject::GraphicsResourceObject()
	: m_manager(nullptr)
{
}

static int _pp = 0;

//------------------------------------------------------------------------------
GraphicsResourceObject::~GraphicsResourceObject()
{
}

//------------------------------------------------------------------------------
void GraphicsResourceObject::finalize_()
{
	Dispose();
}

//------------------------------------------------------------------------------
void GraphicsResourceObject::initialize()
{
	m_manager = detail::EngineDomain::GetGraphicsManager();
	m_manager->AddResourceObject(this);
}

//------------------------------------------------------------------------------
void GraphicsResourceObject::Dispose()
{
	if (m_manager != nullptr)
	{
		m_manager->RemoveResourceObject(this);
		m_manager = nullptr;
	}
}

namespace detail {
//==============================================================================
// GraphicsManager
//==============================================================================

static GraphicsManager* g_GraphicsManagerInstance = nullptr;

//------------------------------------------------------------------------------
GraphicsManager* GraphicsManager::GetInstance(GraphicsManager* priority)
{
	if (priority != nullptr)
	{
		return priority;
	}
	return g_GraphicsManagerInstance;
}

//------------------------------------------------------------------------------
GraphicsManager::GraphicsManager()
	: m_animationManager(nullptr)
	, m_fileManager(nullptr)
	, m_physicsManager(nullptr)
	, m_mainWindow(nullptr)
	, m_fontManager(nullptr)
	, m_graphicsDevice(nullptr)
	, m_mainSwapChain(nullptr)
	, m_renderingType(GraphicsRenderingType::Immediate)
	, m_dummyDeviceTexture(nullptr)
	, m_dymmyBlackTexture(nullptr)
	, m_dymmyWhiteTexture(nullptr)
	, m_renderer(nullptr)
	, m_renderingThread(nullptr)
	, m_lockPresentCommandList()
	, m_internalContext(nullptr)
	, m_activeContext(nullptr)
	, m_textRendererCore(nullptr)
	, m_bitmapTextRenderer(nullptr)
{
}

//------------------------------------------------------------------------------
GraphicsManager::~GraphicsManager()
{
	LN_SAFE_RELEASE(m_defaultVertexDeclaration);
	LN_SAFE_RELEASE(m_bitmapTextRenderer);

	if (m_textRendererCore != nullptr)
	{
		m_textRendererCore->Dispose();
		LN_SAFE_RELEASE(m_textRendererCore);
	}
	m_dymmyBlackTexture.safeRelease();
	m_dymmyWhiteTexture.safeRelease();
	LN_SAFE_RELEASE(m_dummyDeviceTexture);
	LN_SAFE_RELEASE(m_mainSwapChain);
	LN_SAFE_RELEASE(m_renderer);
	LN_SAFE_RELEASE(m_fileManager);

	if (m_fontManager != nullptr)
	{
		LN_SAFE_RELEASE(m_fontManager);
	}

	if (m_graphicsDevice != nullptr)
	{
		m_graphicsDevice->Finalize();
		LN_SAFE_RELEASE(m_graphicsDevice);
	}

	if (g_GraphicsManagerInstance == this)
	{
		g_GraphicsManagerInstance = nullptr;
	}
}


//------------------------------------------------------------------------------
void GraphicsManager::initialize(const ConfigData& configData)
{
	m_renderingType = configData.renderingType;
	m_animationManager = configData.animationManager;
	LN_REFOBJ_SET(m_fileManager, configData.fileManager);
	m_physicsManager = configData.physicsManager;
	m_mainWindow = configData.mainWindow;
	m_platformTextureLoading = configData.platformTextureLoading;

	// フォント管理
	m_fontManager = LN_NEW FontManager();
	m_fontManager->initialize(m_fileManager, this);

#if defined(LN_OS_WIN32)
	if (configData.graphicsAPI == GraphicsAPI::DirectX9)
	{
		Driver::DX9GraphicsDevice::ConfigData data;
		data.MainWindow = configData.mainWindow;
		data.FileManager = configData.fileManager;
		data.D3D9Device = (IDirect3DDevice9*)configData.D3D9Device;
		data.BackbufferSize = configData.backBufferSize;
		data.EnableVSyncWait = false;			// TODO
		data.EnableFPUPreserve = configData.fpuPreserveEnabled;
		auto* device = LN_NEW Driver::DX9GraphicsDevice();
		device->initialize(data);
		ChangeDevice(device);
		device->release();
	}
	else if (configData.graphicsAPI == GraphicsAPI::OpenGL)
	{
		m_platformTextureLoading = false;

		Driver::WGLGraphicsDevice::ConfigData data;
		data.mainWindow = configData.mainWindow;
		data.openGLMajorVersion = 2;
		data.openGLMinorVersion = 1;
		data.createSharedRenderingContext = (m_renderingType == GraphicsRenderingType::Threaded);
		auto* device = LN_NEW Driver::WGLGraphicsDevice();
		device->initialize(data);
		ChangeDevice(device);
		device->release();
	}
	else {
		LN_THROW(0, ArgumentException);
    }
    
#elif defined(LN_OS_MAC)
    m_platformTextureLoading = false;
    
    Driver::NSGLGraphicsDevice::ConfigData data;
    data.MainWindow = configData.mainWindow;
    data.OpenGLMajorVersion = 2;
    data.OpenGLMinorVersion = 1;

    auto* device = LN_NEW Driver::NSGLGraphicsDevice();
    device->initialize(data);
    ChangeDevice(device);
    device->Release();
    
#elif defined(LN_X11)
    m_platformTextureLoading = false;
    
	auto* device = LN_NEW Driver::GLXGraphicsDevice();
	device->initialize(data);
	ChangeDevice(device);
	device->Release();
#else
	LN_THROW(0, NotImplementedException);
#endif

	Driver::IRenderer* renderer = m_graphicsDevice->GetRenderer();
	renderer->SetDiag(configData.diag);

	// Renderer
	m_renderer = LN_NEW Details::Renderer(this);

	Driver::ISwapChain* deviceSwapChain = m_graphicsDevice->GetDefaultSwapChain();
	if (deviceSwapChain != nullptr) {
		m_mainSwapChain = LN_NEW SwapChain();
		m_mainSwapChain->InitializeDefault(this);
	}


	// PainterEngine
	//m_painterEngine = LN_NEW PainterEngine();
	//m_painterEngine->create(this);

	m_textRendererCore = LN_NEW detail::TextRendererCore();
	m_textRendererCore->initialize(this);

	//m_graphicsContext = LN_NEW GraphicsContext();
	//m_graphicsContext->initialize(this);

	m_bitmapTextRenderer = LN_NEW BitmapTextRenderer();
	m_bitmapTextRenderer->initialize(this);

	m_internalContext = RefPtr<InternalContext>::makeRef();
	m_internalContext->initialize(this);

	m_shapesRendererCommandListCache = RefPtr<ShapesRendererCommandListCache>::makeRef();
	m_nanoVGCommandListCache = RefPtr<NanoVGCommandListCache>::makeRef();

	m_defaultVertexDeclaration = LN_NEW VertexDeclaration();
	m_defaultVertexDeclaration->initialize(this);
	m_defaultVertexDeclaration->AddVertexElement(0, VertexElementType_Float3, VertexElementUsage_Position, 0);
	m_defaultVertexDeclaration->AddVertexElement(0, VertexElementType_Float2, VertexElementUsage_TexCoord, 0);
	m_defaultVertexDeclaration->AddVertexElement(0, VertexElementType_Float3, VertexElementUsage_Normal, 0);
	m_defaultVertexDeclaration->AddVertexElement(0, VertexElementType_Float4, VertexElementUsage_Color, 0);

	
	// Shader common header.
	{
		static const unsigned char EffectHeader_Data[] =
		{
#include "Resource/EffectHeader.fxh.h"
		};
		static const size_t EffectHeader_Data_Len = LN_ARRAY_SIZE_OF(EffectHeader_Data);
		m_commonShaderHeader.assignCStr((const char*)EffectHeader_Data, EffectHeader_Data_Len);
	}
	
	// デフォルトシェーダ
	{
		static const byte_t shaderData[] =
		{
#include "Resource/NoLightingRendering.fx.h"
		};
		static const size_t shaderDataLen = LN_ARRAY_SIZE_OF(shaderData);
		auto shader = RefPtr<Shader>::makeRef();
		shader->initialize(this, (const char*)shaderData, shaderDataLen);
		m_builtinShaders[(int)BuiltinShader::Sprite] = shader;
	}
	{
		static const byte_t shaderData[] =
		{
#include "Resource/ForwardRendering.fx.h"
		};
		static const size_t shaderDataLen = LN_ARRAY_SIZE_OF(shaderData);
		auto shader = RefPtr<Shader>::makeRef();
		shader->initialize(this, (const char*)shaderData, shaderDataLen);
		m_builtinShaders[(int)BuiltinShader::LegacyDiffuse] = shader;
	}
	


	if (m_renderingType == GraphicsRenderingType::Threaded)
	{
		// 描画スレッドを立ち上げる
		m_renderingThread = LN_NEW RenderingThread();
		m_renderingThread->Reset(m_graphicsDevice);
		m_renderingThread->start();
	}

	if (g_GraphicsManagerInstance == nullptr)
	{
		g_GraphicsManagerInstance = this;
	}

	CreateGlobalObjects();
}


//------------------------------------------------------------------------------
void GraphicsManager::Dispose()
{
	if (m_renderingThread != nullptr)
	{
		m_renderingThread->Dispose();
		LN_SAFE_DELETE(m_renderingThread);
	}

	if (m_fontManager != nullptr)
	{
		m_fontManager->Dispose();
	}

	// TODO: なんか量がすごいことになっている。多分 VertexDecl。Diag で監視する。
	auto deleteList = m_resourceObjectList;
	for (auto& obj : deleteList)
	{
		obj->Dispose();
	}
	deleteList.clear();
	m_resourceObjectList.clear();

	m_internalContext.safeRelease();
}

//------------------------------------------------------------------------------
GraphicsAPI GraphicsManager::GetGraphicsAPI() const
{
	return m_graphicsDevice->GetGraphicsAPI();
}

//------------------------------------------------------------------------------
void GraphicsManager::PauseDevice()
{
	// TODO: ユーザーコールバック
	for (auto* listener : m_deviceResetListenerList)
	{
		listener->OnLostDevice();
	}
	m_graphicsDevice->OnLostDevice();
}

//------------------------------------------------------------------------------
void GraphicsManager::ResumeDevice()
{
	m_graphicsDevice->OnResetDevice();
	for (auto* listener : m_deviceResetListenerList)
	{
		listener->OnResetDevice();
	}
	// TODO: ユーザーコールバック
}

//------------------------------------------------------------------------------
void GraphicsManager::ChangeDevice(Driver::IGraphicsDevice* device)
{
	if (m_renderingThread != NULL) {
		LN_THROW(0, InvalidOperationException);
	}

	if (device == NULL)
	{
		// 全オブジェクトに通知
		for (auto& obj : m_resourceObjectList) {
			obj->OnChangeDevice(NULL);
		}

		// 色々解放
		LN_SAFE_RELEASE(m_dummyDeviceTexture);

		if (m_graphicsDevice != NULL)
		{
			if (m_renderingType == GraphicsRenderingType::Immediate) {
				m_graphicsDevice->DetachRenderingThread();
			}

			m_graphicsDevice->Finalize();
			LN_SAFE_RELEASE(m_graphicsDevice);
		}
	}
	else
	{
		LN_REFOBJ_SET(m_graphicsDevice, device);

		// Immediate の場合は Initialize したスレッドをレンダリングスレッドとする
		if (m_renderingType == GraphicsRenderingType::Immediate) {
			m_graphicsDevice->AttachRenderingThread();
		}

		// ダミーテクスチャ
		m_dummyDeviceTexture = m_graphicsDevice->CreateTexture(SizeI(32, 32), false, TextureFormat::R8G8B8A8, NULL);
		{
			BitmapPainter painter(m_dummyDeviceTexture->Lock());
			painter.clear(Color32::White);
			m_dummyDeviceTexture->Unlock();
		}

		// 全オブジェクトに通知
		for (auto& obj : m_resourceObjectList) {
			obj->OnChangeDevice(device);
		}
	}
}

//------------------------------------------------------------------------------
void GraphicsManager::SwitchActiveContext(detail::ContextInterface* context)
{
	if (context != m_activeContext)
	{
		if (m_activeContext != nullptr)
		{
			m_activeContext->OnDeactivated();
		}

		m_activeContext = context;

		if (m_activeContext != nullptr)
		{
			m_activeContext->OnActivated();
		}
	}
}

//------------------------------------------------------------------------------
void GraphicsManager::AddResourceObject(GraphicsResourceObject* obj)
{
	m_resourceObjectList.add(obj);
}

//------------------------------------------------------------------------------
void GraphicsManager::RemoveResourceObject(GraphicsResourceObject* obj)
{
	m_resourceObjectList.remove(obj);
}

//------------------------------------------------------------------------------
InternalContext* GraphicsManager::GetInternalContext() const
{
	return m_internalContext;
}

//------------------------------------------------------------------------------
const RefPtr<Shader>& GraphicsManager::GetBuiltinShader(BuiltinShader shader) const
{
	return m_builtinShaders[(int)shader];
}

//------------------------------------------------------------------------------
void GraphicsManager::PresentSwapChain(SwapChain* swapChain)
{
	if (GetRenderingType() == GraphicsRenderingType::Immediate)
	{
		swapChain->PresentInternal();

		// 一時メモリの解放とかをやっておく
		GetPrimaryRenderingCommandList()->PostExecute();
	}
	else
	{
		// ごく稀に RenderingCommandList::Execute() でイテレータアクセス assert する問題があった。
		// この assert が発生する原因は、イテレート中に他のスレッドから Add とかされた時。
		// でも、パッと見原因になりそうなところが見つからなかったので、もしかしたら
		// キャッシュにリストのポインタが残っていたことが原因かもしれない。
		// 念のためここでキャッシュをフラッシュし、様子を見る。
		MutexScopedLock lock(m_lockPresentCommandList);

		// 前回この SwapChain から発行したコマンドリストがまだ処理中である。待ち状態になるまで待機する。
		swapChain->WaitForPresent();

		// 実行状態にする。Present コマンドが実行された後、コマンドリストクラスから True がセットされる。
		// ※ PresentCommandList() の前に false にしておかないとダメ。
		//    後で false にすると、PresentCommandList() と同時に全部のコマンドが実行されて、描画スレッドから
		//    true がセットされるのに、その後 false をセットしてしまうことがある。
		swapChain->m_waiting.SetFalse();

		GetGraphicsDevice()->FlushResource();

		// Primary コマンドリストの末尾に Present を追加し、キューへ追加する
		GetRenderer()->PresentCommandList(swapChain);
	}
}

//------------------------------------------------------------------------------
RenderingCommandList* GraphicsManager::GetPrimaryRenderingCommandList()
{
	return m_renderer->m_primaryCommandList;
}

//------------------------------------------------------------------------------
void GraphicsManager::CreateGlobalObjects()
{
	// 作成の式を簡単にするためのヘルパー
	auto colorBrush = [this](const Color& c)
	{
		auto brush = NewObject<SolidColorBrush>(c);
		this->m_globalBrushes.add(brush);
		return brush;
	};

	// brushes
	Brush::White = colorBrush(Color(1.0, 1.0, 1.0, 1.0));
	Brush::Black = colorBrush(Color(0.0, 0.0, 0.0, 1.0));
	Brush::Gray = colorBrush(Color(0.5, 0.5, 0.5, 1.0));
	Brush::Red = colorBrush(Color(1.0, 0.0, 0.0, 1.0));
	Brush::Green = colorBrush(Color(0.0, 1.0, 0.0, 1.0));
	Brush::Blue = colorBrush(Color(0.0, 0.0, 1.0, 1.0));
	Brush::DimGray = colorBrush(Color(0.25, 0.25, 0.25, 1.0));

	// black texture
	m_dymmyBlackTexture = NewObject<Texture2D>(SizeI(32, 32), TextureFormat::R8G8B8A8, false, ResourceUsage::Static);
	m_dymmyBlackTexture->clear(Color32::Black);

	// white texture
	m_dymmyWhiteTexture = NewObject<Texture2D>(SizeI(32, 32), TextureFormat::R8G8B8A8, false, ResourceUsage::Static);
	m_dymmyWhiteTexture->clear(Color32::White);
}

//==============================================================================
// ShaderVariableCommitSerializeHelper
//==============================================================================
//------------------------------------------------------------------------------
ShaderVariableCommitSerializeHelper::ShaderVariableCommitSerializeHelper()
{
	m_writerBuffer = MemoryStream::create();
	m_writer = RefPtr<BinaryWriter>::makeRef(m_writerBuffer);
}

//------------------------------------------------------------------------------
void ShaderVariableCommitSerializeHelper::BeginSerialize()
{
	m_writer->seek(0, SeekOrigin_Begin);
}

//------------------------------------------------------------------------------
void ShaderVariableCommitSerializeHelper::WriteValue(Driver::IShaderVariable* targetVariable, const ShaderValue& value)
{
	m_writer->writeInt8(value.getType());
	m_writer->writeUInt64((intptr_t)targetVariable);

	switch (value.getType())
	{
	case ShaderVariableType_Bool:
		m_writer->writeUInt8(value.GetBool() ? 1 : 0);
		break;
	case ShaderVariableType_BoolArray:
		m_writer->writeUInt8(value.GetArrayLength());
		m_writer->write(value.GetBoolArray(), sizeof(bool) * value.GetArrayLength());
		break;
	case ShaderVariableType_Int:
		m_writer->writeInt32(value.getInt());
		break;
	case ShaderVariableType_Float:
		m_writer->writeFloat(value.GetFloat());
		break;
	case ShaderVariableType_FloatArray:
		m_writer->writeUInt8(value.GetArrayLength());
		m_writer->write(value.GetFloatArray(), sizeof(float) * value.GetArrayLength());
		break;
	case ShaderVariableType_Vector:
		m_writer->write(&value.GetVector(), sizeof(Vector4));
		break;
	case ShaderVariableType_VectorArray:
		m_writer->writeUInt8(value.GetArrayLength());
		m_writer->write(value.GetVectorArray(), sizeof(Vector4) * value.GetArrayLength());
		break;
	case ShaderVariableType_Matrix:
		m_writer->write(&value.GetMatrix(), sizeof(Matrix));
		break;
	case ShaderVariableType_MatrixArray:
		m_writer->writeUInt8(value.GetArrayLength());
		m_writer->write(value.GetMatrixArray(), sizeof(Matrix) * value.GetArrayLength());
		break;
	case ShaderVariableType_DeviceTexture:
		m_writer->writeUInt64((intptr_t)value.GetDeviceTexture());
		break;
	case ShaderVariableType_ManagedTexture:
		m_writer->writeUInt64((intptr_t)((value.GetManagedTexture()) ? value.GetManagedTexture()->ResolveDeviceObject() : nullptr));
		break;
	default:
		// TODO: シェーダ変数に値が1度もセットされなかった場合ここに来る。デフォルト値を使うべき？
		LN_THROW(0, ArgumentException);
		break;
	}
}

//------------------------------------------------------------------------------
void* ShaderVariableCommitSerializeHelper::GetSerializeData()
{
	return m_writerBuffer->GetBuffer();
}

//------------------------------------------------------------------------------
size_t ShaderVariableCommitSerializeHelper::GetSerializeDataLength()
{
	return (size_t)m_writerBuffer->getPosition();
}

//------------------------------------------------------------------------------
void ShaderVariableCommitSerializeHelper::Deserialize(const void* data, size_t length)
{
	const byte_t* raw = (const byte_t*)data;
	MemoryStream buffer(data, length);
	BinaryReader reader(&buffer);

	while (!reader.isEOF())
	{
		uint8_t type = reader.readInt8();
		Driver::IShaderVariable* variable = (Driver::IShaderVariable*)reader.readUInt64();

		switch (type)
		{
			case ShaderVariableType_Bool:
			{
				variable->SetBool(reader.readUInt8() != 0);
				break;
			}
			case ShaderVariableType_BoolArray:
			{
				size_t size = reader.readUInt8();
				variable->SetBoolArray((const bool*)&raw[buffer.getPosition()], size);
				reader.seek(sizeof(bool) * size);
				break;
			}
			case ShaderVariableType_Int:
			{
				variable->setInt(reader.readInt32());
				break;
			}
			case ShaderVariableType_Float:
			{
				variable->SetFloat(reader.readFloat());
				break;
			}
			case ShaderVariableType_FloatArray:
			{
				size_t size = reader.readUInt8();
				variable->SetFloatArray((const float*)&raw[buffer.getPosition()], size);
				reader.seek(sizeof(float) * size);
				break;
			}
			case ShaderVariableType_Vector:
			{
				Vector4 v;
				reader.read(&v, sizeof(Vector4));
				variable->SetVector(v);
				break;
			}
			case ShaderVariableType_VectorArray:
			{
				size_t size = reader.readUInt8();
				variable->SetVectorArray((const Vector4*)&raw[buffer.getPosition()], size);
				reader.seek(sizeof(Vector4) * size);
				break;
			}
			case ShaderVariableType_Matrix:
			{
				Matrix v;
				reader.read(&v, sizeof(Matrix));
				variable->SetMatrix(v);
				break;
			}
			case ShaderVariableType_MatrixArray:
			{
				size_t size = reader.readUInt8();
				variable->SetMatrixArray((const Matrix*)&raw[buffer.getPosition()], size);
				reader.seek(sizeof(Matrix) * size);
				break;
			}
			case ShaderVariableType_DeviceTexture:
			case ShaderVariableType_ManagedTexture:
				variable->SetTexture((Driver::ITexture*)reader.readUInt64());
				break;
			default:
				LN_THROW(0, InvalidOperationException);
				break;
		}
	}
}

} // namespace detail 
LN_NAMESPACE_END
