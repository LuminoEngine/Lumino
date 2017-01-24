
#pragma once
#include "Common.h"
#include "TokenList.h"

LN_NAMESPACE_BEGIN
namespace parser
{

class ParserUtils
{
public:

	// begin の次から始めて、次の空白以外のトークンの位置または end を返す
	template<typename TItr>
	static TItr SkipNextSpaceOrComment(TItr begin, TItr end)
	{
		++begin;
		while (begin < end && begin->IsSpaceOrComment()) ++begin;
		return begin;
	}

	// end の前から始めて、前の空白以外のトークンの位置または begin を返す
	template<typename TItr>
	static TItr SkipPrevSpaceOrComment(TItr begin, TItr end)
	{
		--end;
		while (begin < end && end->IsSpaceOrComment()) --end;
		return end;
	}


};

} // namespace Parser
LN_NAMESPACE_END

