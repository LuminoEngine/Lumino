
#include "Internal.hpp"
#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "../Asset/AssetManager.hpp"
#include "BvhImporter.hpp"

namespace ln {
namespace detail {

//==============================================================================
// AsciiLineReader

AsciiLineReader::AsciiLineReader()
	: m_stream(nullptr)
	, m_bufferPos(0)
	, m_bufferLength(0)
    , m_lineHead(nullptr)
    , m_lineLength(0)
{
    m_lineCache.resize(1024);
}

void AsciiLineReader::reset(Stream* stream)
{
    m_stream = stream;
    m_bufferPos = 0;
    m_bufferLength = 0;
    m_lineHead = nullptr;
    m_lineLength = 0;
}

bool AsciiLineReader::readLine()
{
    bool result = readLineRaw();
    if (result) {
        StringHelper::trim(m_lineCache.data(), m_lineCache.size(), &m_lineHead, &m_lineLength);
        splitLineTokens();
        m_lineTokenIndex = -1;
        return true;
    }
    else {
        return false;
    }
}

bool AsciiLineReader::readToken()
{
    m_lineTokenIndex++;
    return m_lineTokenIndex < m_lineTokens.size();
}

bool AsciiLineReader::readLineRaw()
{
    if (m_bufferPos == m_bufferLength) {
        if (readBuffer() == 0) {
            return false; // EOF
        }
    }

    m_lineCache.clear();

    do {
        int i = m_bufferPos;
        do {
            const char* buf = m_buffer.data();
            Char ch = m_buffer[i];
            if (ch == '\r' || ch == '\n') {
                std::copy(buf + m_bufferPos, buf + i, std::back_inserter(m_lineCache));
                m_bufferPos = i + 1;

                if (ch == '\r' && (m_bufferPos < m_bufferLength || readBuffer())) {
                    if (buf[m_bufferPos] == '\n') {
                        m_bufferPos++;    // skip LF
                    }
                }
                return true;
            }
            i++;
        } while (i < m_bufferLength);

        // ここに来るのは、charBuffer の現在位置 ～ 終端までに改行が無かったとき。
        // 現在の残りバッファを結合して、次のバッファを ReadBuffer() で読み出す。
        const char* buf = m_buffer.data();
        std::copy(buf + m_bufferPos, buf + m_bufferLength, std::back_inserter(m_lineCache));

    } while (readBuffer() > 0);

    return true;
}

bool AsciiLineReader::readBuffer()
{
    m_bufferPos = 0;
	m_bufferLength = m_stream->read(m_buffer.data(), m_buffer.size());
	if (m_bufferLength == 0) {
		return false;
	}
	else {

        return true;
	}
}

void AsciiLineReader::splitLineTokens()
{
    m_lineTokens.clear();
    const char* pos = lineHead();
    const char* end = lineEnd();
    const char* tokenBegin = nullptr;
    for (; pos < end; pos++) {
        if (!tokenBegin) {
            if (!StringHelper::isSpace(*pos)) {
                tokenBegin = pos;
            }
        }
        else {
            if (StringHelper::isSpace(*pos)) {
                m_lineTokens.push_back({ tokenBegin, static_cast<int>(pos - tokenBegin) });
                tokenBegin = nullptr;
            }
        }
    }

    if (tokenBegin) {
        m_lineTokens.push_back({ tokenBegin, static_cast<int>(pos - tokenBegin) });
    }
}

//==============================================================================
// BvhImporter
// http://www.dcs.shef.ac.uk/intranet/research/public/resmes/CS0111.pdf

BvhImporter::BvhImporter(AssetManager* assetManager, DiagnosticsManager* diag)
	: m_assetManager(assetManager)
	, m_diag(diag)
{
}

bool BvhImporter::import(AnimationClip* clip, const AssetPath& assetPath)
{
	auto stream = m_assetManager->openStreamFromAssetPath(assetPath);
    m_reader.reset(stream);

    while (m_reader.readLine())
    {
        if (m_reader.startWith("HIERARCHY", 9)) {
            if (!readHierarchy()) {
                return false;
            }
        }
        else if (m_reader.startWith("MOTION", 6)) {
            if (!readMotion()) {
                return false;
            }
        }
    }

	return true;
}

bool BvhImporter::readHierarchy()
{
    int level = 0;
    bool is_site = false;
    bool blockClosed = false;

    while (m_reader.readLine()) {
        if (!m_reader.readToken()) {
            return false;
        }

        if (m_reader.token().is("ROOT", 4) ||
            m_reader.token().is("JOINT", 5)) {

            if (!m_reader.readToken()) {
                m_diag->reportError(u"Expected joint name.");
                return false;
            }

            auto joint = std::make_shared<Joint>();
            joint->name = std::string(m_reader.token().begin, m_reader.token().length);
            joint->index = m_joints.size();

            m_joints.push_back(joint);
        }
        else if (m_reader.token().is("End", 3)) {
            is_site = true;
        }
        else if (m_reader.token().is("OFFSET", 6)) {
            if (!m_reader.readToken()) {
                m_diag->reportError(u"Expected offset value x.");
                return false;
            }
            float x = m_reader.token().toFloat();

            if (!m_reader.readToken()) {
                m_diag->reportError(u"Expected offset value y.");
                return false;
            }
            float y = m_reader.token().toFloat();

            if (!m_reader.readToken()) {
                m_diag->reportError(u"Expected offset value z.");
                return false;
            }
            float z = m_reader.token().toFloat();

            Joint* joint = currentJoint();
            if (is_site) {
                joint->hasSite = true;
                joint->site.x = x;
                joint->site.y = y;
                joint->site.z = z;
            }
            else {
                joint->offset.x = x;
                joint->offset.y = y;
                joint->offset.z = z;
            }
        }
        else if (m_reader.token().is("CHANNELS", 8)) {
            if (!m_reader.readToken()) {
                m_diag->reportError(u"Expected channels count.");
                return false;
            }

            int count = m_reader.token().toInt();
            Joint* joint = currentJoint();
            joint->channels.resize(count);
            for (int i = 0; i < count; i++) {
                auto channel = std::make_shared<Channel>();
                channel->index = m_channels.size();
                m_channels.push_back(channel);

                if (!m_reader.readToken()) {
                    m_diag->reportError(u"Expected channel.");
                    return false;
                }

                if (m_reader.token().is("Xrotation", 9))
                    channel->type = X_ROTATION;
                else if (m_reader.token().is("Yrotation", 9))
                    channel->type = Y_ROTATION;
                else if (m_reader.token().is("Zrotation", 9))
                    channel->type = Z_ROTATION;
                else if (m_reader.token().is("Xposition", 9))
                    channel->type = X_POSITION;
                else if (m_reader.token().is("Yposition", 9))
                    channel->type = Y_POSITION;
                else if (m_reader.token().is("Zposition", 9))
                    channel->type = Z_POSITION;
            }
        }
        else if (m_reader.token().is("{", 1)) {
            level++;
        }
        else if (m_reader.token().is("}", 1)) {
            is_site = false;
            level--;
            if (level == 0) {
                // End "HIERARCHY"
                blockClosed = true;
                break;
            }
        }
    }

    if (!blockClosed) {
        m_diag->reportError(u"HIERARCHY block is not closed.");
        return false;
    }

#if 1
    for (auto& joint : m_joints) {
        std::cout << joint->name << std::endl;
        std::cout << "  " << joint->offset.x << ", " << joint->offset.y << ", " << joint->offset.z << std::endl;
    }
#endif
}

bool BvhImporter::readMotion()
{
    // Frames:
    if (!m_reader.readLine()) {
        m_diag->reportError(u"Expected Frames line.");
        return false;
    }
    if (m_reader.tokenCount() != 2) {
        m_diag->reportError(u"Expected Frames token.");
        return false;
    }
    if (!m_reader.token(0).is("Frames:", 7)) {
        m_diag->reportError(u"Expected Frames.");
        return false;
    }
    m_frames = m_reader.token(1).toInt();

    // Frame Time:
    if (!m_reader.readLine()) {
        m_diag->reportError(u"Expected Frame Time line.");
        return false;
    }
    if (m_reader.tokenCount() != 3) {
        m_diag->reportError(u"Expected Frame Time token.");
        return false;
    }
    if (!m_reader.startWith("Frame Time:", 11)) {
        m_diag->reportError(u"Expected Frame Time.");
        return false;
    }
    m_frameTime = m_reader.token(2).toFloat();

    m_frameData.resize(m_frames * m_channels.size());
    for (int iFrame = 0; iFrame < m_frames; iFrame++) {
        if (!m_reader.readLine()) {
            m_diag->reportError(u"Expected frame data line.");
            return false;
        }

        if (m_reader.tokenCount() != m_channels.size()) {
            m_diag->reportError(u"Insufficient frame data.");
            return false;
        }

        for (int iToken = 0; iToken < m_reader.tokenCount(); iToken++) {
            setFrameData(iFrame, iToken, m_reader.token(iToken).toFloat());
        }
    }

    return true;
}

} // namespace detail
} // namespace ln

