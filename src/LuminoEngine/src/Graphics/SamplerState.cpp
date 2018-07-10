/*
	DirectX 等のグラフィックスAPI レベルでは、Filter は min/Mag/Mip の3種、
	Wrap は U、V 方向の2種設定できる。
	しかし、実際のケースとしてそこまで細かく設定する必要があるのか？という考えや
	Unity 等のゲームエンジンでは細かくしていないことから、実装をシンプルにするために
	項目を減らしている。

	DirectX と OpenGL の対応表
	http://dench.flatlib.jp/opengl/glsl_hlsl
*/
#include "Internal.hpp"
#include <Lumino/Graphics/SamplerState.hpp>

namespace ln {

//------------------------------------------------------------------------------
SamplerState::SamplerState()
	: FilterMode(TextureFilterMode_Point)
	, WrapMode(TextureWrapMode_Repeat)
{}

} // namespace ln
