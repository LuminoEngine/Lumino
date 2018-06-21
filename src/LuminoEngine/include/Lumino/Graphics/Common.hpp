#pragma once

namespace ln {

enum class GraphicsAPI
{
	OpenGL,
};

enum class ClearFlags
{
	None	= 0x0000,
	Color	= 0x0001,					///< カラーバッファをクリアします。
	Depth	= 0x0002,					///< 深度バッファをクリアします。
	Stencil = 0x0004,					///< ステンシルバッファをクリアします。
	All		= Color | Depth | Stencil,	///< 全てのバッファをクリアします。
};
LN_FLAGS_OPERATORS(ClearFlags);

enum class ShaderCompilationResultLevel
{
	Success,
	Warning,
	Error,
};

struct ShaderCompilationDiag
{
	ShaderCompilationResultLevel level;
	std::string message;
};

enum class ShaderVariableType
{
	Unknown,
	Bool,
	BoolArray,
	Int,
	Float,
	FloatArray,
	Vector,
	VectorArray,
	Matrix,
	MatrixArray,
	Texture,
	Pointer,

	/*
	*	Texture は、DirectX9HLSL では texture 型を表し、GLSL では sampler 型を表す。
	*	GLSL では sampler しか無いが、DirectX9HLSL では texture と sampler の2種類が存在する。
	*/
};

namespace detail {

using ShaderRefrectionParameterType = ShaderVariableType;
//enum class ShaderRefrectionParameterType
//{
//	Unknown,
//	Bool,
//	BoolArray,
//	Int,
//	Float,
//	FloatArray,
//	Vector,
//	VectorArray,
//	Matrix,
//	MatrixArray,
//	Texture,
//};

struct ShaderUniformTypeDesc
{
	ShaderRefrectionParameterType type;
	int rows;
	int columns;
	int elements;

	static bool equals(const ShaderUniformTypeDesc& a, const ShaderUniformTypeDesc& b)
	{
		return
			a.type == b.type &&
			a.rows == b.rows &&
			a.columns == b.columns &&
			a.elements == b.elements;
	}
};

} // namespace detail

} // namespace ln
