#pragma once
#include "../Graphics/Common.hpp"
#include "Shader.hpp"

namespace ln {
namespace detail {

	
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

} // namespace detail
} // namespace ln
