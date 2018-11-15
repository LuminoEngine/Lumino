
#include "Internal.hpp"
#include <LuminoEngine/Shader/ShaderHelper.hpp>
#include "UnifiedShader.hpp"

namespace ln {
namespace detail {


template<typename TValue>
static void writeOptionalUInt8(BinaryWriter* w, const Optional<TValue>& value)
{
	w->writeUInt8(value.hasValue());
	if (value.hasValue())
		w->writeUInt8((uint8_t)value.value());
	else
		w->writeUInt8(0);
}

static void writeOptionalBool(BinaryWriter* w, const Optional<bool>& value)
{
	w->writeUInt8(value.hasValue());
	if (value.hasValue())
		w->writeUInt8((value.value()) ? 1 : 0);
	else
		w->writeUInt8(0);
}

template<typename TValue>
static void readOptionalUInt8(BinaryReader* r, Optional<TValue>* outValue)
{
	uint8_t has = r->readUInt8();
	uint8_t value = r->readUInt8();
	if (has) {
		*outValue = (TValue)value;
	}
}

static void readOptionalBool(BinaryReader* r, Optional<bool>* outValue)
{
	uint8_t has = r->readUInt8();
	uint8_t value = r->readUInt8();
	if (has) {
		*outValue = value;
	}
}


//=============================================================================
// UnifiedShader

const String UnifiedShader::FileExt = u"lufx";

UnifiedShader::UnifiedShader(DiagnosticsManager* diag)
	: m_diag(diag)
{
}

UnifiedShader::~UnifiedShader()
{
}

bool UnifiedShader::save(const Path& filePath)
{
	auto file = FileStream::create(filePath, FileOpenMode::Write | FileOpenMode::Truncate);
	auto writer = makeRef<ln::BinaryWriter>(file);

	// File header
	{
		writer->write("lufx", 4);		// magic number
		writer->writeUInt32(FileVersion);
	}

	// Code container
	{
		writer->write("lufx.c..", 8);	// Chunk signature

		writer->writeUInt32(m_codeContainers.size());
		for (int i = 0; i < m_codeContainers.size(); i++)
		{
			CodeContainerInfo* info = &m_codeContainers[i];
			writeString(writer, info->entryPointName);

			// GLSL
			{
				writer->write("GLSL....", 8);	// Chunk signature
				writeString(writer, info->codes[(int)CodeKind::Glsl]);
			}
		}
	}

	// Technique
	{
		writer->write("lufx.t..", 8);	// Chunk signature

		writer->writeUInt32(m_techniques.size());
		for (int i = 0; i < m_techniques.size(); i++)
		{
			TechniqueInfo* info = &m_techniques[i];
			writeString(writer, info->name);

			// passes
			writer->writeUInt32(info->passes.size());
			for (int iPass = 0; iPass < info->passes.size(); iPass++)
			{
				writer->writeUInt32(info->passes[iPass]);
			}
		}
	}

	// Pass
	{
		writer->write("lufx.p..", 8);	// Chunk signature

		writer->writeUInt32(m_passes.size());
		for (int i = 0; i < m_passes.size(); i++)
		{
			PassInfo* info = &m_passes[i];
			writeString(writer, info->name);
			writer->writeUInt32(info->vertexShader);
			writer->writeUInt32(info->pixelShader);

			// ShaderRenderState
			{
				ShaderRenderState* renderState = info->renderState;

				writeOptionalBool(writer, renderState->blendEnable);
				writeOptionalUInt8(writer, renderState->sourceBlend);
				writeOptionalUInt8(writer, renderState->destinationBlend);
				writeOptionalUInt8(writer, renderState->blendOp);
				writeOptionalUInt8(writer, renderState->sourceBlendAlpha);
				writeOptionalUInt8(writer, renderState->destinationBlendAlpha);
				writeOptionalUInt8(writer, renderState->blendOpAlpha);

				writeOptionalUInt8(writer, renderState->fillMode);
				writeOptionalUInt8(writer, renderState->cullMode);

				writeOptionalUInt8(writer, renderState->depthTestFunc);
				writeOptionalBool(writer, renderState->depthWriteEnabled);

				writeOptionalBool(writer, renderState->stencilEnabled);
				writeOptionalUInt8(writer, renderState->stencilReferenceValue);
				writeOptionalUInt8(writer, renderState->stencilFailOp);
				writeOptionalUInt8(writer, renderState->stencilDepthFailOp);
				writeOptionalUInt8(writer, renderState->stencilPassOp);
				writeOptionalUInt8(writer, renderState->stencilFunc);
			}
		}
	}

	return true;
}

bool UnifiedShader::load(Stream* stream)
{
	auto reader = ln::makeRef<BinaryReader>(stream);

	int fileVersion = 0;

	// File header
	{
		if (!checkSignature(reader, "lufx", 4, m_diag)) {
			return false;
		}

		fileVersion = reader->readUInt32();
	}

	// Code container
	{
		if (!checkSignature(reader, "lufx.c..", 8, m_diag)) {
			return false;
		}

		size_t count = reader->readUInt32();
		for (size_t i = 0; i < count; i++)
		{
			CodeContainerInfo info;
			info.entryPointName = readString(reader);

			// GLSL
			{
				if (!checkSignature(reader, "GLSL....", 8, m_diag)) {
					return false;
				}

				info.codes[(int)CodeKind::Glsl] = readString(reader);
			}

			m_codeContainers.add(info);
		}
	}

	// Technique
	{
		if (!checkSignature(reader, "lufx.t..", 8, m_diag)) {
			return false;
		}

		size_t count = reader->readUInt32();
		for (size_t iTech = 0; iTech < count; iTech++)
		{
			TechniqueInfo info;
			info.name = readString(reader);

			// passes
			size_t passCount = reader->readUInt32();
			for (size_t iPass = 0; iPass < passCount; iPass++)
			{
				info.passes.add(reader->readUInt32());
			}

			m_techniques.add(info);
		}
	}

	// Pass
	{
		if (!checkSignature(reader, "lufx.p..", 8, m_diag)) {
			return false;
		}

		size_t count = reader->readUInt32();
		for (size_t i = 0; i < count; i++)
		{
			PassInfo info;
			info.name = readString(reader);
			info.vertexShader = reader->readUInt32();
			info.pixelShader = reader->readUInt32();

			// ShaderRenderState
			{
				auto renderState = makeRef<ShaderRenderState>();
				info.renderState = renderState;

				readOptionalBool(reader, &renderState->blendEnable);
				readOptionalUInt8(reader, &renderState->sourceBlend);
				readOptionalUInt8(reader, &renderState->destinationBlend);
				readOptionalUInt8(reader, &renderState->blendOp);
				readOptionalUInt8(reader, &renderState->sourceBlendAlpha);
				readOptionalUInt8(reader, &renderState->destinationBlendAlpha);
				readOptionalUInt8(reader, &renderState->blendOpAlpha);

				readOptionalUInt8(reader, &renderState->fillMode);
				readOptionalUInt8(reader, &renderState->cullMode);

				readOptionalUInt8(reader, &renderState->depthTestFunc);
				readOptionalBool(reader, &renderState->depthWriteEnabled);

				readOptionalBool(reader, &renderState->stencilEnabled);
				readOptionalUInt8(reader, &renderState->stencilReferenceValue);
				readOptionalUInt8(reader, &renderState->stencilFailOp);
				readOptionalUInt8(reader, &renderState->stencilDepthFailOp);
				readOptionalUInt8(reader, &renderState->stencilPassOp);
				readOptionalUInt8(reader, &renderState->stencilFunc);
			}

			m_passes.add(info);
		}
	}

	return true;
}

bool UnifiedShader::addCodeContainer(const std::string& entryPointName, CodeContainerId* outId)
{
	if (findCodeContainerInfoIndex(entryPointName) >= 0) {
		m_diag->reportError(String::fromStdString("Code entory point '" + entryPointName + "' is already exists."));
		return false;
	}

	m_codeContainers.add({ entryPointName });
	*outId = indexToId(m_codeContainers.size() - 1);
	return true;
}

void UnifiedShader::setCode(CodeContainerId container, CodeKind kind, const std::string& code)
{
	m_codeContainers[idToIndex(container)].codes[(int)kind] = code;
}

void UnifiedShader::setCode(const std::string& entryPointName, CodeKind kind, const std::string& code)
{
	int index = findCodeContainerInfoIndex(entryPointName);
	if (index < 0) {
		CodeContainerId newId;
		addCodeContainer(entryPointName, &newId);
		index = idToIndex(newId);
	}

	setCode(indexToId(index), kind, code);
}

bool UnifiedShader::hasCode(const std::string& entryPointName, CodeKind kind) const
{
	int index = findCodeContainerInfoIndex(entryPointName);
	if (index >= 0) {
		return !m_codeContainers[indexToId(index)].codes[(int)kind].empty();
	}
	else {
		return false;
	}
}

bool UnifiedShader::findCodeContainer(const std::string& entryPointName, CodeContainerId* outId) const
{
	*outId = indexToId(findCodeContainerInfoIndex(entryPointName));
	return (*outId) >= 0;
}

const std::string& UnifiedShader::getCode(CodeContainerId conteinreId, CodeKind kind) const
{
	return m_codeContainers[idToIndex(conteinreId)].codes[(int)kind];
}

bool UnifiedShader::addTechnique(const std::string& name, TechniqueId* outTech)
{
	if (findTechniqueInfoIndex(name) >= 0) {
		m_diag->reportError(String::fromStdString("Technique '" + name + "' is already exists."));
		return false;
	}

	m_techniques.add({ name });
	*outTech = indexToId(m_techniques.size() - 1);
	return true;
}

bool UnifiedShader::addPass(TechniqueId parentTech, const std::string& name, PassId* outPass)
{
	if (findPassInfoIndex(name) >= 0) {
		m_diag->reportError(String::fromStdString("Pass '" + name + "' is already exists."));
		return false;
	}

	m_passes.add({ name });
	int index = m_passes.size() - 1;
	m_techniques[idToIndex(parentTech)].passes.add(indexToId(index));
	*outPass = indexToId(index);
	return true;
}

void UnifiedShader::setVertexShader(PassId pass, CodeContainerId code)
{
	m_passes[idToIndex(pass)].vertexShader = code;
}

void UnifiedShader::setPixelShader(PassId pass, CodeContainerId code)
{
	m_passes[idToIndex(pass)].pixelShader = code;
}

void UnifiedShader::setRenderState(PassId pass, ShaderRenderState* state)
{
	m_passes[idToIndex(pass)].renderState = state;
}

UnifiedShader::CodeContainerId UnifiedShader::vertexShader(PassId pass) const
{
	return m_passes[idToIndex(pass)].vertexShader;
}

UnifiedShader::CodeContainerId UnifiedShader::pixelShader(PassId pass) const
{
	return m_passes[idToIndex(pass)].pixelShader;
}

ShaderRenderState* UnifiedShader::renderState(PassId pass) const
{
	return m_passes[idToIndex(pass)].renderState;
}

int UnifiedShader::findCodeContainerInfoIndex(const std::string& entryPointName) const
{
	return m_codeContainers.indexOfIf([&](const CodeContainerInfo& info) { return info.entryPointName == entryPointName; });
}

int UnifiedShader::findTechniqueInfoIndex(const std::string& name) const
{
	return m_techniques.indexOfIf([&](const TechniqueInfo& info) { return info.name == name; });
}

int UnifiedShader::findPassInfoIndex(const std::string& name) const
{
	return m_passes.indexOfIf([&](const PassInfo& info) { return info.name == name; });
}

void UnifiedShader::writeString(BinaryWriter* w, const std::string& str)
{
	w->writeUInt32(str.length());
	w->write(str.data(), str.length());
}

std::string UnifiedShader::readString(BinaryReader* r)
{
	uint32_t len = r->readUInt32();
	if (len == 0) {
		return std::string();
	}
	else if (len <= 255) {	// min str optimaize
		char buf[255] = { 0 };
		r->read(buf, len);
		return std::string(buf, len);
	}
	else {
		std::vector<char> buf;
		buf.resize(len);
		r->read(buf.data(), len);
		return std::string(buf.begin(), buf.end());
	}
}

bool UnifiedShader::checkSignature(BinaryReader* r, const char* sig, size_t len, DiagnosticsManager* diag)
{
	char buf[8];
	size_t size = r->read(buf, len);
	if (size != len || strncmp(buf, sig, len) != 0) {
		diag->reportError(u"Invalid code container signature. (" + String::fromCString(sig) + u")");
		return false;
	}
	return true;
}

#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER


} // namespace detail
} // namespace ln
