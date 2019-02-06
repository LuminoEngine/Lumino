
#pragma once
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Shader/Common.hpp>
#include "../Grammar/Token.hpp"

namespace ln {
class Token;

namespace detail {
class ShaderManager;
class GraphicsManager;
class ShaderRenderState;

enum class ShaderCodeStage
{
    Vertex,
    Fragment,
};

enum ShaderUniformType
{
	ShaderUniformType_Unknown = 0,
	ShaderUniformType_Bool = 1,
	//ShaderUniformType_BoolArray = 2,
	ShaderUniformType_Int = 3,
	//ShaderUniformType_IntArray = 4,
	ShaderUniformType_Float = 5,
	//ShaderUniformType_FloatArray = 6,
	ShaderUniformType_Vector = 7,
	//ShaderUniformType_VectorArray = 8,
	ShaderUniformType_Matrix = 9,
	//ShaderUniformType_MatrixArray = 10,
	ShaderUniformType_Texture = 11,
};

struct ShaderUniformInfo
{
	std::string name;
	uint16_t type;		// ShaderUniformType
	//uint16_t ownerBufferIndex;

	uint16_t offset;		// UniformBuffer 先頭からのオフセットバイト数
	//uint16_t size;		// Uniform 1つ分の全体サイズ (配列、行列の分も含む)

	uint16_t vectorElements;
	uint16_t arrayElements;	// 配列要素数
	uint16_t matrixRows;
	uint16_t matrixColumns;

	//uint16_t arrayStride;		// 配列の場合、要素のバイトサイズ (float2=8, float3=12)
	//uint16_t matrixStride;	// 行列の "横" 方向のバイト数。実際のメモリ上に値を並べる時の行間を表す。(float4x4=16)
};

struct ShaderUniformBufferInfo
{
	std::string name;
	uint32_t size;		// UniformBuffer 全体のサイズ
	std::vector<ShaderUniformInfo> members;

	void mergeFrom(const ShaderUniformBufferInfo& other);

	static void mergeBuffers(
		const std::vector<ShaderUniformBufferInfo>& a,
		const std::vector<ShaderUniformBufferInfo>& b,
		std::vector<ShaderUniformBufferInfo>* out);
};

// シェーダコード１つ分。
// HLSL 入力可能だが、technique 構文はあらかじめ取り除いておく必要がある。
class ShaderCodeTranspiler
{
public:

    static void initializeGlobals();
    static void finalizeGlobals();

    ShaderCodeTranspiler(ShaderManager* manager);

    bool parseAndGenerateSpirv(
        ShaderCodeStage stage,
        const char* code,
        size_t length,
        const std::string& entryPoint,
        const List<Path>& includeDir,
        const List<String>* definitions,
        DiagnosticsManager* diag);

	const std::vector<VertexInputAttribute>& attributes() const { return m_attributes; }
	const std::vector<ShaderUniformBufferInfo>& uniformBuffers() const { return m_uniformBuffers; }
	std::vector<byte_t> spirvCode() const;
    std::vector<byte_t> generateGlsl(uint32_t version, bool es);

private:
    ShaderManager* m_manager;
    ShaderCodeStage m_stage;
	std::vector<VertexInputAttribute> m_attributes;
	std::vector<ShaderUniformBufferInfo> m_uniformBuffers;
    std::vector<uint32_t> m_spirvCode;

};

} // namespace detail
} // namespace ln

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
