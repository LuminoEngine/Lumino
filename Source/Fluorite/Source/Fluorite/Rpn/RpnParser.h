
#pragma once
#include <Lumino/Base/Stack.h>
#include <Lumino/IO/PathName.h>
#include <Fluorite/Token.h>
#include <Fluorite/Diagnostics.h>

namespace fl {

enum RpnTokenType
{
	RPN_TT_Unknown = 0,

	RPN_TT_Identifier,				///< 識別子
	//RPN_TT_NumericLiteral,			///< 数値リテラル

	RPN_TT_OP_GroupStart,			// (
	RPN_TT_OP_GroupEnd,				// )

	RPN_TT_OP_UnaryPlus,			// + (Unary)
	RPN_TT_OP_UnaryMinus,			// - (Unary)
	RPN_TT_OP_Multiply,				// *
	RPN_TT_OP_Divide,				// /
	RPN_TT_OP_Modulus,				// %
	RPN_TT_OP_BinaryPlus,			// + (Binary)
	RPN_TT_OP_BinaryMinus,			// - (Binary)

	RPN_TT_OP_LeftShift,			// <<
	RPN_TT_OP_RightShift,			// >>

	RPN_TT_OP_CompLessThan,			// <
	RPN_TT_OP_CompLessThanEqual,	// <=
	RPN_TT_OP_CompGreaterThen,		// >
	RPN_TT_OP_CompGreaterThenEqual,	// >=
	RPN_TT_OP_CompEqual,			// ==
	RPN_TT_OP_CompNotEqual,			// !=

	RPN_TT_OP_BitwiseNot,			// ~ (Unary)
	RPN_TT_OP_BitwiseAnd,			// &
	RPN_TT_OP_BitwiseXor,			// ^
	RPN_TT_OP_BitwiseOr,			// |

	RPN_TT_OP_LogicalNot,			// ! (Unary)
	RPN_TT_OP_LogicalAnd,			// &&
	RPN_TT_OP_LogicalOr,			// ||

	RPN_TT_OP_CondTrue,				// ? (条件演算子)
	RPN_TT_OP_CondFalse,			// : (条件演算子)

	RPN_TT_OP_Comma,				// , (カンマ演算子 or 実引数区切り文字)

	RPN_TT_OP_FuncCall,				///< 関数呼び出し (識別子部分を指す)

	RPN_TT_NumericLitaral_Null,
	RPN_TT_NumericLitaral_True,
	RPN_TT_NumericLitaral_False,
	RPN_TT_NumericLitaral_Int32,	/**< C/C++ の char/wchar_t もこれになる */
	RPN_TT_NumericLitaral_UInt32,
	RPN_TT_NumericLitaral_Int64,
	RPN_TT_NumericLitaral_UInt64,
	RPN_TT_NumericLitaral_Float,	/**< 32bit */
	RPN_TT_NumericLitaral_Double,	/**< 64bit */

	RPN_TT_Max,	
};

enum class RpnTokenGroup
{
	Unknown,
	Literal,
	//Constant,
	Identifier,
	Operator,
	CondTrue,
	CondFalse,
	FunctionCall,
	Assignment,
};

/** 演算子のグループ (エラー処理などが共通) */
enum class RpnOperatorGroup
{
	Unknown,
	Arithmetic,
	Comparison,
	Logical,
	Bitwise,
	Conditional,
	Assignment,
};

/// 演算子の結合方向
enum OpeatorAssociation
{
	OpeatorAssociation_Left,
	OpeatorAssociation_Right,
};

/**
	@brief	逆ポーランド式を構成する要素です。
*/
class RpnToken
{
public:
	RpnToken()
	{
		SourceToken = NULL;
		CondGoto = 0;
		ElementCount = 0;
	}

public:
	bool IsOperator() const { return RPN_TT_OP_GroupStart <= Type && Type <= RPN_TT_OP_FuncCall; }

	RpnTokenGroup GetTokenGroup() const;
	RpnOperatorGroup GetOperatorGroup() const;
	bool IsUnary() const;

public:
	RpnTokenType		Type;
	int					Precedence;		///< 優先順位
	OpeatorAssociation	Association;	///< 結合方向
	const Token*		SourceToken;
	int					GroupLevel;		///< () ネストの深さ。ルートは 0

	int					CondGoto;		///< (Type が CondTrue または CondFalse のときに使用する)
	int					ElementCount;	///< , で区切られた要素数 (Type が GroupStart または FuncCall のときに使用する)


};

class RpnTokenList
	: public RefObject
	, public Collection<RpnToken>
{
};

/*
	メモリ効率のため、再利用される
*/
class RpnParser
{
public:
	typedef List<Token*>::const_iterator Position;

public:
	// TODO: RefPtr
	//static RefPtr<RPNTokenList> ParseCppConstExpression(Position exprBegin, Position exprEnd, DiagnosticsItemSet* diag);

	ResultState ParseCppConstExpression2(Position exprBegin, Position exprEnd, DiagnosticsItemSet* diag);
	const RpnTokenList* GetTokenList() const { return m_rpnTokenList; }

private:
	void initialize(DiagnosticsItemSet* diag);
	void TokenizeCppConst(Position exprBegin, Position exprEnd);
	void Parse();
	void PushOpStack(RpnToken* token);
	RpnToken* PopOpStackGroupEnd(bool fromArgsSeparator);
	RpnToken* PopOpStackCondFalse();
	void CloseGroup(bool fromArgsSeparator);

private:
	RefPtr<RpnTokenList>	m_tokenList;
	RefPtr<RpnTokenList>	m_rpnTokenList;
	DiagnosticsItemSet*		m_diag;

	List<RpnToken*>			m_tmpRPNTokenList;
	Stack<RpnToken*>		m_opStack;			// 演算子用の作業スタック
	Stack<RpnToken*>		m_condStack;		// 条件演算子用の作業スタック。: を格納していく
	Stack<RpnToken*>		m_groupStack;		// () の作業スタック。( または FuncCall を格納していく
	RpnToken*				m_lastToken;
};



/** RPN 評価に使用するオペランドの型 */
LN_ENUM(RpnOperandType)
{
	Unknown,
	Null,
	Boolean,
	Int32,		/* uint8 等はこれに拡張される。*/
	UInt32,
	Int64,
	UInt64,
	Float,
	Double,

	/* 後ろへ行くほど優先度が高い */
};
LN_ENUM_REFLECTION(RpnOperandType, Null, Boolean, Int32, UInt32, Int64, UInt64, Float, Double);
LN_ENUM_DECLARE(RpnOperandType);

/** RPN 評価に使用するオペランド */
class RpnOperand
{
public:
	RpnOperandType	type;

	union
	{
		bool		valueBoolean;
		int32_t		valueInt32;
		uint32_t	valueUInt32;
		int64_t		valueInt64;
		uint64_t	valueUInt64;
		float		valueFloat;
		double		valueDouble;
	};

	void Set(bool value) { valueBoolean = value; type = RpnOperandType::Boolean; }
	void Set(int32_t value) { valueInt32 = value; type = RpnOperandType::Int32; }
	void Set(uint32_t value) { valueUInt32 = value; type = RpnOperandType::UInt32; }
	void Set(int64_t value) { valueInt64 = value; type = RpnOperandType::Int64; }
	void Set(uint64_t value) { valueUInt64 = value; type = RpnOperandType::UInt64; }
	void Set(float value) { valueFloat = value; type = RpnOperandType::Float; }
	void Set(double value) { valueDouble = value; type = RpnOperandType::Double; }

	/** 整数型であるか (Boolean は含まない) */
	bool IsIntager() const;
	
	bool IsFuzzyTrue() const;
};

/**
	@brief	RPN トークンのリストを評価します。
*/
class RpnEvaluator
{
public:

	/**
		@brief	指定された RPN トークンのリストを評価し、値を作成します。
		@return	成功した場合 true。
	*/
	ResultState TryEval(InputFile* inputFile, const RpnTokenList* tokenList, DiagnosticsItemSet* diag, RpnOperand* outValue);

private:
	bool MakeOperand(const RpnToken& token, RpnOperand* outOperand);
	bool EvalOperator(const RpnToken& token, const RpnOperand& lhs, const RpnOperand& rhs, RpnOperand* outOperand);
	bool CallFunction(const RpnToken& token, List<RpnOperand> args, RpnOperand* outOperand);
	bool EvalOperand(RpnTokenType tokenType, const RpnOperand& lhs, const RpnOperand& rhs, RpnOperand* outOperand);

	static RpnOperandType ExpandOperands(const RpnOperand& lhs, const RpnOperand& rhs, RpnOperand* outlhs, RpnOperand* outrhs);
	static void CastOperand(RpnOperand* operand, RpnOperandType to);

private:
	InputFile*			m_inputFile;
	DiagnosticsItemSet*	m_diag;
};

} // namespace fl
