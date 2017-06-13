
#include "../Internal.h"
#include "../Lexer/CppLexer.h"
#include "RpnParser.h"

namespace fl {

/*
	C++ の演算子と優先順位
	http://ja.cppreference.com/w/cpp/language/operator_precedence
	1+2+3	→ (1+2)+3	… 左結合 (Left-to-right)
	a=b=1	→ a=(b=1)	… 右結合 (Right-to-left)
*/

/*
	■ 条件演算子の解析

		1 ? 6 ? 7 : 8 : 3 ? 4 : 5		分かりやすく括弧を付けると→ 1 ? (6 ? 7 : 8) : (3 ? 4 : 5)

	という式は、Parse では以下のように展開できれば良い。

		idx   >  0  1  2  3  4  5  6  7  8  9 10 11 12
		token >  1  ?  6  ?  7  :  8  :  3  ?  4  :  5
		goto  >     8     6    13    13    12    13

	Eval では、
	・トークンは左から右へ読んでいく。
	・? の goto は、条件式(オペランstack top)が false だった場合にジャンプする idx を示す。
	・: の goto は、ここにたどり着いたときにジャンプする idx を示す。(一律、式の終端(※)である)
	※「式の終端」とは、')' またはバッファ終端である。

	?: 以下の優先度の演算子は代入系とthrow、, だが、これらを考慮しない場合は
	たとえどれだけ : で区切られようとも : でジャンプするのは必ず式の終端である。

*/

/*
	■ 関数呼び出しの解析

	"1+2" のような関数呼び出しの無い単純な式でも、一番外側にはダミーの関数呼び出しがあるものとして解析する。
	イメージとしては "_(1+2)"。

	式の終端を ) であると考えることで、条件演算子や , 演算子との兼ね合いも少し楽になる。かも。
	↓
	, は全て、引数の区切りと考える。カンマ演算子も。
	そして、ダミー関数は一番後ろの引数の評価結果を返す関数とする。

	"A(1+1,2)" のパース結果は、
	
		1 1 + 2 A _

	となり、トークン A と _ の種別は「関数」。プロパティとして、引数の数を持つ。(_ はダミー関数である点に注意)
	評価では、この引数の数だけオペランド(評価済み定数)を取り出し、関数の処理を行う。
	A はユーザー定義。_ は最後の引数を返す。


	"A(1+2,3+4,5+6)" をパースするときは・・・
	, が見つかるたびに、opStack にある FuncCall までを出力リストに移す。
	) が見つかったら、FuncCall までを移し変えた後、FuncCall も消す。
	
	■ カンマ演算子の解析
	
	前述の通り、ダミー関数を利用する。
	ダミー関数は一番後ろの引数の評価結果を返す関数とする。
*/


// LN_RPN_OPERATOR_DEFINE_xxxx マクロで、RPN トークン種別のテーブルに登録する演算子の実装
// http://en.cppreference.com/w/cpp/language/expressions
// http://en.cppreference.com/w/cpp/language/operator_logical
struct RpnOperator
{
	template<typename T> static void UnaryPlus(T /*lhs*/, T rhs, RpnOperand* out) { out->set(rhs); }
	template<typename T> static void UnaryMinus(T /*lhs*/, T rhs, RpnOperand* out) { out->set(-rhs); }
	template<> static void UnaryMinus<uint32_t>(uint32_t /*lhs*/, uint32_t rhs, RpnOperand* out) { out->set((uint32_t)-((int32_t)rhs)); }	// 警告回避
	template<> static void UnaryMinus<uint64_t>(uint64_t /*lhs*/, uint64_t rhs, RpnOperand* out) { out->set((uint64_t)-((int64_t)rhs)); }	// 警告回避

	template<typename T> static void multiply(T lhs, T rhs, RpnOperand* out)	{ out->set(lhs * rhs); }
	template<typename T> static void Divide(T lhs, T rhs, RpnOperand* out)		{ out->set(lhs / rhs); }
	template<typename T> static void Modulus(T lhs, T rhs, RpnOperand* out)		{ out->set(lhs % rhs); }
	template<typename T> static void BinaryPlus(T lhs, T rhs, RpnOperand* out)	{ out->set(lhs + rhs); }
	template<typename T> static void BinaryMinus(T lhs, T rhs, RpnOperand* out) { out->set(lhs - rhs); }

	template<typename T> static void LeftShift(T lhs, T rhs, RpnOperand* out)	{ out->set(lhs << rhs); }
	template<typename T> static void RightShift(T lhs, T rhs, RpnOperand* out)	{ out->set(lhs >> rhs); }

	template<typename T> static void CompLessThan(T lhs, T rhs, RpnOperand* out)			{ out->set(lhs < rhs); }
	template<typename T> static void CompLessThanEqual(T lhs, T rhs, RpnOperand* out)		{ out->set(lhs <= rhs); }
	template<typename T> static void CompGreaterThen(T lhs, T rhs, RpnOperand* out)			{ out->set(lhs > rhs); }
	template<typename T> static void CompGreaterThenEqual(T lhs, T rhs, RpnOperand* out)	{ out->set(lhs >= rhs); }
	template<typename T> static void CompEqual(T lhs, T rhs, RpnOperand* out)				{ out->set(lhs == rhs); }
	template<typename T> static void CompNotEqual(T lhs, T rhs, RpnOperand* out)			{ out->set(lhs != rhs); }

	template<typename T> static void BitwiseNot(T /*lhs*/, T rhs, RpnOperand* out)	{ out->set(~rhs); }
	template<typename T> static void BitwiseAnd(T lhs, T rhs, RpnOperand* out)		{ out->set(lhs & rhs); }
	template<typename T> static void BitwiseXor(T lhs, T rhs, RpnOperand* out)		{ out->set(lhs ^ rhs); }
	template<typename T> static void BitwiseOr(T lhs, T rhs, RpnOperand* out)		{ out->set(lhs | rhs); }

	template<typename T> static void LogicalNot(T /*lhs*/, T rhs, RpnOperand* out)		{ out->set(!(rhs != 0)); }
	template<typename T> static void LogicalAnd(T lhs, T rhs, RpnOperand* out)			{ out->set((lhs != 0) && (rhs != 0)); }
	template<typename T> static void LogicalOr(T lhs, T rhs, RpnOperand* out)			{ out->set((lhs != 0) || (rhs != 0)); }
	template<> static void LogicalNot<bool>(bool /*lhs*/, bool rhs, RpnOperand* out)	{ out->set(!rhs); }
	template<> static void LogicalAnd<bool>(bool lhs, bool rhs, RpnOperand* out)		{ out->set(lhs && rhs); }
	template<> static void LogicalOr<bool>(bool lhs, bool rhs, RpnOperand* out)			{ out->set(lhs || rhs); }
};

typedef void(*NullOperator)(nullptr_t lhs, nullptr_t rhs, RpnOperand* out);
typedef void(*BooleanOperator)(bool lhs, bool rhs, RpnOperand* out);
typedef void(*Int32Operator)(int32_t lhs, int32_t rhs, RpnOperand* out);
typedef void(*UInt32Operator)(uint32_t lhs, uint32_t rhs, RpnOperand* out);
typedef void(*Int64Operator)(int64_t lhs, int64_t rhs, RpnOperand* out);
typedef void(*UInt64Operator)(uint64_t lhs, uint64_t rhs, RpnOperand* out);
typedef void(*FloatOperator)(float lhs, float rhs, RpnOperand* out);
typedef void(*DoubleOperator)(double lhs, double rhs, RpnOperand* out);

struct RpnTypedOperatorTableItem
{
	NullOperator	operaotrNullptr;
	BooleanOperator	operaotrBoolean;
	Int32Operator	operaotrInt32;
	UInt32Operator	operaotrUInt32;
	Int64Operator	operaotrInt64;
	UInt64Operator	operaotrUInt64;
	FloatOperator	operaotrFloat;
	DoubleOperator	operaotrDouble;
};

#define LN_RPN_OPERATOR_DEFINE_NONE \
	{ nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, }

#define LN_RPN_OPERATOR_DEFINE(op) \
	{ nullptr, nullptr, RpnOperator::op<int32_t>, RpnOperator::op<uint32_t>, RpnOperator::op<int64_t>, RpnOperator::op<uint64_t>, RpnOperator::op<float>, RpnOperator::op<double>, }

#define LN_RPN_OPERATOR_DEFINE_INTEGER(op) \
	{ nullptr, nullptr, RpnOperator::op<int32_t>, RpnOperator::op<uint32_t>, RpnOperator::op<int64_t>, RpnOperator::op<uint64_t>, nullptr, nullptr, }

#define LN_RPN_OPERATOR_DEFINE_LOGICAL(op) \
	{ nullptr, RpnOperator::op<bool>, RpnOperator::op<int32_t>, RpnOperator::op<uint32_t>, RpnOperator::op<int64_t>, RpnOperator::op<uint64_t>, RpnOperator::op<float>, RpnOperator::op<double>, }

struct TokenTypeTableItem
{
	RpnTokenGroup		tokenGroup;
	RpnOperatorGroup	operatorGroup;
	bool				isUnary;
	RpnTypedOperatorTableItem	eval;
};
static TokenTypeTableItem g_tokenTypeTable[] = 
{
	{ RpnTokenGroup::Unknown,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_Unknown = 0,
	{ RpnTokenGroup::Identifier,	RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_Identifier,				///< 識別子
	//{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLiteral,			///< 数値リテラル
	{ RpnTokenGroup::Unknown,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_OP_GroupStart,			// (	※ 括弧はパースで取り除かれるので Unknown
	{ RpnTokenGroup::Unknown,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_OP_GroupEnd,				// )	※ 括弧はパースで取り除かれるので Unknown
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Arithmetic,	true,	LN_RPN_OPERATOR_DEFINE(UnaryPlus),},			// RPN_TT_OP_UnaryPlus,			// + (Unary)
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Arithmetic,	true,	LN_RPN_OPERATOR_DEFINE(UnaryMinus), },			// RPN_TT_OP_UnaryMinus,			// - (Unary)
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Arithmetic,	false,	LN_RPN_OPERATOR_DEFINE(multiply), },		// RPN_TT_OP_Multiply,				// *
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Arithmetic,	false,	LN_RPN_OPERATOR_DEFINE(Divide), },		// RPN_TT_OP_Divide,				// /
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Arithmetic,	false,	LN_RPN_OPERATOR_DEFINE_INTEGER(Modulus), },		// RPN_TT_OP_Modulus,				// %
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Arithmetic,	false,	LN_RPN_OPERATOR_DEFINE(BinaryPlus), },		// RPN_TT_OP_BinaryPlus,			// + (Binary)
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Arithmetic,	false,	LN_RPN_OPERATOR_DEFINE(BinaryMinus), },		// RPN_TT_OP_BinaryMinus,			// - (Binary)
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Bitwise,		false,	LN_RPN_OPERATOR_DEFINE_INTEGER(LeftShift), },		// RPN_TT_OP_LeftShift,			// <<
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Bitwise,		false,	LN_RPN_OPERATOR_DEFINE_INTEGER(RightShift), },		// RPN_TT_OP_RightShift,			// >>
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Comparison,	false,	LN_RPN_OPERATOR_DEFINE_LOGICAL(CompLessThan), },		// RPN_TT_OP_CompLessThan,			// <
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Comparison,	false,	LN_RPN_OPERATOR_DEFINE_LOGICAL(CompLessThanEqual), },		// RPN_TT_OP_CompLessThanEqual,	// <=
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Comparison,	false,	LN_RPN_OPERATOR_DEFINE_LOGICAL(CompGreaterThen), },		// RPN_TT_OP_CompGreaterThen,		// >
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Comparison,	false,	LN_RPN_OPERATOR_DEFINE_LOGICAL(CompGreaterThenEqual), },		// RPN_TT_OP_CompGreaterThenEqual,	// >=
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Comparison,	false,	LN_RPN_OPERATOR_DEFINE_LOGICAL(CompEqual), },		// RPN_TT_OP_CompEqual,			// ==
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Comparison,	false,	LN_RPN_OPERATOR_DEFINE_LOGICAL(CompNotEqual), },		// RPN_TT_OP_CompNotEqual,			// !=
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Bitwise,		true,	LN_RPN_OPERATOR_DEFINE_INTEGER(BitwiseNot), },			// RPN_TT_OP_BitwiseNot,			// ~ (Unary)
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Bitwise,		false,	LN_RPN_OPERATOR_DEFINE_INTEGER(BitwiseAnd), },		// RPN_TT_OP_BitwiseAnd,			// &
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Bitwise,		false,	LN_RPN_OPERATOR_DEFINE_INTEGER(BitwiseXor), },		// RPN_TT_OP_BitwiseXor,			// ^
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Bitwise,		false,	LN_RPN_OPERATOR_DEFINE_INTEGER(BitwiseOr), },		// RPN_TT_OP_BitwiseOr,			// |
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Logical,		true,	LN_RPN_OPERATOR_DEFINE_LOGICAL(LogicalNot), },			// RPN_TT_OP_LogicalNot,			// ! (Unary)
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Logical,		false,	LN_RPN_OPERATOR_DEFINE_LOGICAL(LogicalAnd), },		// RPN_TT_OP_LogicalAnd,			// &&
	{ RpnTokenGroup::Operator,		RpnOperatorGroup::Logical,		false,	LN_RPN_OPERATOR_DEFINE_LOGICAL(LogicalOr), },		// RPN_TT_OP_LogicalOr,			// ||
	{ RpnTokenGroup::CondTrue,		RpnOperatorGroup::Conditional,	false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_OP_CondTrue,				// ? (条件演算子)
	{ RpnTokenGroup::CondFalse,		RpnOperatorGroup::Conditional,	false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_OP_CondFalse,			// : (条件演算子)
	{ RpnTokenGroup::Unknown,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_OP_Comma,				// , (カンマ演算子 or 実引数区切り文字)	※未対応
	{ RpnTokenGroup::FunctionCall,	RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_OP_FuncCall,				///< 関数呼び出し (識別子部分を指す)
	
	{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLitaral_Null,
	{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLitaral_True,
	{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLitaral_False,
	{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLitaral_Int32,
	{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLitaral_UInt32,
	{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLitaral_Int64,
	{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLitaral_UInt64,
	{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLitaral_Float,	/**< 32bit */
	{ RpnTokenGroup::Literal,		RpnOperatorGroup::Unknown,		false,	LN_RPN_OPERATOR_DEFINE_NONE, },		// RPN_TT_NumericLitaral_Double,	/**< 64bit */
};

//==============================================================================
// RpnToken
//==============================================================================

//------------------------------------------------------------------------------
RpnTokenGroup RpnToken::GetTokenGroup() const
{
	return g_tokenTypeTable[Type].tokenGroup;
}

//------------------------------------------------------------------------------
RpnOperatorGroup RpnToken::GetOperatorGroup() const
{
	return g_tokenTypeTable[Type].operatorGroup;
}

//------------------------------------------------------------------------------
bool RpnToken::IsUnary() const
{
	return g_tokenTypeTable[Type].isUnary;
}

//==============================================================================
// RpnParser
//==============================================================================

//------------------------------------------------------------------------------
ResultState RpnParser::ParseCppConstExpression2(Position exprBegin, Position exprEnd, DiagnosticsItemSet* diag)
{
	initialize(diag);
	TokenizeCppConst(exprBegin, exprEnd);
	parse();
	return ResultState::Success;
}

//------------------------------------------------------------------------------
void RpnParser::initialize(DiagnosticsItemSet* diag)
{
	if (m_tokenList != nullptr) {
		m_tokenList->clear();
	}
	else {
		m_tokenList.attach(LN_NEW RpnTokenList());
	}
	if (m_rpnTokenList != nullptr) {
		m_rpnTokenList->clear();
	}
	else {
		m_rpnTokenList.attach(LN_NEW RpnTokenList());
	}
	m_diag = diag;
	m_tmpRPNTokenList.clear();
	m_opStack.clear();
	m_condStack.clear();
	m_groupStack.clear();
	m_lastToken = nullptr;
}

//------------------------------------------------------------------------------
void RpnParser::TokenizeCppConst(Position exprBegin, Position exprEnd)
{
	// とりあえず入力トークン数でメモリ確保 (スペースが含まれていれば Tokenize 後の使用量は少なくなるはず)
	m_tokenList->reserve(exprEnd - exprBegin + 2);

	// 実引数リスト解析処理が , 演算子の解析を兼ねられるように、リスト先頭にダミーの FuncCall を入れておく
	RpnToken headToken;
	headToken.Type = RPN_TT_OP_FuncCall;
	m_tokenList->add(headToken);

	Position pos = exprBegin;

	for (; pos != exprEnd; ++pos)
	{
		switch ((*pos)->GetTokenGroup())
		{
		case TokenGroup::SpaceSequence:
		case TokenGroup::NewLine:
		case TokenGroup::Comment:
		case TokenGroup::Unknown:
			break;	// これらは空白扱い。何もせず次へ
		case TokenGroup::Identifier:
		{
			RpnToken token;
			token.Type = RPN_TT_Identifier;
			token.SourceToken = (*pos);
			m_tokenList->add(token);
			break;
		}
		case TokenGroup::ArithmeticLiteral:
		{
			RpnToken token;
			switch ((*pos)->getTokenType())
			{
			case TT_NumericLitaralType_Char:		token.Type = RPN_TT_NumericLitaral_Int32; break;
			case TT_NumericLitaralType_WideChar:	token.Type = RPN_TT_NumericLitaral_Int32; break;
			case TT_NumericLitaralType_Int32:		token.Type = RPN_TT_NumericLitaral_Int32; break;
			case TT_NumericLitaralType_UInt32:		token.Type = RPN_TT_NumericLitaral_UInt32; break;
			case TT_NumericLitaralType_Int64:		token.Type = RPN_TT_NumericLitaral_Int64; break;
			case TT_NumericLitaralType_UInt64:		token.Type = RPN_TT_NumericLitaral_UInt64; break;
			case TT_NumericLitaralType_Float:		token.Type = RPN_TT_NumericLitaral_Float; break;
			case TT_NumericLitaralType_Double:		token.Type = RPN_TT_NumericLitaral_Double; break;
			default:
				m_diag->Report(DiagnosticsCode::RpnEvaluator_InvalidNumericType);
				return;	// TODO: Result
			}
			token.SourceToken = (*pos);
			m_tokenList->add(token);
			break;
		}
		case TokenGroup::Operator:
		{
			static const RpnTokenType CppTypeToRPNType[] =
			{
				/* TT_CppOP_SeparatorBegin		*/	RPN_TT_Unknown,
				/* TT_CppOP_SharpSharp  	##	*/	RPN_TT_Unknown,
				/* TT_CppOP_Sharp			#	*/	RPN_TT_Unknown,
				/* TT_CppOP_ArrowAsterisk	->*	*/	RPN_TT_Unknown,
				/* TT_CppOP_Arrow			->	*/	RPN_TT_Unknown,
				/* TT_CppOP_Comma			,	*/	RPN_TT_OP_Comma,
				/* TT_CppOP_Increment		++	*/	RPN_TT_Unknown,
				/* TT_CppOP_Decrement		--	*/	RPN_TT_Unknown,
				/* TT_CppOP_LogicalAnd		&&	*/	RPN_TT_OP_LogicalAnd,
				/* TT_CppOP_LogicalOr		||	*/	RPN_TT_OP_LogicalOr,
				/* TT_CppOP_LessThenEqual	<=	*/	RPN_TT_OP_CompLessThanEqual,
				/* TT_CppOP_GreaterThenEqual>=	*/	RPN_TT_OP_CompGreaterThenEqual,
				/* TT_CppOP_CmpEqual		==	*/	RPN_TT_OP_CompEqual,
				/* TT_CppOP_LeftShiftEqual	<<=	*/	RPN_TT_Unknown,
				/* TT_CppOP_RightShiftEqual	>>=	*/	RPN_TT_Unknown,
				/* TT_CppOP_PlusEqual		+=	*/	RPN_TT_Unknown,
				/* TT_CppOP_MinusEqual		-=	*/	RPN_TT_Unknown,
				/* TT_CppOP_MulEqual		*=	*/	RPN_TT_Unknown,
				/* TT_CppOP_DivEqual		/=	*/	RPN_TT_Unknown,
				/* TT_CppOP_ModEqual		%=	*/	RPN_TT_Unknown,
				/* TT_CppOP_AndEqual		&=	*/	RPN_TT_Unknown,
				/* TT_CppOP_OrEqual			|=	*/	RPN_TT_Unknown,
				/* TT_CppOP_NotEqual		!=	*/	RPN_TT_OP_CompNotEqual,
				/* TT_CppOP_Equal			=	*/	RPN_TT_Unknown,
				/* TT_CppOP_LeftShift		<<	*/	RPN_TT_OP_LeftShift,
				/* TT_CppOP_RightShift		>>	*/	RPN_TT_OP_RightShift,
				/* TT_CppOP_Plus			+	*/	RPN_TT_OP_BinaryPlus,
				/* TT_CppOP_Minul			-	*/	RPN_TT_OP_BinaryMinus,
				/* TT_CppOP_Asterisk		*	*/	RPN_TT_OP_Multiply,
				/* TT_CppOP_Slash			/	*/	RPN_TT_OP_Divide,
				/* TT_CppOP_Parseint		%	*/	RPN_TT_OP_Modulus,
				/* TT_CppOP_Ampersand		&	*/	RPN_TT_OP_BitwiseAnd,
				/* TT_CppOP_Pipe			|	*/	RPN_TT_OP_BitwiseOr,
				/* TT_CppOP_Tilde			~	*/	RPN_TT_OP_BitwiseNot,
				/* TT_CppOP_Caret,			^	*/	RPN_TT_OP_BitwiseXor,
				/* TT_CppOP_Exclamation		!	*/	RPN_TT_OP_LogicalNot,
				/* TT_CppOP_Ellipsis		...	*/	RPN_TT_Unknown,
				/* TT_CppOP_DotAsterisk		.*	*/	RPN_TT_Unknown,
				/* TT_CppOP_Dot				.	*/	RPN_TT_Unknown,
				/* TT_CppOP_DoubleColon		::	*/	RPN_TT_Unknown,
				/* TT_CppOP_Question		?	*/	RPN_TT_OP_CondTrue,
				/* TT_CppOP_Colon			:	*/	RPN_TT_OP_CondFalse,
				/* TT_CppOP_Semicolon		;	*/	RPN_TT_Unknown,
				/* TT_CppOP_LeftBrace		{	*/	RPN_TT_Unknown,
				/* TT_CppOP_RightBrace		}	*/	RPN_TT_Unknown,
				/* TT_CppOP_LeftBracket		[	*/	RPN_TT_Unknown,
				/* TT_CppOP_RightBracket	]	*/	RPN_TT_Unknown,
				/* TT_CppOP_LeftParen		(	*/	RPN_TT_OP_GroupStart,
				/* TT_CppOP_RightParen		)	*/	RPN_TT_OP_GroupEnd,
				/* TT_CppOP_LeftAngle		<	*/	RPN_TT_OP_CompLessThan,
				/* TT_CppOP_RightAngle		>	*/	RPN_TT_OP_CompGreaterThen,
			};
			assert(LN_ARRAY_SIZE_OF(CppTypeToRPNType) == (TT_CppOP_SeparatorEnd - TT_CppOP_SeparatorBegin));

			struct CppTokenInfo
			{
				int					Precedence;		///< 優先順位
				OpeatorAssociation	Association;	///< 結合方向
			};
			static const CppTokenInfo TokenInfoTable[] =
			{
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_Unknown,				// (Dummy)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_Identifier,			// (Dummy)
				//{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLiteral,		// (Dummy)
				{ 2,	OpeatorAssociation_Left },	// RPN_TT_OP_GroupStart,		// (
				{ 2,	OpeatorAssociation_Left },	// RPN_TT_OP_GroupEnd,			// )
				{ 3,	OpeatorAssociation_Right },	// RPN_TT_OP_UnaryPlus,			// +
				{ 3,	OpeatorAssociation_Right },	// RPN_TT_OP_UnaryMinus,		// -
				{ 5,	OpeatorAssociation_Left },	// RPN_TT_OP_Multiply,			// *
				{ 5,	OpeatorAssociation_Left },	// RPN_TT_OP_Divide,			// /
				{ 5,	OpeatorAssociation_Left },	// RPN_TT_OP_IntegerDivide,		// %
				{ 6,	OpeatorAssociation_Left },	// RPN_TT_OP_BinaryPlus,		// + (Binary)
				{ 6,	OpeatorAssociation_Left },	// RPN_TT_OP_BinaryMinus,		// - (Binary)
				{ 7,	OpeatorAssociation_Left },	// RPN_TT_OP_LeftShift,			// <<
				{ 7,	OpeatorAssociation_Left },	// RPN_TT_OP_RightShift,		// >>
				{ 8,	OpeatorAssociation_Left },	// RPN_TT_OP_CompLessThan,		// <
				{ 8,	OpeatorAssociation_Left },	// RPN_TT_OP_CompLessThanEqual,	// <=
				{ 8,	OpeatorAssociation_Left },	// RPN_TT_OP_CompGreaterThen,	// >
				{ 8,	OpeatorAssociation_Left },	// RPN_TT_OP_CompGreaterThenEqual,// >=
				{ 9,	OpeatorAssociation_Left },	// RPN_TT_OP_CompEqual,			// ==
				{ 9,	OpeatorAssociation_Left },	// RPN_TT_OP_CompNotEqual,		// !=
				{ 3,	OpeatorAssociation_Right },	// RPN_TT_OP_BitwiseNot,		// ~
				{ 10,	OpeatorAssociation_Left },	// RPN_TT_OP_BitwiseAnd,		// &
				{ 11,	OpeatorAssociation_Left },	// RPN_TT_OP_BitwiseXor,		// ^
				{ 12,	OpeatorAssociation_Left },	// RPN_TT_OP_BitwiseOr,			// |
				{ 3,	OpeatorAssociation_Right },	// RPN_TT_OP_LogicalNot,		// !
				{ 13,	OpeatorAssociation_Left },	// RPN_TT_OP_LogicalAnd,		// &&
				{ 14,	OpeatorAssociation_Left },	// RPN_TT_OP_LogicalOr,			// ||
				{ 15,	OpeatorAssociation_Right },	// RPN_TT_OP_CondTrue,			// ? (条件演算子)
				{ 15,	OpeatorAssociation_Left },	// RPN_TT_OP_CondFalse,			// : (条件演算子)
				{ 17,	OpeatorAssociation_Left },	// RPN_TT_OP_Comma,				// , (カンマ演算子 or 実引数区切り文字)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_OP_FuncCall,			// (Dummy)
				
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLitaral_Null,// (Dummy)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLitaral_True,// (Dummy)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLitaral_False,// (Dummy)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLitaral_Int32,// (Dummy)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLitaral_UInt32,// (Dummy)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLitaral_Int64,// (Dummy)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLitaral_UInt64,// (Dummy)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLitaral_Float,// (Dummy)
				{ 0,	OpeatorAssociation_Left },	// RPN_TT_NumericLitaral_Double,// (Dummy)
			};
			assert(LN_ARRAY_SIZE_OF(TokenInfoTable) == RPN_TT_Max);

			RpnToken token;
			token.Type = CppTypeToRPNType[(*pos)->getTokenType() - TT_CppOP_SeparatorBegin];

			// ( かつひとつ前が識別子の場合は関数呼び出しとする
			if (token.Type == RPN_TT_OP_GroupStart &&
				!m_tokenList->isEmpty() &&
				m_tokenList->getLast().Type == RPN_TT_Identifier)
			{
				// Identifer の種類を FuncCall に変更し、( はトークンとして抽出しない
				m_tokenList->getLast().Type = RPN_TT_OP_FuncCall;
			}
			else
			{
				// + or - の場合は単項演算子であるかをここで確認する。
				// ひとつ前の有効トークンが演算子であれば単項演算子である。
				if (token.Type == RPN_TT_OP_BinaryPlus || token.Type == RPN_TT_OP_BinaryMinus)
				{
					if (!m_tokenList->isEmpty() && m_tokenList->getLast().IsOperator())
					{
						if (token.Type == RPN_TT_OP_BinaryPlus) {
							token.Type = RPN_TT_OP_UnaryPlus;
						}
						else {	// if (token.Type == RPN_TT_OP_BinaryMinus)
							token.Type = RPN_TT_OP_UnaryMinus;
						}
					}
				}

				token.Precedence = TokenInfoTable[token.Type].Precedence;
				token.Association = TokenInfoTable[token.Type].Association;
				token.SourceToken = (*pos);
				m_tokenList->add(token);
			}
			break;
		}
		case TokenGroup::Keyword:
		{
			RpnToken token;
			if ((*pos)->getTokenType() == TT_CppKW_true)
			{
				token.Type = RPN_TT_NumericLitaral_True;
				token.SourceToken = (*pos);
				m_tokenList->add(token);
			}
			else if ((*pos)->getTokenType() == TT_CppKW_false)
			{
				token.Type = RPN_TT_NumericLitaral_False;
				token.SourceToken = (*pos);
				m_tokenList->add(token);
			}
			break;
		}
		default:
			// TODO:error
			break;
		}
	}

	// 実引数リスト解析処理が , 演算子の解析を兼ねられるように、リスト終端にダミーの GroupEnd を入れておく
	RpnToken tailToken;
	tailToken.Type = RPN_TT_OP_GroupEnd;
	m_tokenList->add(tailToken);
}

//------------------------------------------------------------------------------
void RpnParser::parse()
{
	m_tmpRPNTokenList.reserve(m_tokenList->getCount());
	m_lastToken = nullptr;

	for (RpnToken& token : *m_tokenList)
	{
		// 現在の () 深さを振っておく
		token.GroupLevel = m_groupStack.getCount();

		// 定数は出力リストへ積んでいく
		if (token.GetTokenGroup() == RpnTokenGroup::Literal)
		{
			m_tmpRPNTokenList.add(&token);
		}
		else if (token.IsOperator())
		{
			switch (token.Type)
			{
			case RPN_TT_OP_FuncCall:
				m_groupStack.push(&token);
				m_opStack.push(&token);
				break;
			case RPN_TT_OP_GroupStart:
				m_groupStack.push(&token);
				m_opStack.push(&token);
				break;
			case RPN_TT_OP_GroupEnd:			// m_tokenList の最後はダミーの ) を入れているため、最後に1度必ず通る
				if (m_lastToken->Type != RPN_TT_OP_GroupStart) {
					m_groupStack.getTop()->ElementCount++;	// () 内の引数の数を増やす。ただし、"Func()" のように実引数が無ければ増やさない
				}
				PopOpStackGroupEnd(false);		// opStack の GroupStart または FuncCall までの内容を出力リストに移す。GroupStart または FuncCall は削除する。
				CloseGroup(false);				// 同レベル () 内の ':' 全てに CondGoto を振る。最後に現在のグループ情報を削除する
				break;
			case RPN_TT_OP_CondTrue:
				PushOpStack(&token);
				m_tmpRPNTokenList.add(&token);
				break;
			case RPN_TT_OP_CondFalse:
			{
				RpnToken* condTrue = PopOpStackCondFalse();
				if (!condTrue) { return; }		// Error : ':' に対応する ? が見つからなかった
				m_tmpRPNTokenList.add(&token);
				m_condStack.push(&token);
				condTrue->CondGoto = m_tmpRPNTokenList.getCount();	// ジャンプ先として ':' の次を指す
				break;
			}
			case RPN_TT_OP_Comma:
				// 現在のグループ内部の実引数の数をインクリメント。最初の , の場合は 1 余分に++する。
				//if (m_groupStack.GetTop()->ElementCount == 0) {	
				//	m_groupStack.GetTop()->ElementCount++;
				//}
				m_groupStack.getTop()->ElementCount++;	

				PopOpStackGroupEnd(true);				// ( ～ , までの opStack の内容を出力リストに移す。ただし、( は残したままにする
				CloseGroup(true);						// グループ内の条件演算子の処理を行う。ただし、その後グループ情報は削除しない
				break;
			// 通常の演算子
			default:
				PushOpStack(&token);
				break;
			}
		}

		m_lastToken = &token;
	}

	PopOpStackCondFalse();

	// 同レベル () 内の ':' 全てに CondGoto を振る (一番外側の GroupEnd を閉じるという考え)
	//CloseGroup();

	// 演算子用スタックに残っている要素を全て出力リストに移す
	while (!m_opStack.isEmpty())
	{
		RpnToken* top;
		m_opStack.pop(&top);
		if (top->Type == RPN_TT_OP_GroupStart) {
			//TODO: error括弧が閉じていない
			break;
		}
		else if (top->Type == RPN_TT_OP_CondTrue) {
			//TODO: error条件演算子が閉じていない
			break;
		}
		m_tmpRPNTokenList.add(top);
	}

	// 出力用のリストへ、トークンのコピーを作成しつつ移す
	m_rpnTokenList->reserve(m_tmpRPNTokenList.getCount());
	for (RpnToken* token : m_tmpRPNTokenList)
	{
		m_rpnTokenList->add(*token);
	}
}

//------------------------------------------------------------------------------
void RpnParser::PushOpStack(RpnToken* token)	// Operator または CondTrue だけ PushOpStack される
{
	// スタックにあるものを取り除く作業。
	// これから入れようとしているものより、top の優先度の方が高ければ取り除く。
	// 同じ場合は取り除かない。
	// スタックには優先度の低いものが残り続けることになる。
	while (!m_opStack.isEmpty())
	{
		RpnToken* top = m_opStack.getTop();
		if (top->Type == RPN_TT_OP_GroupStart || top->Type == RPN_TT_OP_FuncCall) {
			// '(' は特別扱い。とにかく演算子スタックの先頭に積む。(演算子の優先度でどうこうできない)
			// 別途、')' が見つかったとき、対応する '(' までのスタック要素を全てを出力リストへ移す。
			break;
		}

		if (top->Precedence < token->Precedence)	// [+ * ← +] と言う状態なら、*(5) +(6) なので * を取り除く
		{
			// 出力リストへ
			m_tmpRPNTokenList.add(top);
			m_opStack.pop();
		}
		else {
			break;
		}
	}
	m_opStack.push(token);
}

//------------------------------------------------------------------------------
// GroupEnd (')') または , が見つかったら呼ばれる
RpnToken* RpnParser::PopOpStackGroupEnd(bool fromArgsSeparator)
{
	// 対応する GroupStart ('(') が見つかるまでスタックの演算子を出力リストへ移していく。
	RpnToken* top = NULL;
	while (!m_opStack.isEmpty())
	{
		top = m_opStack.getTop();
		if (top->Type == RPN_TT_OP_GroupStart)
		{
			m_opStack.pop();	// GroupStart は捨てる
			break;
		}
		else if (top->Type == RPN_TT_OP_FuncCall)
		{
			// FuncCall は出力リストの末尾に積み、終了する。
			// ただし、, の場合は積まない。
			if (!fromArgsSeparator)
			{
				m_tmpRPNTokenList.add(top);	
				m_opStack.pop();
			}
			break;
		}

		// 出力リストへ
		m_tmpRPNTokenList.add(top);
		m_opStack.pop();
		top = nullptr;
	}

	if (!top) {
		// TODO: error 必ず GroupStart が無ければならない
		// ↑×。最後にも1度呼ばれるためエラーにしてはいけない
		return nullptr;
	}
	return top;
}

//------------------------------------------------------------------------------
// CondFalse (':') が見つかったら呼ばれる
RpnToken* RpnParser::PopOpStackCondFalse()
{
	// 対応する CondStart ('?') が見つかるまでスタックの演算子を出力リストへ移していく。
	RpnToken* top = nullptr;
	while (!m_opStack.isEmpty())
	{
		top = m_opStack.getTop();
		if (top->Type == RPN_TT_OP_CondTrue) {
			m_opStack.pop();	// CondTrue は捨てる
			break;
		}

		// 出力リストへ
		m_tmpRPNTokenList.add(top);
		m_opStack.pop();
	}
	/* 
		↑直前の ? を探しに行くだけで良い。
		( ) と同じ考え方。
		x    ?    x ? x : x    :    x ? x : x
	*/

	if (!top) {
		// TODO: error 必ず CondTrue が無ければならない
		return nullptr;
	}
	return top;
}

//------------------------------------------------------------------------------
void RpnParser::CloseGroup(bool fromArgsSeparator)
{
	// 現在の () レベルの ':' 全てに CondGoto を振り、スタックから取り除く
	while (!m_condStack.isEmpty())
	{
		RpnToken* condFalse = m_condStack.getTop();
		if (condFalse->GroupLevel < m_groupStack.getCount()) {
			break;
		}
		condFalse->CondGoto = m_tmpRPNTokenList.getCount();
		m_condStack.pop();
	}

	// グループ情報を1つ削除する。ただし、, の場合は残しておく。
	if (!fromArgsSeparator) {
		m_groupStack.pop();
	}
}


//==============================================================================
// RpnOperand
//==============================================================================

//------------------------------------------------------------------------------
bool RpnOperand::IsIntager() const
{
	return
		type == RpnOperandType::Int32 ||
		type == RpnOperandType::UInt32 ||
		type == RpnOperandType::Int64 ||
		type == RpnOperandType::UInt64;
}

//------------------------------------------------------------------------------
bool RpnOperand::IsFuzzyTrue() const
{
	switch (type)
	{
	case RpnOperandType::Boolean:	return valueBoolean;
	case RpnOperandType::Int32:		return valueInt32 != 0;
	case RpnOperandType::UInt32:	return valueUInt32 != 0;
	case RpnOperandType::Int64:		return valueInt64 != 0;
	case RpnOperandType::UInt64:	return valueUInt64 != 0;
	case RpnOperandType::Float:		return valueFloat != 0;
	case RpnOperandType::Double:	return valueDouble != 0;
	default:
		return false;
	}
}

//==============================================================================
// RpnEvaluator
//==============================================================================

//------------------------------------------------------------------------------
ResultState RpnEvaluator::TryEval(InputFile* inputFile, const RpnTokenList* tokenList, DiagnosticsItemSet* diag, RpnOperand* outValue)
{
	assert(tokenList != nullptr);
	assert(diag != nullptr);
	m_inputFile = inputFile;
	m_diag = diag;

	Stack<RpnOperand> operandStack;
	List<RpnOperand> funcCallArgs;
	RpnOperand operand, lhs, rhs;
	for (int iToken = 0; iToken < tokenList->getCount(); ++iToken)
	{
		bool skipPush = false;
		const RpnToken& token = tokenList->getAt(iToken);
		switch (token.GetTokenGroup())
		{
			case RpnTokenGroup::Literal:
				if (!MakeOperand(token, &operand)) return ResultState::Error;
				break;
			//case RpnTokenGroup::Constant:
			//case RpnTokenGroup::Identifier:
			case RpnTokenGroup::Operator:
				if (token.IsUnary() && operandStack.getCount() >= 1)
				{
					operandStack.pop(&rhs);
					lhs.type = RpnOperandType::Unknown;
				}
				else if (operandStack.getCount() >= 2)
				{
					operandStack.pop(&rhs);
					operandStack.pop(&lhs);
				}
				else
				{
					// Error: 引数が足りない
					m_diag->Report(DiagnosticsCode::RpnEvaluator_InvalidOperatorSide);
					return ResultState::Error;
				}
				if (!EvalOperator(token, lhs, rhs, &operand)) return ResultState::Error;
				break;

			case RpnTokenGroup::CondTrue:
			case RpnTokenGroup::CondFalse:
				/*  0 ? 1 : 2 の時はまず CondTrue が来た後、オペランドが 0 なので 2 へジャンプする。
                *  1 ? 1 : 2 の時はまず CondTrue が来た後、ジャンプせずに継続する。
                *  その後、CondFalse で 2 の後ろへジャンプする。
                *  
                * 1 == 2 ? 3 + 4 : 5 + 6 のパース結果は
                * → 1 2 == ? 3 4 + : 5 6 +
                *      ? の goto は 8 (5へ)
                *      : の goto は 11 (終端(配列外)へ)
                */
				if (operandStack.getCount() >= 0)
				{
					if (token.GetTokenGroup() == RpnTokenGroup::CondTrue)
					{
						operandStack.pop(&lhs);		// このオペランドは値を見るだけで捨ててしまう
						if (!lhs.IsFuzzyTrue())
						{
							// : の次へ行く
							iToken = token.CondGoto - 1;
						}
						else
						{
							// ? の次へ行く (特に何もしない)
						}
					}
					else // (token.GetTokenGroup() == RpnTokenGroup::CondFalse)
					{
						// : の次へ行く
						iToken = token.CondGoto - 1;
					}
				}
				else
				{
					// Error: 引数が足りない
					m_diag->Report(DiagnosticsCode::RpnEvaluator_InvalidOperatorSide);
					return ResultState::Error;
				}
				skipPush = true;	// 新しいオペランドの作成などは行われていないので push する必要は無い
				break;

			case RpnTokenGroup::FunctionCall:
				if (operandStack.getCount() >= token.ElementCount)
				{
					funcCallArgs.resize(token.ElementCount);
					for (int i = token.ElementCount - 1; i >= 0; --i)
					{
						operandStack.pop(&funcCallArgs[i]);
					}
					if (!CallFunction(token, funcCallArgs, &operand)) return ResultState::Error;
				}
				else
				{
					// Error: 引数が足りない
					m_diag->Report(DiagnosticsCode::RpnEvaluator_InvalidFuncCallArgsCount);
					return ResultState::Error;
				}
				break;

			case RpnTokenGroup::Assignment:
				return ResultState::Error;
			default:
				m_diag->Report(DiagnosticsCode::RpnEvaluator_UnexpectedToken);
				return ResultState::Error;
		}

		if (!skipPush) {
			operandStack.push(operand);
		}
	}

	// 全てのトークンを読み終えると、スタックに1つだけ結果が出ている
	if (operandStack.getCount() != 1)
	{
		m_diag->Report(DiagnosticsCode::RpnEvaluator_InsufficientToken);
		return ResultState::Error;
	}
	*outValue = operandStack.getTop();
	return ResultState::Success;
}

//------------------------------------------------------------------------------
bool RpnEvaluator::MakeOperand(const RpnToken& token, RpnOperand* outOperand)
{
	// リテラル
	if (token.GetTokenGroup() == RpnTokenGroup::Literal)
	{
		const flChar* str = token.SourceToken->GetCStr(m_inputFile);
		int len = token.SourceToken->getLength();
		const flChar* dummy;
		NumberConversionResult r;
		switch (token.Type)
		{
			case RPN_TT_NumericLitaral_Null:
				outOperand->type = RpnOperandType::Null;
				r = NumberConversionResult::Success;
				break;
			case RPN_TT_NumericLitaral_True:
				outOperand->type = RpnOperandType::Boolean;
				outOperand->valueBoolean = true;
				r = NumberConversionResult::Success;
				break;
			case RPN_TT_NumericLitaral_False:
				outOperand->type = RpnOperandType::Boolean;
				outOperand->valueBoolean = false;
				r = NumberConversionResult::Success;
				break;
			case RPN_TT_NumericLitaral_Int32:
				outOperand->type = RpnOperandType::Int32;
				outOperand->valueInt32 = StringTraits::toInt32(str, len, 0, &dummy, &r);
				break;
			case RPN_TT_NumericLitaral_UInt32:
				outOperand->type = RpnOperandType::UInt32;
				outOperand->valueUInt32 = StringTraits::toInt32(str, len, 0, &dummy, &r);
				break;
			case RPN_TT_NumericLitaral_Int64:
				outOperand->type = RpnOperandType::Int64;
				outOperand->valueInt64 = StringTraits::toInt64(str, len, 0, &dummy, &r);
				break;
			case RPN_TT_NumericLitaral_UInt64:
				outOperand->type = RpnOperandType::UInt64;
				outOperand->valueUInt64 = StringTraits::toUInt64(str, len, 0, &dummy, &r);
				break;
			case RPN_TT_NumericLitaral_Float:
				outOperand->type = RpnOperandType::Float;
				outOperand->valueFloat = (float)StringTraits::toDouble(str, len, &dummy, &r);
				break;
			case RPN_TT_NumericLitaral_Double:
				outOperand->type = RpnOperandType::Double;
				outOperand->valueDouble = StringTraits::toDouble(str, len, &dummy, &r);
				break;
		}
		if (r == NumberConversionResult::Success)
		{
			return true;
		}
	}

	// Error: 数値にできなかった
	m_diag->Report(DiagnosticsCode::RpnEvaluator_InvalidNumericLiteral);
	return false;
}

//------------------------------------------------------------------------------
bool RpnEvaluator::EvalOperator(const RpnToken& token, const RpnOperand& lhs_, const RpnOperand& rhs_, RpnOperand* outOperand)
{
	RpnOperand lhs, rhs;
	ExpandOperands(lhs_, rhs_, &lhs, &rhs);
	return EvalOperand(token.Type, lhs, rhs, outOperand);
}

//------------------------------------------------------------------------------
bool RpnEvaluator::CallFunction(const RpnToken& token, List<RpnOperand> args, RpnOperand* outOperand)
{
	// 対応するトークンが無ければ、これはカンマ演算子解析用のダミー。一番後ろの引数を返すだけ。
	if (token.SourceToken == nullptr)
	{
		*outOperand = args.getLast();
		return true;
	}
	else
	{
		LN_THROW(0, NotImplementedException);
		return false;
	}
}

//------------------------------------------------------------------------------
bool RpnEvaluator::EvalOperand(RpnTokenType tokenType, const RpnOperand& lhs, const RpnOperand& rhs, RpnOperand* outOperand)
{
	auto item = g_tokenTypeTable[tokenType].eval;
	outOperand->type = rhs.type;	// 単項の場合は lhs が Unknown になっているので rhs で見る
	switch (rhs.type)
	{
	case RpnOperandType::Boolean:	if (item.operaotrBoolean != nullptr) { item.operaotrBoolean(lhs.valueBoolean, rhs.valueBoolean, outOperand); return true; } break;
	case RpnOperandType::Int32:		if (item.operaotrInt32 != nullptr) { item.operaotrInt32(lhs.valueInt32, rhs.valueInt32, outOperand); return true; } break;
	case RpnOperandType::UInt32:	if (item.operaotrUInt32 != nullptr) { item.operaotrUInt32(lhs.valueUInt32, rhs.valueUInt32, outOperand); return true; } break;
	case RpnOperandType::Int64:		if (item.operaotrInt64 != nullptr) { item.operaotrInt64(lhs.valueInt64, rhs.valueInt64, outOperand); return true; } break;
	case RpnOperandType::UInt64:	if (item.operaotrUInt64 != nullptr) { item.operaotrUInt64(lhs.valueUInt64, rhs.valueUInt64, outOperand); return true; } break;
	case RpnOperandType::Float:		if (item.operaotrFloat != nullptr) { item.operaotrFloat(lhs.valueFloat, rhs.valueFloat, outOperand); return true; } break;
	case RpnOperandType::Double:	if (item.operaotrDouble != nullptr) { item.operaotrDouble(lhs.valueDouble, rhs.valueDouble, outOperand); return true; } break;
	default:
		assert(0);
		break;
	}
	// Error: 指定されたオペランドの型が不正。
	m_diag->Report(DiagnosticsCode::RpnEvaluator_OperatorInvalidType, rhs.type.toString().toStringA());
	return false;
}

//------------------------------------------------------------------------------
RpnOperandType RpnEvaluator::ExpandOperands(const RpnOperand& lhs, const RpnOperand& rhs, RpnOperand* outlhs, RpnOperand* outrhs)
{
	RpnOperandType type = std::max(lhs.type, rhs.type);
	*outlhs = lhs;
	*outrhs = rhs;
	CastOperand(outlhs, type);
	CastOperand(outrhs, type);
	return type;
}

//------------------------------------------------------------------------------
void RpnEvaluator::CastOperand(RpnOperand* op, RpnOperandType to)
{
	if (op->type == RpnOperandType::Unknown || to == RpnOperandType::Unknown || op->type == to) return;
	RpnOperand t = *op;
	switch (op->type)
	{
		case RpnOperandType::Int32:
		{
			switch (to)
			{
			case RpnOperandType::Boolean:	op->valueBoolean = (t.valueInt32 != 0); break;
			case RpnOperandType::Int32:		op->valueInt32 = (int32_t)	t.valueInt32; break;
			case RpnOperandType::UInt32:	op->valueUInt32 = (uint32_t)t.valueInt32; break;
			case RpnOperandType::Int64:		op->valueInt64 = (int64_t)	t.valueInt32; break;
			case RpnOperandType::UInt64:	op->valueUInt64 = (uint64_t)t.valueInt32; break;
			case RpnOperandType::Float:		op->valueFloat = (float)	t.valueInt32; break;
			case RpnOperandType::Double:	op->valueDouble = (double)	t.valueInt32; break;
			}
			break;
		}
		case RpnOperandType::UInt32:
		{
			switch (to)
			{
			case RpnOperandType::Boolean:	op->valueBoolean = (t.valueUInt32 != 0); break;
			case RpnOperandType::Int32:		op->valueInt32 = (int32_t)	t.valueUInt32; break;
			case RpnOperandType::UInt32:	op->valueUInt32 = (uint32_t)t.valueUInt32; break;
			case RpnOperandType::Int64:		op->valueInt64 = (int64_t)	t.valueUInt32; break;
			case RpnOperandType::UInt64:	op->valueUInt64 = (uint64_t)t.valueUInt32; break;
			case RpnOperandType::Float:		op->valueFloat = (float)	t.valueUInt32; break;
			case RpnOperandType::Double:	op->valueDouble = (double)	t.valueUInt32; break;
			}
			break;
		}
		case RpnOperandType::Int64:
		{
			switch (to)
			{
			case RpnOperandType::Boolean:	op->valueBoolean = (t.valueInt64 != 0); break;
			case RpnOperandType::Int32:		op->valueInt32 = (int32_t)	t.valueInt64; break;
			case RpnOperandType::UInt32:	op->valueUInt32 = (uint32_t)t.valueInt64; break;
			case RpnOperandType::Int64:		op->valueInt64 = (int64_t)	t.valueInt64; break;
			case RpnOperandType::UInt64:	op->valueUInt64 = (uint64_t)t.valueInt64; break;
			case RpnOperandType::Float:		op->valueFloat = (float)	t.valueInt64; break;
			case RpnOperandType::Double:	op->valueDouble = (double)	t.valueInt64; break;
			}
			break;
		}
		case RpnOperandType::UInt64:
		{
			switch (to)
			{
			case RpnOperandType::Boolean:	op->valueBoolean = (t.valueUInt64 != 0); break;
			case RpnOperandType::Int32:		op->valueInt32 = (int32_t)	t.valueUInt64; break;
			case RpnOperandType::UInt32:	op->valueUInt32 = (uint32_t)t.valueUInt64; break;
			case RpnOperandType::Int64:		op->valueInt64 = (int64_t)	t.valueUInt64; break;
			case RpnOperandType::UInt64:	op->valueUInt64 = (uint64_t)t.valueUInt64; break;
			case RpnOperandType::Float:		op->valueFloat = (float)	t.valueUInt64; break;
			case RpnOperandType::Double:	op->valueDouble = (double)	t.valueUInt64; break;
			}
			break;
		}
		case RpnOperandType::Float:
		{
			switch (to)
			{
			case RpnOperandType::Boolean:	op->valueBoolean = (t.valueFloat != 0); break;
			case RpnOperandType::Int32:		op->valueInt32 = (int32_t)	t.valueFloat; break;
			case RpnOperandType::UInt32:	op->valueUInt32 = (uint32_t)t.valueFloat; break;
			case RpnOperandType::Int64:		op->valueInt64 = (int64_t)	t.valueFloat; break;
			case RpnOperandType::UInt64:	op->valueUInt64 = (uint64_t)t.valueFloat; break;
			case RpnOperandType::Float:		op->valueFloat = (float)	t.valueFloat; break;
			case RpnOperandType::Double:	op->valueDouble = (double)	t.valueFloat; break;
			}
			break;
		}
		case RpnOperandType::Double:
		{
			switch (to)
			{
			case RpnOperandType::Boolean:	op->valueBoolean = (t.valueDouble != 0); break;
			case RpnOperandType::Int32:		op->valueInt32 = (int32_t)	t.valueDouble; break;
			case RpnOperandType::UInt32:	op->valueUInt32 = (uint32_t)t.valueDouble; break;
			case RpnOperandType::Int64:		op->valueInt64 = (int64_t)	t.valueDouble; break;
			case RpnOperandType::UInt64:	op->valueUInt64 = (uint64_t)t.valueDouble; break;
			case RpnOperandType::Float:		op->valueFloat = (float)	t.valueDouble; break;
			case RpnOperandType::Double:	op->valueDouble = (double)	t.valueDouble; break;
			}
			break;
		}
	}

	op->type = to;
}

} // namespace fl
