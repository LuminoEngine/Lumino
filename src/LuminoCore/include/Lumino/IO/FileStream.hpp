// Copyright (c) 2018 lriki. Distributed under the MIT license.
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
		@brief	ファイルを開きます。
		@param	filePath	: ファイル名
		@param	openMode	: ファイルを開く方法 (FileOpenMode のフラグの組み合わせ)
	*/
    static Ref<FileStream> create(const Char* filePath, FileOpenMode openMode);

    /** ファイルを閉じます。*/
    void close();

    /** ファイルの絶対パスを取得します。*/
    const Path& getFilePath() const { return m_filePath; }

public:
    // Stream interface
    virtual bool canRead() const;
    virtual bool canWrite() const;
    virtual int64_t length() const;
    virtual int64_t position() const;
    virtual size_t read(void* buffer, size_t byteCount);
    virtual void write(const void* data, size_t byteCount);
    virtual void seek(int64_t offset, SeekOrigin origin);
    virtual void flush();

private:
    FileStream();
    virtual ~FileStream();
    bool open(const Char* filePath, FileOpenMode openMode);
    void checkOpen() const;
    void open() const;

private:
    mutable FILE* m_stream;
	Path m_filePath;
    Flags<FileOpenMode> m_openModeFlags;
};

} // namespace ln
