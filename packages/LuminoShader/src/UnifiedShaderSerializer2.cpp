// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include "pch.hpp"
#include <fstream>
#include <utility>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>

namespace ln {
namespace shader {

namespace {

//----------------------------------------------------------------------
// Thin binary writer/reader over std streams

class BinaryWriter2 final {
public:
    explicit BinaryWriter2(std::ostream& s) : m_stream(s) {}

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

class BinaryReader2 final {
public:
    BinaryReader2(const uint8_t* data, size_t length)
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

    size_t pos() const { return m_pos; }
    void skip(size_t size) { m_pos = std::min(m_pos + size, m_length); }

private:
    const uint8_t* m_data;
    size_t m_length;
    size_t m_pos;
};

//----------------------------------------------------------------------
// String / byte-array helpers

void writeString(BinaryWriter2& w, const std::string& str) {
    w.writeUInt32(static_cast<uint32_t>(str.length()));
    w.write(str.data(), str.length());
}

void writeByteArray(BinaryWriter2& w, const std::vector<uint8_t>& data) {
    w.writeUInt32(static_cast<uint32_t>(data.size()));
    w.write(data.data(), data.size());
}

std::string readString(BinaryReader2& r) {
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

VoidResult checkSignature(BinaryReader2& r, const char* sig, size_t len) {
    char buf[8] = {};
    size_t size = r.read(buf, len);
    if (size != len || strncmp(buf, sig, len) != 0) {
        return LNSHADER_MAKE_ERROR(std::string("Invalid code container signature. (") + sig + ")");
    }
    return LNSHADER_OK();
}

//----------------------------------------------------------------------
// v2 Binding layout write/read

void writeBindingLayout2(BinaryWriter2& writer, const TargetBindingLayout2& layout) {
    const auto& bindings = layout.bindings;
    writer.writeInt16(static_cast<int16_t>(bindings.size()));
    for (const auto& b : bindings) {
        writeString(writer, b.name);
        writer.writeInt8(static_cast<int8_t>(b.kind));
        writer.writeInt16(b.setIndex);
        writer.writeInt16(b.bindingIndex);
        writer.writeInt16(b.size);
        writer.writeInt8(static_cast<int8_t>(b.used));
    }
}

void readBindingLayout2(BinaryReader2& reader, TargetBindingLayout2& layout) {
    int16_t count = reader.readInt16();
    for (int16_t i = 0; i < count; i++) {
        TargetBinding2 b;
        b.name = readString(reader);
        b.kind = static_cast<ParameterBlockElementKind>(reader.readInt8());
        b.setIndex = reader.readInt16();
        b.bindingIndex = reader.readInt16();
        b.size = reader.readInt16();
        b.used = static_cast<ShaderStageFlags>(reader.readInt8());
        layout.bindings.push_back(std::move(b));
    }
}

} // anonymous namespace

//----------------------------------------------------------------------
// UnifiedShaderSerializer2

VoidResult UnifiedShaderSerializer2::saveToFile(const UnifiedShader2* shader, const std::filesystem::path& filePath) {
    std::ofstream fs(filePath, std::ios::binary | std::ios::trunc);
    if (!fs) {
        return LNSHADER_MAKE_ERROR("Failed to open file for writing: " + filePath.string());
    }
    BinaryWriter2 writer(fs);

    // File header
    {
        writer.write("lcs2", 4);
        writer.writeInt16(FileVersion_Current);
        // v3: シェーダの識別名 (実行時のエラーメッセージ用)
        writeString(writer, shader->sourceName());
    }

    // Blob
    {
        writer.write("lcs2.bl.", 8);
        const auto& blobs = shader->m_blobs;
        writer.writeInt16(static_cast<int16_t>(blobs.size()));
        for (size_t i = 0; i < blobs.size(); i++) {
            Blob* data = blobs[i].get();
            writeByteArray(writer, data->data);
        }
    }

    // ParameterBlockLayout
    {
        writer.write("lcs2.pb.", 8);
        const auto& blocks = shader->m_parameterBlocks;
        writer.writeInt16(static_cast<int16_t>(blocks.size()));
        for (const auto& block : blocks) {
            writeString(writer, block.name);
            writer.writeInt16(block.setIndex);
            writer.writeInt8(block.hasImplicitConstantBuffer ? 1 : 0);
            writer.writeInt16(static_cast<int16_t>(block.elements.size()));
            for (const auto& elem : block.elements) {
                writeString(writer, elem.name);
                writer.writeInt8(static_cast<int8_t>(elem.kind));
                writer.writeInt16(elem.constantBufferSize);
            }
            // v2: GlobalMemberInfo
            writer.writeInt16(static_cast<int16_t>(block.members.size()));
            for (const auto& m : block.members) {
                writeString(writer, m.name);
                writer.writeInt16(m.offset);
                writer.writeInt16(m.size);
            }
        }
    }

    // TargetEntryPoint
    {
        writer.write("lcs2.te.", 8);
        const auto& entryPoints = shader->m_targetEntryPoints;
        writer.writeInt16(static_cast<int16_t>(entryPoints.size()));
        for (const auto& ep : entryPoints) {
            writer.writeInt8(static_cast<int8_t>(ep->target));
            writeString(writer, ep->name);
            writer.writeInt16(ep->codeBlobId);
            writeBindingLayout2(writer, ep->bindingLayout);
            // inputAttributes
            {
                writer.writeInt16(static_cast<int16_t>(ep->inputAttributes.size()));
                for (const auto& attr : ep->inputAttributes) {
                    writer.writeInt8(static_cast<int8_t>(attr.usage));
                    writer.writeInt16(static_cast<int16_t>(attr.index));
                    writer.writeInt16(static_cast<int16_t>(attr.layoutLocation));
                }
            }
        }
    }

    // TargetShaderPass
    {
        writer.write("lcs2.tp.", 8);
        const auto& passes = shader->m_targetShaderPasses;
        writer.writeInt16(static_cast<int16_t>(passes.size()));
        for (const auto& pass : passes) {
            writer.writeInt16(pass->vertEntryPointId);
            writer.writeInt16(pass->fragEntryPointId);
            writer.writeInt16(pass->compEntryPointId);
            writeBindingLayout2(writer, pass->bindingLayout);
        }
    }

    // GlobalShaderPass
    {
        writer.write("lcs2.gp.", 8);
        const auto& passes = shader->m_globalShaderPasses;
        writer.writeInt16(static_cast<int16_t>(passes.size()));
        for (const auto& pass : passes) {
            writeString(writer, pass->name);
            writeString(writer, pass->vertexEntryPoint);
            writeString(writer, pass->fragmentEntryPoint);
            writeString(writer, pass->computeEntryPoint);
            // targetShaderPassIds
            writer.writeInt16(static_cast<int16_t>(pass->targetShaderPassIds.size()));
            for (auto id : pass->targetShaderPassIds) {
                writer.writeInt8(static_cast<int8_t>(id));
            }
        }
    }

    return LNSHADER_OK();
}

Result<Ref<UnifiedShader2>> UnifiedShaderSerializer2::loadFromData(
    const void* data, size_t length, ShaderTarget target) {
    BinaryReader2 reader(static_cast<const uint8_t*>(data), length);
    Ref<UnifiedShader2> shader = Ref<UnifiedShader2>::adopt(new UnifiedShader2());
    int fileVersion = 0;

    // File header
    {
        auto r1 = checkSignature(reader, "lcs2", 4);
        if (!r1) return tl::make_unexpected(r1.error());

        fileVersion = reader.readInt16();
        if (fileVersion < FileVersion_1 || fileVersion >= FileVersion_Last) {
            return LNSHADER_MAKE_ERROR("Unsupported file version");
        }
        // v3: シェーダの識別名 (実行時のエラーメッセージ用)
        if (fileVersion >= FileVersion_3) {
            shader->setSourceName(readString(reader));
        }
    }

    // Blob
    // どの blob がどのターゲットのものかは TargetEntryPoint を読むまで分からないため、
    // ここでは位置と長さだけを控えて読み飛ばし、実体化はエントリポイントを読んだあとで行う。
    std::vector<std::pair<size_t, uint32_t>> blobRanges;
    {
        auto r1 = checkSignature(reader, "lcs2.bl.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t i = 0; i < count; i++) {
            shader->createBlob();
            uint32_t blobLength = reader.readUInt32();
            if (reader.pos() + blobLength > length) {
                return LNSHADER_MAKE_ERROR("Blob range is out of bounds");
            }
            blobRanges.emplace_back(reader.pos(), blobLength);
            reader.skip(blobLength);
        }
    }

    // ParameterBlockLayout
    {
        auto r1 = checkSignature(reader, "lcs2.pb.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t i = 0; i < count; i++) {
            ParameterBlockLayout2 block;
            block.name = readString(reader);
            block.setIndex = reader.readInt16();
            block.hasImplicitConstantBuffer = reader.readInt8() != 0;
            int16_t elemCount = reader.readInt16();
            for (int16_t j = 0; j < elemCount; j++) {
                ParameterBlockElement2 elem;
                elem.name = readString(reader);
                elem.kind = static_cast<ParameterBlockElementKind>(reader.readInt8());
                elem.constantBufferSize = reader.readInt16();
                block.elements.push_back(std::move(elem));
            }
            // v2: GlobalMemberInfo
            if (fileVersion >= FileVersion_2) {
                int16_t memberCount = reader.readInt16();
                for (int16_t j = 0; j < memberCount; j++) {
                    GlobalMemberInfo m;
                    m.name = readString(reader);
                    m.offset = reader.readInt16();
                    m.size = reader.readInt16();
                    block.members.push_back(std::move(m));
                }
            }
            shader->addParameterBlock(std::move(block));
        }
    }

    // TargetEntryPoint
    {
        auto r1 = checkSignature(reader, "lcs2.te.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t i = 0; i < count; i++) {
            TargetEntryPoint2* ep = shader->createEntryPoint();
            ep->target = static_cast<ShaderTarget>(reader.readInt8());
            ep->name = readString(reader);
            ep->codeBlobId = reader.readInt16();
            readBindingLayout2(reader, ep->bindingLayout);
            // inputAttributes
            {
                int16_t acount = reader.readInt16();
                for (int16_t j = 0; j < acount; j++) {
                    VertexInputAttribute attr;
                    attr.usage = static_cast<AttributeUsage>(reader.readUInt8());
                    attr.index = reader.readInt16();
                    attr.layoutLocation = reader.readInt16();
                    ep->inputAttributes.push_back(attr);
                }
            }
        }
    }

    // 必要なコード blob だけを実体化する。
    {
        const uint8_t* base = static_cast<const uint8_t*>(data);
        auto materialize = [&](size_t i) {
            const auto& [offset, size] = blobRanges[i];
            shader->m_blobs[i]->data.assign(base + offset, base + offset + size);
        };
        if (target == ShaderTarget_UNKNOWN) {
            for (size_t i = 0; i < blobRanges.size(); i++) materialize(i);
        }
        else {
            for (const auto& ep : shader->m_targetEntryPoints) {
                if (ep->target == target && ep->codeBlobId >= 0 &&
                    static_cast<size_t>(ep->codeBlobId) < blobRanges.size()) {
                    materialize(ep->codeBlobId);
                }
            }
        }
    }

    // TargetShaderPass
    {
        auto r1 = checkSignature(reader, "lcs2.tp.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t i = 0; i < count; i++) {
            TargetShaderPass2* pass = shader->createTargetShaderPass();
            pass->vertEntryPointId = reader.readInt16();
            pass->fragEntryPointId = reader.readInt16();
            pass->compEntryPointId = reader.readInt16();
            readBindingLayout2(reader, pass->bindingLayout);
        }
    }

    // GlobalShaderPass
    {
        auto r1 = checkSignature(reader, "lcs2.gp.", 8);
        if (!r1) return tl::make_unexpected(r1.error());

        int16_t count = reader.readInt16();
        for (int16_t i = 0; i < count; i++) {
            GlobalShaderPass2* pass = shader->createGlobalShaderPass();
            pass->name = readString(reader);
            pass->vertexEntryPoint = readString(reader);
            pass->fragmentEntryPoint = readString(reader);
            pass->computeEntryPoint = readString(reader);
            // targetShaderPassIds
            {
                int16_t tcount = reader.readInt16();
                pass->targetShaderPassIds.clear();
                for (int16_t j = 0; j < tcount; j++) {
                    pass->targetShaderPassIds.push_back(static_cast<TargetShaderPassId2>(reader.readInt8()));
                }
            }
        }
    }

    return shader;
}

} // namespace shader
} // namespace ln
