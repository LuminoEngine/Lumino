
#include "Internal.hpp"
#include <Lumino/Shader/Shader.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Graphics/RenderingCommandList.hpp"
#include "ShaderAnalyzer.hpp"


namespace ln {

//static void alignIntToBuffer(const int* values, int valuesCount, int elements, size_t offset, size_t stride, void* buffer)
//{
//	byte_t* head = static_cast<byte_t*>(buffer) + offset;
//	int loop = std::min(valuesCount, elements);
//	for (int i = 0; i < loop; i++) {
//		memcpy(head + stride * i, values + i, sizeof(int));
//	}
//
//	memcpy(static_cast<byte_t*>(buffer) + offset, &value, sizeof(float) * columns);
//}

//struct IntVector4
//{
//	int x, y, z, w;
//};
//
//static void alignIntVector4ToBuffer(const IntVector4& value, int columns, size_t offset, void* buffer)
//{
//	memcpy(static_cast<byte_t*>(buffer) + offset, &value, sizeof(int) * columns);
//}
//
//static void alignIntVector4ArrayToBuffer(const IntVector4* values, int valuesCount, int elements, size_t offset, size_t stride, void* buffer)
//{
//	int loop = std::min(valuesCount, elements);
//	for (int i = 0; i < loop; i++) {
//		alignIntVector4ToBuffer(values[i], stride / sizeof(int), offset + stride * i, buffer);
//	}
//}
//
//static void alignVector4ToBuffer(const Vector4& value, int columns, size_t offset, void* buffer)
//{
//	memcpy(static_cast<byte_t*>(buffer) + offset, &value, sizeof(float) * columns);
//}
//
//static void alignVector4ArrayToBuffer(const Vector4* values, int elements, size_t offset, size_t stride, void* buffer)
//{
//	for (int i = 0; i < elements; i++) {
//		alignVector4ToBuffer(values[i], stride / sizeof(float), offset + stride * i, buffer);
//	}
//}
//
//
//struct AlignmentSource
//{
//	const byte_t* data;
//	size_t unit;			// 1 要素のサイズ。sizeof
//	size_t columns;
//	size_t rows;			// 行列の列数。ベクトルやスカラの時は 1 にする。
//	size_t elements;		// 配列要素数。スカラの時は 1 にする。
//};
//
//struct AligmentTarget
//{
//	byte_t* buffer;
//	size_t bufferSize;
//	size_t unit;			// 1 要素のサイズ。sizeof
//};

// int or float
static void alignScalarsToBuffer(
	const byte_t* source, size_t unitBytes, int unitCount,
	byte_t* buffer, size_t offset, int elements, size_t arrayStride)
{
	int loop = std::min(unitCount, elements);
	byte_t* head = buffer + offset;
	for (int i = 0; i < elements; i++) {
		memcpy(head + arrayStride * i, source + unitBytes * i, unitBytes);
	}
}

// vector
static void alignVectorsToBuffer(
	const byte_t* source, int sourceColumns, int sourceElementCount,
	byte_t* buffer, size_t offset, int elements, size_t arrayStride, int columns)
{
	size_t sourceVectorSize = sizeof(float) * sourceColumns;
	size_t copySize = std::min(sourceVectorSize, sizeof(float) * columns);
	int loop = std::min(sourceElementCount, elements);
	byte_t* head = buffer + offset;
	for (int i = 0; i < elements; i++) {
		memcpy(head + arrayStride * i, source + sourceVectorSize * i, copySize);
	}
}

//=============================================================================
// Shader

Ref<Shader> Shader::create(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath)
{
	return ln::newObject<Shader>(vertexShaderFilePath, pixelShaderFilePath, ShaderCodeType::RawGLSL);
}

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::initialize()
{
	GraphicsResource::initialize();
}

void Shader::initialize(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCodeType codeType)
{
	Shader::initialize();
	m_diag = newObject<DiagnosticsManager>();

	auto vsData = FileSystem::readAllBytes(vertexShaderFilePath);
	auto psData = FileSystem::readAllBytes(pixelShaderFilePath);

	buildShader(
		reinterpret_cast<const char*>(vsData.data()), vsData.size(),
		reinterpret_cast<const char*>(psData.data()), psData.size());

	if (m_diag->hasItems())	{
		m_diag->dumpToLog();
	}
}

void Shader::dispose()
{
	for (auto& tech : m_techniques)
	{
		for (auto& pass : tech->passes())
		{
			pass->dispose();
		}
	}

	GraphicsResource::dispose();
}

void Shader::onChangeDevice(detail::IGraphicsDeviceContext* device)
{
	LN_NOTIMPLEMENTED();
}

void Shader::buildShader(const char* vsData, size_t vsLen, const char* psData, size_t psLen)
{
	detail::ShaderCode vsCodeGen;
	if (!vsCodeGen.parseAndGenerateSpirv(detail::ShaderCodeStage::Vertex, vsData, vsLen, "main", m_diag))
	{
		return;
	}

	detail::ShaderCode psCodeGen;
	if (!psCodeGen.parseAndGenerateSpirv(detail::ShaderCodeStage::Fragment, psData, psLen, "main", m_diag))
	{
		return;
	}

	std::string vsCode = vsCodeGen.generateGlsl();
	std::string psCode = psCodeGen.generateGlsl();

	ShaderCompilationDiag diag;
	auto rhiPass = deviceContext()->createShaderPass(
		reinterpret_cast<const byte_t*>(vsCode.c_str()), vsCode.length(),
		reinterpret_cast<const byte_t*>(psCode.c_str()), psCode.length(), &diag);

	auto tech = newObject<ShaderTechnique>();
	tech->setOwner(this);
	m_techniques.add(tech);

	auto pass = newObject<ShaderPass>(rhiPass);
	tech->addShaderPass(pass);
	pass->setupParameters();
}

#if 0
void Shader::setBool(const StringRef& name, bool value)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setBool(value);
}

void Shader::setInt(const StringRef& name, int value)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setInt(value);
}

void Shader::setBoolArray(const StringRef& name, const bool* value, int count)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setBoolArray(value, count);
}

void Shader::setFloat(const StringRef& name, float value)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setFloat(value);
}

void Shader::setFloatArray(const StringRef& name, const float* value, int count)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setFloatArray(value, count);
}

void Shader::setVector(const StringRef& name, const Vector4& value)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setVector(value);
}

void Shader::setVectorArray(const StringRef& name, const Vector4* value, int count)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setVectorArray(value, count);
}

void Shader::setMatrix(const StringRef& name, const Matrix& value)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setMatrix(value);
}

void Shader::setMatrixArray(const StringRef& name, const Matrix* value, int count)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setMatrixArray(value, count);
}

void Shader::setTexture(const StringRef& name, Texture* value)
{
	ShaderParameter* param = findParameter(name);
	if (param) param->setTexture(value);
}
#endif

//ShaderParameter* Shader::findParameter(const StringRef& name)
//{
//	for (auto& param : m_parameters)
//	{
//		if (param->name() == name)
//		{
//			return param;
//		}
//	}
//	return nullptr;
//}

ShaderConstantBuffer* Shader::findConstantBuffer(const StringRef& name)
{
	auto result = m_buffers.findIf([name](const ShaderConstantBuffer* buf) { return buf->name() == name; });
	return (result) ? *result : nullptr;
}

//ShaderParameter* Shader::getShaderParameter(const detail::ShaderUniformTypeDesc& desc, const String& name)
//{
//	for (auto& param : m_parameters)
//	{
//		if (detail::ShaderUniformTypeDesc::equals(param->desc(), desc) && param->name() == name)
//		{
//			return param;
//		}
//	}
//
//	auto param = newObject<ShaderParameter>(desc, name);
//	m_parameters.add(param);
//	return param;
//}

ShaderConstantBuffer* Shader::getOrCreateConstantBuffer(detail::IShaderUniformBuffer* rhiBuffer)
{
	for (auto& buffer : m_buffers)
	{
		if (buffer->getRhiObject()->name() == rhiBuffer->name())
		{
			return buffer;
		}
	}

	auto buffer = newObject<ShaderConstantBuffer>(this, rhiBuffer);
	m_buffers.add(buffer);
	return buffer;
}

//=============================================================================
// ShaderParameter

ShaderParameter::ShaderParameter()
{
}

ShaderParameter::~ShaderParameter()
{
}

void ShaderParameter::initialize(ShaderConstantBuffer* owner, const detail::ShaderUniformTypeDesc& desc, const String& name)
{
	Object::initialize();
	m_owner = owner;
	m_desc = desc;
	m_name = name;
	m_value.reset(desc.type, desc.elements);
}

void ShaderParameter::dispose()
{
	Object::dispose();
}


void ShaderParameter::setInt(int value)
{
	alignScalarsToBuffer((const byte_t*)&value, sizeof(int), 1, m_owner->buffer().data(), m_desc.offset, 1, 0);
}

void ShaderParameter::setIntArray(const int* value, int count)
{
	LN_NOTIMPLEMENTED();
}

void ShaderParameter::setFloat(float value)
{
	alignScalarsToBuffer((const byte_t*)&value, sizeof(float), 1, m_owner->buffer().data(), m_desc.offset, 1, 0);
}

void ShaderParameter::setFloatArray(const float* value, int count)
{
	alignScalarsToBuffer((const byte_t*)value, sizeof(float), count, m_owner->buffer().data(), m_desc.offset, m_desc.elements, m_desc.arrayStride);
}

void ShaderParameter::setVector(const Vector4& value)
{
	alignVectorsToBuffer((const byte_t*)&value, 4, 1, m_owner->buffer().data(), m_desc.offset, 1, 0, m_desc.columns);
}

void ShaderParameter::setVectorArray(const Vector4* value, int count)
{
	alignVectorsToBuffer((const byte_t*)value, 4, count, m_owner->buffer().data(), m_desc.offset, m_desc.elements, m_desc.arrayStride, m_desc.columns);
}

#if 0
void ShaderParameter::setBool(bool value)
{
	m_value.setBool(value);
}


void ShaderParameter::setBoolArray(const bool* value, int count)
{
	m_value.setBoolArray(value, count);
}


void ShaderParameter::setMatrix(const Matrix& value)
{
	m_value.setMatrix(value);
}

void ShaderParameter::setMatrixArray(const Matrix* value, int count)
{
	m_value.setMatrixArray(value, count);
}

void ShaderParameter::setTexture(Texture* value)
{
	m_value.setTexture(value);
}

void ShaderParameter::setPointer(void* value)
{
	m_value.setPointer(value);
}
#endif

//=============================================================================
// ShaderConstantBuffer

ShaderConstantBuffer::ShaderConstantBuffer()
	: m_rhiObject(nullptr)
	, m_owner(nullptr)
{
}

ShaderConstantBuffer::~ShaderConstantBuffer()
{
}

void ShaderConstantBuffer::initialize(Shader* owner, detail::IShaderUniformBuffer* rhiObject)
{
	Object::initialize();
	m_owner = owner;
	m_rhiObject = rhiObject;
	m_name = String::fromStdString(m_rhiObject->name());
	m_buffer.resize(m_rhiObject->bufferSize());

	for (int i = 0; i < m_rhiObject->getUniformCount(); i++)
	{
		detail::IShaderUniform* field = m_rhiObject->getUniform(i);
		m_parameters.add(newObject<ShaderParameter>(this, field->desc(), String::fromStdString(field->name())));
	}
}

void ShaderConstantBuffer::setData(const void* data, int size)
{
	m_buffer.assign(data, size);
}

ShaderParameter* ShaderConstantBuffer::findParameter(const StringRef& name) const
{
	auto result = m_parameters.findIf([name](const ShaderParameter* param) { return param->name() == name; });
	return (result) ? *result : nullptr;
}

void ShaderConstantBuffer::commit()
{
	auto* manager = owner()->manager();
	detail::RenderBulkData data = manager->primaryRenderingCommandList()->allocateBulkData(m_buffer.size());
	memcpy(data.writableData(), m_buffer.data(), data.size());

	LN_ENQUEUE_RENDER_COMMAND_2(
		ShaderConstantBuffer_commit, manager,
		detail::RenderBulkData, data,
		Ref<detail::IShaderUniformBuffer>, m_rhiObject,
		{
			m_rhiObject->setData(data.data(), data.size());
		});
}

//=============================================================================
// ShaderTechnique

ShaderTechnique::ShaderTechnique()
{
}

ShaderTechnique::~ShaderTechnique()
{
}

void ShaderTechnique::initialize()
{
	Object::initialize();
}

void ShaderTechnique::addShaderPass(ShaderPass* pass)
{
	m_passes.add(pass);
	pass->setOwner(this);
}

//=============================================================================
// ShaderPass

ShaderPass::ShaderPass()
	: m_rhiPass(nullptr)
{
}

ShaderPass::~ShaderPass()
{
}

void ShaderPass::initialize(detail::IShaderPass* rhiPass)
{
	if (LN_REQUIRE(rhiPass)) return;
	Object::initialize();

	m_rhiPass = rhiPass;
}

void ShaderPass::dispose()
{
	m_rhiPass = nullptr;

	Object::dispose();
}

void ShaderPass::setupParameters()
{
	//m_parameters.clear();

	//for (int i = 0; i < m_rhiPass->getUniformCount(); i++)
	//{
	//	detail::IShaderUniform* uni = m_rhiPass->getUniform(i);
	//	ShaderParameter* param = m_owner->owner()->getShaderParameter(uni->desc(), String::fromStdString(uni->name()));
	//	m_parameters.add(param);
	//}

	m_buffers.clear();

	for (int i = 0; i < m_rhiPass->getUniformBufferCount(); i++)
	{
		detail::IShaderUniformBuffer* rhi = m_rhiPass->getUniformBuffer(i);
		ShaderConstantBuffer* buf = m_owner->owner()->getOrCreateConstantBuffer(rhi);
		m_buffers.add(buf);
	}
}

//// TODO:
//static void tttt(const detail::RenderBulkData& data,
//	const Ref<detail::IShaderPass>& m_rhiPass)
//{
//	detail::ShaderValueDeserializer deserializer(data.data(), data.size());
//	for (int i = 0; i < m_rhiPass->getUniformCount(); i++)
//	{
//		size_t size = 0;
//		ShaderVariableType type = ShaderVariableType::Unknown;
//		const void* rawData = deserializer.readValue(&size, &type);
//		m_rhiPass->setUniformValue(i, rawData, size);
//	}
//
//}
//

void ShaderPass::commit()
{
	auto* manager = m_owner->owner()->manager();

#if 0
	detail::RenderBulkData data = manager->primaryRenderingCommandList()->allocateBulkData(detail::ShaderValueSerializer::measureBufferSize(this));

	detail::ShaderValueSerializer serializer(data.writableData(), data.size());
	
	for (auto& param : m_parameters) {
		serializer.writeValue(param->m_value);
	}

	LN_ENQUEUE_RENDER_COMMAND_2(
		ShaderPass_commit, manager,
		detail::RenderBulkData, data,
		Ref<detail::IShaderPass>, m_rhiPass,
		{
			tttt(data, m_rhiPass);
		});
#endif

	for (auto& buffer : m_buffers)
	{
		buffer->commit();
	}

	//LN_ENQUEUE_RENDER_COMMAND_1(
	//	ShaderPass_commit, manager,
	//	Ref<detail::IShaderPass>, m_rhiPass,
	//	{
	//		m_rhiPass->a
	//	});
}

detail::IShaderPass* ShaderPass::resolveRHIObject()
{
	commit();
	return m_rhiPass;
}

//=============================================================================
// ShaderParameterValue
namespace detail {

ShaderParameterValue::ShaderParameterValue()
	: m_type(ShaderVariableType::Unknown)
	, m_buffer()
{
}

void ShaderParameterValue::reset(ShaderVariableType type, int elements)
{
	switch (type)
	{
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
	if (m_type != ShaderVariableType::Texture)
	{
		m_type = ShaderVariableType::Texture;
		m_data.texture = texture;
		if (m_data.texture) {
			RefObjectHelper::retain(m_data.texture);
		}
	}
	else
	{
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
	switch (m_type)
	{
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
	if (size > m_buffer.size())
	{
		m_buffer.resize(size);
	}
}

bool ShaderParameterValue::isShortSizeType(ShaderVariableType type)
{
	return
		type == ShaderVariableType::Bool ||
		type == ShaderVariableType::Int ||
		type == ShaderVariableType::Float ||
		type == ShaderVariableType::Vector ||
		type == ShaderVariableType::Matrix;
}

} // namespace detail

//=============================================================================
// ShaderParameterValue
namespace detail {

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

	switch (value.type())
	{
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
		return nullptr;	// EOF
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
	//		variable->setTexture((Driver::ITexture*)m_reader.readUInt64());
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
