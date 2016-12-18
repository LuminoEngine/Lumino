
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
#include "NanoVGRenderer.h"
#include "Text/FontManager.h"
#include "Text/TextRenderer.h"
#include "Text/BitmapTextRenderer.h"
#include <Lumino/Graphics/Viewport.h>
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/Rendering.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// GraphicsResourceObject
//==============================================================================

//------------------------------------------------------------------------------
GraphicsResourceObject::GraphicsResourceObject()
	: m_manager(nullptr)
{
}

//------------------------------------------------------------------------------
GraphicsResourceObject::~GraphicsResourceObject()
{
	Finalize();
}

//------------------------------------------------------------------------------
void GraphicsResourceObject::Initialize(detail::GraphicsManager* manager)
{
	m_manager = manager;
	m_manager->AddResourceObject(this);
}

//------------------------------------------------------------------------------
void GraphicsResourceObject::Finalize()
{
	if (m_manager != nullptr)
	{
		m_manager->RemoveResourceObject(this);
		m_manager = nullptr;
	}
}

//------------------------------------------------------------------------------
void GraphicsResourceObject::ApplyModifies()
{
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
	LN_SAFE_RELEASE(m_textRendererCore);
	m_dymmyWhiteTexture.SafeRelease();
	LN_SAFE_RELEASE(m_dummyDeviceTexture);
	LN_SAFE_RELEASE(m_mainSwapChain);
	LN_SAFE_RELEASE(m_renderer);
	LN_SAFE_RELEASE(m_fileManager);

	if (m_fontManager != nullptr)
	{
		m_fontManager->Finalize();
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
void GraphicsManager::Initialize(const ConfigData& configData)
{
	m_renderingType = configData.renderingType;
	m_animationManager = configData.animationManager;
	LN_REFOBJ_SET(m_fileManager, configData.fileManager);
	m_physicsManager = configData.physicsManager;
	m_mainWindow = configData.mainWindow;
	m_platformTextureLoading = configData.platformTextureLoading;

	// フォント管理
	m_fontManager = LN_NEW FontManager();
	m_fontManager->Initialize(m_fileManager, this);

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
		device->Initialize(data);
		ChangeDevice(device);
		device->Release();
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
		device->Initialize(data);
		ChangeDevice(device);
		device->Release();
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
    device->Initialize(data);
    ChangeDevice(device);
    device->Release();
    
#elif defined(LN_X11)
    m_platformTextureLoading = false;
    
	auto* device = LN_NEW Driver::GLXGraphicsDevice();
	device->Initialize(data);
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
	//m_painterEngine->Create(this);

	m_textRendererCore = LN_NEW detail::TextRendererCore();
	m_textRendererCore->Initialize(this);

	//m_graphicsContext = LN_NEW GraphicsContext();
	//m_graphicsContext->Initialize(this);

	m_bitmapTextRenderer = LN_NEW BitmapTextRenderer();
	m_bitmapTextRenderer->Initialize(this);

	m_internalContext = RefPtr<InternalContext>::MakeRef();
	m_internalContext->Initialize(this);

	m_nanoVGCommandListCache = RefPtr<NanoVGCommandListCache>::MakeRef();

	m_defaultVertexDeclaration = LN_NEW VertexDeclaration();
	m_defaultVertexDeclaration->Initialize(this);
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
		m_commonShaderHeader.AssignCStr((const char*)EffectHeader_Data, EffectHeader_Data_Len);
	}
	
	// デフォルトシェーダ
	{
		static const byte_t shaderData[] =
		{
#include "Resource/NoLightingRendering.fx.h"
		};
		static const size_t shaderDataLen = LN_ARRAY_SIZE_OF(shaderData);
		auto shader = RefPtr<Shader>::MakeRef();
		shader->Initialize(this, (const char*)shaderData, shaderDataLen);
		m_defaultShaders[(int)DefaultShader::NoLightingRendering] = shader;
	}
	{
		static const byte_t shaderData[] =
		{
#include "Resource/ForwardRendering.fx.h"
		};
		static const size_t shaderDataLen = LN_ARRAY_SIZE_OF(shaderData);
		auto shader = RefPtr<Shader>::MakeRef();
		shader->Initialize(this, (const char*)shaderData, shaderDataLen);
		m_defaultShaders[(int)DefaultShader::ForwardRendering] = shader;
	}
	


	if (m_renderingType == GraphicsRenderingType::Threaded)
	{
		// 描画スレッドを立ち上げる
		m_renderingThread = LN_NEW RenderingThread();
		m_renderingThread->Reset(m_graphicsDevice);
		m_renderingThread->Start();
	}

	if (g_GraphicsManagerInstance == nullptr)
	{
		g_GraphicsManagerInstance = this;
	}


	m_dymmyWhiteTexture = RefPtr<Texture2D>::MakeRef();
	m_dymmyWhiteTexture->Initialize(this, SizeI(32, 32), TextureFormat::R8G8B8A8, false, ResourceUsage::Static);
	m_dymmyWhiteTexture->Clear(Color32::White);
}


//------------------------------------------------------------------------------
void GraphicsManager::Finalize()
{
	if (m_renderingThread != nullptr)
	{
		m_renderingThread->Dispose();
		LN_SAFE_DELETE(m_renderingThread);
	}

	auto deleteList = m_resourceObjectList;
	for (auto* obj : deleteList)
	{
		obj->Finalize();
	}

	//LN_SAFE_RELEASE(m_renderingContext);
	m_internalContext.SafeRelease();
}

//------------------------------------------------------------------------------
GraphicsAPI GraphicsManager::GetGraphicsAPI() const
{
	return m_graphicsDevice->GetGraphicsAPI();
}

//------------------------------------------------------------------------------
//RenderingCommandList* GraphicsManager::GetPrimaryRenderingCommandList()
//{
//	return m_renderer->m_primaryCommandList;
//}

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
		for (auto* obj : m_resourceObjectList) {
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
			painter.Clear(Color32::White);
			m_dummyDeviceTexture->Unlock();
		}

		// 全オブジェクトに通知
		for (auto* obj : m_resourceObjectList) {
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
InternalContext* GraphicsManager::GetInternalContext() const
{
	return m_internalContext;
}

//------------------------------------------------------------------------------
Shader* GraphicsManager::GetDefaultShader(DefaultShader type) const
{
	return m_defaultShaders[(int)type];
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
//uint64_t GraphicsManager::CalcFontSettingHash(const FontData& fontData)
//{
//	uint32_t v[2];
//	v[0] = Hash::CalcHash(fontData.Family.c_str());
//
//	uint8_t* v2 = (uint8_t*)&v[1];
//	v2[0] = fontData.Size;
//	//v2[1] = fontData.EdgeSize;
//	v2[3] =
//		(((fontData.IsBold) ? 1 : 0)) |
//		(((fontData.IsItalic) ? 1 : 0) << 1) |
//		(((fontData.IsAntiAlias) ? 1 : 0) << 2);
//
//	return *((uint64_t*)&v);
//}


//------------------------------------------------------------------------------
RenderingCommandList* GraphicsManager::GetPrimaryRenderingCommandList()
{
	return m_renderer->m_primaryCommandList;
}

//------------------------------------------------------------------------------
//RawFont* GraphicsManager::FontData::CreateFontFromData(FontManager* m) const
//{
//	RawFont* font = LN_NEW FreeTypeFont(m);
//	font->SetName(Family);
//	font->SetSize(Size);
//	//font->SetEdgeSize(EdgeSize);
//	font->SetBold(IsBold);
//	font->SetItalic(IsItalic);
//	font->SetAntiAlias(IsAntiAlias);
//	return font;
//}


//==============================================================================
// ShaderVariableCommitSerializeHelper
//==============================================================================
//------------------------------------------------------------------------------
ShaderVariableCommitSerializeHelper::ShaderVariableCommitSerializeHelper()
{
	m_writerBuffer = MemoryStream::Create();
	m_writer = RefPtr<BinaryWriter>::MakeRef(m_writerBuffer);
}

//------------------------------------------------------------------------------
void ShaderVariableCommitSerializeHelper::BeginSerialize()
{
	m_writer->Seek(0, SeekOrigin_Begin);
}

//------------------------------------------------------------------------------
void ShaderVariableCommitSerializeHelper::WriteValue(Driver::IShaderVariable* targetVariable, const ShaderValue& value)
{
	m_writer->WriteInt8(value.GetType());
	m_writer->WriteUInt64((intptr_t)targetVariable);

	switch (value.GetType())
	{
	case ShaderVariableType_Bool:
		m_writer->WriteUInt8(value.GetBool() ? 1 : 0);
		break;
	case ShaderVariableType_BoolArray:
		m_writer->WriteUInt8(value.GetArrayLength());
		m_writer->Write(value.GetBoolArray(), sizeof(bool) * value.GetArrayLength());
		break;
	case ShaderVariableType_Int:
		m_writer->WriteInt32(value.GetInt());
		break;
	case ShaderVariableType_Float:
		m_writer->WriteFloat(value.GetFloat());
		break;
	case ShaderVariableType_FloatArray:
		m_writer->WriteUInt8(value.GetArrayLength());
		m_writer->Write(value.GetFloatArray(), sizeof(float) * value.GetArrayLength());
		break;
	case ShaderVariableType_Vector:
		m_writer->Write(&value.GetVector(), sizeof(Vector4));
		break;
	case ShaderVariableType_VectorArray:
		m_writer->WriteUInt8(value.GetArrayLength());
		m_writer->Write(value.GetVectorArray(), sizeof(Vector4) * value.GetArrayLength());
		break;
	case ShaderVariableType_Matrix:
		m_writer->Write(&value.GetMatrix(), sizeof(Matrix));
		break;
	case ShaderVariableType_MatrixArray:
		m_writer->WriteUInt8(value.GetArrayLength());
		m_writer->Write(value.GetMatrixArray(), sizeof(Matrix) * value.GetArrayLength());
		break;
	case ShaderVariableType_DeviceTexture:
		m_writer->WriteUInt64((intptr_t)value.GetDeviceTexture());
		break;
	case ShaderVariableType_ManagedTexture:
		m_writer->WriteUInt64((intptr_t)((value.GetManagedTexture()) ? value.GetManagedTexture()->ResolveDeviceObject() : nullptr));
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
	return (size_t)m_writerBuffer->GetPosition();
}

//------------------------------------------------------------------------------
void ShaderVariableCommitSerializeHelper::Deserialize(const void* data, size_t length)
{
	const byte_t* raw = (const byte_t*)data;
	MemoryStream buffer(data, length);
	BinaryReader reader(&buffer);

	while (!reader.IsEOF())
	{
		uint8_t type = reader.ReadInt8();
		Driver::IShaderVariable* variable = (Driver::IShaderVariable*)reader.ReadUInt64();

		switch (type)
		{
			case ShaderVariableType_Bool:
			{
				variable->SetBool(reader.ReadUInt8() != 0);
				break;
			}
			case ShaderVariableType_BoolArray:
			{
				size_t size = reader.ReadUInt8();
				variable->SetBoolArray((const bool*)&raw[buffer.GetPosition()], size);
				reader.Seek(sizeof(bool) * size);
				break;
			}
			case ShaderVariableType_Int:
			{
				variable->SetInt(reader.ReadInt32());
				break;
			}
			case ShaderVariableType_Float:
			{
				variable->SetFloat(reader.ReadFloat());
				break;
			}
			case ShaderVariableType_FloatArray:
			{
				size_t size = reader.ReadUInt8();
				variable->SetFloatArray((const float*)&raw[buffer.GetPosition()], size);
				reader.Seek(sizeof(float) * size);
				break;
			}
			case ShaderVariableType_Vector:
			{
				Vector4 v;
				reader.Read(&v, sizeof(Vector4));
				variable->SetVector(v);
				break;
			}
			case ShaderVariableType_VectorArray:
			{
				size_t size = reader.ReadUInt8();
				variable->SetVectorArray((const Vector4*)&raw[buffer.GetPosition()], size);
				reader.Seek(sizeof(Vector4) * size);
				break;
			}
			case ShaderVariableType_Matrix:
			{
				Matrix v;
				reader.Read(&v, sizeof(Matrix));
				variable->SetMatrix(v);
				break;
			}
			case ShaderVariableType_MatrixArray:
			{
				size_t size = reader.ReadUInt8();
				variable->SetMatrixArray((const Matrix*)&raw[buffer.GetPosition()], size);
				reader.Seek(sizeof(Matrix) * size);
				break;
			}
			case ShaderVariableType_DeviceTexture:
			case ShaderVariableType_ManagedTexture:
				variable->SetTexture((Driver::ITexture*)reader.ReadUInt64());
				break;
			default:
				LN_THROW(0, InvalidOperationException);
				break;
		}
	}
}

} // namespace detail 
LN_NAMESPACE_END
