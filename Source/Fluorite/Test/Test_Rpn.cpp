#include "TestConfig.h"
#include "../Source/Fluorite/Rpn/RpnParser.h"
#include <algorithm>

class Test_Rpn : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}

	AnalyzerContext	m_context;
	RpnParser m_parser;

	const RpnTokenList* parse(const char* code)
	{
		auto file = m_context.RegisterInputMemoryCode("test", code);
		m_context.LexFile(file);
		auto tokens = file->GetTokenList();
		m_parser.ParseCppConstExpression2(tokens->begin(), tokens->end(), file->getDiag());
		LN_THROW(file->getDiag()->getItems()->isEmpty(), InvalidOperationException);
		return m_parser.GetTokenList();
	}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Rpn, parse)
{
	//int a = (1 == 2 ? 3 + 4 : 5 + 6);					// 11
	//int b = (0 ?     1 ? 1 : 2     :     1 ? 3 : 4);	// 3

	// <Test> 普通に
	{
		auto rpnTokens = parse("1 + 2");
		ASSERT_EQ(4, rpnTokens->getCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->getAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(3).Type);
	}
	// <Test> 優先順序
	{
		auto rpnTokens = parse("1 + 2 * 3");
		ASSERT_EQ(6, rpnTokens->getCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(1).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_Multiply, rpnTokens->getAt(3).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->getAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(5).Type);
	}
	// <Test> 優先順序
	{
		auto rpnTokens = parse("1 * 2 + 3");
		ASSERT_EQ(6, rpnTokens->getCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_Multiply, rpnTokens->getAt(2).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(3).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->getAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(5).Type);
	}
	// <Test> 括弧
	{
		auto rpnTokens = parse("1 * (2 + 3)");
		ASSERT_EQ(6, rpnTokens->getCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(1).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->getAt(3).Type);
		ASSERT_EQ(RPN_TT_OP_Multiply, rpnTokens->getAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(5).Type);
	}
	// <Test> 単項演算子
	{
		auto rpnTokens = parse("1 + -2");
		ASSERT_EQ(5, rpnTokens->getCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_UnaryMinus, rpnTokens->getAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->getAt(3).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(4).Type);
	}
	// <Test> 条件演算子
	{
		auto rpnTokens = parse("1 != 2 ? 3 + 4 : 5 + 6");
		ASSERT_EQ(12, rpnTokens->getCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_CompNotEqual, rpnTokens->getAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->getAt(3).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(4).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(5).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->getAt(6).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->getAt(7).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(8).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(9).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->getAt(10).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(11).Type);
	}
	// <Test> 条件演算子
	{
		auto rpnTokens = parse("1 ? (5 ? 6 : 7) : (3 ? 4 : (5 ? 6 : 7))");
		ASSERT_EQ(18, rpnTokens->getCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(0).Type); 
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->getAt(1).Type);	ASSERT_EQ(8, rpnTokens->getAt(1).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->getAt(3).Type);	ASSERT_EQ(6, rpnTokens->getAt(3).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->getAt(5).Type);	ASSERT_EQ(7, rpnTokens->getAt(5).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(6).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->getAt(7).Type);	ASSERT_EQ(18, rpnTokens->getAt(7).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(8).Type);
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->getAt(9).Type);	ASSERT_EQ(12, rpnTokens->getAt(9).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(10).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->getAt(11).Type);	ASSERT_EQ(17, rpnTokens->getAt(11).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(12).Type);
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->getAt(13).Type);	ASSERT_EQ(16, rpnTokens->getAt(13).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(14).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->getAt(15).Type);	ASSERT_EQ(17, rpnTokens->getAt(15).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(16).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(17).Type);
	}

	//{
	//	ByteBuffer buf(_T("1 + 2 ? 3 + 4 : 5 + 6"));
	//	ErrorManager err;
	//	TokenListPtr tokens(CppLexer<Char>::Lex(&buf, &err));
	//	RPNTokenListPtr rpnTokens(RPNParser<Char>::ParseCppConstExpression(tokens->begin(), tokens->end(), &err));
	//	ASSERT_EQ(11, rpnTokens->GetCount());

	//	// 1+1 ?   2 ? 1: 2  : 3
	// 1 ? 2 : (3 ? 4 : (5 ? 6 : 7))
	// 1 ? 6 ? 7 : 8 : 3 ? 4 : 5
	//}

	// <Test> 関数呼び出し
	{
		auto rpnTokens = parse("Func(1)");
		ASSERT_EQ(3, rpnTokens->getCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(0).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(2).Type);
	}
	// <Test> 関数呼び出し (複数の引数)
	{
		auto rpnTokens = parse("F1(1+1, F2(2*2) + 2)");
		ASSERT_EQ(11, rpnTokens->getCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->getAt(2).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(3).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_Multiply, rpnTokens->getAt(5).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(6).Type);	ASSERT_EQ(1, rpnTokens->getAt(6).ElementCount);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->getAt(7).Type);
		ASSERT_EQ(RPN_TT_OP_UnaryPlus, rpnTokens->getAt(8).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(9).Type);	ASSERT_EQ(2, rpnTokens->getAt(9).ElementCount);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->getAt(10).Type);	ASSERT_EQ(1, rpnTokens->getAt(10).ElementCount);
	}
}




class Test_Parser_RpnEvaluator : public ::testing::Test
{
protected:
	Test_Parser_RpnEvaluator()
		: m_diag("")
	{}
	virtual void SetUp() {}
	virtual void TearDown() {}


	AnalyzerContext	m_context;
	RpnParser m_parser;
	RpnEvaluator m_eval;
	RpnOperand m_value;
	DiagnosticsItemSet m_diag;

	void Eval(const char* code)
	{
		bool r = TryEval(code);
		if (!r) {
			LN_THROW(0, InvalidOperationException);
		}
	}

	bool TryEval(const char* code)
	{
		auto file = m_context.RegisterInputMemoryCode("test", code);
		m_context.LexFile(file);
		auto tokens = file->GetTokenList();
		m_parser.ParseCppConstExpression2(tokens->begin(), tokens->end(), &m_diag);
		if (!file->getDiag()->getItems()->isEmpty()) return false;
		ResultState r = m_eval.TryEval(file, m_parser.GetTokenList(), &m_diag, &m_value);
		if (r != ResultState::Success) return false;
		if (!file->getDiag()->getItems()->isEmpty()) return false;
		return true;
	}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_RpnEvaluator, Basic)
{
	// <Test> リテラル1つだけ
	{
		Eval("true");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
		Eval("false");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
		Eval("7");
		ASSERT_EQ(RpnOperandType::Int32, m_value.type);
		ASSERT_EQ(7, m_value.valueInt32);
		Eval("7u");
		ASSERT_EQ(RpnOperandType::UInt32, m_value.type);
		ASSERT_EQ(7, m_value.valueUInt32);
		Eval("7ll");
		ASSERT_EQ(RpnOperandType::Int64, m_value.type);
		ASSERT_EQ(7, m_value.valueInt64);
		Eval("7ull");
		ASSERT_EQ(RpnOperandType::UInt64, m_value.type);
		ASSERT_EQ(7, m_value.valueUInt64);
		Eval("7.f");
		ASSERT_EQ(RpnOperandType::Float, m_value.type);
		ASSERT_EQ(7, m_value.valueFloat);
		Eval("7.0");
		ASSERT_EQ(RpnOperandType::Double, m_value.type);
		ASSERT_EQ(7, m_value.valueDouble);
	}
	// <Test> + (Unary)
	{
		Eval("+2");
		ASSERT_EQ(2, m_value.valueInt32);
	}
	// <Test> - (Unary)
	{
		Eval("-2");
		ASSERT_EQ(-2, m_value.valueInt32);
	}
	// <Test> *
	{
		Eval("2*3");
		ASSERT_EQ(6, m_value.valueInt32);
	}
	// <Test> /
	{
		Eval("10/5");
		ASSERT_EQ(2, m_value.valueInt32);
	}
	// <Test> %
	{
		Eval("7%3");
		ASSERT_EQ(1, m_value.valueInt32);
	}
	// <Test> +
	{
		Eval("1+2");
		ASSERT_EQ(3, m_value.valueInt32);
	}
	// <Test> -
	{
		Eval("1-2");
		ASSERT_EQ(-1, m_value.valueInt32);
	}


	// <Test> <<
	{
		Eval("1<<2");
		ASSERT_EQ(4, m_value.valueInt32);
	}
	// <Test> >>
	{
		Eval("8>>2");
		ASSERT_EQ(2, m_value.valueInt32);
	}

	// <Test> <
	{
		Eval("1<2");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
		Eval("2<2");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
	}
	// <Test> <=
	{
		Eval("2<=2");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
		Eval("3<=2");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
	}
	// <Test> >
	{
		Eval("2>1");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
		Eval("2>2");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
	}
	// <Test> >=
	{
		Eval("2>=2");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
		Eval("2>=3");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
	}
	// <Test> ==
	{
		Eval("1==1");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
		Eval("1==2");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
	}
	// <Test> ==
	{
		Eval("1!=2");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
		Eval("1!=1");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
	}

	// <Test> ~
	{
		Eval("~0x00FF00FF");
		ASSERT_EQ(0xFF00FF00, m_value.valueInt32);
	}
	// <Test> &
	{
		Eval("0x0A&0x09");	// 1010 & 1001
		ASSERT_EQ(0x08, m_value.valueInt32);
	}
	// <Test> ^
	{
		Eval("0x0A^0x09");	// 1010 ^ 1001
		ASSERT_EQ(0x03, m_value.valueInt32);
	}
	// <Test> |
	{
		Eval("0x0A|0x09");	// 1010 ^ 1001
		ASSERT_EQ(0x0B, m_value.valueInt32);
	}

	// <Test> !
	{
		Eval("!true");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
	}
	// <Test> &&
	{
		Eval("true&&true");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
		Eval("false&&false");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
		Eval("true&&false");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
	}
	// <Test> ||
	{
		Eval("true||true");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
		Eval("false||false");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(false, m_value.valueBoolean);
		Eval("true||false");
		ASSERT_EQ(RpnOperandType::Boolean, m_value.type);
		ASSERT_EQ(true, m_value.valueBoolean);
	}
	// <Test> ? :
	{
		Eval("1?2:3");
		ASSERT_EQ(2, m_value.valueInt32);
		Eval("0?2:3");
		ASSERT_EQ(3, m_value.valueInt32);
		Eval("1?0?2:5:3");
		ASSERT_EQ(5, m_value.valueInt32);
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_RpnEvaluator, Error)
{
	// <Test> RpnEvaluator_OperatorInvalidType
	{
		ASSERT_EQ(false, TryEval("7%3.f"));
		ASSERT_EQ(1, m_diag.getItems()->getCount());
		ASSERT_EQ(DiagnosticsCode::RpnEvaluator_OperatorInvalidType, m_diag.getItems()->getAt(0).GetCode());
		ASSERT_EQ("Float", m_diag.getItems()->getAt(0).GetOptions().getAt(0));
	}
}
