/**
	@file	ArchiveMaker.h
*/
#pragma once
#include <Lumino/IO/PathName.h>

LN_NAMESPACE_BEGIN

/**
	@brief	暗号化アーカイブを作成するためのクラスです。
	@code
			// 以下のようにすることで、Data.lna をフォルダのようにみなして file1.txt にアクセスできるアーカイブを作成できる
			ArchiveMaker maker;
			maker.Open("C:\\TestApp\\Data.lna", "password");
			maker.AddFile("C:\\TestApp\\Data\\file1.txt", "file1.txt");
			maker.Close();
	@endcode
*/
class ArchiveMaker
{
public:
	ArchiveMaker();
	~ArchiveMaker();

public:

	/*
		@brief		新しく暗号化アーカイブファイルを作成します。
		@param[in]  filename	: 新しく作成するアーカイブファイルの名前
		@param[in]  key			: 暗号化に使う文字列 (最大128文字)
		@return		true=成功 / false=失敗
	*/
	bool Open(const char* filePath, const char* key);
	bool Open(const wchar_t* filePath, const char* key);
	bool Open(const PathName& filePath, const char* key);

	/**
		@brief		暗号化アーカイブファイルのストリームを閉じます。
		@details	デストラクタでも呼び出されます。
	*/
    void Close();
	
	/**
		@brief		ファイルを追加します。
		@param[in]	filePath	: 追加するファイルのパス
		@param[in]	aliasPath	: アーカイブ内のファイルにアクセスするときの別名
		@return		true=成功 / false=失敗
		@details	デストラクタでも呼び出されます。
	*/
	bool AddFile(const PathName& filePath, String aliasPath);

private:

	/// パディングを考慮して、開いているアーカイブファイルにデータを書き込む
	void WritePadding16(const byte_t* data, uint32_t size);

	/// パディングを考慮して、整数を書き込む (ファイル名長さ、ファイルサイズ用)
	void WriteU32Padding16(uint32_t v0, uint32_t v1);

	/// ディレクトリセパレータを / に変換する
	void NormalizePath(StringW* path);

private:

	// camellia key table type.
	static const int L_CAMELLIA_TABLE_BYTE_LEN = 272;
	static const int L_CAMELLIA_TABLE_WORD_LEN = (L_CAMELLIA_TABLE_BYTE_LEN / 4);
	typedef unsigned int KEY_TABLE_TYPE[L_CAMELLIA_TABLE_WORD_LEN];

    static const int KEY_SIZE  = 128;
	static const uint8_t CurrentVersion = 1;

	FILE*					m_stream;			///< アーカイブファイルのファイルストリーム
	std::string				m_encryptionKey;
	uint32_t				m_fileCount;		///< ファイル数
	KEY_TABLE_TYPE			m_keyTable;
    int						m_blockLength;
};

LN_NAMESPACE_END

