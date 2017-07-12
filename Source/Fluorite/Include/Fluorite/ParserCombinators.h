/*
	一番シンプル
		static Result<Data> Statement(ParserContext& parser)
		{
			auto t1 = parser.Eval(Token(ln::parser::CommonTokenType::Identifier));
			auto t2 = parser.Eval(Token(ln::parser::CommonTokenType::Operator));
			auto t3 = parser.Eval(Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)));
			auto t4 = parser.Eval(Token(ln::parser::CommonTokenType::Operator));
			return parser.Success(Data{ t1.ToString(), t3.ToString() }, parser.GetPosition());
		}
	↑のをちゃんとした Parser を返すようにしたもの
		static Parser<Data> Statement(ParserContext& parser)
		{
			return [](ParserContext& parser)
			{
				auto t1 = parser.Eval(Token(ln::parser::CommonTokenType::Identifier));
				auto t2 = parser.Eval(Token(ln::parser::CommonTokenType::Operator));
				auto t3 = parser.Eval(Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)));
				auto t4 = parser.Eval(Token(ln::parser::CommonTokenType::Operator));
				return parser.Success(Data{ t1.ToString(), t3.ToString() }, parser.GetPosition());
			};
		}
	本当に関数型っぽくするならこうなる 1
		static Parser<Data> Statement(ParserContext& parser)
		{
			Parser<Token> p, t1, t2, t3, t4;
			p = (t1 = Token(ln::parser::CommonTokenType::Identifier))
			&&	(t2 = Token(ln::parser::CommonTokenType::Operator)))
			&&	(t3 = Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)))
			&&	(t4 = Token(ln::parser::CommonTokenType::Operator));
			return [p, t1, t3](ParserContext& parser) { return p(parser).Then( Data(t1.GetValue, t2.GetValue()) ) };
		}
	
	本当に関数型っぽくするならこうなる 2
		static Parser<Data> Statement(ParserContext& parser)	// 値渡しにするか、呼び出し元を DoParse とか1つかませないとだめ
		{
			return
				(parser[0] = Token(ln::parser::CommonTokenType::Identifier))
			|	(parser[1] = Token(ln::parser::CommonTokenType::Operator)))
			|	(parser[2] = Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)))
			|	(parser[3] = Token(ln::parser::CommonTokenType::Operator));
			->	[](ParserContext& parser) { return Data(parser[0].GetValue(), parser[1].GetValue()) };
		}
	
	一番シンプルなのをマクロ使うようにすると、例外longjmpしなくて済む
		static Result<Data> Statement(ParserContext& parser)
		{
			LN_PARSE(t1, Token(ln::parser::CommonTokenType::Identifier));
			LN_PARSE(t2, Token(ln::parser::CommonTokenType::Operator));
			LN_PARSE(t3, Or(ParseLib::Token(ln::parser::CommonTokenType::Identifier), Parser<ln::parser::Token>(Parser_texture_variable)));
			LN_PARSE(t4, Token(ln::parser::CommonTokenType::Operator));
			return parser.Success(Data{ t1.ToString(), t3.ToString() });
		}
*/

#pragma once

#define LN_PARSE(result, parser) \
	auto result##_ = (parser)(input); \
	if (result##_.IsFailed()) return input.Fail(result##_); \
	input.next(result##_); \
	auto result = result##_.getValue();

#define LN_PARSE_RESULT(result, parser) \
	auto result = (parser)(input); \
	if (result.IsFailed()) return input.Fail(result); \
	input.next(result);

//#define LN_PARSE_SUCCESS(value)	\
//	Success(value, input);

namespace fl {
namespace combinators {

using Iterator = TokenList::const_iterator;

namespace detail {

template<typename TCursor>
class ParserFailure
{
public:
	ParserFailure(const TCursor& cursor, const flStringRef& message_)
		: remainder(cursor)
		, message(message_)
	{}

	TCursor		remainder;
	flStringRef	message;
};

} // namespace detail


// ParserResult の決まりごと
//	- 成否 (true/false) を持つ
//	- 値を持つ (失敗の場合は不正値)
//	- 次の読み取り位置を持つ (remainder)
// T はパーサ関数の戻す値
template<typename T, typename TCursor>
class GenericParserResult
{
public:
	static GenericParserResult<T, TCursor> Success(const T& value, Iterator matchBegin, Iterator matchEnd, const TCursor& remainder)
	{
		return GenericParserResult<T, TCursor>(value, matchBegin, matchEnd, remainder, true, flString::getEmpty());
	}
	static GenericParserResult<T, TCursor> Fail(const TCursor& remainder, const flStringRef& message)
	{
		return GenericParserResult<T, TCursor>(T(), remainder, false, message);
	}

	const T& getValue() const { return m_value; }
	const TCursor& GetRemainder() const { return m_remainder; }	// 評価後の次の読み取り位置
	int GetRemainderPosition() const { return m_remainder.getPosition(); }
	bool IsSucceed() const { return m_isSuccess; }
	bool IsFailed() const { return !m_isSuccess; }
	Iterator GetMatchBegin() const { return m_matchBegin; }
	Iterator GetMatchEnd() const { return m_matchEnd; }
	const flString& getMessage() const { return m_message; }

	// Parser 関数の return で Fail() によって、Parse 失敗であることを伝え、Value を捨てるために使う
	GenericParserResult(const detail::ParserFailure<TCursor>& failer)
		: m_value()
		, m_matchBegin()
		, m_matchEnd()
		, m_remainder(failer.remainder)
		, m_isSuccess(false)
		, m_message(failer.message)
	{
	}
	
private:
	GenericParserResult(const T& value, Iterator matchBegin, Iterator matchEnd, const TCursor& remainder, bool isSuccess, const flStringRef& message)
		: m_value(value)
		, m_matchBegin(matchBegin)
		, m_matchEnd(matchEnd)
		, m_remainder(remainder)
		, m_isSuccess(isSuccess)
		, m_message(message)
	{
	}
	GenericParserResult(const T& value, const TCursor& remainder, bool isSuccess, const flStringRef& message)
		: m_value(value)
		, m_matchBegin()
		, m_matchEnd()
		, m_remainder(remainder)
		, m_isSuccess(isSuccess)
		, m_message(message)
	{
	}

	T			m_value;
	Iterator			m_matchBegin;
	Iterator			m_matchEnd;
	TCursor		m_remainder;
	bool		m_isSuccess;
	flString	m_message;
};

template<typename T>
class Option
{
public:
	// TODO: move

	static Option Some(const T& value) { return Option(value); }
	static Option None() { return Option(); }

	bool isEmpty() const { return m_empty; }
	const T& getValue() const { return m_value; }

private:
	Option(const T& value)
		: m_value(value)
		, m_empty(false)
	{}
	Option()
		: m_value()
		, m_empty(true)
	{}

	T		m_value;
	bool	m_empty;
};

struct ParserCursorConditional
{
	struct Always
	{
		//template<typename T>
		//bool operator()(const T& value)
		//{
		//	return true;
		//}
		static bool FilterToken(Token* value)
		{
			return true;
		}
	};
};

template<typename TTokenFilter = ParserCursorConditional::Always>
class GenericParserCursor
{
public:
	using FilterType = TTokenFilter;

	GenericParserCursor()
		: m_current()
		, m_end()
	{}

	// パース開始時の初期化用
	GenericParserCursor(const TokenList* tokenList)
		: m_current(tokenList->begin())
		, m_end(tokenList->end())
	{
	}

	// パース開始時の初期化用
	GenericParserCursor(const Iterator& begin, const Iterator& end)
		: m_current(begin)
		, m_end(end)
	{
	}
	
	//GenericParserCursor(const TokenList* tokenList, int position)
	//	: m_tokenList(tokenList)
	//	, m_position(position)
	//	, m_end(tokenList->GetCount())
	//{
	//}

	GenericParserCursor(const GenericParserCursor& obj)
		: m_current(obj.m_current)
		, m_end(obj.m_end)
	{
	}

	GenericParserCursor& operator=(const GenericParserCursor& obj)
	{
		m_current = obj.m_current;
		m_end = obj.m_end;
		return *this;
	}

	Token* GetCurrentValue() const
	{
		return *m_current;
	}

	Iterator getPosition() const
	{
		return m_current;
	}

	GenericParserCursor Cuing() const
	{
		auto pos = m_current;
		while (!TTokenFilter::FilterToken(*pos))
		{
			++pos;
		};
		return GenericParserCursor(pos, m_end);
	}

	GenericParserCursor Advance() const
	{
		if (m_current == m_end)
		{
			LN_THROW(0, ln::InvalidOperationException, "end of source.");
		}

		auto pos = m_current;
		do
		{
			++pos;
		} while (pos < m_end && !TTokenFilter::FilterToken(*pos));

		return GenericParserCursor(pos, m_end);
	}

private:
	//const TokenList*	m_tokenList;
	Iterator	m_current;
	Iterator	m_end;
	//int					m_position;
	//int					m_end;
};


template<typename TCursor>
class GenericParserContext
{
public:
	GenericParserContext(const TCursor& input)
		: m_start(input)
		, m_current(input)
	{
	}

	GenericParserContext(const GenericParserContext& obj)
		: m_start(obj.m_current)
		, m_current(obj.m_current)
	{
	}

	TCursor GetNext() const
	{
		return m_current.Advance();
		//m_current = m_current.Advance();
		//return m_current;
	}

	Token* GetCurrentValue() const
	{
		return m_current.GetCurrentValue();
	}

	const TCursor& GetStartCursor() const
	{
		return m_start;
	}

	Iterator GetStartPosition() const
	{
		return m_start.getPosition();
	}

	Iterator GetLastMatchEndPosition() const
	{
		return m_last.getPosition() + 1;	// 最後のマッチ位置の次
	}

	const TCursor& GetCurrentCursor() const
	{
		return m_current;
	}

	template<typename T>
	void next(const GenericParserResult<T, TCursor>& result/*const TCursor& newPos*/)
	{
		m_last = m_current;
		m_current = result.GetRemainder();
	}

	template<typename T>
	GenericParserResult<T, TCursor> Success(const T& value)
	{
		return GenericParserResult<T, TCursor>::Success(value, GetStartPosition(), GetLastMatchEndPosition(), m_current);
	}

	template<typename TResult>
	detail::ParserFailure<TCursor> Fail(const TResult& result)
	{
		return detail::ParserFailure<TCursor>(GetCurrentCursor(), result.getMessage());
	}

private:
	TCursor		m_start;
	TCursor		m_current;
	TCursor		m_last;
};


template<typename TValue, typename TCursor, typename TContext>
class GenericParser : public std::function<GenericParserResult<TValue, TCursor>(TContext)>//public ln::Delegate<GenericParserResult<TValue, TCursor>(TContext)>
{
public:
	template<typename TParserFunc>
	GenericParser(TParserFunc func)
		: std::function<GenericParserResult<TValue, TCursor>(TContext)>(func)
	{
	}

	GenericParser operator||(const GenericParser& second) const
	{
		return ParseLib<TCursor::FilterType>::Or(*this, second);
	}
};




template<typename TTokenFilter = ParserCursorConditional::Always>
class ParseLib
{
public:
	using ValueT = Token*;

	using ParserContext = GenericParserContext<GenericParserCursor<TTokenFilter>>;

	template<typename TValue>
	using Parser = GenericParser<TValue, GenericParserCursor<TTokenFilter>, ParserContext>;

	template<typename TValue>
	using ParserResult = GenericParserResult<TValue, GenericParserCursor<TTokenFilter>>;

	using ParserCursor = GenericParserCursor<TTokenFilter>;


	/** 指定した1文字のトークンにマッチする */
	static Parser<ValueT> TokenChar(char ch)
	{
		return [ch](ParserContext input)
		{
			fl::Token* tok = input.GetCurrentValue();
			if (tok->EqualChar(ch))
				return ParserResult<ValueT>::Success(tok, input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor(), flString::format("Unexpected token \"{0}\". expected \"{1}\"", tok->getString(), ch));
		};
	}

	static Parser<ValueT> TokenString(const char* str_)
	{
		flString str = str_;
		return [str](ParserContext input)
		{
			fl::Token* tok = input.GetCurrentValue();
			if (tok->EqualString(str.c_str(), str.getLength()))
				return ParserResult<ValueT>::Success(tok, input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor(), flString::format("Unexpected token \"{0}\". expected \"{1}\"", tok->getString(), str));
		};
	}

	static Parser<ValueT> Token(TokenGroup type)
	{
		return [type](ParserContext input)
		{
			fl::Token* tok = input.GetCurrentValue();
			if (tok->GetTokenGroup() == type)
				return ParserResult<ValueT>::Success(tok, input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor(), flString::format("Unexpected token group \"{0}\". expected \"{1}\"", (int)tok->GetTokenGroup(), (int)type));
		};
	}

	static Parser<ValueT> Token(TokenGroup type, char ch)
	{
		return [type, ch](ParserContext input)
		{
			if (input.GetCurrentValue().GetCommonType() == type && input.GetCurrentValue().EqualChar(ch))
				return ParserResult<ValueT>::Success(input.GetCurrentValue(), input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor());
		};
	}

	static Parser<ValueT> Token(TokenGroup type, const char* string, int len)
	{
		return [type, string, len](ParserContext input)
		{
			if (input.GetCurrentValue().GetCommonType() == type && input.GetCurrentValue().EqualString(string, len))
				return ParserResult<ValueT>::Success(input.GetCurrentValue(), input.GetStartPosition(), input.GetStartPosition() + 1, input.GetNext());
			return ParserResult<ValueT>::Fail(input.GetCurrentCursor());
		};
	}

	// 0回以上の繰り返し
	template<typename T>
	static Parser<List<T>> Many(const Parser<T>& parser)
	{
		return [parser](ParserContext input)
		{
			List<T> list;
			auto r = parser(input);
			//input = r.GetRemainder();

			while (r.IsSucceed())
			{
				list.add(r.getValue());
				r = parser(r.GetRemainder());
				//input = r.GetRemainder();
			}
			return ParserResult<List<T>>::Success(list, input.GetStartPosition(), r.GetMatchEnd(), r.GetRemainder());
		};
	}

	template<typename T>
	static Parser<T> Or(const Parser<T>& first, const Parser<T>& second)
	{
		//Parser<T> second(second_);
		return [first, second](ParserContext input)
		{
			auto fr = first(input);
			if (fr.IsFailed())
			{
				return second(input);
			}
			return fr;
		};
	}

	template<typename T>
	static Parser<Option<T>> Optional(const Parser<T>& parser)
	{
		return [parser](ParserContext input)
		{
			auto r = parser(input);
			if (r.IsSucceed())
			{
				return ParserResult<Option<T>>::Success(Option<T>::Some(r.getValue()), input.GetStartPosition(), r.GetMatchEnd(), r.GetRemainder());
			}
			return ParserResult<Option<T>>::Success(Option<T>::None(), input.GetStartPosition(), input.GetStartPosition(), input.GetStartCursor());
		};
	}

	//// term までをマッチの範囲とし、
	//// ターミネータを result に含まず、ターミネータを消費しない
	//template<typename T>
	//static Parser<List<T>> UntilPrev(const Parser<T>& term)
	//{
	//	return [term](ParserContext input)
	//	{
	//		//List<T> list;
	//		//auto lastResultCursor = input.GetCurrentCursor();
	//		//auto r = term(input);	// ParserResults

	//		//while (r.IsFailed())
	//		//{
	//		//	list.Add(r.GetValue());
	//		//	lastResult = r;
	//		//	r = term(r.GetRemainder().Advance());
	//		//}


	//		List<T> list;
	//		auto mathEnd = input.GetStartPosition();
	//		auto lastCursor = input.GetCurrentCursor();

	//		auto r = term(input);
	//		auto lastResult = r;

	//		while (r.IsFailed())
	//		{
	//			list.Add(r.GetValue());
	//			lastResult = r;
	//			mathEnd = rlastResult.GetMatchEnd();
	//			lastCursor = lastResult.GetRemainder();

	//			r = term(r.GetRemainder().Advance());
	//		}

	//		// TODO: ストリーム末尾までfailedだったらパース失敗

	//		return ParserResult<List<T>>::Success(list, input.GetStartPosition(), lastResultCursor.GetLastMatchEndPosition(), lastCursor);
	//	};
	//}

	// term までをマッチの範囲とし、
	// ターミネータを result に含む
	template<typename T>
	static Parser<List<T>> UntilMore(const Parser<T>& term)
	{
		return [term](ParserContext input)
		{
			List<T> list;
			auto r = term(input);
			auto lastResult = r;

			while (r.IsFailed())
			{
				list.add(r.getValue());
				lastResult = r;
				r = term(r.GetRemainder().Advance());
			}

			// TODO: ストリーム末尾までfailedだったらパース失敗

			return ParserResult<List<T>>::Success(list, input.GetStartPosition(), r.GetMatchEnd(), r.GetRemainder());
		};
	}

	template<typename T>
	static ParserResult<T> TryParse(const Parser<T>& parser, const TokenList* tokenList)
	{
		ParserCursor input(tokenList);
		ParserResult<T> result = parser(input.Cuing());
		return result;
	}

	template<typename T>
	static ParserResult<T> TryParse(ParserResult<T>(*parser)(ParserContext), const TokenList* tokenList)
	{
		ParserCursor input(tokenList);
		ParserResult<T> result = parser(input.Cuing());
		return result;
	}

	template<typename T>
	static ParserResult<T> TryParse(ParserResult<T>(*parser)(ParserContext), Iterator begin, Iterator end)
	{
		ParserCursor input(begin, end);
		ParserResult<T> result = parser(input.Cuing());
		return result;
	}

	static bool FilterToken(fl::Token* token)
	{
		return true;
	}
};


//struct ParserCursorCondition_SkipSpace
//{
//	bool operator()(const Token& token)
//	{
//		return
//			token.EqualChar('<') || token.EqualChar('>') || token.EqualChar('{') || token.EqualChar('}') ||
//			token.EqualString("sampler_state", 13) ||
//			token.EqualString("technique", 9) ||
//			token.EqualString("pass", 4) ||
//			token.IsEof();	// TODO: これが無くてもいいようにしたい。今はこれがないと、Many中にEOFしたときOutOfRangeする
//	}
//};
//
//using ParserCursor_SkipSpace = combinators::GenericParserCursor<ParserCursorCondition_SkipSpace>;

} // namespace combinators
} // namespace fl
