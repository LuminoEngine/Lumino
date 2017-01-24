/**
	@file	BinaryReader.h
*/
#pragma once

#include "../Base/Common.h"
#include "../Base/Environment.h"

LN_NAMESPACE_BEGIN
class Stream;

/**
	@brief		ストリームからバイナリデータを読み取るクラスです。

	@section	Wave ファイルのフォーマットを読み取る
	~~~~~~~~~~~~{.cpp}
	struct WaveFileHeader
	{
		byte_t		RIFF[4];
		uint32_t	Size;
		byte_t		WaveHeader[4];
	};

	FileStream stream("sound.wav");
	BinaryReader reader(stream);

	WaveFileHeader header;
	reader.Read(&header, sizeof(WaveFileHeader));
	header.RIFF;		// "RIFF"
	header.WaveHeader;	// "WAVE"

	uint32_t ChunkSize		= reader.ReadUInt32();
	uint16_t FormatTag		= reader.ReadUInt16();
	uint16_t Channels		= reader.ReadUInt16();
	uint32_t SamplesPerSec	= reader.ReadUInt32();
	uint32_t AvgBytesPerSec	= reader.ReadUInt32();
	uint16_t BlockAlign		= reader.ReadUInt16();
	uint16_t BitsPerSample	= reader.ReadUInt16();
	~~~~~~~~~~~~
*/
class BinaryReader
{
public:
	BinaryReader(Stream* stream);
	~BinaryReader();

public:

	/**
		@brief		符号付き 1 バイト整数を読み取り、ストリームの現在位置を 1 バイト進めます。
	*/
	int8_t ReadInt8();

	/**
		@brief		符号付き 2 バイト整数を読み取り、ストリームの現在位置を 2 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	int16_t ReadInt16(ByteOrder dataByteOrder = ByteOrder::Little);

	/**
		@brief		符号付き 4 バイト整数を読み取り、ストリームの現在位置を 4 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	int32_t ReadInt32(ByteOrder dataByteOrder = ByteOrder::Little);

	/**
		@brief		符号付き 8 バイト整数を読み取り、ストリームの現在位置を 8 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	int64_t ReadInt64(ByteOrder dataByteOrder = ByteOrder::Little);

	/**
		@brief		符号無し 1 バイト整数を読み取り、ストリームの現在位置を 1 バイト進めます。
	*/
	uint8_t ReadUInt8();

	/**
		@brief		符号無し 2 バイト整数を読み取り、ストリームの現在位置を 2 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	uint16_t ReadUInt16(ByteOrder dataByteOrder = ByteOrder::Little);

	/**
		@brief		符号無し 4 バイト整数を読み取り、ストリームの現在位置を 4 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	uint32_t ReadUInt32(ByteOrder dataByteOrder = ByteOrder::Little);

	/**
		@brief		符号無し 8 バイト整数を読み取り、ストリームの現在位置を 8 バイト進めます。
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	uint64_t ReadUInt64(ByteOrder dataByteOrder = ByteOrder::Little);

	/**
		@brief		指定したバイト数の符号付き整数を読み取り、ストリームの現在位置を進めます。
		@param[in]	byteCount	: 読み取るバイト数 (1, 2, 4, 8のいずれか)
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	int64_t ReadInt(int byteCount, ByteOrder dataByteOrder = ByteOrder::Little);

	/**
		@brief		指定したバイト数の符号無し整数を読み取り、ストリームの現在位置を進めます。
		@param[in]	byteCount	: 読み取るバイト数 (1, 2, 4, 8のいずれか)
		@param[in]	dataByteOrder	: 読み取るデータのバイトオーダー
	*/
	uint64_t ReadUInt(int byteCount, ByteOrder dataByteOrder = ByteOrder::Little);

	/**
		@brief		4 バイト浮動小数点値を読み取り、ストリームの現在位置を 4 バイト進めます。
	*/
	float ReadFloat();

	/**
		@brief		8 バイト浮動小数点値を読み取り、ストリームの現在位置を 8 バイト進めます。
	*/
	double ReadDouble();

	/**
		@brief		指定したバイト数のデータを読み取り、ストリームの現在位置を進めます。
	*/
	size_t Read(void* buffer, size_t count);

	/**
		@brief		ストリームの現在位置を、指定したオフセットだけ進めます。
	*/
	void Seek(int offset);

	/**
		@brief		ストリームの現在位置を取得します。
	*/
	int64_t GetPosition() const;

	/**
		@brief		ファイル終端に到達しているかを確認します。
	*/
	bool IsEOF() const;

private:
	Stream*   m_stream;
};

LN_NAMESPACE_END
