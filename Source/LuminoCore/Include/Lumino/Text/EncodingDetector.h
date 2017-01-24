#pragma once

#include "Encoding.h"

LN_NAMESPACE_BEGIN

class IMBSCodeDetector
{
public:

	virtual ~IMBSCodeDetector() {}

	/// 解析
	/// untilUnmatch が true の場合は不一致が見つかったらその時点で解析を終了する。
	/// そのとき、不一致文字までは解析済みとする。(UnMatch Count が 1 であること)
	/// また、Detector は状態を保持する。呼び出し側は Detect(true) → Detect(false) の2回
	/// 呼び出しを行うが、2回目は1回目に解析終了した位置から開始する。(負荷軽減のため)
	virtual void Detect(bool untilUnmatch) = 0;

	/// EncodingType
	virtual EncodingType GetEncodingType() = 0;

	/// マッチポイント
	virtual int GetScore() = 0;

	/// 不正なバイトシーケンスが見つかったか
	virtual int GetUnMatchCount() = 0;

	/// 不正なバイトシーケンスを見つけたときの行番号
	virtual int GetUnMatchLine() = 0;
};

/**
	@brief	UTF8N 評価クラス
*/
class UTF8NDetector : public IMBSCodeDetector
{
public:
	UTF8NDetector(const void* bytes, size_t bytesSize);
	virtual ~UTF8NDetector() {}
	virtual EncodingType GetEncodingType() { return EncodingType::UTF8N; }
	virtual void Detect(bool untilUnmatch);
	virtual int GetScore() { return m_score; }
	virtual int GetUnMatchCount() { return m_unmatch; }
	virtual int GetUnMatchLine() { return m_lineNum; }

private:
	byte_t*	m_buffer;
	size_t	m_bufferSize;
	size_t	m_pos;
	int		m_lineNum;
	int		m_score;
	int		m_unmatch;
};

/**
	@brief	SJIS 評価クラス
*/
class SJISDetector : public IMBSCodeDetector
{
public:
	SJISDetector(const void* bytes, size_t bytesSize);
	virtual ~SJISDetector() {}
	virtual EncodingType GetEncodingType() { return EncodingType::SJIS; }
	virtual void Detect(bool untilUnmatch);
	virtual int GetScore() { return m_score; }
	virtual int GetUnMatchCount() { return m_unmatch; }
	virtual int GetUnMatchLine() { return m_lineNum; }

private:
	byte_t*	m_buffer;
	size_t	m_bufferSize;
	size_t	m_pos;
	int		m_lineNum;
	int		m_score;
	int		m_unmatch;
};

/**
	@bfief		文字コードを自動検出するクラスです。
*/
class EncodingDetector
{
public:
	typedef unsigned char Byte;

public:
	EncodingType Detect(const void* bytes, size_t bytesSize);

	/// 可能性の一番高い文字コード
	EncodingType GetEncodingType() const { return m_type; }

private:
	bool CheckASCII() const;
	EncodingType CheckUTFBom();

private:
	const Byte*		m_buffer;
	size_t			m_bufferSize;
	EncodingType	m_type;
};

LN_NAMESPACE_END
