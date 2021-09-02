// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once

#include "Stream.hpp"
#include "Path.hpp"

namespace ln {

/** ファイル用の Stream です。 */
class FileStream
    : public Stream
{
public:
    /**
     * ファイルを開き、FileStream を作成します。
     * @param[in] filePath  : ファイル名
     * @param[in] openMode  : ファイルを開く方法 (FileOpenMode のフラグの組み合わせ)
     */
    static Ref<FileStream> create(const StringRef& filePath, FileOpenMode openMode = FileOpenMode::Read);

    /** ファイルを閉じます。*/
    void close();

    /** ファイルの絶対パスを取得します。*/
    const Path& filePath() const { return m_filePath; }

public:
    // Stream interface
    virtual bool canRead() const override;
    virtual bool canWrite() const override;
    virtual int64_t length() const override;
    virtual int64_t position() const override;
    virtual size_t read(void* buffer, size_t byteCount) override;
    virtual void write(const void* data, size_t byteCount) override;
    virtual void seek(int64_t offset, SeekOrigin origin) override;
    virtual void flush() override;

private:
    FileStream();
    virtual ~FileStream();
    bool open(const StringRef& filePath, FileOpenMode openMode);
    void checkOpen() const;
    void open() const;

    mutable FILE* m_stream;
    Path m_filePath;
    Flags<FileOpenMode> m_openModeFlags;
    int64_t m_writeLen;
};

} // namespace ln
