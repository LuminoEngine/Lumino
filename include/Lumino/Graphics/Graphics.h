#pragma once
#include "Common.h"

LN_NAMESPACE_BEGIN

/**
	@brief	レンダリングに関する機能を提供します。
*/
class Graphics
{
public:

	/**
		@brief		レンダリングを開始します。
		@details	Application::UpdateFrame() を使用する場合、この関数を呼び出す必要はありません。
	*/
	//void BeginRendering();
	
	/**
		@brief		レンダリングを終了します。
	*/
	//void EndRendering();

	static void ChangeDirectX9Device(void* id3d9device);
};

LN_NAMESPACE_END
