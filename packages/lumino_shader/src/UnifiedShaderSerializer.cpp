// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include "pch.hpp"
#include <lumino_shader/UnifiedShader.hpp>
#include <lumino_shader/UnifiedShaderSerializer.hpp>

namespace ln {
namespace shader {

namespace {

//----------------------------------------------------------------------
// Thin binary writer/reader over std streams

class BinaryWriter final {
public:
    explicit BinaryWriter(std::ostream& s) : m_stream(s) {}

    void write(const void* data, size_t size) {
        m_stream.write(static_cast<const char*>(data), size);
    }
    void writeInt8(int8_t v)   { write(&v, 1); }
    void writeUInt8(uint8_t v) { write(&v, 1); }
    void writeInt16(int16_t v) { write(&v, 2); }
    void writeUInt32(uint32_t v) { write(&v, 4); }

private:
    std::ostream& m_stream;
};

class BinaryReader final {
public:
    BinaryReader(const uint8_t* data, size_t length)
        : m_data(data), m_length(length), m_pos(0) {}

    size_t read(void* buf, size_t size) {
        size_t avail = (m_pos + size <= m_length) ? size : (m_length - m_pos);
        memcpy(buf, m_data + m_pos, avail);
        m_pos += avail;
        return avail;
    }
    int8_t readInt8()    { int8_t v;  read(&v, 1); return v; }
    uint8_t readUInt8()  { uint8_t v; read(&v, 1); return v; }
    int16_t readInt16()  { int16_t v; read(&v, 2); return v; }
    uint32_t readUInt32(){ uint32_t v; read(&v, 4); return v; }

private:
    const uint8_t* m_data;
    size_t m_length;
    size_t m_pos;
};

//----------------------------------------------------------------------
// String / byte窶疎rray helpers

void writeString(BinaryWriter& w, const std::string& str) {
    w.writeUInt32(static_cast<uint32_t>(str.length()));
    w.write(str.data(), str.length());
}

void writeByteArray(BinaryWriter& w, const std::vector<uint8_t>& data) {
    w.writeUInt32(static_cast<uint32_t>(data.size()));
    w.write(data.data(), data.size());
}

std::string readString(BinaryReader& r) {
    uint32_t len = r.readUInt32();
    if (len == 0) {
        return std::string();
    }
    else if (len <= 255) {
        char buf[255] = { 0 };
        r.read(buf, len);
        return std::string(buf, len);
    }
    else {
        std::vector<char> buf(len);
        r.read(buf.data(), len);
        return std::string(buf.begin(), buf.end());
    }
}

std::vector<uint8_t> readByteArray(BinaryReader& r) {
    uint32_t len = r.readUInt32();
    std::vector<uint8_t> buf(len);
    r.read(buf.data(), len);
    return buf;
}

VoidResult checkSignature(BinaryReader& r, const char* sig, size_t len) {
    char buf[8] = {};
    size_t size = r.read(buf, len);
    if (size != len || strncmp(buf, sig, len) != 0) {
        return LNSHADER_MAKE_ERROR(std::string("Invalid code container signature. (") + sig + ")");
    }
    return LNSHADER_OK();
}

//----------------------------------------------------------------------
// Binding write/read helpers

void writeBindingLayout(BinaryWriter& writer, const TargetBindingLayoutInfo& layout) {
    const auto& values = layout.bindings;
    writer.writeInt16(static_cast<int16_t>(values.size()));
    for (size_t iBinding = 0; iBinding < values.size(); iBinding++) {
        const TargetBindingInfo& value = values[iBinding];
        writeString(writer, value.name);
        writer.writeInt8(static_cast<int8_t>(value.category));
        writer.writeInt16(value.size);
        writer.writeInt16(value.space);
        writer.writeInt16(value.index);
        writer.writeInt16(value.count);
        writer.writeInt8(static_cast<int8_t>(value.used));
        writer.writeInt8(static_cast<int8_t>(value.descriptorEntryCategory));
        writer.writeInt16(value.descriptorEntryIndex);
        // members
        {
            const auto& members = value.members;
            writer.writeInt16(static_cast<int16_t>(members.size()));
            for (size_t iMember = 0; iMember < members.size(); iMember++) {
                const TargetBindingConstantBufferMemberInfo& member = members[iMember];
                writeString(writer, member.name);
                writer.writeInt16(member.offset);
                writer.writeInt16(member.size);
            }
        }
    }
}

void readBindingLayout(BinaryReader& reader, TargetBindingLayoutInfo& layout) {
    int16_t count = reader.readInt16();
    for (int16_t iBinding = 0; iBinding < count; iBinding++) {
        TargetBindingInfo info;
        info.name = readString(reader);
        info.category = static_cast<BindingResourceCategory>(reader.readInt8());
        info.size = reader.readInt16();
        info.space = reader.readInt16();
        info.index = reader.readInt16();
        info.count = reader.readInt16();
        info.used = static_cast<ShaderStageFlags>(reader.readInt8());
        info.descriptorEntryCategory = static_cast<RegisterCategory>(reader.readInt8());
        info.descriptorEntryIndex = reader.readInt16();
        // members
        {
            int16_t mcount = reader.readInt16();
            for (int16_t iMember = 0; iMember < mcount; iMember++) {
                TargetBindingConstantBufferMemberInfo member;
                member.name = readString(reader);
                member.offset = reader.readInt16();
                member.size = reader.readInt16();
                info.members.push_back(std::move(member));
            }
        }
        layout.bindings.push_back(std::move(info));
    }
}

} // anonymous namespace

//----------------------------------------------------------------------
// UnifiedShaderSerializer

VoidResult UnifiedShaderSerializer::saveToFile(const UnifiedShader* shader, const std::filesystem::path& filePath) {
    std::ofstream fs(filePath, std::ios::binary | std::ios::trunc);
    if (!fs) {
        return LNSHADER_MAKE_ERROR("Failed to open file for writing: " + filePath.string());
    }
    BinaryWriter writer(fs);

    // File header
    {
        writer.write("lcsh", 4);
        writer.writeInt16(FileVersion_Current);
    }

    // Blob
    {
        writer.write("lcsh.bl.", 8);
        const auto& blobs = shader->m_blobs;
        writer.writeInt16(static_cast<int16_t>(blobs.size()));
        for (size_t i = 0; i < blobs.size(); i++) {
            Blob* data = blobs[i].get();
            writeByteArray(writer, data->data);
        }
    }

    // TargetEntryPoint
    {
        writer.write("lcsh.te.", 8);
        const auto& entryPoints = shader->m_targetEntryPoints;
        writer.writeInt16(static_cast<int16_t>(entryPoints.size()));
        for (size_t iTargetEntryPoint = 0; iTargetEntryPoint < entryPoints.size(); iTargetEntryPoint++) {
            TargetEntryPoint* entryPoint = entryPoints[iTargetEntryPoint].get();
            writer.writeInt8(static_cast<int8_t>(entryPoint->target));
            writeString(writer, entryPoint->name);
            writer.writeInt16(entryPoint->codeBlobId);
            writeBindingLayout(writer, entryPoint->bindingLayout);
            // inputAttributes
            {
                const auto& values = entryPoint->inputAttributes;
                writer.writeInt16(static_cast<int16_t>(values.size()));
                for (size_t i = 0; i < values.size(); i++) {
                    const VertexInputAttribute& value = values[i];
                    writer.writeInt8(static_cast<int8_t>(value.usage));
                    writer.writeInt16(static_cast<int16_t>(value.index));
                    writer.writeInt16(static_cast<int16_t>(value.layoutLocation));
                }
            }
        }
    }

    // TargetShaderPass
    {
        writer.write("lcsh.tp.", 8);
        const auto& targetShaderPasses = shader->m_targetShaderPasses;
        writer.writeInt16(static_cast<int16_t>(targetShaderPasses.size()));
        for (size_t iTargetShaderPass = 0; iTargetShaderPass < targetShaderPasses.size(); iTargetShaderPass++) {
            TargetShaderPass* pass = targetShaderPasses[iTargetShaderPass].get();
            writer.writeInt16(pass->vertEntryPointId);
            writer.writeInt16(pass->fragEntryPointId);
            writer.writeInt16(pass->compEntryPointId);
            writeBindingLayout(writer, pass->bindingLayout);
        }
    }

    // GlobalShaderPass
    {
        writer.write("lcsh.gp.", 8);
        const auto& globalShaderPasses = shader->m_globalShaderPasses;
        writer.writeInt16(static_cast<int16_t>(globalShaderPasses.size()));
        for (size_t iGlobalShaderPass = 0; iGlobalShaderPass < globalShaderPasses.size(); iGlobalShaderPass++) {
            GlobalShaderPass* pass = globalShaderPasses[iGlobalShaderPass].get();
            writeString(writer, pass->name);
            writeString(writer, pass->vertexEntryPoint);
            writeString(writer, pass->fragmentEntryPoint);
            writeString(writer, pass->computeEntryPoint);
            // targetShaderPassIds
            {
                writer.writeInt16(static_cast<int16_t>(pass->targetShaderPassIds.size()));
                for (size_t i = 0; i < pass->targetShaderPassIds.size(); i++) {
                    writer.writeInt8(static_cast<int8_t>(pass->targetShaderPassIds[i]));
                }
            }
            // descriptorLayout
            {
                const auto& dl = pass->descriptorLayout;
                writer.writeInt16(static_cast<int16_t>(dl.buffers.size()));
                for (auto v : dl.buffers) writer.writeInt16(v);
                writer.writeInt16(static_cast<int16_t>(dl.textures.size()));
                for (auto v : dl.textures) writer.writeInt16(v);
                writer.writeInt16(static_cast<int16_t>(dl.samplers.size()));
                for (auto v : dl.samplers) writer.writeInt16(v);
                writer.writeInt16(static_cast<int16_t>(dl.storages.size()));
                for (auto v : dl.storages) writer.writeInt16(v);
            }
        }
    }

    // GlobalConstantBufferMember
    {
        writer.write("lcsh.gm.", 8);
        const auto& globalMembers = shader->m_globalMembers;
        writer.writeInt16(static_cast<int16_t>(globalMembers.size()));
        for (size_t i = 0; i < globalMembers.size(); i++) {
            GlobalConstantBufferMember* member = globalMembers[i].get();
            writeString(writer, member->name);
            writer.writeInt8(static_cast<int8_t>(member->type));
            writer.writeInt8(static_cast<int8_t>(member->kind));
            writer.writeInt16(static_cast<int16_t>(member->arrayElements));
            writer.writeInt16(static_cast<int16_t>(member->vectorElements));
            writer.writeInt16(static_cast<int16_t>(member->matrixRows));
            writer.writeInt16(static_cast<int16_t>(member->matrixColumns));
        }
    }

    // GlobalResourceLayout
    {
        writer.write("lcsh.gl.", 8);
        const auto& grl = shader->m_globalResourceLayout;
        auto writeSlots = [&](const std::vector<GlobalResourceSlotInfo>& slots) {
            writer.writeInt16(static_cast<int16_t>(slots.size()));
            for (const auto& slot : slots) {
                writeString(writer, slot.name);
                writer.writeInt16(slot.constantBufferSize);
                writer.writeInt16(slot.arrayElementCount);
            }
        };
        writeSlots(grl->buffers);
        writeSlots(grl->textures);
        writeSlots(grl->samplers);
        writeSlots(grl->storages);
    }

    return LNSHADER_OK();
}

Result<Ref<UnifiedShader>> UnifiedShaderSerializer::loadFromData(const void* data, size_t length) {
    BinaryReader reader(static_cast<const uint8_t*>(data), length);
    Ref<UnifiedShader> shader = Ref<UnifiedShader>::adopt(new UnifiedShader());

    // File header
    {
        auto r1 = checkSignature(reader, "lcsh", 4);
        if (!r1) return tl::make_unexpected(r1.error());

        int fileVersion = reader.readInt16();
        if (fileVersion < FileVersion_1 || fileVersion >= FileVersion_Last) {
            return LNSHADER_MAKE_ERROR("Unsupported file version");
        }
    }

    // Blob
    {
        auto r1 = checkSignature(reader, "lcsh.bl.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t i = 0; i < count; i++) {
            Blob* blob = shader->createBlob();
            blob->data = readByteArray(reader);
        }
    }

    // TargetEntryPoint
    {
        auto r1 = checkSignature(reader, "lcsh.te.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t iTargetEntryPoint = 0; iTargetEntryPoint < count; iTargetEntryPoint++) {
            TargetEntryPoint* entryPoint = shader->createEntryPoint();
            entryPoint->target = static_cast<ShaderTarget>(reader.readInt8());
            entryPoint->name = readString(reader);
            entryPoint->codeBlobId = reader.readInt16();
            readBindingLayout(reader, entryPoint->bindingLayout);
            // inputAttributes
            {
                int16_t acount = reader.readInt16();
                for (int16_t j = 0; j < acount; j++) {
                    VertexInputAttribute attr;
                    attr.usage = static_cast<AttributeUsage>(reader.readUInt8());
                    attr.index = reader.readInt16();
                    attr.layoutLocation = reader.readInt16();
                    entryPoint->inputAttributes.push_back(attr);
                }
            }
        }
    }

    // TargetShaderPass
    {
        auto r1 = checkSignature(reader, "lcsh.tp.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t iTargetShaderPass = 0; iTargetShaderPass < count; iTargetShaderPass++) {
            TargetShaderPass* pass = shader->createTargetShaderPass();
            pass->vertEntryPointId = reader.readInt16();
            pass->fragEntryPointId = reader.readInt16();
            pass->compEntryPointId = reader.readInt16();
            readBindingLayout(reader, pass->bindingLayout);
        }
    }

    // GlobalShaderPass
    {
        auto r1 = checkSignature(reader, "lcsh.gp.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t iGlobalShaderPass = 0; iGlobalShaderPass < count; iGlobalShaderPass++) {
            GlobalShaderPass* pass = shader->createGlobalShaderPass();
            pass->name = readString(reader);
            pass->vertexEntryPoint = readString(reader);
            pass->fragmentEntryPoint = readString(reader);
            pass->computeEntryPoint = readString(reader);
            // targetShaderPassIds
            {
                int16_t tcount = reader.readInt16();
                pass->targetShaderPassIds.clear();
                for (int16_t i = 0; i < tcount; i++) {
                    pass->targetShaderPassIds.push_back(static_cast<TargetShaderPassId>(reader.readInt8()));
                }
            }
            // descriptorLayout
            {
                int16_t c1 = reader.readInt16();
                for (int16_t i = 0; i < c1; i++) pass->descriptorLayout.buffers.push_back(reader.readInt16());
                int16_t c2 = reader.readInt16();
                for (int16_t i = 0; i < c2; i++) pass->descriptorLayout.textures.push_back(reader.readInt16());
                int16_t c3 = reader.readInt16();
                for (int16_t i = 0; i < c3; i++) pass->descriptorLayout.samplers.push_back(reader.readInt16());
                int16_t c4 = reader.readInt16();
                for (int16_t i = 0; i < c4; i++) pass->descriptorLayout.storages.push_back(reader.readInt16());
            }
        }
    }

    // GlobalConstantBufferMember
    {
        auto r1 = checkSignature(reader, "lcsh.gm.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t i = 0; i < count; i++) {
            GlobalConstantBufferMember* member = shader->createGlobalConstantBufferMember();
            member->name = readString(reader);
            member->type = static_cast<ShaderGlobalMemberType>(reader.readInt8());
            member->kind = static_cast<ShaderGlobalMemberKind>(reader.readInt8());
            member->arrayElements = reader.readInt16();
            member->vectorElements = reader.readInt16();
            member->matrixRows = reader.readInt16();
            member->matrixColumns = reader.readInt16();
        }
    }

    // GlobalResourceLayout
    {
        auto r1 = checkSignature(reader, "lcsh.gl.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        GlobalResourceLayout* grl = shader->globalResourceLayout();
        auto readSlots = [&](std::vector<GlobalResourceSlotInfo>& slots) {
            int16_t count = reader.readInt16();
            for (int16_t i = 0; i < count; i++) {
                GlobalResourceSlotInfo slot;
                slot.name = readString(reader);
                slot.constantBufferSize = reader.readInt16();
                slot.arrayElementCount = reader.readInt16();
                slots.push_back(std::move(slot));
            }
        };
        readSlots(grl->buffers);
        readSlots(grl->textures);
        readSlots(grl->samplers);
        readSlots(grl->storages);
    }

    return shader;
}

} // namespace shader
} // namespace ln
