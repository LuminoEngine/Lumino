
#pragma once

/** コード出力用バッファ */
class OutputBuffer
{
public:
	OutputBuffer();
	OutputBuffer(int indent);
	
	/** 空であるか */
	bool IsEmpty() const { return m_buffer.IsEmpty(); }
	
	/** クリアする */
	void Clear();

	/** インデントレベルをひとつ増やす */
	void IncreaseIndent();

	/** インデントレベルをひとつ減らす */
	void DecreaseIndent();
	
	/** 文字列を追加する */
	template<typename... TArgs>
	OutputBuffer& Append(const StringRef& format, const TArgs&... args) { AppendInternal(String::Format(format, args...)); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	OutputBuffer& AppendLine(const StringRef& str) { AppendLineInternal(str); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	template<typename... TArgs>
	OutputBuffer& AppendLine(const StringRef& format, const TArgs&... args) { AppendLineInternal(String::Format(format, args...)); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	OutputBuffer& AppendLines(const StringRef& str) { AppendLinesInternal(str); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	template<typename... TArgs>
	OutputBuffer& AppendLines(const StringRef& format, const TArgs&... args) { AppendLinesInternal(String::Format(format, args...)); return *this; }
	
	/** 既に文字列が存在すれば , を挿入して文字列を追加する */
	template<typename... TArgs>
	OutputBuffer& AppendCommad(const StringRef& format, const TArgs&... args)
	{
		if (!IsEmpty()) AppendInternal(", ");
		AppendInternal(String::Format(format, args...));
		return *this;
	}

	/** 改行する */
	OutputBuffer& NewLine(int count = 1);
	
	/** 現在のインデントレベル分の空白を追加する */
	void Indent();
	
	/** String 取得 */
	String ToString() const;
	
private:
	void AppendInternal(const StringRef& str);
	void AppendLineInternal(const StringRef& str);
	void AppendLinesInternal(const StringRef& str);
	
	StringBuilder	m_buffer;
	int				m_indentLevel;
	String			m_indent;
	String			m_newLineCode;
};

