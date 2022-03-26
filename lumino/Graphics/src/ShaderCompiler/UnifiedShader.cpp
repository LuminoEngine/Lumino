
#include "Internal.hpp"
#include <LuminoGraphics/ShaderCompiler/ShaderHelper.hpp>
#include <LuminoGraphics/ShaderCompiler/detail/UnifiedShader.hpp>
#include "HLSLMetadataParser.hpp"
#include "ShaderTranspiler.hpp"

namespace ln {
namespace kokage {

template<typename TValue>
static void writeOptionalUInt8(BinaryWriter* w, const Optional<TValue>& value) {
    w->writeUInt8(value.hasValue());
    if (value.hasValue())
        w->writeUInt8((uint8_t)value.value());
    else
        w->writeUInt8(0);
}

static void writeOptionalBool(BinaryWriter* w, const Optional<bool>& value) {
    w->writeUInt8(value.hasValue());
    if (value.hasValue())
        w->writeUInt8((value.value()) ? 1 : 0);
    else
        w->writeUInt8(0);
}

template<typename TValue>
static void readOptionalUInt8(BinaryReader* r, Optional<TValue>* outValue) {
    uint8_t has = r->readUInt8();
    uint8_t value = r->readUInt8();
    if (has) {
        *outValue = (TValue)value;
    }
}

static void readOptionalBool(BinaryReader* r, Optional<bool>* outValue) {
    uint8_t has = r->readUInt8();
    uint8_t value = r->readUInt8();
    if (has) {
        *outValue = value;
    }
}

//==============================================================================
// CodeContainer

CodeContainer::CodeContainer(CodeContainerId id)
    : m_id(id) {
}

void CodeContainer::setCode(const UnifiedShaderTriple& triple, const std::vector<byte_t>& code) {
    auto obj = makeURef<Code>();
    obj->triple = triple;
    obj->code = code;
    codes.push(std::move(obj));
}

const Code* CodeContainer::findCode(const UnifiedShaderTriple& triple) const {
    if (LN_REQUIRE(!triple.target.empty())) {
        return nullptr;
    }

    //auto& codes = m_codeContainers[idToIndex(conteinreId)].codes;

    int candidateVersion = 0;
    int candidate = -1;
    for (int iCode = 0; iCode < codes.size(); iCode++) {
        auto& codeTriple = codes[iCode]->triple;
        if (codeTriple.target != triple.target) {
            // not adopted
        }
        else if (!triple.option.empty() && codeTriple.option != triple.option) {
            // not adopted
        }
        else {
            // check version
            if (codeTriple.version <= triple.version && // first, less than requested version
                codeTriple.version > candidateVersion) {
                candidate = iCode;
                candidateVersion = codeTriple.version;
            }
        }
    }

    if (candidate >= 0) {
        return codes[candidate];
    }
    else {
        return nullptr;
    }
}

//==============================================================================
// UnifiedShaderTechnique

UnifiedShaderTechnique::UnifiedShaderTechnique(UnifiedShader* shader, UnifiedShaderTechniqueId id)
    : m_shader(shader)
    , m_id(id) {
}

//==============================================================================
// UnifiedShader

const String UnifiedShader::FileExt = _TT("lcfx");

UnifiedShader::UnifiedShader(DiagnosticsManager* diag)
    : m_diag(diag)
    , m_codeContainers()
    , m_techniques()
    , m_passes() {
}

UnifiedShader::~UnifiedShader() {
}

bool UnifiedShader::save(const Path& filePath) {
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
            CodeContainer* info = m_codeContainers[i];
            writeString(writer, info->entryPointName);

            writer->writeUInt8(info->codes.size());
            for (int iCode = 0; iCode < info->codes.size(); iCode++) {
                Code* codeInfo = info->codes[iCode];
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
            auto& info = m_techniques[i];
            writeString(writer, info->name);

            // variant
            {
                const auto& values = info->variantSet.values;
                writer->writeUInt32(values.size());
                for (int i = 0; i < values.size(); i++) {
                    writeString(writer, values[i]);
                }
            }

            // class
            {
                writer->writeUInt8(static_cast<uint8_t>(info->techniqueClass.defaultTechnique ? 1 : 0));
                writer->writeUInt8(static_cast<uint8_t>(info->techniqueClass.phase));
                writer->writeUInt8(static_cast<uint8_t>(info->techniqueClass.meshProcess));
                writer->writeUInt8(static_cast<uint8_t>(info->techniqueClass.shadingModel));
                writer->writeUInt8(static_cast<uint8_t>(info->techniqueClass.drawMode));
                writer->writeUInt8(static_cast<uint8_t>(info->techniqueClass.normalClass));
                writer->writeUInt8(static_cast<uint8_t>(info->techniqueClass.roughnessClass));
            }

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
            const auto& info = m_passes[i];
            writeString(writer, info->name);
            writer->writeUInt32(info->vertexShader);
            writer->writeUInt32(info->pixelShader);
            writer->writeUInt32(info->computeShader);

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

            // Descriptor layout
            {
                DescriptorLayout* descriptorLayout = &info->descriptorLayout;

                writer->writeUInt32(descriptorLayout->uniformBufferRegister.size());
                for (size_t i = 0; i < descriptorLayout->uniformBufferRegister.size(); i++) {
                    DescriptorLayoutItem* item = &descriptorLayout->uniformBufferRegister[i];
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

                writer->writeUInt32(descriptorLayout->textureRegister.size());
                for (size_t i = 0; i < descriptorLayout->textureRegister.size(); i++) {
                    DescriptorLayoutItem* item = &descriptorLayout->textureRegister[i];
                    writeString(writer, item->name);
                    writer->writeUInt8(item->stageFlags);
                    writer->writeUInt8(item->binding);
                }

                writer->writeUInt32(descriptorLayout->samplerRegister.size());
                for (size_t i = 0; i < descriptorLayout->samplerRegister.size(); i++) {
                    DescriptorLayoutItem* item = &descriptorLayout->samplerRegister[i];
                    writeString(writer, item->name);
                    writer->writeUInt8(item->stageFlags);
                    writer->writeUInt8(item->binding);
                }
            }

            // Attributes
            {
                const auto& attributes = info->attributes;
                writer->writeUInt32(attributes.size());
                for (size_t i = 0; i < attributes.size(); i++) {
                    writer->writeUInt8(attributes[i].usage);
                    writer->writeUInt8(attributes[i].index);
                    writer->writeUInt8(attributes[i].layoutLocation);
                }
            }
        }
    }

    return true;
}

bool UnifiedShader::load(Stream* stream) {
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
            auto info = makeURef<CodeContainer>(indexToId(i));
            info->entryPointName = readString(reader);

            uint8_t count = reader->readUInt8();
            for (int iCode = 0; iCode < count; iCode++) {
                auto code = makeURef<Code>();
                code->triple.target = readString(reader);
                code->triple.version = reader->readUInt32();
                code->triple.option = readString(reader);
                code->code = readByteArray(reader);
                info->codes.push(std::move(code));
            }

            m_codeContainers.push(std::move(info));
        }
    }

    // Technique
    {
        if (!checkSignature(reader, "lufx.t..", 8, m_diag)) {
            return false;
        }

        size_t count = reader->readUInt32();
        for (size_t iTech = 0; iTech < count; iTech++) {
            auto info = makeURef<UnifiedShaderTechnique>(this, indexToId(iTech));
            info->name = readString(reader);

            // variant
            if (fileVersion >= FileVersion_6) {
                size_t count = reader->readUInt32();
                for (size_t i = 0; i < count; i++) {
                    info->variantSet.values.push_back(readString(reader));
                }
            }

            // class
            if (fileVersion >= FileVersion_4) {
                info->techniqueClass.defaultTechnique = reader->readUInt8() != 0;
                info->techniqueClass.phase = static_cast<ShaderTechniqueClass_Phase>(reader->readUInt8());
                info->techniqueClass.meshProcess = static_cast<ShaderTechniqueClass_MeshProcess>(reader->readUInt8());
                info->techniqueClass.shadingModel = static_cast<ShaderTechniqueClass_ShadingModel>(reader->readUInt8());
                info->techniqueClass.drawMode = static_cast<ShaderTechniqueClass_DrawMode>(reader->readUInt8());
                info->techniqueClass.normalClass = static_cast<ShaderTechniqueClass_Normal>(reader->readUInt8());
                info->techniqueClass.roughnessClass = static_cast<ShaderTechniqueClass_Roughness>(reader->readUInt8());
            }

            // passes
            size_t passCount = reader->readUInt32();
            for (size_t iPass = 0; iPass < passCount; iPass++) {
                info->passes.push(reader->readUInt32());
            }

            m_techniques.push(std::move(info));
        }
    }

    // Pass
    {
        if (!checkSignature(reader, "lufx.p..", 8, m_diag)) {
            return false;
        }

        size_t count = reader->readUInt32();
        for (size_t i = 0; i < count; i++) {
            auto info = makeURef<UnifiedShaderPass>();
            info->name = readString(reader);
            info->vertexShader = reader->readUInt32();
            info->pixelShader = reader->readUInt32();

            if (fileVersion >= FileVersion_5)
                info->computeShader = reader->readUInt32();
            else
                info->computeShader = 0;

            // ShaderRenderState
            {
                auto renderState = makeRef<ShaderRenderState>();
                info->renderState = renderState;

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

            // Descriptor layout
            {
                DescriptorLayout* descriptorLayout = &info->descriptorLayout;

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

                        descriptorLayout->uniformBufferRegister.push_back(item);
                    }
                }

                {
                    size_t count = reader->readUInt32();
                    for (size_t i = 0; i < count; i++) {
                        DescriptorLayoutItem item;
                        item.name = readString(reader);
                        item.stageFlags = reader->readUInt8();
                        item.binding = reader->readUInt8();
                        descriptorLayout->textureRegister.push_back(item);
                    }
                }

                {
                    size_t count = reader->readUInt32();
                    for (size_t i = 0; i < count; i++) {
                        DescriptorLayoutItem item;
                        item.name = readString(reader);
                        item.stageFlags = reader->readUInt8();
                        item.binding = reader->readUInt8();
                        descriptorLayout->samplerRegister.push_back(item);
                    }
                }
            }

            // Attributes
            if (fileVersion >= FileVersion_3) {
                size_t count = reader->readUInt32();
                for (size_t i = 0; i < count; i++) {
                    VertexInputAttribute attr;
                    attr.usage = static_cast<AttributeUsage>(reader->readUInt8());
                    attr.index = reader->readUInt8();
                    attr.layoutLocation = reader->readUInt8();
                    info->attributes.push_back(attr);
                }
            }
            else {
                // LN_VSInput format
                info->attributes = {
                    { AttributeUsage_Position, 0, 0 },
                    { AttributeUsage_Normal, 0, 1 },
                    { AttributeUsage_TexCoord, 0, 2 },
                    { AttributeUsage_Color, 0, 3 },
                    { AttributeUsage_BlendWeight, 0, 4 },
                    { AttributeUsage_BlendIndices, 0, 5 },
                };
            }

            m_passes.push(std::move(info));
        }
    }

    makeGlobalDescriptorLayout();

    return true;
}

CodeContainer* UnifiedShader::addCodeContainer(ShaderStage2 stage, const std::string& entryPointName) {
    auto obj = makeURef<CodeContainer>(indexToId(m_codeContainers.length()));
    obj->stage = stage;
    obj->entryPointName = entryPointName;
    m_codeContainers.push(std::move(obj));
    return m_codeContainers.back();
}

void UnifiedShader::makeGlobalDescriptorLayout() {
    m_globalDescriptorLayout.clear();
    for (const auto& pass : m_passes) {
        m_globalDescriptorLayout.mergeFrom(pass->descriptorLayout);
    }
}

UnifiedShaderPass* UnifiedShader::addPass(TechniqueId parentTech, const std::string& name) {
    if (findPassInfoIndex(parentTech, name) >= 0) {
        m_diag->reportError(String::fromStdString("Pass '" + name + "' in '" + m_techniques[idToIndex(parentTech)]->name + "' is already exists."));
        return nullptr;
    }

    auto pass = makeURef<UnifiedShaderPass>();
    pass->id = indexToId(m_passes.length());
    pass->name = name;

    m_techniques[idToIndex(parentTech)]->passes.push(pass->id);

    m_passes.push(std::move(pass));
    return m_passes.back();
}

void UnifiedShader::addMergeDescriptorLayoutItem(UnifiedShaderPass* pass, const DescriptorLayout& layout) {
    DescriptorLayout& descriptorLayout = pass->descriptorLayout;
    descriptorLayout.mergeFrom(layout);

    // Apply global
    m_globalDescriptorLayout.mergeFrom(descriptorLayout);
}

UnifiedShaderTechnique* UnifiedShader::addTechnique(const std::string& name, const ShaderTechniqueClass& techniqueClass) {
    if (findTechniqueInfoIndex(name) >= 0) {
        m_diag->reportError(String::fromStdString("Technique '" + name + "' is already exists."));
        return false;
    }

    auto tech = makeURef<UnifiedShaderTechnique>(this, indexToId(m_techniques.length()));
    tech->name = name;
    tech->techniqueClass = techniqueClass;
    m_techniques.push(std::move(tech));
    return m_techniques.back();
}

UnifiedShaderTechnique* UnifiedShader::addVariantTechnique(const std::string& name, const VariantSet& variantSet) {
    auto tech = makeURef<UnifiedShaderTechnique>(this, indexToId(m_techniques.length()));
    tech->name = name;
    tech->variantSet = variantSet;
    m_techniques.push(std::move(tech));
    return m_techniques.back();
}

//bool UnifiedShader::addPass(TechniqueId parentTech, const std::string& name, PassId* outPass)
//{
//
//    PassInfo info;
//    info.name = name;
//    info.vertexShader = 0;
//    info.pixelShader = 0;
//    info.computeShader = 0;
//    //info.refrection = makeRef<UnifiedShaderRefrectionInfo>();
//
//    m_passes.add(std::move(info));
//    int index = m_passes.size() - 1;
//    m_techniques[idToIndex(parentTech)].passes.add(indexToId(index));
//    *outPass = indexToId(index);
//    return true;
//}

//int UnifiedShader::getPassCountInTechnique(TechniqueId parentTech) const {
//    return m_techniques[idToIndex(parentTech)].passes.size();
//}
//
//UnifiedShader::PassId UnifiedShader::getPassIdInTechnique(TechniqueId parentTech, int index) const {
//    return m_techniques[idToIndex(parentTech)].passes[index];
//}
//
//void UnifiedShader::setVertexShader(PassId pass, CodeContainerId code)
//{
//    m_passes[idToIndex(pass)].vertexShader = code;
//}
//
//void UnifiedShader::setPixelShader(PassId pass, CodeContainerId code)
//{
//    m_passes[idToIndex(pass)].pixelShader = code;
//}
//
//void UnifiedShader::setComputeShader(PassId pass, CodeContainerId code)
//{
//    m_passes[idToIndex(pass)].computeShader = code;
//}
//
//void UnifiedShader::setRenderState(PassId pass, ShaderRenderState* state)
//{
//    m_passes[idToIndex(pass)].renderState = state;
//}

//void UnifiedShader::addMergeDescriptorLayoutItem(PassId pass, const DescriptorLayout& layout) {
//    DescriptorLayout& descriptorLayout = m_passes[idToIndex(pass)]->descriptorLayout;
//    descriptorLayout.mergeFrom(layout);
//
//    // Apply global
//    m_globalDescriptorLayout.mergeFrom(descriptorLayout);
//}

//void UnifiedShader::setRefrection(PassId pass, UnifiedShaderRefrectionInfo* buffers)
//{
//	m_passes[idToIndex(pass)].refrection = buffers;
//}

//CodeContainerId UnifiedShader::vertexShader(PassId pass) const
//{
//    return m_passes[idToIndex(pass)].vertexShader;
//}
//
//CodeContainerId UnifiedShader::pixelShader(PassId pass) const
//{
//    return m_passes[idToIndex(pass)].pixelShader;
//}
//
//CodeContainerId UnifiedShader::computeShader(PassId pass) const
//{
//    return m_passes[idToIndex(pass)].computeShader;
//}
//
//ShaderRenderState* UnifiedShader::renderState(PassId pass) const
//{
//    return m_passes[idToIndex(pass)].renderState;
//}
//
//const DescriptorLayout& UnifiedShader::descriptorLayout(PassId pass) const
//{
//	return m_passes[idToIndex(pass)].descriptorLayout;
//}

//void UnifiedShader::setAttributes(PassId pass, const std::vector<VertexInputAttribute>& attrs)
//{
//    m_passes[idToIndex(pass)].attributes = attrs;
//}
//
//const std::vector<VertexInputAttribute>& UnifiedShader::attributes(PassId pass) const
//{
//	return m_passes[idToIndex(pass)].attributes;
//}

void UnifiedShader::saveCodes(const StringView& perfix) const {
    for (const auto& container : m_codeContainers) {
        for (auto& code : container->codes) {
            auto file = ln::format(
                _TT("{}container.{}.{}-{}-{}"),
                perfix,
                String::fromNumber(container->id()),
                String::fromStdString(code->triple.target),
                code->triple.version,
                String::fromStdString(code->triple.option));
            FileSystem::writeAllBytes(file, code->code.data(), code->code.size());
        }
    }

    //for (int iTech = 0; iTech < m_techniques.length(); iTech++) {
    //    const auto& tech = m_techniques[iTech];

    //    for (int iPass = 0; iPass < tech->passes.length(); iPass++) {
    //        UnifiedShader::PassId passId = tech->passes[iPass];
    //        auto* pass = this->pass(passId);
    //        CodeContainerId containerIds[] = { pass->vertexShader, pass->pixelShader };
    //        for (auto containerId : containerIds) {
    //            auto& container = m_codeContainers[idToIndex(containerId)];
    //            for (auto& code : container->codes) {
    //                auto file = ln::format(
    //                    _TT("{0}.{1}.{2}.{3}.{4}-{5}-{6}"),
    //                    perfix,
    //                    String::fromStdString(tech->name),
    //                    String::fromStdString(pass->name),
    //                    String::fromStdString(container->entryPointName),
    //                    String::fromStdString(code->triple.target),
    //                    code->triple.version,
    //                    String::fromStdString(code->triple.option));
    //                FileSystem::writeAllBytes(file, code->code.data(), code->code.size());
    //            }
    //        }
    //    }
    //}

    //for (auto& container : m_codeContainers) {
    //    for (auto& code : container.codes) {
    //        auto file = String::format(_TT("{0}-{1}-{2}-{3}-{4}", perfix, String::fromStdString(container.entryPointName), String::fromStdString(code.triple.target), code.triple.version, String::fromStdString(code.triple.option));
    //        FileSystem::writeAllBytes(file, code.code.data(), code.code.size());
    //    }
    //}
}

//UnifiedShaderRefrectionInfo* UnifiedShader::refrection(PassId pass) const
//{
//    return m_passes[idToIndex(pass)].refrection;
//}

//int UnifiedShader::findCodeContainerInfoIndex(ShaderStage2 stage, const std::string& entryPointName) const
//{
//    return m_codeContainers.indexOfIf([&](const CodeContainerInfo& info) { return info.stage == stage && info.entryPointName == entryPointName; });
//}

int UnifiedShader::findTechniqueInfoIndex(const std::string& name) const {
    return m_techniques.indexOfIf([&](const auto& info) { return info->name == name; });
}

int UnifiedShader::findPassInfoIndex(TechniqueId tech, const std::string& name) const {
    auto& t = m_techniques[idToIndex(tech)];
    if (t->hasVariant()) return -1;

    for (auto& passId : t->passes) {
        int index = idToIndex(passId);
        if (m_passes[index]->name == name) {
            return index;
        }
    }
    return -1;
}

void UnifiedShader::writeString(BinaryWriter* w, const std::string& str) {
    w->writeUInt32(str.length());
    w->write(str.data(), str.length());
}

void UnifiedShader::writeByteArray(BinaryWriter* w, const std::vector<byte_t>& data) {
    w->writeUInt32(data.size());
    w->write(data.data(), data.size());
}

std::string UnifiedShader::readString(BinaryReader* r) {
    uint32_t len = r->readUInt32();
    if (len == 0) {
        return std::string();
    }
    else if (len <= 255) { // min str optimaize
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

std::vector<byte_t> UnifiedShader::readByteArray(BinaryReader* r) {
    uint32_t len = r->readUInt32();
    std::vector<byte_t> buf;
    buf.resize(len);
    r->read(buf.data(), len);
    return buf;
}

bool UnifiedShader::checkSignature(BinaryReader* r, const char* sig, size_t len, DiagnosticsManager* diag) {
    char buf[8];
    size_t size = r->read(buf, len);
    if (size != len || strncmp(buf, sig, len) != 0) {
        diag->reportError(_TT("Invalid code container signature. (") + String::fromCString(sig) + _TT(")"));
        return false;
    }
    return true;
}

//==============================================================================
// DescriptorLayout

void DescriptorLayout::clear() {
    uniformBufferRegister.clear();
    unorderdRegister.clear();
    textureRegister.clear();
    samplerRegister.clear();
}

std::vector<DescriptorLayoutItem>& DescriptorLayout::getLayoutItems(DescriptorType registerType) {
    switch (registerType) {
        case DescriptorType_UniformBuffer:
            return uniformBufferRegister;
        case DescriptorType_UnorderdAccess:
            return unorderdRegister;
        case DescriptorType_Texture:
            return textureRegister;
        case DescriptorType_SamplerState:
            return samplerRegister;
        default:
            LN_UNREACHABLE();
            return uniformBufferRegister;
    }
}

const std::vector<DescriptorLayoutItem>& DescriptorLayout::getLayoutItems(DescriptorType registerType) const {
    switch (registerType) {
        case DescriptorType_UniformBuffer:
            return uniformBufferRegister;
        case DescriptorType_UnorderdAccess:
            return unorderdRegister;
        case DescriptorType_Texture:
            return textureRegister;
        case DescriptorType_SamplerState:
            return samplerRegister;
        default:
            LN_UNREACHABLE();
            return uniformBufferRegister;
    }
}

bool DescriptorLayout::isReferenceFromVertexStage(DescriptorType registerType) const {
    auto& items = getLayoutItems(registerType);
    auto itr = std::find_if(items.begin(), items.end(), [](const DescriptorLayoutItem& x) { return (x.stageFlags & ShaderStageFlags_Vertex) != 0; });
    return itr != items.end();
}

bool DescriptorLayout::isReferenceFromPixelStage(DescriptorType registerType) const {
    auto& items = getLayoutItems(registerType);
    auto itr = std::find_if(items.begin(), items.end(), [](const DescriptorLayoutItem& x) { return (x.stageFlags & ShaderStageFlags_Pixel) != 0; });
    return itr != items.end();
}

bool DescriptorLayout::isReferenceFromComputeStage(DescriptorType registerType) const {
    auto& items = getLayoutItems(registerType);
    auto itr = std::find_if(items.begin(), items.end(), [](const DescriptorLayoutItem& x) { return (x.stageFlags & ShaderStageFlags_Compute) != 0; });
    return itr != items.end();
}

LayoutSlotIndex DescriptorLayout::findUniformBufferRegisterIndex(const std::string& name) const {
    for (int i = 0; i < uniformBufferRegister.size(); i++) {
        if (uniformBufferRegister[i].name == name) return LayoutSlotIndex(i);
    }
    return LayoutSlotIndex(-1);
}

LayoutSlotIndex DescriptorLayout::findUnorderdRegisterIndex(const std::string& name) const {
    for (int i = 0; i < unorderdRegister.size(); i++) {
        if (unorderdRegister[i].name == name) return LayoutSlotIndex(i);
    }
    return LayoutSlotIndex(-1);
}

LayoutSlotIndex DescriptorLayout::findTextureRegisterIndex(const std::string& name) const {
    for (int i = 0; i < textureRegister.size(); i++) {
        if (textureRegister[i].name == name) return LayoutSlotIndex(i);
    }
    return LayoutSlotIndex(-1);
}

LayoutSlotIndex DescriptorLayout::findSamplerRegisterIndex(const std::string& name) const {
    for (int i = 0; i < samplerRegister.size(); i++) {
        if (samplerRegister[i].name == name) return LayoutSlotIndex(i);
    }
    return LayoutSlotIndex(-1);
}

int DescriptorLayout::findUniformBufferMemberOffset(const std::string& name) const {
    for (int i = 0; i < uniformBufferRegister.size(); i++) {
        for (int j = 0; j < uniformBufferRegister[i].members.size(); j++) {
            if (uniformBufferRegister[i].members[j].name == name) return uniformBufferRegister[i].members[j].offset;
        }
    }
    return -1;
}

// VertexShader と PixelShader の合計2回呼ばれる。
// 片方だけだと使われていない Uniform が inactive になっていたりするが、マージすることで、 ShaderPass 全体として何が必要なのかが明らかになる。
void DescriptorLayout::mergeFrom(const DescriptorLayout& other) {
    for (int iType = 0; iType < DescriptorType_Count; iType++) {

        std::vector<DescriptorLayoutItem>* list = &getLayoutItems((DescriptorType)iType);
        const std::vector<DescriptorLayoutItem>& srcList = other.getLayoutItems((DescriptorType)iType);

        for (auto& item : srcList) {
            auto itr = std::find_if(list->begin(), list->end(), [&](const DescriptorLayoutItem& x) { return x.name == item.name; });
            if (itr != list->end()) {
                itr->stageFlags |= item.stageFlags;

                // Merge members
                for (auto& m : item.members) {
                    auto itr2 = std::find_if(itr->members.begin(), itr->members.end(), [&](const ShaderUniformInfo& x) { return x.name == m.name; });
                    if (itr2 == itr->members.end()) {
                        itr->members.push_back(m);
                    }
                }
            }
            else {
                list->push_back(item);
                list->back().binding = list->size() - 1;
            }
        }
    }
}

} // namespace kokage
} // namespace ln
