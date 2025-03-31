// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include <LuminoEngine/Graphics/RenderState.hpp>
//#include "../Graphics/GraphicsResource.hpp"
//#include "../Graphics/ShaderInterfaceFramework.hpp"

namespace ln {
class Texture;
class Shader;
class ShaderTechnique;
class ShaderPass;
class DiagnosticsManager;


/** ShaderParameter を分類します。 */
enum class ShaderParameterClass
{
    /** 定数バッファに含まれるスカラー、ベクトル、行列 */
    Constant,

    /** texture 型の変数 */
    Texture,

    /** SamplerState 型の変数 */
    SamplerState,
};

namespace detail {
class ShaderManager;
} // namespace detail

namespace kokage {

// Rendering モジュールで、Pass が ShaderTechnique を検索するときに使うデータ
struct ShaderTechniqueClass
{
    bool defaultTechnique;  // "Default" とだけ書かれた tech は　SceneRederer のデフォルトよりも優先。主に DebugGrid など、ライトがないことなどでシェーディングモデルが切り替わっても同じ tech を使いたいケース用。
    //ShaderTechniqueClass_Ligiting ligiting = ShaderTechniqueClass_Ligiting::Forward;
    ShaderTechniqueClass_Phase phase = ShaderTechniqueClass_Phase::Forward;
    ShaderTechniqueClass_MeshProcess meshProcess = ShaderTechniqueClass_MeshProcess::StaticMesh;
    ShaderTechniqueClass_ShadingModel shadingModel = ShaderTechniqueClass_ShadingModel::Default;
    ShaderTechniqueClass_DrawMode drawMode = ShaderTechniqueClass_DrawMode::Primitive;
    ShaderTechniqueClass_Normal normalClass = ShaderTechniqueClass_Normal::Default;
    ShaderTechniqueClass_Roughness roughnessClass = ShaderTechniqueClass_Roughness::Default;

    static void parseTechniqueClassString(const String& str, ShaderTechniqueClass* outClassSet);
    static bool equals(const ShaderTechniqueClass& a, const ShaderTechniqueClass& b);
};




class ShaderRenderState
    : public RefObject
{
public:
    // RenderTargetBlendDesc
    Optional_deprecated<bool> blendEnable;
    Optional_deprecated<BlendFactor> sourceBlend;
    Optional_deprecated<BlendFactor> destinationBlend;
    Optional_deprecated<BlendOp> blendOp;
    Optional_deprecated<BlendFactor> sourceBlendAlpha;
    Optional_deprecated<BlendFactor> destinationBlendAlpha;
    Optional_deprecated<BlendOp> blendOpAlpha;

    // RasterizerStateDesc
    Optional_deprecated<FillMode> fillMode;
    Optional_deprecated<CullMode> cullMode;

    // DepthStencilStateDesc
    Optional_deprecated<ComparisonFunc> depthTestFunc;
    Optional_deprecated<bool> depthWriteEnabled;

    // StencilOpDesc
    Optional_deprecated<bool> stencilEnabled;
    Optional_deprecated<uint8_t> stencilReferenceValue;
    Optional_deprecated<StencilOp> stencilFailOp;
    Optional_deprecated<StencilOp> stencilDepthFailOp;
    Optional_deprecated<StencilOp> stencilPassOp;
    Optional_deprecated<ComparisonFunc> stencilFunc;
};

class ShaderHelper
{
public:
    static bool resolveStd140Layout(const ShaderUniformInfo& info, size_t* outAligndElemenSize);

    // for testing.
    static bool buildShader(const Path& inputFile, const Path& outputFile, const Path& exportDir);
    static bool generateShader(detail::ShaderManager* manager, const Path& inputFile, const Path& outputFile, const Path& exportDir, DiagnosticsManager* diag);


    // int or float
    static void alignScalarsToBuffer(
        const byte_t* source,
        size_t unitBytes,
        int unitCount,
        byte_t* buffer,
        size_t offset,
        int elements,
        size_t arrayStride) LN_NOEXCEPT
    {
        byte_t* head = buffer + offset;
        int loop = std::min(unitCount, elements);
        for (int i = 0; i < loop; i++) {
            memcpy(head + arrayStride * i, source + unitBytes * i, unitBytes);
        }
    }

    // vector
    static void alignVectorsToBuffer(
        const byte_t* source,
        int sourceColumns,
        int sourceElementCount,
        byte_t* buffer,
        size_t offset,
        int elements,
        size_t arrayStride,
        int columns) LN_NOEXCEPT
    {
        size_t srcVectorSize = sizeof(float) * sourceColumns;
        size_t copySize = std::min(srcVectorSize, sizeof(float) * columns);
        byte_t* head = buffer + offset;
        int loop = std::min(sourceElementCount, elements);
        for (int i = 0; i < loop; i++) {
            memcpy(head + arrayStride * i, source + srcVectorSize * i, copySize);
        }
    }

    // matrix
    static void alignMatricesToBuffer(
        const byte_t* source,
        int sourceColumns,
        int sourceRows,
        int sourceElementCount,
        byte_t* buffer,
        size_t offset,
        int elements,
        size_t matrixStride,
        size_t arrayStride,
        int rows,
        int columns,
        bool transpose) LN_NOEXCEPT
    {
        size_t srcRowSize = sizeof(float) * sourceColumns;
        size_t dstRowSize = matrixStride;
        size_t copySize = std::min(srcRowSize, dstRowSize);
        byte_t* head = buffer + offset;
        int loop = std::min(sourceElementCount, elements);
        int rowLoop = std::min(sourceRows, rows);
        for (int i = 0; i < loop; i++) {
            const byte_t* srcMatHead = source + (sourceColumns * sourceRows * sizeof(float)) * i;
            byte_t* dstMatHead = head + arrayStride * i;

            float tmp[16];
            if (transpose) {
                assert(sourceColumns == 4 && sourceRows == 4);
                *reinterpret_cast<Matrix*>(tmp) = Matrix::makeTranspose(*reinterpret_cast<const Matrix*>(srcMatHead));
                srcMatHead = reinterpret_cast<const byte_t*>(tmp);
            }

            for (int j = 0; j < rowLoop; j++) {
                memcpy(dstMatHead + matrixStride * j, srcMatHead + srcRowSize * j, copySize);
            }
        }
    }
};




} // namespace kokage
} // namespace ln
