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
	DeviceTexture,
	ManagedTexture,
	String,

	/*
	*	Texture は、DirectX9HLSL では texture 型を表し、GLSL では sampler 型を表す。
	*	GLSL では sampler しか無いが、DirectX9HLSL では texture と sampler の2種類が存在する。
	*/
};

namespace detail {

struct ShaderUniformTypeDesc
{
	ShaderVariableType type;
	int rows;
	int columns;
	int elements;
};

} // namespace detail

} // namespace ln
