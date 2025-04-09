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
        writer->write("lufx", 4); // magic number
        writer->writeUInt32(FileVersion_Current);
    }

    // Blob
    {
        writer->write("lufx.bl.", 8); // Chunk signature

        const auto& blobs = shader->m_blobs;
        writer->writeUInt32(blobs.size());
        for (int i = 0; i < blobs.size(); i++) {
            Blob* data = blobs[i];
            writeByteArray(writer, data->data);
        }
    }

    return LN_MAKE_SUCCESS();
}

Result<Ref<UnifiedShader2>> UnifiedShaderSerializer::loadFromFile(const std::filesystem::path& filePath) {
    auto stream = FileStream::create(Path::fromStdPath(filePath), FileOpenMode::Read);
    auto reader = makeRef<BinaryReader>(stream);
    Ref<UnifiedShader2> shader = makeRef<UnifiedShader2>();

    // File header
    {
        auto r1 = checkSignature(reader, "lufx", 4);
        if (!r1) return LN_TO_ERROR(r1);

        int fileVersion = reader->readUInt32();
        if (fileVersion < FileVersion_1 || fileVersion >= FileVersion_Last) {
            return LN_MAKE_ERROR("Unsupported file version");
        }
    }

    // Blob
    {
        auto r1 = checkSignature(reader, "lufx.bl.", 8);
        if (!r1) return LN_TO_ERROR(r1);

        size_t count = reader->readUInt32();
        for (size_t i = 0; i < count; i++) {
            Blob* blob = shader->createBlob();
            blob->data = readByteArray(reader);
        }
    }

    return shader;
}

MaybeResult UnifiedShaderSerializer::checkSignature(BinaryReader* r, const char* sig, size_t len) {
    char buf[8];
    size_t size = r->read(buf, len);
    if (size != len || strncmp(buf, sig, len) != 0) {
        return LN_MAKE_ERROR("Invalid code container signature. (%s)", sig);
    }
    return LN_MAKE_SUCCESS();
}

} // namespace kokage
} // namespace ln
