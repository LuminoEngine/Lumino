
#pragma once
#include "Graphics/Common.h"

namespace Lumino
{

/**
	@brief		アプリケーションの初期化設定です。
*/
class ApplicationSettings
{
public:
	struct ArchiveFileEntry
	{
		PathName	FilePath;
		String		Password;
	};

public:

	/**
		@brief		デバッグ用のログファイルの出力有無を設定します。(初期値:Debugモードの場合true、それ以外は false)
	*/
	bool ApplicationLogEnabled;

	/**
		@brief		標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(初期値:false)
	*/
	bool ConsoleEnabled;

	/**
		@brief		登録するアーカイブファイルのリストです。
	*/
	Array<ArchiveFileEntry>	ArchiveFileEntryList;

	/**
		@brief		グラフィックス機能で使用する描画 API を指定します。
		@details	初期値は Windows の場合 DirectX9 で、それ以外は OpenGL です。
	*/
	GraphicsAPI	GraphicsAPI;

	/**
		@brief		グラフィックス機能で使用するレンダリング方法です。(初期値:Deferred)
	*/
	RenderingType	RenderingType;

	/** ユーザー定義のウィンドウハンドル (windows の場合は HWND、X11 は Window*。ただし、X11 は未対応) */
	void*	UserMainWindow;

#ifdef LN_OS_WIN32
	/** 既に作成済みの IDirect3DDevice9 インターフェイスを利用する場合、そのポインタを指定します。*/
	void*	D3D9Device;
#endif

public:
	ApplicationSettings()
		: ApplicationLogEnabled(false)
		, ConsoleEnabled(false)
		, GraphicsAPI(Lumino::GraphicsAPI::DirectX9)
		, RenderingType(Lumino::RenderingType::Deferred)
		, UserMainWindow(NULL)
#ifdef LN_OS_WIN32
		, D3D9Device(NULL)
#endif
	{
#ifdef LN_DEBUG
		ApplicationLogEnabled = true;
#endif
	}
};

} // namespace Lumino
