
#pragma once

/** コード出力用バッファ */
class OutputBuffer
{
public:
	OutputBuffer();
	OutputBuffer(int indent);

	void setNewLineCode(const ln::String& code) { m_newLineCode = code; }
	
	/** 空であるか */
	bool isEmpty() const { return m_buffer.isEmpty(); }
	
	/** クリアする */
	void clear();

	/** インデントレベルをひとつ増やす */
	void IncreaseIndent(int count = 1);

	/** インデントレベルをひとつ減らす */
	void DecreaseIndent();

	/** 文字列を追加する */
	OutputBuffer& append(const ln::StringRef& str) { AppendInternal(str); return *this; }

	/** 文字列を追加する */
	OutputBuffer& append(const OutputBuffer& buf) { AppendInternal(buf.toString()); return *this; }

	/** 文字列を追加する */
	template<typename... TArgs>
	OutputBuffer& append(const ln::StringRef& format, const TArgs&... args) { AppendInternal(ln::String::format(format, args...)); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	OutputBuffer& AppendLine(const ln::StringRef& str) { AppendLineInternal(str); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	template<typename... TArgs>
	OutputBuffer& AppendLine(const ln::StringRef& format, const TArgs&... args) { AppendLineInternal(ln::String::format(format, args...)); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	OutputBuffer& AppendLine(const OutputBuffer& buf) { AppendLineInternal(buf.toString()); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	OutputBuffer& AppendLines(const ln::StringRef& str) { AppendLinesInternal(str, ln::String::Empty); return *this; }

	/** 文字列を追加する (各行の先頭をインデント) */
	template<typename... TArgs>
	OutputBuffer& AppendLines(const ln::StringRef& format, const TArgs&... args) { AppendLinesInternal(ln::String::format(format, args...), ln::String::Empty); return *this; }
	
	/** 文字列を追加する (各行の先頭をインデント) */
	OutputBuffer& AppendLinesHeaderd(const ln::StringRef& str, const ln::StringRef& lineHeader) { AppendLinesInternal(str, lineHeader); return *this; }

    /** 文字列を追加する (各行の先頭をインデント) */
    OutputBuffer& AppendLinesHeaderd2(const ln::StringRef& lineHeader, const ln::StringRef& str) { AppendLinesInternal(str, lineHeader); return *this; }

	/** 既に文字列が存在すれば , を挿入して文字列を追加する */
	template<typename... TArgs>
	OutputBuffer& AppendCommad(const ln::StringRef& format, const TArgs&... args)
	{
		if (!isEmpty()) AppendInternal(m_commandSeparator);
		AppendInternal(ln::String::format(format, args...));
		return *this;
	}

	/** 改行する */
	OutputBuffer& NewLine(int count = 1);
	
	/** 現在のインデントレベル分の空白を追加する */
	void indent();
	
	/** ln::String 取得 */
	ln::String toString() const;

	void setCommandSeparator(const ln::String& value) { m_commandSeparator = value; }

	
private:
	void AppendInternal(const ln::StringRef& str);
	void AppendLineInternal(const ln::StringRef& str);
	void AppendLinesInternal(const ln::StringRef& str, const ln::StringRef& lineHeader);
	
	enum class State
	{
		LineHead,
		Text,
	};

	ln::String m_buffer;
	int m_indentLevel;
	ln::String m_indent;
	ln::String m_newLineCode;
	ln::String m_commandSeparator;
	State m_state;
};

