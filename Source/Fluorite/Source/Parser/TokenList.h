
#pragma once
#include "Token.h"

LN_NAMESPACE_BEGIN
namespace parser
{
	
/**
	@brief	
*/
class TokenList
	: public RefObject
	, public Collection<Token>
{
//public:
//	typedef Collection<Token>::iterator iterator;
//	typedef Collection<Token>::const_iterator const_iterator;

public:
	TokenList() {}
	~TokenList() {}

	String ToString(int begin, int end) const
	{
		StringBuilder sb;
		for (int i = begin; i < end; ++i)
		{
			sb.Append(GetAt(i).GetBegin(), GetAt(i).GetLength());
		}
		return sb.ToString();
	}

	String ToStringValidCode() const
	{
		StringBuilder sb;
		for (int i = 0; i < GetCount(); ++i)
		{
			if (GetAt(i).IsValid())
			{
				sb.Append(GetAt(i).GetBegin(), GetAt(i).GetLength());
			}
		}
		return sb.ToString();
	}

//	void Reserve(size_t size) { m_list.Reserve(size); }
//	void Resize(size_t size) { m_list.Resize(size); }
//	void Add(const Token& token) { m_list.Add(token); }
//	Token& GetAt(int index) { return m_list.GetAt(index); }
//	int GetCount() const { return m_list.GetCount(); }
//
//private:
//	Array<Token>	m_list;
};

} // namespace Parser
LN_NAMESPACE_END

