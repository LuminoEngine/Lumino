#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShaderSerializer.hpp>

namespace ln {
namespace kokage {

void writeString(BinaryWriter* w, const std::string& str) {
    w->writeUInt32(str.length());
    w->write(str.data(), str.length());
}

void writeByteArray(BinaryWriter* w, const std::vector<uint8_t>& data) {
    w->writeUInt32(data.size());
    w->write(data.data(), data.size());
}

std::string readString(BinaryReader* r) {
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

std::vector<byte_t> readByteArray(BinaryReader* r) {
    uint32_t len = r->readUInt32();
    std::vector<byte_t> buf;
    buf.resize(len);
    r->read(buf.data(), len);
    return buf;
}

MaybeResult UnifiedShaderSerializer::saveToFile(const UnifiedShader2* shader, const std::filesystem::path& filePath) {
    auto stream = FileStream::create(Path::fromStdPath(filePath), FileOpenMode::Write | FileOpenMode::Truncate);
    auto writer = makeRef<BinaryWriter>(stream);

    // File header
    {
        writer->write("lcsh", 4); // Magic number
        writer->writeInt16(FileVersion_Current);
    }

    // Blob
    {
        writer->write("lcsh.bl.", 8); // Chunk signature

        const auto& blobs = shader->m_blobs;
        writer->writeInt16(blobs.size());
        for (int16_t i = 0; i < blobs.size(); i++) {
            Blob* data = blobs[i];
            writeByteArray(writer, data->data);
        }
    }

    // TargetEntryPoint
    {
        writer->write("lcsh.te.", 8); // Chunk signature

        const auto& entryPoints = shader->m_targetEntryPoints;
        writer->writeInt16(entryPoints.size());
        for (int16_t iTargetEntryPoint = 0; iTargetEntryPoint < entryPoints.size(); iTargetEntryPoint++) {
            TargetEntryPoint* entryPoint = entryPoints[iTargetEntryPoint];
            writer->writeInt8(static_cast<int8_t>(entryPoint->target));
            writeString(writer, entryPoint->name);
            writer->writeInt16(entryPoint->codeBlobId);
            // bindingLayout
            {
                const auto& values = entryPoint->bindingLayout.bindings;
                writer->writeInt16(values.size());
                for (int16_t iBinding = 0; iBinding < values.size(); iBinding++) {
                    const TargetBindingInfo& value = values[iBinding];
                    writeString(writer, value.name);
                    writer->writeInt8(static_cast<int8_t>(value.category));
                    writer->writeInt16(value.size);
                    writer->writeInt16(value.space);
                    writer->writeInt16(value.index);
                    writer->writeInt16(value.count);
                    writer->writeInt8(value.used);
                    writer->writeInt8(static_cast<int8_t>(value.descriptorEntryCategory));
                    writer->writeInt16(value.descriptorEntryIndex);
                    // members
                    {
                        const auto& members = value.members;
                        writer->writeInt16(members.size());
                        for (int16_t iMember = 0; iMember < members.size(); iMember++) {
                            const TargetBindingConstantBufferMemberInfo& member = members[iMember];
                            writeString(writer, member.name);
                            writer->writeInt16(member.offset);
                            writer->writeInt16(member.size);
                        }
                    }
                }
            }
            // inputAttributes
            {
                const auto& values = entryPoint->inputAttributes;
                writer->writeInt16(values.size());
                for (int16_t i = 0; i < values.size(); i++) {
                    const VertexInputAttribute& value = values[i];
                    writer->writeInt8(static_cast<int8_t>(value.usage));
                    writer->writeInt16(value.index);
                    writer->writeInt16(value.layoutLocation);
                }
            }
        }
    }

    // TargetShaderPass
    {
        writer->write("lcsh.tp.", 8); // Chunk signature

        const auto& targetShaderPasses = shader->m_targetShaderPasses;
        writer->writeInt16(targetShaderPasses.size());
        for (int16_t iTargetShaderPass = 0; iTargetShaderPass < targetShaderPasses.size(); iTargetShaderPass++) {
            TargetShaderPass* pass = targetShaderPasses[iTargetShaderPass];
            writer->writeInt16(pass->vertEntryPointId);
            writer->writeInt16(pass->fragEntryPointId);
            writer->writeInt16(pass->compEntryPointId);
            // bindingLayout
            {
                const auto& values = pass->bindingLayout.bindings;
                writer->writeInt16(values.size());
                for (int16_t iBinding = 0; iBinding < values.size(); iBinding++) {
                    const TargetBindingInfo& value = values[iBinding];
                    writeString(writer, value.name);
                    writer->writeInt8(static_cast<int8_t>(value.category));
                    writer->writeInt16(value.size);
                    writer->writeInt16(value.space);
                    writer->writeInt16(value.index);
                    writer->writeInt16(value.count);
                    writer->writeInt8(value.used);
                    writer->writeInt8(static_cast<int8_t>(value.descriptorEntryCategory));
                    writer->writeInt16(value.descriptorEntryIndex);
                    // members
                    {
                        const auto& members = value.members;
                        writer->writeInt16(members.size());
                        for (int16_t iMember = 0; iMember < members.size(); iMember++) {
                            const TargetBindingConstantBufferMemberInfo& member = members[iMember];
                            writeString(writer, member.name);
                            writer->writeInt16(member.offset);
                            writer->writeInt16(member.size);
                        }
                    }
                }
            }
        }
    }

    // GlobalShaderPass
    {
        writer->write("lcsh.gp.", 8); // Chunk signature

        const auto& globalShaderPasses = shader->m_globalShaderPasses;
        writer->writeInt16(globalShaderPasses.size());
        for (int16_t iGlobalShaderPass = 0; iGlobalShaderPass < globalShaderPasses.size(); iGlobalShaderPass++) {
            GlobalShaderPass* pass = globalShaderPasses[iGlobalShaderPass];
            writeString(writer, pass->name);
            writeString(writer, pass->vertexEntryPoint);
            writeString(writer, pass->fragmentEntryPoint);
            writeString(writer, pass->computeEntryPoint);
            // targetShaderPassIds
            {
                writer->writeInt16(pass->targetShaderPassIds.size());
                for (int16_t iTargetShaderPass = 0; iTargetShaderPass < pass->targetShaderPassIds.size();
                     iTargetShaderPass++) {
                    writer->writeInt8(static_cast<int8_t>(pass->targetShaderPassIds[iTargetShaderPass]));
                }
            }
            // descriptorLayout
            {
                const auto& values = pass->descriptorLayout.buffers;
                writer->writeInt16(values.size());
                for (int16_t iBinding = 0; iBinding < values.size(); iBinding++) {
                    writer->writeInt16(values[iBinding]);
                }
                const auto& values2 = pass->descriptorLayout.textures;
                writer->writeInt16(values2.size());
                for (int16_t iBinding = 0; iBinding < values2.size(); iBinding++) {
                    writer->writeInt16(values2[iBinding]);
                }
                const auto& values3 = pass->descriptorLayout.samplers;
                writer->writeInt16(values3.size());
                for (int16_t iBinding = 0; iBinding < values3.size(); iBinding++) {
                    writer->writeInt16(values3[iBinding]);
                }
                const auto& values4 = pass->descriptorLayout.storages;
                writer->writeInt16(values4.size());
                for (int16_t iBinding = 0; iBinding < values4.size(); iBinding++) {
                    writer->writeInt16(values4[iBinding]);
                }
            }
        }
    }

    // GlobalConstantBufferMember
    {
        writer->write("lcsh.gm.", 8); // Chunk signature

        const auto& globalMembers = shader->m_globalMembers;
        writer->writeInt16(globalMembers.size());
        for (int16_t i = 0; i < globalMembers.size(); i++) {
            GlobalConstantBufferMember* member = globalMembers[i];
            writeString(writer, member->name);
            writer->writeInt8(static_cast<int8_t>(member->type));
            writer->writeInt8(static_cast<int8_t>(member->kind));
            writer->writeInt16(member->arrayElements);
            writer->writeInt16(member->vectorElements);
            writer->writeInt16(member->matrixRows);
            writer->writeInt16(member->matrixColumns);
        }
    }

    // GlobalResourceLayout
    {
        writer->write("lcsh.gl.", 8); // Chunk signature

        const auto& globalResourceLayout = shader->m_globalResourceLayout;
        writer->writeInt16(globalResourceLayout->buffers.size());
        for (const auto& slot : globalResourceLayout->buffers) {
            writeString(writer, slot.name);
            writer->writeInt16(slot.constantBufferSize);
            writer->writeInt16(slot.arrayElementCount);
        }
        const auto& textures = globalResourceLayout->textures;
        writer->writeInt16(textures.size());
        for (const auto& slot : textures) {
            writeString(writer, slot.name);
            writer->writeInt16(slot.constantBufferSize);
            writer->writeInt16(slot.arrayElementCount);
        }
        const auto& samplers = globalResourceLayout->samplers;
        writer->writeInt16(samplers.size());
        for (const auto& slot : samplers) {
            writeString(writer, slot.name);
            writer->writeInt16(slot.constantBufferSize);
            writer->writeInt16(slot.arrayElementCount);
        }
        const auto& storages = globalResourceLayout->storages;
        writer->writeInt16(storages.size());
        for (const auto& slot : storages) {
            writeString(writer, slot.name);
            writer->writeInt16(slot.constantBufferSize);
            writer->writeInt16(slot.arrayElementCount);
        }
    }

    return LN_MAKE_SUCCESS();
}

Result<Ref<UnifiedShader2>> UnifiedShaderSerializer::loadFromData(const void* data, int32_t length) {
    //auto stream = FileStream::create(Path::fromStdPath(filePath), FileOpenMode::Read);
    MemoryStream stream(data, length);
    auto reader = makeRef<BinaryReader>(&stream);
    Ref<UnifiedShader2> shader = makeRef<UnifiedShader2>();

    // File header
    {
        auto r1 = checkSignature(reader, "lcsh", 4);
        if (!r1) return LN_TO_ERROR(r1);

        int fileVersion = reader->readInt16();
        if (fileVersion < FileVersion_1 || fileVersion >= FileVersion_Last) {
            return LN_MAKE_ERROR("Unsupported file version");
        }
    }

    // Blob
    {
        auto r1 = checkSignature(reader, "lcsh.bl.", 8);
        if (!r1) return LN_TO_ERROR(r1);

        int16_t count = reader->readInt16();
        for (int16_t i = 0; i < count; i++) {
            Blob* blob = shader->createBlob();
            blob->data = readByteArray(reader);
        }
    }

    // TargetEntryPoint
    {
        auto r1 = checkSignature(reader, "lcsh.te.", 8);
        if (!r1) return LN_TO_ERROR(r1);

        int16_t count = reader->readInt16();
        for (int16_t iTargetEntryPoint = 0; iTargetEntryPoint < count; iTargetEntryPoint++) {
            TargetEntryPoint* entryPoint = shader->createEntryPoint();
            entryPoint->target = static_cast<ShaderTarget>(reader->readInt8());
            entryPoint->name = readString(reader);
            entryPoint->codeBlobId = reader->readInt16();
            // bindingLayout
            {
                int16_t count = reader->readInt16();
                for (size_t iBinding = 0; iBinding < count; iBinding++) {
                    TargetBindingInfo info;
                    info.name = readString(reader);
                    info.category = static_cast<BindingResourceCategory>(reader->readInt8());
                    info.size = reader->readInt16();
                    info.space = reader->readInt16();
                    info.index = reader->readInt16();
                    info.count = reader->readInt16();
                    info.used = static_cast<ShaderStageFlags>(reader->readInt8());
                    info.descriptorEntryCategory = static_cast<RegisterCategory>(reader->readInt8());
                    info.descriptorEntryIndex = reader->readInt16();
                    // members
                    {
                        int16_t count = reader->readInt16();
                        for (int16_t iMember = 0; iMember < count; iMember++) {
                            TargetBindingConstantBufferMemberInfo member;
                            member.name = readString(reader);
                            member.offset = reader->readInt16();
                            member.size = reader->readInt16();
                            info.members.push_back(std::move(member));
                        }
                    }
                    entryPoint->bindingLayout.bindings.push_back(std::move(info));
                }
            }
            // inputAttributes
            {
                int16_t count = reader->readInt16();
                for (int16_t j = 0; j < count; j++) {
                    VertexInputAttribute attr;
                    attr.usage = static_cast<AttributeUsage>(reader->readUInt8());
                    attr.index = reader->readInt16();
                    attr.layoutLocation = reader->readInt16();
                    entryPoint->inputAttributes.push_back(attr);
                }
            }
        }
    }

    // TargetShaderPass
    {
        auto r1 = checkSignature(reader, "lcsh.tp.", 8);
        if (!r1) return LN_TO_ERROR(r1);

        int16_t count = reader->readInt16();
        for (int16_t iTargetShaderPass = 0; iTargetShaderPass < count; iTargetShaderPass++) {
            TargetShaderPass* pass = shader->createTargetShaderPass();
            pass->vertEntryPointId = reader->readInt16();
            pass->fragEntryPointId = reader->readInt16();
            pass->compEntryPointId = reader->readInt16();
            // bindingLayout
            {
                int16_t count = reader->readInt16();
                for (size_t iBinding = 0; iBinding < count; iBinding++) {
                    TargetBindingInfo info;
                    info.name = readString(reader);
                    info.category = static_cast<BindingResourceCategory>(reader->readInt8());
                    info.size = reader->readInt16();
                    info.space = reader->readInt16();
                    info.index = reader->readInt16();
                    info.count = reader->readInt16();
                    info.used = static_cast<ShaderStageFlags>(reader->readInt8());
                    info.descriptorEntryCategory = static_cast<RegisterCategory>(reader->readInt8());
                    info.descriptorEntryIndex = reader->readInt16();
                    // members
                    {
                        int16_t count = reader->readInt16();
                        for (int16_t iMember = 0; iMember < count; iMember++) {
                            TargetBindingConstantBufferMemberInfo member;
                            member.name = readString(reader);
                            member.offset = reader->readInt16();
                            member.size = reader->readInt16();
                            info.members.push_back(std::move(member));
                        }
                    }
                    pass->bindingLayout.bindings.push_back(std::move(info));
                }
            }
        }
    }
    
    // GlobalShaderPass
    {
        auto r1 = checkSignature(reader, "lcsh.gp.", 8);
        if (!r1) return LN_TO_ERROR(r1);

        int16_t count = reader->readInt16();
        for (int16_t iGlobalShaderPass = 0; iGlobalShaderPass < count; iGlobalShaderPass++) {
            GlobalShaderPass* pass = shader->createGlobalShaderPass();
            pass->name = readString(reader);
            pass->vertexEntryPoint = readString(reader);
            pass->fragmentEntryPoint = readString(reader);
            pass->computeEntryPoint = readString(reader);
            // targetShaderPassIds
            {
                int16_t count = reader->readInt16();
                pass->targetShaderPassIds.clear();
                for (int16_t iTargetShaderPass = 0; iTargetShaderPass < count; iTargetShaderPass++) {
                    pass->targetShaderPassIds.push_back(static_cast<TargetShaderPassId>(reader->readInt8()));
                }
            }
            // descriptorLayout
            {
                int16_t count = reader->readInt16();
                for (int16_t iBinding = 0; iBinding < count; iBinding++) {
                    pass->descriptorLayout.buffers.push_back(reader->readInt16());
                }
                int16_t count2 = reader->readInt16();
                for (int16_t iBinding = 0; iBinding < count2; iBinding++) {
                    pass->descriptorLayout.textures.push_back(reader->readInt16());
                }
                int16_t count3 = reader->readInt16();
                for (int16_t iBinding = 0; iBinding < count3; iBinding++) {
                    pass->descriptorLayout.samplers.push_back(reader->readInt16());
                }
                int16_t count4 = reader->readInt16();
                for (int16_t iBinding = 0; iBinding < count4; iBinding++) {
                    pass->descriptorLayout.storages.push_back(reader->readInt16());
                }
            }
        }
    }

    // GlobalConstantBufferMember
    {
        auto r1 = checkSignature(reader, "lcsh.gm.", 8);
        if (!r1) return LN_TO_ERROR(r1);

        int16_t count = reader->readInt16();
        for (int16_t i = 0; i < count; i++) {
            GlobalConstantBufferMember* member = shader->createGlobalConstantBufferMember();
            member->name = readString(reader);
            member->type = static_cast<ShaderGlobalMemberType>(reader->readInt8());
            member->kind = static_cast<ShaderGlobalMemberKind>(reader->readInt8());
            member->arrayElements = reader->readInt16();
            member->vectorElements = reader->readInt16();
            member->matrixRows = reader->readInt16();
            member->matrixColumns = reader->readInt16();
        }
    }

    // GlobalResourceLayout
    {
        auto r1 = checkSignature(reader, "lcsh.gl.", 8);
        if (!r1) return LN_TO_ERROR(r1);

        GlobalResourceLayout* globalResourceLayout = shader->globalResourceLayout();
        int16_t count = reader->readInt16();
        for (int16_t i = 0; i < count; i++) {
            GlobalResourceSlotInfo slot;
            slot.name = readString(reader);
            slot.constantBufferSize = reader->readInt16();
            slot.arrayElementCount = reader->readInt16();
            globalResourceLayout->buffers.push_back(std::move(slot));
        }
        int16_t count2 = reader->readInt16();
        for (int16_t i = 0; i < count2; i++) {
            GlobalResourceSlotInfo slot;
            slot.name = readString(reader);
            slot.constantBufferSize = reader->readInt16();
            slot.arrayElementCount = reader->readInt16();
            globalResourceLayout->textures.push_back(std::move(slot));
        }
        int16_t count3 = reader->readInt16();
        for (int16_t i = 0; i < count3; i++) {
            GlobalResourceSlotInfo slot;
            slot.name = readString(reader);
            slot.constantBufferSize = reader->readInt16();
            slot.arrayElementCount = reader->readInt16();
            globalResourceLayout->samplers.push_back(std::move(slot));
        }
        int16_t count4 = reader->readInt16();
        for (int16_t i = 0; i < count4; i++) {
            GlobalResourceSlotInfo slot;
            slot.name = readString(reader);
            slot.constantBufferSize = reader->readInt16();
            slot.arrayElementCount = reader->readInt16();
            globalResourceLayout->storages.push_back(std::move(slot));
        }
    }

    return shader;
}

MaybeResult UnifiedShaderSerializer::checkSignature(BinaryReader* r, const char* sig, size_t len) {
    char buf[8] = {};
    size_t size = r->read(buf, len);
    if (size != len || strncmp(buf, sig, len) != 0) {
        return LN_MAKE_ERROR("Invalid code container signature. (%s)", sig);
    }
    return LN_MAKE_SUCCESS();
}

} // namespace kokage
} // namespace ln
