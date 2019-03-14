
#include "Internal.hpp"
#include <LuminoEngine/Shader/ShaderHelper.hpp>
#include "ShaderTranspiler.hpp"
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
    auto writer = makeRef<BinaryWriter>(file);

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
				CodeInfo* codeInfo = &info->codes[iCode];
                writeString(writer, codeInfo->triple.target);
                writer->writeUInt32(codeInfo->triple.version);
                writeString(writer, codeInfo->triple.option);
				writeByteArray(writer, codeInfo->code);

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
        }
    }

    // Descriptor layout
    {
        writer->write("lufx.l..", 8); // Chunk signature

        writer->writeUInt32(m_descriptorLayout.uniformBufferRegister.size());
        for (size_t i = 0; i < m_descriptorLayout.uniformBufferRegister.size(); i++) {
            DescriptorLayoutItem* item = &m_descriptorLayout.uniformBufferRegister[i];
            writeString(writer, item->name);
            writer->writeUInt8(item->stageFlags);
            writer->writeUInt8(item->binding);

            // Buffer members
            {
                writer->writeUInt32(item->size);

                writer->writeUInt32(item->members.size());
                for (size_t iMember = 0; iMember < item->members.size(); iMember++) {
                    auto& member = item->members[iMember];
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

        writer->writeUInt32(m_descriptorLayout.textureRegister.size());
        for (size_t i = 0; i < m_descriptorLayout.textureRegister.size(); i++) {
            DescriptorLayoutItem* item = &m_descriptorLayout.textureRegister[i];
            writeString(writer, item->name);
            writer->writeUInt8(item->stageFlags);
            writer->writeUInt8(item->binding);
        }

        writer->writeUInt32(m_descriptorLayout.samplerRegister.size());
        for (size_t i = 0; i < m_descriptorLayout.samplerRegister.size(); i++) {
            DescriptorLayoutItem* item = &m_descriptorLayout.samplerRegister[i];
            writeString(writer, item->name);
            writer->writeUInt8(item->stageFlags);
            writer->writeUInt8(item->binding);
        }
    }

    return true;
}

bool UnifiedShader::load(Stream* stream)
{
    auto reader = makeRef<BinaryReader>(stream);

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
                code.refrection = makeRef<UnifiedShaderRefrectionInfo>();

				//// Uniform buffers
				//{
				//	size_t count = reader->readUInt32();
				//	for (size_t i = 0; i < count; i++) {
				//		ShaderUniformBufferInfo buffer;
				//		buffer.name = readString(reader);
				//		buffer.size = reader->readUInt32();

				//		size_t memberCount = reader->readUInt32();
				//		for (size_t iMember = 0; iMember < memberCount; iMember++) {
				//			ShaderUniformInfo member;
				//			member.name = readString(reader);
				//			member.type = reader->readUInt16();
				//			member.offset = reader->readUInt16();
				//			member.vectorElements = reader->readUInt16();
				//			member.arrayElements = reader->readUInt16();
				//			member.matrixRows = reader->readUInt16();
				//			member.matrixColumns = reader->readUInt16();
				//			buffer.members.push_back(std::move(member));
				//		}

    //                    code.refrection->buffers.push_back(std::move(buffer));
				//	}
				//}

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

            m_passes.add(std::move(info));
        }
    }

    // Descriptor layout
    {
        if (!checkSignature(reader, "lufx.l..", 8, m_diag)) {
            return false;
        }

        {
            size_t count = reader->readUInt32();
            for (size_t i = 0; i < count; i++) {
                DescriptorLayoutItem item;
                item.name = readString(reader);
                item.stageFlags = reader->readUInt8();
                item.binding = reader->readUInt8();

                // Buffer members
                {
                    item.size = reader->readUInt32();

                    size_t count = reader->readUInt32();
                    for (size_t iMember = 0; iMember < count; iMember++) {
                        ShaderUniformInfo member;
                        member.name = readString(reader);
                        member.type = reader->readUInt16();
                        member.offset = reader->readUInt16();
                        member.vectorElements = reader->readUInt16();
                        member.arrayElements = reader->readUInt16();
                        member.matrixRows = reader->readUInt16();
                        member.matrixColumns = reader->readUInt16();
                        item.members.push_back(std::move(member));
                    }
                }

                m_descriptorLayout.uniformBufferRegister.push_back(item);
            }
        }

        {
            size_t count = reader->readUInt32();
            for (size_t i = 0; i < count; i++) {
                DescriptorLayoutItem item;
                item.name = readString(reader);
                item.stageFlags = reader->readUInt8();
                item.binding = reader->readUInt8();
                m_descriptorLayout.textureRegister.push_back(item);
            }
        }

        {
            size_t count = reader->readUInt32();
            for (size_t i = 0; i < count; i++) {
                DescriptorLayoutItem item;
                item.name = readString(reader);
                item.stageFlags = reader->readUInt8();
                item.binding = reader->readUInt8();
                m_descriptorLayout.samplerRegister.push_back(item);
            }
        }
    }

    return true;
}

void UnifiedShader::addMergeDescriptorLayoutItem(DescriptorType registerType, const DescriptorLayoutItem& item)
{
    std::vector<DescriptorLayoutItem>* list = nullptr;
    switch (registerType)
    {
    case DescriptorType_UniformBuffer:
        list = &m_descriptorLayout.uniformBufferRegister;
        break;
    case DescriptorType_Texture:
        list = &m_descriptorLayout.textureRegister;
        break;
    case DescriptorType_SamplerState:
        list = &m_descriptorLayout.samplerRegister;
        break;
    default:
        LN_UNREACHABLE();
        return;
    }

    auto itr = std::find_if(list->begin(), list->end(), [&](const DescriptorLayoutItem& x) { return x.name == item.name; });
    if (itr != list->end()) {
        itr->stageFlags |= item.stageFlags;
    }
    else {
        list->push_back(item);
    }
}

bool UnifiedShader::addCodeContainer(ShaderStage2 stage, const std::string& entryPointName, CodeContainerId* outId)
{
    if (findCodeContainerInfoIndex(stage, entryPointName) >= 0) {
        m_diag->reportError(String::fromStdString("Code entory point '" + entryPointName + "' is already exists."));
        return false;
    }

    m_codeContainers.add({ stage, entryPointName});
    *outId = indexToId(m_codeContainers.size() - 1);
    return true;
}

void UnifiedShader::setCode(CodeContainerId container, const UnifiedShaderTriple& triple, const std::vector<byte_t>& code, UnifiedShaderRefrectionInfo* refrection)
{
    if (LN_REQUIRE(refrection)) return;
	m_codeContainers[idToIndex(container)].codes.push_back({triple, code, refrection });
}

void UnifiedShader::setCode(ShaderStage2 stage, const std::string& entryPointName, const UnifiedShaderTriple& triple, const std::vector<byte_t>& code, UnifiedShaderRefrectionInfo* refrection)
{
    int index = findCodeContainerInfoIndex(stage, entryPointName);
    if (index < 0) {
        CodeContainerId newId;
        addCodeContainer(stage, entryPointName, &newId);
        index = idToIndex(newId);
    }

    setCode(indexToId(index), triple, code, refrection);
}

bool UnifiedShader::hasCode(ShaderStage2 stage, const std::string& entryPointName, const UnifiedShaderTriple& triple) const
{
    int index = findCodeContainerInfoIndex(stage, entryPointName);
    if (index >= 0) {
        return findCode(indexToId(index), triple) != nullptr;
    } else {
        return false;
    }
}

bool UnifiedShader::findCodeContainer(ShaderStage2 stage, const std::string& entryPointName, CodeContainerId* outId) const
{
    *outId = indexToId(findCodeContainerInfoIndex(stage, entryPointName));
    return (*outId) >= 0;
}

const UnifiedShader::CodeInfo* UnifiedShader::findCode(CodeContainerId conteinreId, const UnifiedShaderTriple& triple) const
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
        return &codes[candidate];
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

    PassInfo info;
    info.name = name;
    //info.refrection = makeRef<UnifiedShaderRefrectionInfo>();

    m_passes.add(std::move(info));
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

void UnifiedShader::setAttributeSemantics(PassId pass, const std::vector<VertexInputAttribute>& value)
{
	m_passes[idToIndex(pass)].attributeSemantics = value;
}

//void UnifiedShader::setRefrection(PassId pass, UnifiedShaderRefrectionInfo* buffers)
//{
//	m_passes[idToIndex(pass)].refrection = buffers;
//}

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

void UnifiedShader::saveCodes(const StringRef& perfix) const
{
    for (auto& container : m_codeContainers) {
        for (auto& code : container.codes) {
            auto file = String::format(u"{0}-{1}-{2}-{3}-{4}", perfix, String::fromStdString(container.entryPointName), String::fromStdString(code.triple.target), code.triple.version, String::fromStdString(code.triple.option));
            FileSystem::writeAllBytes(file, code.code.data(), code.code.size());
        }
    }
}

//UnifiedShaderRefrectionInfo* UnifiedShader::refrection(PassId pass) const
//{
//    return m_passes[idToIndex(pass)].refrection;
//}

int UnifiedShader::findCodeContainerInfoIndex(ShaderStage2 stage, const std::string& entryPointName) const
{
    return m_codeContainers.indexOfIf([&](const CodeContainerInfo& info) { return info.stage == stage && info.entryPointName == entryPointName; });
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


#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

//=============================================================================
// UnifiedShaderCompiler

UnifiedShaderCompiler::UnifiedShaderCompiler(ShaderManager* manager, DiagnosticsManager* diag)
	: m_manager(manager)
	, m_diag(diag)
{
}

bool UnifiedShaderCompiler::compile(
	char* inputCode, size_t inputCodeLength,
	const List<Path>& includeDirectories, const List<String>& definitions)
{
	m_metadataParser.parse(inputCode, inputCodeLength, m_diag);
	if (m_diag->hasError()) {
		return false;
	}

	// glslang は hlsl の technique ブロックを理解できないので、空白で潰しておく
	for (auto& tech : m_metadataParser.techniques) {
		memset((inputCode + tech.blockBegin), ' ', tech.blockEnd - tech.blockBegin);
	}

	// まずは compile と link を行う
	for (auto& tech : m_metadataParser.techniques)
	{
		for (auto& pass : tech.passes)
		{
			// Vertex shader
			{
				auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
				transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, inputCode, inputCodeLength, pass.vertexShader, includeDirectories, &definitions, m_diag);
				if (m_diag->hasError()) {
					return false;
				}
				m_transpilerMap[makeKey(ShaderStage2_Vertex, pass.vertexShader)] = transpiler;
			}

			// Pixel shader
			{
				auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
				transpiler->compileAndLinkFromHlsl(ShaderStage2_Fragment, inputCode, inputCodeLength, pass.pixelShader, includeDirectories, &definitions, m_diag);
				if (m_diag->hasError()) {
					return false;
				}
				m_transpilerMap[makeKey(ShaderStage2_Fragment, pass.pixelShader)] = transpiler;
			}
		}
	}

	return true;
}


bool UnifiedShaderCompiler::compileSingleCodes(
	const char* vsData, size_t vsLen, const std::string& vsEntryPoint,
	const char* psData, size_t psLen, const std::string& psEntryPoint,
	const List<Path>& includeDirectories, const List<String>& definitions)
{
	HLSLTechnique tech;
	tech.name = "MainTech";

	HLSLPass pass;
	pass.name = "Pass1";
	pass.renderState = makeRef<ShaderRenderState>();

	// Vertex shader
	{
		auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
		transpiler->compileAndLinkFromHlsl(ShaderStage2_Vertex, vsData, vsLen, vsEntryPoint, includeDirectories, &definitions, m_diag);
		if (m_diag->hasError()) {
			return false;
		}
		m_transpilerMap[makeKey(ShaderStage2_Vertex, vsEntryPoint)] = transpiler;

		pass.vertexShader = vsEntryPoint;
	}

	// Pixel shader
	{
		auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
		transpiler->compileAndLinkFromHlsl(ShaderStage2_Fragment, psData, psLen, psEntryPoint, includeDirectories, &definitions, m_diag);
		if (m_diag->hasError()) {
			return false;
		}
		m_transpilerMap[makeKey(ShaderStage2_Fragment, psEntryPoint)] = transpiler;

		pass.pixelShader = psEntryPoint;
	}

	tech.passes.push_back(std::move(pass));
	m_metadataParser.techniques.push_back(std::move(tech));

	return true;
}

bool UnifiedShaderCompiler::link()
{
	m_unifiedShader = makeRef<UnifiedShader>(m_diag);


	// vertex shader の最大 binding 数を求める
	size_t maxVertexShaderBindingCounts[DescriptorType_Count] = {};
	for (auto& pair : m_transpilerMap) {
		if (pair.second->stage() == ShaderStage2_Vertex) {
			maxVertexShaderBindingCounts[DescriptorType_UniformBuffer] = std::max(maxVertexShaderBindingCounts[DescriptorType_UniformBuffer], pair.second->descriptorLayout.uniformBufferRegister.size());
			maxVertexShaderBindingCounts[DescriptorType_Texture] = std::max(maxVertexShaderBindingCounts[DescriptorType_Texture], pair.second->descriptorLayout.textureRegister.size());
			maxVertexShaderBindingCounts[DescriptorType_SamplerState] = std::max(maxVertexShaderBindingCounts[DescriptorType_SamplerState], pair.second->descriptorLayout.samplerRegister.size());
		}
	}
	// 求めた maxVertexShaderBindingCount を PixelShader の binding の開始値としてマッピングする
	for (auto& pair : m_transpilerMap) {
		if (pair.second->stage() == ShaderStage2_Vertex) {
			for (size_t i = 0; i < pair.second->descriptorLayout.uniformBufferRegister.size(); i++) {
				pair.second->descriptorLayout.uniformBufferRegister[i].binding = i;
				m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_UniformBuffer, pair.second->descriptorLayout.uniformBufferRegister[i]);
			}
			for (size_t i = 0; i < pair.second->descriptorLayout.textureRegister.size(); i++) {
				pair.second->descriptorLayout.textureRegister[i].binding = i;
				m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_Texture, pair.second->descriptorLayout.textureRegister[i]);
			}
			for (size_t i = 0; i < pair.second->descriptorLayout.samplerRegister.size(); i++) {
				pair.second->descriptorLayout.samplerRegister[i].binding = i;
				m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_SamplerState, pair.second->descriptorLayout.samplerRegister[i]);
			}
		}
		if (pair.second->stage() == ShaderStage2_Fragment) {
			for (size_t i = 0; i < pair.second->descriptorLayout.uniformBufferRegister.size(); i++) {
				pair.second->descriptorLayout.uniformBufferRegister[i].binding = maxVertexShaderBindingCounts[DescriptorType_UniformBuffer] + i;
				m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_UniformBuffer, pair.second->descriptorLayout.uniformBufferRegister[i]);
			}
			for (size_t i = 0; i < pair.second->descriptorLayout.textureRegister.size(); i++) {
				pair.second->descriptorLayout.textureRegister[i].binding = maxVertexShaderBindingCounts[DescriptorType_Texture] + i;
				m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_Texture, pair.second->descriptorLayout.textureRegister[i]);
			}
			for (size_t i = 0; i < pair.second->descriptorLayout.samplerRegister.size(); i++) {
				pair.second->descriptorLayout.samplerRegister[i].binding = maxVertexShaderBindingCounts[DescriptorType_SamplerState] + i;
				m_unifiedShader->addMergeDescriptorLayoutItem(DescriptorType_SamplerState, pair.second->descriptorLayout.samplerRegister[i]);
			}
		}
	}

	// Code を作る
	for (auto& pair : m_transpilerMap) {
		auto& tp = pair.second;

		LN_LOG_VERBOSE << "gen " << pair.first;

		if (!tp->mapIOAndGenerateSpirv(m_unifiedShader->descriptorLayout())) {
			return false;
		}

		{
			UnifiedShaderTriple triple = { "spv", 110, "" };
			if (!m_unifiedShader->hasCode(tp->stage(), tp->entryPoint(), triple)) {
				m_unifiedShader->setCode(tp->stage(), tp->entryPoint(), triple, tp->spirvCode(), tp->refrection());
			}
		}

		{
			UnifiedShaderTriple triple = { "glsl", 400, "" };
			if (!m_unifiedShader->hasCode(tp->stage(), tp->entryPoint(), triple)) {
				m_unifiedShader->setCode(tp->stage(), tp->entryPoint(), triple, tp->generateGlsl(400, false), makeRef<UnifiedShaderRefrectionInfo>());
			}
		}

		{
			UnifiedShaderTriple triple = { "glsl", 300, "es" };
			if (!m_unifiedShader->hasCode(tp->stage(), tp->entryPoint(), triple)) {
				m_unifiedShader->setCode(tp->stage(), tp->entryPoint(), triple, tp->generateGlsl(300, true), makeRef<UnifiedShaderRefrectionInfo>());
			}
		}
	}

	// まずは Code を作る
 //   for (auto& tech : metadataParser.techniques)
	//{
 //       for (auto& pass : tech.passes)
	//	{
 //           // Vertex shader
 //           {
	//			auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
	//			transpiler->compileAndLinkFromHlsl(ShaderCodeStage::Vertex, inputCode, inputCodeLength, pass.vertexShader, includeDirectories, &definitions, m_diag);
	//			if (m_diag->hasError()) {
	//				return false;
	//			}
	//			m_transpilerMap[pass.vertexShader] = transpiler;

	//			{
	//				UnifiedShaderTriple triple = { "spv", 110, "" };
	//				if (!m_unifiedShader->hasCode(pass.vertexShader, triple)) {
	//					m_unifiedShader->setCode(pass.vertexShader, triple, transpiler->spirvCode(), transpiler->refrection());
	//				}
	//			}

	//			{
	//				UnifiedShaderTriple triple = { "glsl", 400, "" };
	//				if (!m_unifiedShader->hasCode(pass.vertexShader, triple)) {
	//					m_unifiedShader->setCode(pass.vertexShader, triple, transpiler->generateGlsl(400, false), makeRef<UnifiedShaderRefrectionInfo>());
	//				}
	//			}

	//			{
	//				UnifiedShaderTriple triple = { "glsl", 300, "es" };
	//				if (!m_unifiedShader->hasCode(pass.vertexShader, triple)) {
	//					m_unifiedShader->setCode(pass.vertexShader, triple, transpiler->generateGlsl(300, true), makeRef<UnifiedShaderRefrectionInfo>());
	//				}
	//			}
 //           }

 //           // Pixel shader
 //           {
	//			auto transpiler = std::make_shared<ShaderCodeTranspiler>(m_manager);
	//			transpiler->compileAndLinkFromHlsl(ShaderCodeStage::Fragment, inputCode, inputCodeLength, pass.pixelShader, includeDirectories, &definitions, m_diag);
	//			if (m_diag->hasError()) {
	//				return false;
	//			}
	//			m_transpilerMap[pass.pixelShader] = transpiler;

	//			{
	//				UnifiedShaderTriple triple = { "spv", 110, "" };
	//				if (!m_unifiedShader->hasCode(pass.pixelShader, triple)) {
	//					m_unifiedShader->setCode(pass.pixelShader, triple, transpiler->spirvCode(), transpiler->refrection());
	//				}
	//			}

	//			{
	//				UnifiedShaderTriple triple = { "glsl", 400, "" };
	//				if (!m_unifiedShader->hasCode(pass.pixelShader, triple)) {
	//					m_unifiedShader->setCode(pass.pixelShader, triple, transpiler->generateGlsl(400, false), makeRef<UnifiedShaderRefrectionInfo>());
	//				}
	//			}

	//			{
	//				UnifiedShaderTriple triple = { "glsl", 300, "es" };
	//				if (!m_unifiedShader->hasCode(pass.pixelShader, triple)) {
	//					m_unifiedShader->setCode(pass.pixelShader, triple, transpiler->generateGlsl(300, true), makeRef<UnifiedShaderRefrectionInfo>());
	//				}
	//			}
 //           }
 //       }
 //   }

	// Tech と Pass を作る
	for (auto& tech : m_metadataParser.techniques)
	{
		UnifiedShader::TechniqueId techId;
		if (!m_unifiedShader->addTechnique(tech.name, &techId)) {
			return false;
		}

		for (auto& pass : tech.passes)
		{
			UnifiedShader::CodeContainerId codeId;
			UnifiedShader::PassId passId;
			if (!m_unifiedShader->addPass(techId, pass.name, &passId)) {
				return false;
			}

			// VertexShader
			if (!m_unifiedShader->findCodeContainer(ShaderStage2_Vertex, pass.vertexShader, &codeId)) {
				return false;
			}
			m_unifiedShader->setVertexShader(passId, codeId);

			// PixelShader
			if (!m_unifiedShader->findCodeContainer(ShaderStage2_Fragment, pass.pixelShader, &codeId)) {
				return false;
			}
			m_unifiedShader->setPixelShader(passId, codeId);

			// ShaderRenderState
			m_unifiedShader->setRenderState(passId, pass.renderState);

			// InputLayout
			m_unifiedShader->setAttributeSemantics(passId, m_transpilerMap[makeKey(ShaderStage2_Vertex, pass.vertexShader)]->attributes());

			// UniformBuffers
   //         auto refrection = makeRef<UnifiedShaderRefrectionInfo>();
			//ShaderUniformBufferInfo::mergeBuffers(
			//	m_transpilerMap[pass.vertexShader]->uniformBuffers(),
			//	m_transpilerMap[pass.pixelShader]->uniformBuffers(),
			//	&refrection->buffers);

			//m_unifiedShader->setRefrection(passId, refrection);
		}
	}

	return true;
}

std::string UnifiedShaderCompiler::makeKey(ShaderStage2 stage, const std::string& entryPoint)
{
	switch (stage)
	{
	case ShaderStage2_Vertex:
		return "1v:" + entryPoint;
	case ShaderStage2_Fragment:
		return "2p:" + entryPoint;
	default:
		LN_UNREACHABLE();
		return std::string();
	}
}

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

} // namespace detail
} // namespace ln
