
#include "Internal.hpp"
#include <LuminoGraphics/Texture.hpp>
#include <LuminoGraphics/ShaderParameterValue.hpp>

namespace ln {
namespace detail {
	
//=============================================================================
// ShaderParameterValue

ShaderParameterValue::ShaderParameterValue()
    : m_type(ShaderVariableType::Unknown)
    , m_buffer()
{
}

ShaderParameterValue::~ShaderParameterValue()
{
	if (m_type == ShaderVariableType::Texture) {
		if (m_data.texture) {
			RefObjectHelper::release(m_data.texture);
		}
	}
}

void ShaderParameterValue::reset(ShaderVariableType type, int elements)
{
    switch (type) {
        case ln::ShaderVariableType::Bool:
            setBool(false);
            break;
        case ln::ShaderVariableType::BoolArray:
            setBoolArray(nullptr, elements);
            break;
        case ln::ShaderVariableType::Int:
            setInt(0);
            break;
        case ln::ShaderVariableType::Float:
            setFloat(0);
            break;
        case ln::ShaderVariableType::FloatArray:
            setFloatArray(nullptr, elements);
            break;
        case ln::ShaderVariableType::Vector:
            setVector(Vector4());
            break;
        case ln::ShaderVariableType::VectorArray:
            setVectorArray(nullptr, elements);
            break;
        case ln::ShaderVariableType::Matrix:
            setMatrix(Matrix());
            break;
        case ln::ShaderVariableType::MatrixArray:
            setMatrixArray(nullptr, elements);
            break;
        case ln::ShaderVariableType::Texture:
            setTexture(nullptr);
            break;
        case ln::ShaderVariableType::Pointer:
            setPointer(nullptr);
            break;
        default:
            LN_UNREACHABLE();
            break;
    }
}

void ShaderParameterValue::setBool(bool value)
{
    m_type = ShaderVariableType::Bool;
    m_data.Bool = value;
    dirty();
}

void ShaderParameterValue::setInt(int value)
{
    m_type = ShaderVariableType::Int;
    m_data.Int = value;
    dirty();
}

void ShaderParameterValue::setBoolArray(const bool* values, int count)
{
    m_type = ShaderVariableType::BoolArray;
    allocValueBuffer(sizeof(bool) * count);
    if (values) {
        memcpy(m_buffer.data(), values, sizeof(bool) * count);
    }
    dirty();
}

void ShaderParameterValue::setIntArray(const int* values, int count)
{
    m_type = ShaderVariableType::BoolArray;
    allocValueBuffer(sizeof(int) * count);
    if (values) {
        memcpy(m_buffer.data(), values, sizeof(int) * count);
    }
    dirty();
}

void ShaderParameterValue::setFloat(float value)
{
    m_type = ShaderVariableType::Float;
    m_data.Float = value;
    dirty();
}

void ShaderParameterValue::setFloatArray(const float* values, int count)
{
    m_type = ShaderVariableType::FloatArray;
    allocValueBuffer(sizeof(float) * count);
    if (values) {
        memcpy(m_buffer.data(), values, sizeof(float) * count);
    }
    dirty();
}

void ShaderParameterValue::setVector(const Vector4& vec)
{
    m_type = ShaderVariableType::Vector;
    *reinterpret_cast<Vector4*>(m_data.Vector4) = vec;
    dirty();
}

void ShaderParameterValue::setVectorArray(const Vector4* vectors, int count)
{
    m_type = ShaderVariableType::VectorArray;
    allocValueBuffer(sizeof(Vector4) * count);
    if (vectors) {
        memcpy(m_buffer.data(), vectors, sizeof(Vector4) * count);
    }
    dirty();
}

void ShaderParameterValue::setMatrix(const Matrix& matrix)
{
    m_type = ShaderVariableType::Matrix;
    allocValueBuffer(sizeof(Matrix));
    *reinterpret_cast<Matrix*>(m_data.Matrix4x4) = matrix;
    dirty();
}

void ShaderParameterValue::setMatrixArray(const Matrix* matrices, int count)
{
    m_type = ShaderVariableType::MatrixArray;
    allocValueBuffer(sizeof(Matrix) * count);
    if (matrices) {
        memcpy(m_buffer.data(), matrices, sizeof(Matrix) * count);
        dirty();
    }
}

void ShaderParameterValue::setTexture(Texture* texture)
{
    if (m_type != ShaderVariableType::Texture) {
        m_type = ShaderVariableType::Texture;
        m_data.texture = texture;
        if (m_data.texture) {
            RefObjectHelper::retain(m_data.texture);
        }
    } else {
        if (m_data.texture) {
            RefObjectHelper::release(m_data.texture);
        }
        m_data.texture = texture;
        if (m_data.texture) {
            RefObjectHelper::retain(m_data.texture);
        }
    }

    dirty();
}

void ShaderParameterValue::setPointer(void* value)
{
    m_type = ShaderVariableType::Pointer;
    m_data.Pointer = value;
    dirty();
}

int ShaderParameterValue::getDataByteSize() const
{
    switch (m_type) {
        case ln::ShaderVariableType::Bool:
            return sizeof(bool);
        case ln::ShaderVariableType::BoolArray:
            return m_buffer.size();
        case ln::ShaderVariableType::Int:
            return sizeof(int);
        case ln::ShaderVariableType::Float:
            return sizeof(float);
        case ln::ShaderVariableType::FloatArray:
            return m_buffer.size();
        case ln::ShaderVariableType::Vector:
            return sizeof(Vector4);
        case ln::ShaderVariableType::VectorArray:
            return m_buffer.size();
        case ln::ShaderVariableType::Matrix:
            return sizeof(Matrix);
        case ln::ShaderVariableType::MatrixArray:
            return m_buffer.size();
        case ln::ShaderVariableType::Texture:
            return sizeof(Texture*);
        case ln::ShaderVariableType::Pointer:
            return sizeof(void*);
        default:
            LN_UNREACHABLE();
            return 0;
    }
}

int ShaderParameterValue::getArrayLength() const
{
    if (m_type == ShaderVariableType::BoolArray) {
        return m_buffer.size() / sizeof(bool);
    }
    if (m_type == ShaderVariableType::FloatArray) {
        return m_buffer.size() / sizeof(float);
    }
    if (m_type == ShaderVariableType::VectorArray) {
        return m_buffer.size() / sizeof(Vector4);
    }
    if (m_type == ShaderVariableType::MatrixArray) {
        return m_buffer.size() / sizeof(Matrix);
    }
    return 0;
}

void ShaderParameterValue::dirty()
{
}

void ShaderParameterValue::allocValueBuffer(size_t size)
{
    if (size > m_buffer.size()) {
        m_buffer.resize(size);
    }
}

bool ShaderParameterValue::isShortSizeType(ShaderVariableType type)
{
    return type == ShaderVariableType::Bool ||
           type == ShaderVariableType::Int ||
           type == ShaderVariableType::Float ||
           type == ShaderVariableType::Vector ||
           type == ShaderVariableType::Matrix;
}

//=============================================================================
// ShaderParameterValue

size_t ShaderValueSerializer::measureBufferSize(const ShaderPass* pass)
{
    LN_NOTIMPLEMENTED();
    return 0;

    //size_t size = 0;
    //for (auto& param : pass->m_parameters)
    //{
    //	size += sizeof(uint8_t);	// type
    //	size += sizeof(uint32_t);	// data size
    //	size += param->m_value.getDataByteSize();
    //}
    //return size;
}

ShaderValueSerializer::ShaderValueSerializer(void* buffer, size_t size)
    : m_stream(buffer, size)
    , m_writer(&m_stream)
{
}

//void ShaderValueSerializer::beginWriteValues(void* buffer)
//{
//}

void ShaderValueSerializer::writeValue(const ShaderParameterValue& value)
{
    size_t byteSize = value.getDataByteSize();

    m_writer.writeUInt8(static_cast<uint8_t>(value.type()));
    m_writer.writeUInt32(static_cast<uint32_t>(byteSize));

    switch (value.type()) {
    case ShaderVariableType::Bool:
        m_writer.writeUInt8(value.getBool() ? 1 : 0);
        break;
    case ShaderVariableType::BoolArray:
        m_writer.write(value.getBoolArray(), byteSize);
        break;
    case ShaderVariableType::Int:
        m_writer.writeInt32(value.getInt());
        break;
    case ShaderVariableType::Float:
        m_writer.writeFloat(value.getFloat());
        break;
    case ShaderVariableType::FloatArray:
        m_writer.write(value.getFloatArray(), byteSize);
        break;
    case ShaderVariableType::Vector:
        m_writer.write(&value.getVector(), sizeof(Vector4));
        break;
    case ShaderVariableType::VectorArray:
        m_writer.write(value.getVectorArray(), byteSize);
        break;
    case ShaderVariableType::Matrix:
        m_writer.write(&value.getMatrix(), sizeof(Matrix));
        break;
    case ShaderVariableType::MatrixArray:
        m_writer.write(value.getMatrixArray(), byteSize);
        break;
    case ShaderVariableType::Texture:
        m_writer.writeUInt64((intptr_t)value.getTexture());
        break;
    case ShaderVariableType::Pointer:
        m_writer.writeUInt64((intptr_t)value.getPointer());
        break;
    default:
        LN_UNREACHABLE();
        break;
    }
}

//void ShaderValueSerializer::endWriteValues()
//{
//}


//=============================================================================
// ShaderValueDeserializer

ShaderValueDeserializer::ShaderValueDeserializer(const void* buffer, size_t size)
    : m_stream(buffer, size, false)
    , m_reader(&m_stream)
{
}

//void ShaderValueDeserializer::beginReadValues(const void* buffer)
//{
//}

const void* ShaderValueDeserializer::readValue(size_t* outSize, ShaderVariableType* outType)
{
    if (m_stream.position() >= m_stream.length()) {
        return nullptr; // EOF
    }

    ShaderVariableType type = static_cast<ShaderVariableType>(m_reader.readUInt8());
    size_t dataSize = m_reader.readUInt32();

    const void* data = m_stream.head();
    m_reader.seek(dataSize);

    *outType = type;
    *outSize = dataSize;
    return data;

    //switch (type)
    //{
    //	case ShaderVariableType::Bool:
    //	{
    //		variable->setBool(m_reader.readUInt8() != 0);
    //		break;
    //	}
    //	case ShaderVariableType::BoolArray:
    //	{
    //		size_t size = m_reader.readUInt8();
    //		variable->setBoolArray((const bool*)&raw[buffer.getPosition()], size);
    //		m_reader.seek(sizeof(bool) * size);
    //		break;
    //	}
    //	case ShaderVariableType::Int:
    //	{
    //		variable->setInt(m_reader.readInt32());
    //		break;
    //	}
    //	case ShaderVariableType::Float:
    //	{
    //		variable->setFloat(m_reader.readFloat());
    //		break;
    //	}
    //	case ShaderVariableType::FloatArray:
    //	{
    //		size_t size = m_reader.readUInt8();
    //		variable->setFloatArray((const float*)&raw[buffer.getPosition()], size);
    //		m_reader.seek(sizeof(float) * size);
    //		break;
    //	}
    //	case ShaderVariableType::Vector:
    //	{
    //		Vector4 v;
    //		m_reader.read(&v, sizeof(Vector4));
    //		variable->setVector(v);
    //		break;
    //	}
    //	case ShaderVariableType::VectorArray:
    //	{
    //		size_t size = m_reader.readUInt8();
    //		variable->setVectorArray((const Vector4*)&raw[buffer.getPosition()], size);
    //		m_reader.seek(sizeof(Vector4) * size);
    //		break;
    //	}
    //	case ShaderVariableType::Matrix:
    //	{
    //		Matrix v;
    //		m_reader.read(&v, sizeof(Matrix));
    //		variable->setMatrix(v);
    //		break;
    //	}
    //	case ShaderVariableType::MatrixArray:
    //	{
    //		size_t size = m_reader.readUInt8();
    //		variable->setMatrixArray((const Matrix*)&raw[buffer.getPosition()], size);
    //		m_reader.seek(sizeof(Matrix) * size);
    //		break;
    //	}
    //	case ShaderVariableType::DeviceTexture:
    //	case ShaderVariableType::ManagedTexture:
    //		variable->setTexture((Driver::RHIResource*)m_reader.readUInt64());
    //		break;
    //	default:
    //		LN_UNREACHABLE();
    //		break;
    //}
}

//void ShaderValueDeserializer::endReadValues()
//{
//}


} // namespace detail
} // namespace ln
