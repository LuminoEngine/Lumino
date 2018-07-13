
#pragma once

namespace ln {

enum class ShaderCodeStage
{
	Vertex,
	Fragment,
};

// シェーダコード１つ分。
// HLSL 入力可能だが、technique 構文はあらかじめ取り除いておく必要がある。
class ShaderCode
{
public:
	ShaderCode();

	bool parse(const char* code, size_t length);

private:
};

} // namespace ln
