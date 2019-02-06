
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

const String UnifiedShader::FileExt = u"lcfx";

UnifiedShader::UnifiedShader(DiagnosticsManager* diag)
    : m_diag(diag)
    , m_codeContainers()
    , m_techniques()
    , m_passes()
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
        writer->write("lufx", 4); // magic number
        writer->writeUInt32(FileVersion_Current);
    }

    // Code container
    {
        writer->write("lufx.c..", 8); // Chunk signature

        writer->writeUInt32(m_codeContainers.size());
        for (int i = 0; i < m_codeContainers.size(); i++) {
            CodeContainerInfo* info = &m_codeContainers[i];
            writeString(writer, info->entryPointName);

            writer->writeUInt8(info->codes.size());
            for (int iCode = 0; iCode < info->codes.size(); iCode++) {
                writeString(writer, info->codes[iCode].triple.target);
                writer->writeUInt32(info->codes[iCode].triple.version);
                writeString(writer, info->codes[iCode].triple.option);
				writeByteArray(writer, info->codes[iCode].code);
            }
        }
    }

    // Technique
    {
        writer->write("lufx.t..", 8); // Chunk signature

        writer->writeUInt32(m_techniques.size());
        for (int i = 0; i < m_techniques.size(); i++) {
            TechniqueInfo* info = &m_techniques[i];
            writeString(writer, info->name);

            // passes
            writer->writeUInt32(info->passes.size());
            for (int iPass = 0; iPass < info->passes.size(); iPass++) {
                writer->writeUInt32(info->passes[iPass]);
            }
        }
    }

    // Pass
    {
        writer->write("lufx.p..", 8); // Chunk signature

        writer->writeUInt32(m_passes.size());
        for (int i = 0; i < m_passes.size(); i++) {
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

			// Input attribute semantices
			{
				auto& semantics = info->attributeSemantics;
				writer->writeUInt32(semantics.size());
				for (size_t i = 0; i < semantics.size(); i++) {
					writer->writeUInt8(semantics[i].usage);
					writer->writeUInt8(semantics[i].index);
					writer->writeUInt8(semantics[i].layoutLocation);
				}
			}

			// Uniform buffers
			{
				auto& buffers = info->uniformBuffers;
				writer->writeUInt32(buffers.size());
				for (size_t i = 0; i < buffers.size(); i++) {
					writeString(writer, buffers[i].name);
					writer->writeUInt32(buffers[i].size);

					writer->writeUInt32(buffers[i].members.size());
					for (size_t iMember = 0; iMember < buffers[i].members.size(); iMember++) {
						auto& member = buffers[i].members[iMember];
						writeString(writer, member.name);
						writer->writeUInt16(member.type);
						writer->writeUInt16(member.offset);
						writer->writeUInt16(member.vectorElements);
						writer->writeUInt16(member.arrayElements);
						writer->writeUInt16(member.matrixRows);
						writer->writeUInt16(member.matrixColumns);
					}
				}
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
        for (size_t i = 0; i < count; i++) {
            CodeContainerInfo info;
            info.entryPointName = readString(reader);

            uint8_t count = reader->readUInt8();
            for (int iCode = 0; iCode < count; iCode++) {
                CodeInfo code;
                code.triple.target = readString(reader);
                code.triple.version = reader->readUInt32();
                code.triple.option = readString(reader);
                code.code = readByteArray(reader);
                info.codes.push_back(std::move(code));
            }

            // GLSL
            //{
            //	if (!checkSignature(reader, "GLSL....", 8, m_diag)) {
            //		return false;
            //	}

            //	info.codes[(int)CodeKind::Glsl] = readString(reader);
            //}

            m_codeContainers.add(std::move(info));
        }
    }

    // Technique
    {
        if (!checkSignature(reader, "lufx.t..", 8, m_diag)) {
            return false;
        }

        size_t count = reader->readUInt32();
        for (size_t iTech = 0; iTech < count; iTech++) {
            TechniqueInfo info;
            info.name = readString(reader);

            // passes
            size_t passCount = reader->readUInt32();
            for (size_t iPass = 0; iPass < passCount; iPass++) {
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
        for (size_t i = 0; i < count; i++) {
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

			// Input attribute semantices
			if (fileVersion >= FileVersion_2)
			{
				size_t count = reader->readUInt32();
				for (size_t i = 0; i < count; i++) {
					VertexInputAttribute attr;
					attr.usage = (AttributeUsage)reader->readUInt8();
					attr.index = reader->readUInt8();
					attr.layoutLocation = reader->readUInt8();
					info.attributeSemantics.push_back(attr);
				}
			}

			// Uniform buffers
			{
				size_t count = reader->readUInt32();
				for (size_t i = 0; i < count; i++) {
					ShaderUniformBufferInfo buffer;
					buffer.name = readString(reader);
					buffer.size = reader->readUInt32();

					size_t memberCount = reader->readUInt32();
					for (size_t iMember = 0; iMember < memberCount; iMember++) {
						ShaderUniformInfo member;
						member.name = readString(reader);
						member.type = reader->readUInt16();
						member.offset = reader->readUInt16();
						member.vectorElements = reader->readUInt16();
						member.arrayElements = reader->readUInt16();
						member.matrixRows = reader->readUInt16();
						member.matrixColumns = reader->readUInt16();
						buffer.members.push_back(std::move(member));
					}

					info.uniformBuffers.push_back(std::move(buffer));
				}
			}

            m_passes.add(std::move(info));
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

    m_codeContainers.add({entryPointName});
    *outId = indexToId(m_codeContainers.size() - 1);
    return true;
}

void UnifiedShader::setCode(CodeContainerId container, const UnifiedShaderTriple& triple, const std::vector<byte_t>& code)
{
    m_codeContainers[idToIndex(container)].codes.push_back({triple, code});
}

void UnifiedShader::setCode(const std::string& entryPointName, const UnifiedShaderTriple& triple, const std::vector<byte_t>& code)
{
    int index = findCodeContainerInfoIndex(entryPointName);
    if (index < 0) {
        CodeContainerId newId;
        addCodeContainer(entryPointName, &newId);
        index = idToIndex(newId);
    }

    setCode(indexToId(index), triple, code);
}

bool UnifiedShader::hasCode(const std::string& entryPointName, const UnifiedShaderTriple& triple) const
{
    int index = findCodeContainerInfoIndex(entryPointName);
    if (index >= 0) {
        return findCode(indexToId(index), triple) != nullptr;
    } else {
        return false;
    }
}

bool UnifiedShader::findCodeContainer(const std::string& entryPointName, CodeContainerId* outId) const
{
    *outId = indexToId(findCodeContainerInfoIndex(entryPointName));
    return (*outId) >= 0;
}

const std::vector<byte_t>* UnifiedShader::findCode(CodeContainerId conteinreId, const UnifiedShaderTriple& triple) const
{
    if (LN_REQUIRE(!triple.target.empty())) {
        return nullptr;
    }

    auto& codes = m_codeContainers[idToIndex(conteinreId)].codes;

    int candidateVersion = 0;
    int candidate = -1;
    for (int iCode = 0; iCode < codes.size(); iCode++) {
        auto& codeTriple = codes[iCode].triple;
        if (codeTriple.target != triple.target) {
            // not adopted
        } else if (!triple.option.empty() && codeTriple.option != triple.option) {
            // not adopted
        } else {
            // check version
            if (codeTriple.version <= triple.version && // first, less than requested version
                codeTriple.version > candidateVersion) {
                candidate = iCode;
                candidateVersion = codeTriple.version;
            }
        }
    }

    if (candidate >= 0) {
        return &codes[candidate].code;
    } else {
        return nullptr;
    }
}

bool UnifiedShader::addTechnique(const std::string& name, TechniqueId* outTech)
{
    if (findTechniqueInfoIndex(name) >= 0) {
        m_diag->reportError(String::fromStdString("Technique '" + name + "' is already exists."));
        return false;
    }

    m_techniques.add({name});
    *outTech = indexToId(m_techniques.size() - 1);
    return true;
}

bool UnifiedShader::addPass(TechniqueId parentTech, const std::string& name, PassId* outPass)
{
    if (findPassInfoIndex(parentTech, name) >= 0) {
        m_diag->reportError(String::fromStdString("Pass '" + name + "' in '" + m_techniques[idToIndex(parentTech)].name + "' is already exists."));
        return false;
    }

    m_passes.add({name});
    int index = m_passes.size() - 1;
    m_techniques[idToIndex(parentTech)].passes.add(indexToId(index));
    *outPass = indexToId(index);
    return true;
}

int UnifiedShader::getPassCountInTechnique(TechniqueId parentTech) const
{
    return m_techniques[idToIndex(parentTech)].passes.size();
}

UnifiedShader::PassId UnifiedShader::getPassIdInTechnique(TechniqueId parentTech, int index) const
{
    return m_techniques[idToIndex(parentTech)].passes[index];
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

void UnifiedShader::setAttributeSemantics(PassId pass, const std::vector<VertexInputAttribute>& semantics)
{
	m_passes[idToIndex(pass)].attributeSemantics = semantics;
}

void UnifiedShader::setUniformBuffers(PassId pass, const std::vector<ShaderUniformBufferInfo>& buffers)
{
	m_passes[idToIndex(pass)].uniformBuffers = buffers;
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

const std::vector<VertexInputAttribute>& UnifiedShader::attributeSemantics(PassId pass) const
{
	return m_passes[idToIndex(pass)].attributeSemantics;
}

int UnifiedShader::findCodeContainerInfoIndex(const std::string& entryPointName) const
{
    return m_codeContainers.indexOfIf([&](const CodeContainerInfo& info) { return info.entryPointName == entryPointName; });
}

int UnifiedShader::findTechniqueInfoIndex(const std::string& name) const
{
    return m_techniques.indexOfIf([&](const TechniqueInfo& info) { return info.name == name; });
}

int UnifiedShader::findPassInfoIndex(TechniqueId tech, const std::string& name) const
{
    auto& t = m_techniques[idToIndex(tech)];
    for (auto& passId : t.passes) {
        int index = idToIndex(passId);
        if (m_passes[index].name == name) {
            return index;
        }
    }
    return -1;
}

void UnifiedShader::writeString(BinaryWriter* w, const std::string& str)
{
    w->writeUInt32(str.length());
    w->write(str.data(), str.length());
}

void UnifiedShader::writeByteArray(BinaryWriter* w, const std::vector<byte_t>& data)
{
	w->writeUInt32(data.size());
	w->write(data.data(), data.size());
}

std::string UnifiedShader::readString(BinaryReader* r)
{
    uint32_t len = r->readUInt32();
    if (len == 0) {
        return std::string();
    } else if (len <= 255) { // min str optimaize
        char buf[255] = {0};
        r->read(buf, len);
        return std::string(buf, len);
    } else {
        std::vector<char> buf;
        buf.resize(len);
        r->read(buf.data(), len);
        return std::string(buf.begin(), buf.end());
    }
}

std::vector<byte_t> UnifiedShader::readByteArray(BinaryReader* r)
{
	uint32_t len = r->readUInt32();
	std::vector<byte_t> buf;
	buf.resize(len);
	r->read(buf.data(), len);
	return buf;
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

} // namespace detail
} // namespace ln
