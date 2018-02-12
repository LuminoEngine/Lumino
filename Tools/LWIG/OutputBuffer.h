
#pragma once
#include <Lumino/Base/StringBuilder.h>

/** コード出力用バッファ */
class OutputBuffer
{
public:
	OutputBuffer();
	OutputBuffer(int indent);
	
	/** 空であるか */
	bool isEmpty() const { return m_buffer.isEmpty(); }
	
	/** クリアする */
	void clear();

	/** インデントレベルをひとつ増やす */
	void IncreaseIndent(int count = 1);

	/** インデントレベルをひとつ減らす */
	void DecreaseIndent();

	/** 文字列を追加する */
	OutputBuffer& append(const StringRef& str) { AppendInternal(str); return *this; }

	/** 文字列を追加する */
	template<typename... TArgs>
	OutputBuffer& append(const StringRef& format, const TArgs&... args) { AppendInternal(String::format(format, args...)); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	OutputBuffer& AppendLine(const StringRef& str) { AppendLineInternal(str); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	template<typename... TArgs>
	OutputBuffer& AppendLine(const StringRef& format, const TArgs&... args) { AppendLineInternal(String::format(format, args...)); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	OutputBuffer& AppendLines(const StringRef& str) { AppendLinesInternal(str, String::getEmpty()); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	template<typename... TArgs>
	OutputBuffer& AppendLines(const StringRef& format, const TArgs&... args) { AppendLinesInternal(String::format(format, args...), String::getEmpty()); return *this; }
	
	/** 文字列を追加する (各行の先頭をインデント) */
	OutputBuffer& AppendLinesHeaderd(const StringRef& str, const StringRef& lineHeader) { AppendLinesInternal(str, lineHeader); return *this; }

	/** 既に文字列が存在すれば , を挿入して文字列を追加する */
	template<typename... TArgs>
	OutputBuffer& AppendCommad(const StringRef& format, const TArgs&... args)
	{
		if (!isEmpty()) AppendInternal(", ");
		AppendInternal(String::format(format, args...));
		return *this;
	}

	/** 改行する */
	OutputBuffer& NewLine(int count = 1);
	
	/** 現在のインデントレベル分の空白を追加する */
	void indent();
	
	/** String 取得 */
	String toString() const;
	
private:
	void AppendInternal(const StringRef& str);
	void AppendLineInternal(const StringRef& str);
	void AppendLinesInternal(const StringRef& str, const StringRef& lineHeader);
	
	enum class State
	{
		LineHead,
		Text,
	};

	//detail::SimpleStringBuilder<Char>	m_buffer;
	String			m_buffer;
	int				m_indentLevel;
	String			m_indent;
	String			m_newLineCode;
	State			m_state;
};

