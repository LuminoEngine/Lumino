
#pragma once
#include <Lumino/IO/PathName.h>
#include "Audio/Common.h"
#include "Graphics/Common.h"
#include "Input/InputBinding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		アプリケーションの初期化設定です。
*/
class EngineSettings
{
public:
	struct ArchiveFileEntry
	{
		PathName	filePath;
		String		password;
	};

	struct CacheCapacity
	{
		int			objectCount = 32;	/**< キャッシュに保持できる最大オブジェクト数 */
		size_t		memorySize = 0;		/**< キャッシュに保持できる最大メモリ量 (byte単位。0 の場合はメモリ量を考慮しない) */
	};

	/** エンジンのアクセラレータキー */
	struct EngineAcceleratorKeys
	{
		InputBindingPtr	toggleShowDiag;
	};


public:

	/** メインウィンドウのクライアント領域の幅と高さです。(初期値:640x480) */
	Size mainWindowSize = Size(640, 480);

	/** メインウィンドウに対して作成されるバックバッファのサイズです。(初期値:640x480) */
	Size mainBackBufferSize = Size(640, 480);

	/** メインウィンドウのタイトル文字列です。*/
	String mainWindowTitle = _T("");
	
	/** デバッグ用のログファイルの出力有無を設定します。(初期値:Debug ビルドの場合true、それ以外は false) */
	bool applicationLogEnabled = false;

	/**
		@brief		標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(初期値:false)
	*/
	bool ConsoleEnabled;
	// TODO: いらない。Core の Console クラスを利用する。

	/**
		@brief		登録するアーカイブファイルのリストです。
	*/
	Array<ArchiveFileEntry>	ArchiveFileEntryList;
	
	/**
		@brief		ファイルを開く時の検索場所の優先順です。
	*/
	FileAccessPriority	FileAccessPriority;

	/**
		@brief		グラフィックス機能で使用する描画 API を指定します。
		@details	初期値は Windows の場合 DirectX9 で、それ以外は OpenGL です。
	*/
	GraphicsAPI		GraphicsAPI;

	/**
		@brief		グラフィックス機能で使用するレンダリング方法です。(初期値:Deferred)
	*/
	RenderingType	RenderingType;

	/** ユーザー定義のウィンドウハンドル (windows の場合は HWND、X11 は Window*。ただし、X11 は未対応) */
	void*	UserMainWindow;

	/**
		@brief		Direct3D の浮動小数点計算の精度に関する情報です。詳しくは MSDN の D3DCREATE_ENABLE_PRESENTSTATS を参照してください。
	*/
	bool	fpuPreserveEnabled = false;

#ifdef LN_OS_WIN32
	/** 既に作成済みの IDirect3DDevice9 インターフェイスを利用する場合、そのポインタを指定します。*/
	void*	D3D9Device;
#endif
	
	/**
		@brief		音声データのキャッシュ容量です。
	*/
	CacheCapacity	soundCacheCapacity;
	
	/**
		@brief		DirectMusic の初期化方法の指定です。
	*/
	DirectMusicMode	directMusicMode = DirectMusicMode::NotUse;

	/**
		@brief		DirectMusic のリバーブエフェクトの強さです。(規定値:0.75f)
	*/
	float	DirectMusicReverbLevel;

	/** エンジンのアクセラレータキー */
	EngineAcceleratorKeys	acceleratorKeys;

	/**
		@brief	内部的に COM の初期化を行います。
	*/
	bool autoCoInitialize = true;

public:
	EngineSettings()
		: ConsoleEnabled(false)
		, ArchiveFileEntryList()
		, FileAccessPriority(FileAccessPriority_DirectoryFirst)
		, GraphicsAPI(GraphicsAPI::DirectX9)
		, RenderingType(RenderingType::Deferred)
		, UserMainWindow(NULL)
#ifdef LN_OS_WIN32
		, D3D9Device(NULL)
#endif
		, DirectMusicReverbLevel(0.75f)
	{
#ifdef LN_DEBUG
		applicationLogEnabled = true;
		acceleratorKeys.toggleShowDiag = InputBinding::Create("ToggleShowDiag", Key::F3);
#endif
		//engineAcceleratorKeys[(int)EngineAcceleratorKey::ToggleShowDiag] = Key::F3;
	}
};

LN_NAMESPACE_END
