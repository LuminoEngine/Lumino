
#pragma once

#include "ApplicationSettings.h"

LN_NAMESPACE_BEGIN

/**
	@brief		アプリケーション全体にかかわる処理を行います。
*/
class Engine
{
public:

	/**
		@brief		エンジンの初期化処理を行います。
		@params[in]	settings	: 初期化設定
	*/
	static void Initialize(const ApplicationSettings& settings);
	
	/**
		@brief		エンジンの終了処理を行います。
	*/
	static void Finalize();

	/**
		@brief		1フレーム分の更新処理を行います。
		@return		アプリケーションの終了が要求されている場合は false を返します。
		@details	この関数はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。
	*/
	static bool UpdateFrame();


	static bool BeginRendering();
	static void EndRendering();
	static void Render();
};

LN_NAMESPACE_END
