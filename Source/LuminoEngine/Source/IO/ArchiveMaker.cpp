
#include <string>
#include "../../external/camellia/camellia.h"
#include <Lumino/IO/Common.h>
#include <Lumino/IO/FileSystem.h>
#include "Archive.h"
#include "ArchiveMaker.h"

LN_NAMESPACE_BEGIN

//------------------------------------------------------------------------------
ArchiveMaker::ArchiveMaker()
	: m_stream(NULL)
	, m_fileCount(0)
	, m_blockLength(0)
{
}

//------------------------------------------------------------------------------
ArchiveMaker::~ArchiveMaker()
{
	Close();
}

//------------------------------------------------------------------------------
bool ArchiveMaker::Open(const char* filePath, const char* key)
{
	PathName path(filePath);
	return Open(path, key);
}

//------------------------------------------------------------------------------
bool ArchiveMaker::Open(const wchar_t* filePath, const char* key)
{
	PathName path(filePath);
	return Open(path, key);
}

//------------------------------------------------------------------------------
bool ArchiveMaker::Open(const PathName& filePath, const char* key)
{
	m_encryptionKey = (key) ? key : "";
	m_fileCount = 0;
	memset(m_keyTable, 0, sizeof(m_keyTable));

	// 拡張キーの作成
	if (key)
	{
		byte_t key_buf[KEY_SIZE] = { 0 };
		memcpy(key_buf, key, strlen(key));
		memset(m_keyTable, 0, sizeof(m_keyTable));
		Camellia_Ekeygen(KEY_SIZE, key_buf, m_keyTable);
	}

	// ファイルを新規作成
	if (_tfopen_s(&m_stream, filePath.c_str(), _T("wb")) != 0) {
		return false;
	}
	
	// ファイルヘッダ
	ArchiveHeader header;
	memset(&header, 0, sizeof(header));
	header.ID[0] = 'l';
	header.ID[1] = 'n';
	header.ID[2] = 'a';
	header.ID[3] = ' ';
	header.Version = CurrentVersion;
	/*
	header.KeyLength = mEncryptionKey.size();
	_tcsncpy( header.Key, mEncryptionKey.c_str(), 128 );
	*/
	fwrite(&header, sizeof(header), 1, m_stream);

	// 内部キー(シグネチャ)16バイト
	WritePadding16(Archive::InternalKey, 16);

	return true;
}

//------------------------------------------------------------------------------
void ArchiveMaker::Close()
{
	if (m_stream)
	{
		// 一番最後にファイルの数を書き込む
		WriteU32Padding16(m_fileCount, 0);

		// ファイルを閉じる
		fclose(m_stream);
		m_stream = NULL;
	}
}

//------------------------------------------------------------------------------
bool ArchiveMaker::AddFile(const PathName& filePath, String aliasPath)
{
	FILE* stream;
	errno_t err = _tfopen_s(&stream, filePath, _T("rb"));
	if (err == 0)
	{
        // アクセス用の名前がなければ、ファイル名を代わりに使う
		if (aliasPath.IsEmpty())
        {
			aliasPath = filePath.c_str();
        }

        // アクセス用ファイル名のスラッシュをバックスラッシュ化
		StringW filename = StringW::FromNativeCharString(aliasPath.c_str());
		NormalizePath(&filename);

        //-------------------------------------------------
        // ファイル名の長さとファイルのサイズを書き込む
		uint32_t nameSize = filename.GetLength();
		uint32_t fileSize = (uint32_t)FileSystem::GetFileSize(stream);

		WriteU32Padding16(nameSize, fileSize);

        //-------------------------------------------------
        // ファイル名とファイル内容を書き込む

        // ファイル名を書き込む (終端NULLはナシ)
		WritePadding16((byte_t*)filename.c_str(), nameSize * sizeof(wchar_t));

		//wprintf(L"filename : %s\n", filename.c_str());
		//wprintf(L"seek     : %u\n", ftell(m_stream));

        // サイズ分のメモリを確保して追加する内容を読み込む
		byte_t* buffer = LN_NEW byte_t[fileSize];
		fseek( stream, 0, SEEK_SET );
		fread(buffer, sizeof(byte_t), fileSize, stream);

		// 内容を書き込む
		WritePadding16(buffer, fileSize);

		delete[] buffer;
        fclose( stream );

		++m_fileCount;
        return true;
	}
    return false;
}

//------------------------------------------------------------------------------
void ArchiveMaker::NormalizePath(StringW* path)
{
	if (path->GetLength() > 0) {
		for (int i = 0; i < path->GetLength(); ++i) {
			if ((*path)[i] == L'\\') (*path)[i] = L'/';
		}
	}
}

//------------------------------------------------------------------------------
void ArchiveMaker::WritePadding16(const byte_t* data, uint32_t size)
{
	uint32_t ps = (16 - (size % 16)) % 16;
	char padding[16];
	memset(padding, ps, sizeof(padding));

    // 暗号化する場合
	if (m_encryptionKey.size() > 0)
    {
        int i = 0;
		uint32_t total = 0;
		byte_t src_data[16];
		byte_t write_data[16];
		while (true)
		{
			if (total + 16 > size)
			{
				memcpy(src_data, &data[i * 16], 16 - ps);
				memcpy(&src_data[16 - ps], padding, ps);
			}
			else
			{
				memcpy(src_data, &data[i * 16], 16);
			}
			Camellia_EncryptBlock(KEY_SIZE, src_data, m_keyTable, write_data);
			fwrite(write_data, 1, 16, m_stream);
			++i;
			total += 16;
			if (total >= size)
			{
				break;
			}
		}
    }
    else
    {
		fwrite(data, 1, size, m_stream);
		fwrite(padding, 1, ps, m_stream);
    }
}

//------------------------------------------------------------------------------
void ArchiveMaker::WriteU32Padding16(uint32_t v0, uint32_t v1)
{
	// 16 byte のうち、先頭 8 byte に u32 を 2 つ書き込む

    byte_t b[ 16 ] = { 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 8 };
#if 1	// とりあえずリトルエンディアン環境固定で。
	* ((uint32_t*)(b + 0)) = v0;
	*((uint32_t*)(b + 4)) = v1;
#else
    if ( Base::isLittleEndian() )
    {
        *((lnU32*)(b + 0)) = v0;
        *((lnU32*)(b + 4)) = v1;
    }
    else
    {
        *( b + 0 ) = static_cast< lnU8 >(   v0 & 0x000000ff );
        *( b + 1 ) = static_cast< lnU8 >( ( v0 & 0x0000ff00 ) >> 8 );
        *( b + 2 ) = static_cast< lnU8 >( ( v0 & 0x00ff0000 ) >> 16 );
        *( b + 3 ) = static_cast< lnU8 >( ( v0 & 0xff000000 ) >> 24 );

        *( b + 4 ) = static_cast< lnU8 >(   v1 & 0x000000ff );
        *( b + 5 ) = static_cast< lnU8 >( ( v1 & 0x0000ff00 ) >> 8 );
        *( b + 6 ) = static_cast< lnU8 >( ( v1 & 0x00ff0000 ) >> 16 );
        *( b + 7 ) = static_cast< lnU8 >( ( v1 & 0xff000000 ) >> 24 );
    }
#endif

	byte_t buf[16];
	Camellia_EncryptBlock(KEY_SIZE, b, m_keyTable, buf);
    fwrite( &buf, sizeof( b ), 1, m_stream );
}

LN_NAMESPACE_END
