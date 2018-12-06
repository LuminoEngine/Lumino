
#include "Internal.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Shader/Shader.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"
#include "../Graphics/GraphicsManager.hpp"
#include "../Engine/RenderingCommandList.hpp"
#include "UnifiedShader.hpp"
#include "ShaderManager.hpp"
#include "ShaderTranspiler.hpp"
#include "HLSLMetadataParser.hpp"


namespace ln {

namespace detail {



} // namespace detail

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
	byte_t* buffer, size_t offset, int elements, size_t arrayStride) LN_NOEXCEPT
{
	byte_t* head = buffer + offset;
	int loop = std::min(unitCount, elements);
	for (int i = 0; i < loop; i++) {
		memcpy(head + arrayStride * i, source + unitBytes * i, unitBytes);
	}
}

// vector
static void alignVectorsToBuffer(
	const byte_t* source, int sourceColumns, int sourceElementCount,
	byte_t* buffer, size_t offset, int elements, size_t arrayStride, int columns) LN_NOEXCEPT
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
	const byte_t* source, int sourceColumns, int sourceRows, int sourceElementCount,
	byte_t* buffer, size_t offset, int elements, size_t matrixStride, size_t arrayStride, int rows, int columns, bool transpose) LN_NOEXCEPT
{
	size_t srcRowSize = sizeof(float) * sourceColumns;
	size_t dstRowSize = matrixStride;
	size_t copySize = std::min(srcRowSize, dstRowSize);
	byte_t* head = buffer + offset;
	int loop = std::min(sourceElementCount, elements);
	int rowLoop = std::min(sourceRows, rows);
	for (int i = 0; i < loop; i++)
	{
		const byte_t* srcMatHead = source + (sourceColumns * sourceRows * sizeof(float)) * i;
		byte_t* dstMatHead = head + arrayStride * i;

		float tmp[16];
		if (transpose)
		{
			assert(sourceColumns == 4 && sourceRows == 4);
			*reinterpret_cast<Matrix*>(tmp) = Matrix::makeTranspose(*reinterpret_cast<const Matrix*>(srcMatHead));
			srcMatHead = reinterpret_cast<const byte_t*>(tmp);
		}

		for (int j = 0; j < rowLoop; j++)
		{
			memcpy(dstMatHead + matrixStride * j, srcMatHead + srcRowSize * j, copySize);
		}
	}
}

//=============================================================================
// ShaderCompilationProperties

ShaderCompilationProperties::ShaderCompilationProperties()
{
}

ShaderCompilationProperties::~ShaderCompilationProperties()
{
}

void ShaderCompilationProperties::initialize()
{
}

void ShaderCompilationProperties::addIncludeDirectory(const StringRef& value)
{
	m_includeDirectories.add(value);
}

void ShaderCompilationProperties::addDefinition(const StringRef& value)
{
	m_definitions.add(value);
}

void ShaderCompilationProperties::setDiagnostics(DiagnosticsManager* diag)
{
	m_diag = diag;
}

//=============================================================================
// Shader

Ref<Shader> Shader::create(const StringRef& hlslEffectFilePath, ShaderCompilationProperties* properties)
{
	return ln::newObject<Shader>(hlslEffectFilePath, properties);
}

Ref<Shader> Shader::create(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCompilationProperties* properties)
{
	return ln::newObject<Shader>(vertexShaderFilePath, pixelShaderFilePath, ShaderCodeType::RawGLSL, properties);
}

Shader::Shader()
	: m_manager(detail::EngineDomain::shaderManager())
	, m_globalConstantBuffer(nullptr)
{

}

Shader::~Shader()
{
}

void Shader::initialize()
{
	GraphicsResource::initialize();
}

void Shader::initialize(const StringRef& hlslEffectFilePath, ShaderCompilationProperties* properties)
{
	Shader::initialize();
	Ref<DiagnosticsManager> localDiag = nullptr;
	if (properties) localDiag = properties->m_diag;
	if (!localDiag) localDiag = newObject<DiagnosticsManager>();

	if (Path(hlslEffectFilePath).hasExtension(detail::UnifiedShader::FileExt))
	{
        auto file = FileStream::create(hlslEffectFilePath, FileOpenMode::Read);
        createFromUnifiedShader(file, localDiag);
	}
	else
	{
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

		auto data = FileSystem::readAllBytes(hlslEffectFilePath);
		auto code = reinterpret_cast<char*>(data.data());
		auto codeLen = data.size();

		detail::HLSLMetadataParser parser;
		parser.parse(code, codeLen, localDiag);

		// glslang は hlsl の technique ブロックを理解できないので、空白で潰しておく
		for (auto& hlslTech : parser.techniques)
		{
			memset(code + hlslTech.blockBegin, ' ', hlslTech.blockEnd - hlslTech.blockBegin);
		}

		for (auto& hlslTech : parser.techniques)
		{
			auto tech = newObject<ShaderTechnique>(String::fromStdString(hlslTech.name));
			tech->setOwner(this);
			m_techniques.add(tech);

			for (auto& hlslPass : hlslTech.passes)
			{
				auto rhiPass = createShaderPass(
					code, codeLen, hlslPass.vertexShader.c_str(),
					code, codeLen, hlslPass.pixelShader.c_str(),
					localDiag, properties);
				if (rhiPass)
				{
					auto pass = newObject<ShaderPass>(rhiPass, hlslPass.renderState);
					tech->addShaderPass(pass);
					pass->setupParameters();
				}
			}
		}
#else
		LN_NOTIMPLEMENTED();
#endif
	}

	postInitialize();

	if (!properties || !properties->m_diag) {
		if (localDiag->hasError()) {
			LN_ERROR(localDiag->toString());
			return;
		}
		else if (localDiag->hasWarning()) {
			LN_LOG_WARNING << localDiag->toString();
		}
	}
}

void Shader::initialize(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCodeType codeType, ShaderCompilationProperties* properties)
{
	Shader::initialize();
	Ref<DiagnosticsManager> localDiag = nullptr;
	if (properties) localDiag = properties->m_diag;
	if (!localDiag) localDiag = newObject<DiagnosticsManager>();

	auto vsData = FileSystem::readAllBytes(vertexShaderFilePath);
	auto psData = FileSystem::readAllBytes(pixelShaderFilePath);

	createSinglePassShader(
		reinterpret_cast<const char*>(vsData.data()), vsData.size(),
		reinterpret_cast<const char*>(psData.data()), psData.size(),
		localDiag, properties);

	postInitialize();


	if (!properties || !properties->m_diag) {
		if (localDiag->hasError()) {
			LN_ERROR(localDiag->toString());
			return;
		}
		else if (localDiag->hasWarning()) {
			LN_LOG_WARNING << localDiag->toString();
		}
	}
}

void Shader::initialize(Stream* stream)
{
    Shader::initialize();
    Ref<DiagnosticsManager> localDiag = newObject<DiagnosticsManager>();

    createFromUnifiedShader(stream, localDiag);

    if (localDiag->hasError()) {
        LN_ERROR(localDiag->toString());
        return;
    }
    else if (localDiag->hasWarning()) {
        LN_LOG_WARNING << localDiag->toString(); 
    }
}

void Shader::createFromUnifiedShader(Stream* stream, DiagnosticsManager* diag)
{
    detail::UnifiedShader unifiedShader(diag);
    if (unifiedShader.load(stream))
    {
        auto& triple = manager()->deviceContext()->caps().requestedShaderTriple;
        for (int iTech = 0; iTech < unifiedShader.techniqueCount(); iTech++)
        {
            detail::UnifiedShader::TechniqueId techId = unifiedShader.techniqueId(iTech);
            auto tech = newObject<ShaderTechnique>(String::fromStdString(unifiedShader.techniqueName(techId)));
            tech->setOwner(this);
            m_techniques.add(tech);

            for (int iPass = 0; iPass < unifiedShader.passCount(); iPass++)
            {
                detail::UnifiedShader::PassId passId = unifiedShader.passId(iPass);
                detail::UnifiedShader::CodeContainerId vscodeId = unifiedShader.vertexShader(passId);
                detail::UnifiedShader::CodeContainerId pscodeId = unifiedShader.pixelShader(passId);

                const std::string* vscode = nullptr;
                const std::string* pscode = nullptr;
                if (vscodeId) {
                    vscode = unifiedShader.findCode(vscodeId, triple);
                }
                if (pscodeId) {
                    pscode = unifiedShader.findCode(pscodeId, triple);
                }

                auto rhiPass = createRHIShaderPass(
                    (vscode) ? vscode->c_str() : nullptr, (vscode) ? vscode->length() : 0,
                    (pscode) ? pscode->c_str() : nullptr, (pscode) ? pscode->length() : 0,
                    diag);
                if (rhiPass)
                {
                    auto pass = newObject<ShaderPass>(rhiPass);
                    pass->m_renderState = unifiedShader.renderState(passId);
                    tech->addShaderPass(pass);
                    pass->setupParameters();
                }
            }
        }
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
	m_techniques.clear();

	GraphicsResource::dispose();
}

void Shader::onChangeDevice(detail::IGraphicsDeviceContext* device)
{
	LN_NOTIMPLEMENTED();
}

Ref<detail::IShaderPass> Shader::createShaderPass(
	const char* vsData, size_t vsLen, const char* vsEntryPoint,
	const char* psData, size_t psLen, const char* psEntryPoint,
	DiagnosticsManager* diag, ShaderCompilationProperties* properties)
{
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
	List<Path> includeDirs;
	if (properties) {
		for (auto& path : properties->m_includeDirectories) includeDirs.add(path);
	}

	detail::ShaderCodeTranspiler vsCodeGen;
	if (!vsCodeGen.parseAndGenerateSpirv(detail::ShaderCodeStage::Vertex, vsData, vsLen, vsEntryPoint, includeDirs, (properties) ? &properties->m_definitions : nullptr, diag))
	{
		return nullptr;
	}

	detail::ShaderCodeTranspiler psCodeGen;
	if (!psCodeGen.parseAndGenerateSpirv(detail::ShaderCodeStage::Fragment, psData, psLen, psEntryPoint, includeDirs, (properties) ? &properties->m_definitions : nullptr, diag))
	{
		return nullptr;
	}

	std::string vsCode = vsCodeGen.generateGlsl(400, false);
	std::string psCode = psCodeGen.generateGlsl(400, false);

	Ref<detail::IShaderPass> pass = createRHIShaderPass(
		vsCode.c_str(), vsCode.length(),
		psCode.c_str(), psCode.length(),
		diag);
	
	return pass;
#else
	LN_NOTIMPLEMENTED();
	return nullptr;
#endif
}

Ref<detail::IShaderPass> Shader::createRHIShaderPass(
	const char* vsData, size_t vsLen,
	const char* psData, size_t psLen,
	DiagnosticsManager* diag)
{
	ShaderCompilationDiag sdiag;
	Ref<detail::IShaderPass> pass = deviceContext()->createShaderPass(
		reinterpret_cast<const byte_t*>(vsData), vsLen,
		reinterpret_cast<const byte_t*>(psData), psLen, &sdiag);

	if (sdiag.level == ShaderCompilationResultLevel::Error) {
		diag->reportError(String::fromStdString(sdiag.message));
	}
	else if (sdiag.level == ShaderCompilationResultLevel::Warning) {
		diag->reportWarning(String::fromStdString(sdiag.message));
	}

	return pass;
}

void Shader::createSinglePassShader(const char* vsData, size_t vsLen, const char* psData, size_t psLen, DiagnosticsManager* diag, ShaderCompilationProperties* properties)
{
	auto rhiPass = createShaderPass(vsData, vsLen, "main", psData, psLen, "main", diag, properties);

	auto tech = newObject<ShaderTechnique>(u"Main");	// TODO: 名前指定できた方がいいかも
	tech->setOwner(this);
	m_techniques.add(tech);

	auto pass = newObject<ShaderPass>(rhiPass);
	tech->addShaderPass(pass);
	pass->setupParameters();
}

void Shader::postInitialize()
{
	// find global constant buffer.
	{
		m_globalConstantBuffer = findConstantBuffer(u"_Global");
		if (!m_globalConstantBuffer && !m_buffers.isEmpty()) {
			m_globalConstantBuffer = m_buffers.front();
		}
	}

	// analyze semantices.
	{
		if (m_globalConstantBuffer) {
			for (auto& param : m_globalConstantBuffer->m_parameters) {
				m_semanticsManager.prepareParameter(param);
			}
		}

		for (auto& param : m_textureParameters) {
			m_semanticsManager.prepareParameter(param);
		}
	}
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

ShaderParameter* Shader::findParameter(const StringRef& name) const
{
	// find global constant buffer.
	if (m_globalConstantBuffer)
	{
		ShaderParameter* result = m_globalConstantBuffer->findParameter(name);
		if (result) {
			return result;
		}
	}

	// find texture params.
	{
		auto result = m_textureParameters.findIf([&](const Ref<ShaderParameter>& param) { return param->name() == name; });
		return (result) ? *result : nullptr;
	}
}

ShaderConstantBuffer* Shader::findConstantBuffer(const StringRef& name) const
{
	auto result = m_buffers.findIf([name](const ShaderConstantBuffer* buf) { return buf->name() == name; });
	return (result) ? *result : nullptr;
}

ShaderTechnique* Shader::findTechnique(const StringRef& name) const
{
	for (auto& var : m_techniques)
	{
		if (String::compare(StringRef(var->name()), 0, StringRef(name), 0, -1, CaseSensitivity::CaseSensitive) == 0)
		{
			return var;
		}
	}
	return nullptr;
}

// TODO: 名前の指定方法をもう少しいい感じにしたい。ImageEffect を Forward_Geometry_UnLighting と書かなければならないなど、煩雑。
ShaderTechnique* Shader::findTechniqueByClass(const detail::ShaderTechniqueClass& techniqueClass) const
{
	for (auto& tech : m_techniques)
	{
		if (detail::ShaderTechniqueClass::equals(tech->techniqueClass(), techniqueClass))
		{
			return tech;
		}
	}
	return nullptr;
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

ShaderParameter* Shader::getOrCreateTextureParameter(const String& name)
{
	auto result = m_textureParameters.findIf([&](const Ref<ShaderParameter>& param) { return param->name() == name; });
	if (result) {
		return *result;
	}
	else
	{
		auto param = newObject<ShaderParameter>(ShaderParameterClass::Texture, name);
		m_textureParameters.add(param);
		return param;
	}
}

//=============================================================================
// ShaderParameter

ShaderParameter::ShaderParameter()
	: m_class(ShaderParameterClass::Constant)
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
	//m_value.reset(desc.type, desc.elements);
}

void ShaderParameter::initialize(ShaderParameterClass parameterClass, const String& name)
{
	Object::initialize();
	m_class = parameterClass;
	m_name = name;
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

void ShaderParameter::setMatrix(const Matrix& value)
{
	alignMatricesToBuffer((const byte_t*)&value, 4, 4, 1, m_owner->buffer().data(), m_desc.offset, 1, m_desc.matrixStride, 0, m_desc.rows, m_desc.columns, true);
}

void ShaderParameter::setMatrixArray(const Matrix* value, int count)
{
	alignMatricesToBuffer((const byte_t*)value, 4, 4, count, m_owner->buffer().data(), m_desc.offset, m_desc.elements, m_desc.matrixStride, m_desc.arrayStride, m_desc.rows, m_desc.columns, true);
}

void ShaderParameter::setTexture(Texture* value)
{
	m_textureValue = value;
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
	: m_owner(nullptr)
	, m_rhiObject(nullptr)
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

void ShaderTechnique::initialize(const String& name)
{
	Object::initialize();
	m_name = name;
	detail::ShaderTechniqueClass::parseTechniqueClassString(m_name, &m_techniqueClass);
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

void ShaderPass::initialize(detail::IShaderPass* rhiPass, detail::ShaderRenderState* renderState)
{
	if (LN_REQUIRE(rhiPass)) return;
	Object::initialize();

	m_rhiPass = rhiPass;
    m_renderState = renderState;
}

void ShaderPass::dispose()
{
	m_rhiPass = nullptr;

	Object::dispose();
}

Shader* ShaderPass::shader() const
{
	return m_owner->shader();
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
		ShaderConstantBuffer* buf = m_owner->shader()->getOrCreateConstantBuffer(rhi);
		m_buffers.add(buf);
	}


	m_textureParameters.clear();
	detail::IShaderSamplerBuffer* samplerBuffer = m_rhiPass->samplerBuffer();
	if (samplerBuffer)
	{
		for (int i = 0; i < samplerBuffer->registerCount(); i++)
		{
			ShaderParameter* param = m_owner->shader()->getOrCreateTextureParameter(String::fromStdString(samplerBuffer->getTextureRegisterName(i)));
			m_textureParameters.add(param);
		}
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
	auto* manager = m_owner->shader()->manager();

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

	// TODO: 1つのバッファにまとめるとか、一括で送りたい。
	detail::IShaderSamplerBuffer* samplerBuffer = m_rhiPass->samplerBuffer();
	if (samplerBuffer)
	{

		for (int i = 0; i < samplerBuffer->registerCount(); i++)
		{
			auto* manager = m_owner->shader()->manager();
			Texture* texture = m_textureParameters[i]->texture();

			SamplerState* sampler;
			if (texture && texture->samplerState()) {
				sampler = texture->samplerState();
			}
			else {
				sampler = m_owner->shader()->manager()->defaultSamplerState();
			}

			detail::ITexture* rhiTexture = (texture) ? texture->resolveRHIObject() : nullptr;
			detail::ISamplerState* rhiSampler = (sampler) ? sampler->resolveRHIObject() : nullptr;
			LN_ENQUEUE_RENDER_COMMAND_4(
				ShaderConstantBuffer_commit_setTexture, manager,
				detail::IShaderSamplerBuffer*, samplerBuffer,
				int, i,
				Ref<detail::ITexture>, rhiTexture,
				Ref<detail::ISamplerState>, rhiSampler,
				{
					samplerBuffer->setTexture(i, rhiTexture);
					samplerBuffer->setSamplerState(i, rhiSampler);
				});

			//samplerBuffer->setTexture(i, m_textureParameters[i]->texture());
		}
	}
}

detail::IShaderPass* ShaderPass::resolveRHIObject()
{
	commit();
	return m_rhiPass;
}

} // namespace ln
