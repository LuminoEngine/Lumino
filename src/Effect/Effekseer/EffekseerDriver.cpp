/*
	[2015/12/19] Effekseer の個別描画について
		機能としては用意されているが効率はあまりよくなさそう。

		DrawHandle() のたびにクリティカルセクションに入っているし、ハンドルからインスタンスを求めるのに set の検索が走る。
		(一括描画ならイテレートするだけ)
		この仕組みは Update() も同じ。

		また、個別に BeginRendering()、EndRedering() する必要があり、ここで全てのステートを保存する。
		(これはステートを自動保存しないようにすることはできるが)


	[2015/12/15] Effekseer メモ

	InstanceContainer
		InstanceContainer
	InstanceContainer
		InstanceContainer
		InstanceContainer

	ManagerImplemented
		Flip() で m_renderingDrawSets に DrawSet が入れられる。
		Draw() で m_renderingDrawSets に入っている DrawSet を描画する。
		↓
		Flip() と Draw() は並列実行できない。

	ManagerImplemented::Play()
		InstanceContainer を作って m_DrawSets に入れる。

	ManagerImplemented::Update()
		サンプルでは Play() と並列実行できることになっている。
		中身は m_renderingDrawSets に対して更新を行っている。

	まとめ
		・Play() と Update() は並列実行できる。
		・1フレームは {Flip()} → {Update()} → {Draw()} の順で実行しなければならない。
		・Play() は Update() または Draw() と並列で実行できる。Flip() とはできない。

	対策
		・Update() は更新スレッドを新しく立て、Draw() は描画スレッドから呼びたい。
		・Update() が遅れているからと言って、Draw() しないのは無し。

		前提
		・Lumino は、描画スレッドが遅れているなら Draw() は呼ばない。

		・2フレーム目以降で、
										[更新スレッド]		[描画スレッド]
			PreUpdateFrame Flip()	→	開始 ※1				↓
			UpdateFrame					・・・				↓
			PostUpdateFrame				・・・				↓
			PreDraw						終了					開始 ※2
			Draw						
			PostDraw				→						コマンド実行開始

			※1 まずは Draw() が終わるまで待つ。
			※2 まずは Update() が終わるまで待つ。

			↑もしこうすると、描画が遅れている時が問題になりそう。
				更新スレッドの開始は、開始時点でまだ Draw 未実行だったら実行されるまで待つ。
				または、今回のフレームでは更新を行わない。
				待つパターンでは、描画遅延がメインスレッドに響くのでやりたくない。
				スキップする場合、スキップしたフレームで遅延が解消したとき、
				次に Draw できるときは Update 未実施。つまりエフェクトが止まって見える。
				遅延中は何フレームか毎に遅延が発生するはずだから、
				遅延→解消→遅延→解消・・・と続くと永遠に止まって見える。

				対策できなくはないと思うけど、複雑すぎる。
				他の案でどうしても速度がでなければ改めて考える。

		・シンプルで安全に実装するなら Update を描画スレッドで行うのが無難。
			・メインスレッドで Manager::Render() したら描画スレッドにコマンド投げる。
			・同時に更新スレッドに開始を通知する。
			・コマンド実行は更新が終わるまで待つ。
			↓
			つまり、UpdateFrame の終了 ～ 描画コマンドの実行開始 までを並列化するという方法。
			あくまで描画の一環と考え、Draw() するべき時は必ずセットで更新開始する。

			更新スレッドに開始を通知するのは、現在描画遅延しているかを判別し、描画することになった直後がベスト。
			既に Draw は終わっているので待つことを考える必要はない。
			また、別モジュールの描画開始前同期処理と並列化できる。

*/
#include "../Internal.h"
#if defined(LN_OS_WIN32)
#include "../../Audio/XAudio2/XAudio2AudioDevice.h"
#include "../../Graphics/Device/DirectX9/DX9GraphicsDevice.h"
#endif
#include "../../Audio/AudioManager.h"
#include "../../Graphics/GraphicsManager.h"
#include "../EffectManager.h"
#include "EffekseerDriver.h"

LN_NAMESPACE_BEGIN
namespace detail
{

//==============================================================================
// EffekseerFileInterface
//==============================================================================

//------------------------------------------------------------------------------
EffekseerFileInterface::EffekseerFileInterface(FileManager* fileManager)
	: m_fileManager(fileManager)
{
}

//------------------------------------------------------------------------------
::Effekseer::FileReader* EffekseerFileInterface::OpenRead(const EFK_CHAR* path)
{
	return new EffekseerFileReader(m_fileManager->CreateFileStream((wchar_t*)path));
}

//------------------------------------------------------------------------------
::Effekseer::FileWriter* EffekseerFileInterface::OpenWrite(const EFK_CHAR* path)
{
	LN_THROW(0, NotImplementedException);
	return nullptr;
}


//==============================================================================
// EffekseerEffectEngine
//==============================================================================

//------------------------------------------------------------------------------
EffekseerEffectEngine::EffekseerEffectEngine()
	: m_efkManager(nullptr)
	, m_efkRenderer(nullptr)
	, m_efkSound(nullptr)
{
	EncodingConversionOptions options;
	options.NullTerminated = true;
	m_TCharToUTF16Converter.SetConversionOptions(options);
	m_TCharToUTF16Converter.SetSourceEncoding(Encoding::GetTCharEncoding());
	m_TCharToUTF16Converter.SetDestinationEncoding(Encoding::GetUTF16Encoding());
}

//------------------------------------------------------------------------------
EffekseerEffectEngine::~EffekseerEffectEngine()
{
}

//------------------------------------------------------------------------------
void EffekseerEffectEngine::Initialize(EffectManager* manager, int cacheObjectCount, size_t cacheMemorySize, int maxSpriteCount)
{
	EffectEngine::Initialize(manager, cacheObjectCount, cacheMemorySize);

	// ファイルIOインターフェイス
	m_fileInterface = LN_NEW EffekseerFileInterface(m_manager->GetFileManager());

#if defined(LN_OS_WIN32)
	auto* dx9Device = dynamic_cast<Driver::DX9GraphicsDevice*>(m_manager->GetGraphicsManager()->GetGraphicsDevice());
	if (dx9Device != nullptr)
	{
		// エフェクト管理用インスタンスの生成(自動フリップは行わない)
		m_efkManager = ::Effekseer::Manager::Create(2000, false);

		// 描画用インスタンスの生成
		m_efkRenderer = ::EffekseerRendererDX9::Renderer::Create(dx9Device->GetIDirect3DDevice9(), maxSpriteCount);

	}
	else
#endif
	{
		//auto* glDevice = dynamic_cast<Driver::DX9GraphicsDevice*>(m_manager->GetGraphicsManager()->GetGraphicsDevice());


		m_efkManager = ::Effekseer::Manager::Create(2000, false);
		m_efkRenderer = ::EffekseerRendererGL::Renderer::Create(maxSpriteCount);

		//LN_THROW(0, NotImplementedException);
	}

	// 描画用インスタンスから描画機能を設定
	m_efkManager->SetSpriteRenderer(m_efkRenderer->CreateSpriteRenderer());
	m_efkManager->SetRibbonRenderer(m_efkRenderer->CreateRibbonRenderer());
	m_efkManager->SetRingRenderer(m_efkRenderer->CreateRingRenderer());
	m_efkManager->SetTrackRenderer(m_efkRenderer->CreateTrackRenderer());
	m_efkManager->SetModelRenderer(m_efkRenderer->CreateModelRenderer());

	// 描画用インスタンスからテクスチャの読込機能を設定
	m_efkManager->SetTextureLoader(m_efkRenderer->CreateTextureLoader(m_fileInterface));
	m_efkManager->SetModelLoader(m_efkRenderer->CreateModelLoader(m_fileInterface));
	m_efkManager->SetEffectLoader(::Effekseer::Effect::CreateEffectLoader(m_fileInterface));

	// Audio
#if defined(LN_OS_WIN32)
	if (m_efkManager != nullptr)
	{
		auto* xa2Device = dynamic_cast<XAudio2AudioDevice*>(m_manager->GetAudioManager()->GetAudioDevice());
		if (xa2Device != nullptr)
		{
			// 音再生用インスタンスの生成
			m_efkSound = ::EffekseerSound::Sound::Create(xa2Device->GetXAudio2(), 16, 16);

			// 音再生用インスタンスから再生機能を指定
			m_efkManager->SetSoundPlayer(m_efkSound->CreateSoundPlayer());

			// 音再生用インスタンスからサウンドデータの読込機能を設定
			m_efkManager->SetSoundLoader(m_efkSound->CreateSoundLoader(m_fileInterface));
		}
		else {
			// サウンドドライバが無いときや NullDevice を使用している時
		}
	}
#endif
}

//------------------------------------------------------------------------------
void EffekseerEffectEngine::Finalize()
{
	EffectEngine::Finalize();

	// Effekseer のサンプルでは先にエフェクト管理用インスタンスを破棄する必要があるようだ
	if (m_efkManager != nullptr) {
		m_efkManager->Destroy();
		m_efkManager = NULL;
	}
	if (m_efkRenderer != nullptr) {
		m_efkRenderer->Destory();
		m_efkRenderer = NULL;
	}
	if (m_efkSound != nullptr) {
		m_efkSound->Destory();
		m_efkSound = NULL;
	}

	LN_SAFE_DELETE(m_fileInterface);
}

//------------------------------------------------------------------------------
void EffekseerEffectEngine::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	if (m_efkRenderer)
	{
		m_efkRenderer->SetCameraMatrix((const ::Effekseer::Matrix44&)view);
		m_efkRenderer->SetProjectionMatrix((const ::Effekseer::Matrix44&)proj);
	}
}

//------------------------------------------------------------------------------
// 非同期更新・描画の同期フェーズで呼ばれる
//------------------------------------------------------------------------------
void EffekseerEffectEngine::UpdateRenderContents()
{
	if (m_efkManager) {
		m_efkManager->Flip();
	}
}

//------------------------------------------------------------------------------
VisualEffect* EffekseerEffectEngine::CreateEffectCore(const PathName& filePath)
{
	CacheKey key(filePath);

	// キャッシュ検索
	RefPtr<EffekseerEffectCore> core(static_cast<EffekseerEffectCore*>(m_effectCoreCache->FindObjectAddRef(key)), false);
	if (core != nullptr) {
		core.SafeAddRef();
		return core;
	}

	// unicode へ
	ByteBuffer utf16 = m_TCharToUTF16Converter.Convert(filePath.c_str(), _tcslen(filePath.c_str()) * sizeof(TCHAR));

	// エフェクトの読込
	Effekseer::Effect* efkEffect = Effekseer::Effect::Create(
		m_efkManager,
		(const EFK_CHAR*)utf16.GetConstData());
	if (efkEffect == NULL) {
		return NULL;
	}

	// EffectCore 作成
	core.Attach(LN_NEW EffekseerEffectCore(this, efkEffect), false);

	// キャッシュに登録
	if (!key.IsNull()) {
		m_effectCoreCache->RegisterCacheObject(key, core);
	}

	core.SafeAddRef();
	return core;
}

//------------------------------------------------------------------------------
void EffekseerEffectEngine::SetViewProjectin(const Matrix& view, const Matrix& proj)
{
	m_efkRenderer->SetCameraMatrix((const ::Effekseer::Matrix44&)view);
	m_efkRenderer->SetProjectionMatrix((const ::Effekseer::Matrix44&)proj);

	// 投影行列を設定
	//	::Effekseer::Matrix44().PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)640 / (float)480, 1.0f, 50.0f));

	//// カメラ行列を設定
	//m_efkRenderer->SetCameraMatrix(
	//	::Effekseer::Matrix44().LookAtRH(::Effekseer::Vector3D(10.0f, 5.0f, 20.0f), ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f)));

}

//------------------------------------------------------------------------------
void EffekseerEffectEngine::UpdateFrame(float elapsedTime)
{
	m_efkManager->Flip();
	m_efkManager->Update();	// TODO: time
}

//------------------------------------------------------------------------------
void EffekseerEffectEngine::Render()
{
	// エフェクトの描画開始処理を行う。
	m_efkRenderer->BeginRendering();

	// エフェクトの描画を行う。
	m_efkManager->Draw();

	// エフェクトの描画終了処理を行う。
	m_efkRenderer->EndRendering();
}

//==============================================================================
// EffekseerEffectCore
//==============================================================================

//------------------------------------------------------------------------------
EffekseerEffectCore::EffekseerEffectCore(EffekseerEffectEngine* engine, ::Effekseer::Effect* efkEffeect)
	: m_effectEngine(engine)
	, m_efkEffect(efkEffeect)
{
}

//------------------------------------------------------------------------------
EffekseerEffectCore::~EffekseerEffectCore()
{
	ES_SAFE_RELEASE(m_efkEffect);
}

//------------------------------------------------------------------------------
VisualEffectInstance* EffekseerEffectCore::PlayNewInstance()
{
	auto handle = GetEffectEngine()->GetEffekseerManager()->Play(GetEffekseerEffect(), 0, 0, 0);
	return LN_NEW EffekseerEffectInstance(this, handle);
}

//==============================================================================
// EffekseerEffectCore
//==============================================================================

//------------------------------------------------------------------------------
EffekseerEffectInstance::EffekseerEffectInstance(EffekseerEffectCore* ownerCore, ::Effekseer::Handle handle)
	: m_ownerEffectCore(ownerCore)
	, m_currentHandle(handle)
{
}

//------------------------------------------------------------------------------
EffekseerEffectInstance::~EffekseerEffectInstance()
{
}

//------------------------------------------------------------------------------
//void EffekseerEffectInstance::Play(bool overlap)
//{
//	if (m_currentHandle != 0)
//	{
//		// 今の状態で更新した後、バックへ
//		::Effekseer::Matrix43 efkMat;
//		LNToEFKMatrix43(m_worldMatrix, &efkMat);
//		m_ownerEffectCore->GetEffectEngine()->GetEffekseerManager()->SetMatrix(m_currentHandle, efkMat);
//		m_drawHandleArray.Add(m_currentHandle);
//	}
//}

//------------------------------------------------------------------------------
void EffekseerEffectInstance::Stop()
{
	if (m_currentHandle != 0)
	{
		::Effekseer::Manager* efkManager = m_ownerEffectCore->GetEffectEngine()->GetEffekseerManager();
		efkManager->StopEffect(m_currentHandle);
		for (::Effekseer::Handle handle : m_drawHandleArray)
		{
			efkManager->StopEffect(handle);
		}
	}
}

//------------------------------------------------------------------------------
bool EffekseerEffectInstance::IsPlaying()
{
	if (m_currentHandle == 0) {
		return false;
	}
	return m_ownerEffectCore->GetEffectEngine()->GetEffekseerManager()->Exists(m_currentHandle);
}

//------------------------------------------------------------------------------
void EffekseerEffectInstance::SetWorldMatrix(const Matrix& matrix)
{
	m_worldMatrix = matrix;
}

//------------------------------------------------------------------------------
void EffekseerEffectInstance::UpdateFrame()
{
	::Effekseer::Manager* efkManager = m_ownerEffectCore->GetEffectEngine()->GetEffekseerManager();

	::Effekseer::Matrix43 efkMat;
	LNToEFKMatrix43(m_worldMatrix, &efkMat);
	efkManager->SetMatrix(m_currentHandle, efkMat);
}

//------------------------------------------------------------------------------
void EffekseerEffectInstance::Draw()
{
	//::Effekseer::Manager* efkManager = mEffectCore->getEffectEngine()->getEffekseerManager();
	//::EffekseerRenderer::Renderer* r = mEffectCore->getEffectEngine()->getEffekseerRenderer();

	//r->BeginRendering();
	//ln_foreach(::Effekseer::Handle handle, mDrawHandleArray) {
	//	efkManager->DrawHandle(handle);
	//}
	//r->EndRendering();

}

//------------------------------------------------------------------------------
void EffekseerEffectInstance::LNToEFKMatrix43(const Matrix& mat, ::Effekseer::Matrix43* efkMat)
{
	efkMat->Value[0][0] = mat.m[0][0];
	efkMat->Value[0][1] = mat.m[0][1];
	efkMat->Value[0][2] = mat.m[0][2];

	efkMat->Value[1][0] = mat.m[1][0];
	efkMat->Value[1][1] = mat.m[1][1];
	efkMat->Value[1][2] = mat.m[1][2];

	efkMat->Value[2][0] = mat.m[2][0];
	efkMat->Value[2][1] = mat.m[2][1];
	efkMat->Value[2][2] = mat.m[2][2];

	efkMat->Value[3][0] = mat.m[3][0];
	efkMat->Value[3][1] = mat.m[3][1];
	efkMat->Value[3][2] = mat.m[3][2];
}

} // namespace detail
LN_NAMESPACE_END

