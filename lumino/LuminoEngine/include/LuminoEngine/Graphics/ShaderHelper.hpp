// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "../Graphics/GraphicsResource.hpp"
#include "../Graphics/RenderState.hpp"
#include "../Graphics/ShaderInterfaceFramework.hpp"

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

class ShaderParameterValue
{
public:
    ShaderParameterValue();
	~ShaderParameterValue();


    void reset(ShaderVariableType type, int elements);

    ShaderVariableType type() const { return m_type; }

    void setBool(bool value);
    bool getBool() const { return m_data.Bool; }

    void setBoolArray(const bool* values, int count);
    const bool* getBoolArray() const { return reinterpret_cast<const bool*>(m_buffer.data()); }

    void setInt(int value);
    int getInt() const { return m_data.Int; }

    void setIntArray(const int* values, int count);
    const bool* getIntArray() const { return reinterpret_cast<const bool*>(m_buffer.data()); }

    void setFloat(float value);
    float getFloat() const { return m_data.Float; }

    void setFloatArray(const float* values, int count);
    const float* getFloatArray() const { return reinterpret_cast<const float*>(m_buffer.data()); }

    void setVector(const Vector4& vec);
    const Vector4& getVector() const { return *reinterpret_cast<const Vector4*>(m_data.Vector4); }

    void setVectorArray(const Vector4* vectors, int count);
    const Vector4* getVectorArray() const { return reinterpret_cast<const Vector4*>(m_buffer.data()); }

    void setMatrix(const Matrix& matrix);
    const Matrix& getMatrix() const { return *reinterpret_cast<const Matrix*>(m_data.Matrix4x4); }
    void setMatrixArray(const Matrix* matrices, int count);

    const Matrix* getMatrixArray() const { return reinterpret_cast<const Matrix*>(m_buffer.data()); }
    void setTexture(Texture* texture);
    Texture* getTexture() const { return m_data.texture; }

    void setPointer(void* value);
    void* getPointer() const { return m_data.Pointer; }

    int getDataByteSize() const;
    int getArrayLength() const;

private:
    LN_DISALLOW_COPY_AND_ASSIGN(ShaderParameterValue);

    void dirty();
    void allocValueBuffer(size_t size);
    static bool isShortSizeType(ShaderVariableType type);

    union Data
    {
        bool Bool;
        int Int;
        float Float;
        float Vector4[4];
        float Matrix4x4[16];
        Texture* texture;
        void* Pointer; // any object pointer (e.g Texture)
    };

    ShaderVariableType m_type;
    Data m_data;
    ByteBuffer m_buffer;
};

class ShaderValueSerializer
{
public:
    static size_t measureBufferSize(const ShaderPass* pass);

    ShaderValueSerializer(void* buffer, size_t size);

    //void beginWriteValues(void* buffer);
    void writeValue(const ShaderParameterValue& value);
    //void endWriteValues();

private:
    MemoryStream m_stream;
    BinaryWriter m_writer;
};

class ShaderValueDeserializer
{
public:
    ShaderValueDeserializer(const void* buffer, size_t size);
    const void* readValue(size_t* outSize, ShaderVariableType* outType); // return nullptr if eof
                                                                         //void endReadValues();

private:
    MemoryStream m_stream;
    BinaryReader m_reader;
};

class ShaderRenderState
    : public RefObject
{
public:
    // RenderTargetBlendDesc
    Optional<bool> blendEnable;
    Optional<BlendFactor> sourceBlend;
    Optional<BlendFactor> destinationBlend;
    Optional<BlendOp> blendOp;
    Optional<BlendFactor> sourceBlendAlpha;
    Optional<BlendFactor> destinationBlendAlpha;
    Optional<BlendOp> blendOpAlpha;

    // RasterizerStateDesc
    Optional<FillMode> fillMode;
    Optional<CullMode> cullMode;

    // DepthStencilStateDesc
    Optional<ComparisonFunc> depthTestFunc;
    Optional<bool> depthWriteEnabled;

    // StencilOpDesc
    Optional<bool> stencilEnabled;
    Optional<uint8_t> stencilReferenceValue;
    Optional<StencilOp> stencilFailOp;
    Optional<StencilOp> stencilDepthFailOp;
    Optional<StencilOp> stencilPassOp;
    Optional<ComparisonFunc> stencilFunc;
};

class ShaderHelper
{
public:
    static ShaderRenderState* getShaderRenderState(ShaderPass* pass);
    static ShaderTechnique* findTechniqueByClass(const Shader* shader, const detail::ShaderTechniqueClass& techniqueClass);
    static const detail::ShaderTechniqueClass& techniqueClass(ShaderTechnique* technique);
    static bool resolveStd140Layout(const ShaderUniformInfo& info, size_t* outAligndElemenSize);

    // for testing.
    static bool buildShader(const Path& inputFile, const Path& outputFile, const Path& exportDir);
    static bool generateShader(ShaderManager* manager, const Path& inputFile, const Path& outputFile, const Path& exportDir, DiagnosticsManager* diag);


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




} // namespace detail
} // namespace ln
