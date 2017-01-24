#include "TestConfig.h"
#include <algorithm>

class Test_Parser_RPNParser : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}


	ByteBuffer m_buffer;
	CppLexer m_lex;
	DiagnosticsItemSet m_diag;
	RpnParser m_parser;

	const RpnTokenList* Parse(const char* code)
	{
		m_diag.ClearItems();
		m_buffer = ByteBuffer(code);
		auto tokens = m_lex.Tokenize(m_buffer, &m_diag);
		m_parser.ParseCppConstExpression2(tokens->cbegin(), tokens->cend(), &m_diag);
		LN_THROW(m_diag.GetItems()->IsEmpty(), InvalidOperationException);
		return m_parser.GetTokenList();
	}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_RPNParser, Parse)
{
	//int a = (1 == 2 ? 3 + 4 : 5 + 6);					// 11
	//int b = (0 ?     1 ? 1 : 2     :     1 ? 3 : 4);	// 3

	// <Test> •’Ê‚É
	{
		auto rpnTokens = Parse("1 + 2");
		ASSERT_EQ(4, rpnTokens->GetCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->GetAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(3).Type);
	}
	// <Test> —Dæ‡˜
	{
		auto rpnTokens = Parse("1 + 2 * 3");
		ASSERT_EQ(6, rpnTokens->GetCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(1).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_Multiply, rpnTokens->GetAt(3).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->GetAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(5).Type);
	}
	// <Test> —Dæ‡˜
	{
		auto rpnTokens = Parse("1 * 2 + 3");
		ASSERT_EQ(6, rpnTokens->GetCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_Multiply, rpnTokens->GetAt(2).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(3).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->GetAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(5).Type);
	}
	// <Test> Š‡ŒÊ
	{
		auto rpnTokens = Parse("1 * (2 + 3)");
		ASSERT_EQ(6, rpnTokens->GetCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(1).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->GetAt(3).Type);
		ASSERT_EQ(RPN_TT_OP_Multiply, rpnTokens->GetAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(5).Type);
	}
	// <Test> ’P€‰‰ŽZŽq
	{
		auto rpnTokens = Parse("1 + -2");
		ASSERT_EQ(5, rpnTokens->GetCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_UnaryMinus, rpnTokens->GetAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->GetAt(3).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(4).Type);
	}
	// <Test> ðŒ‰‰ŽZŽq
	{
		auto rpnTokens = Parse("1 != 2 ? 3 + 4 : 5 + 6");
		ASSERT_EQ(12, rpnTokens->GetCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_CompNotEqual, rpnTokens->GetAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->GetAt(3).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(4).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(5).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->GetAt(6).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->GetAt(7).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(8).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(9).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->GetAt(10).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(11).Type);
	}
	// <Test> ðŒ‰‰ŽZŽq
	{
		auto rpnTokens = Parse("1 ? (5 ? 6 : 7) : (3 ? 4 : (5 ? 6 : 7))");
		ASSERT_EQ(18, rpnTokens->GetCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(0).Type); 
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->GetAt(1).Type);	ASSERT_EQ(8, rpnTokens->GetAt(1).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(2).Type);
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->GetAt(3).Type);	ASSERT_EQ(6, rpnTokens->GetAt(3).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->GetAt(5).Type);	ASSERT_EQ(7, rpnTokens->GetAt(5).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(6).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->GetAt(7).Type);	ASSERT_EQ(18, rpnTokens->GetAt(7).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(8).Type);
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->GetAt(9).Type);	ASSERT_EQ(12, rpnTokens->GetAt(9).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(10).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->GetAt(11).Type);	ASSERT_EQ(17, rpnTokens->GetAt(11).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(12).Type);
		ASSERT_EQ(RPN_TT_OP_CondTrue, rpnTokens->GetAt(13).Type);	ASSERT_EQ(16, rpnTokens->GetAt(13).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(14).Type);
		ASSERT_EQ(RPN_TT_OP_CondFalse, rpnTokens->GetAt(15).Type);	ASSERT_EQ(17, rpnTokens->GetAt(15).CondGoto);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(16).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(17).Type);
	}

	//{
	//	ByteBuffer buf(_T("1 + 2 ? 3 + 4 : 5 + 6"));
	//	ErrorManager err;
	//	TokenListPtr tokens(CppLexer<TCHAR>::Lex(&buf, &err));
	//	RPNTokenListPtr rpnTokens(RPNParser<TCHAR>::ParseCppConstExpression(tokens->begin(), tokens->end(), &err));
	//	ASSERT_EQ(11, rpnTokens->GetCount());

	//	// 1+1 ?   2 ? 1: 2  : 3
	// 1 ? 2 : (3 ? 4 : (5 ? 6 : 7))
	// 1 ? 6 ? 7 : 8 : 3 ? 4 : 5
	//}

	// <Test> ŠÖ”ŒÄ‚Ño‚µ
	{
		auto rpnTokens = Parse("Func(1)");
		ASSERT_EQ(3, rpnTokens->GetCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(0).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(2).Type);
	}
	// <Test> ŠÖ”ŒÄ‚Ño‚µ (•¡”‚Ìˆø”)
	{
		auto rpnTokens = Parse("F1(1+1, F2(2*2) + 2)");
		ASSERT_EQ(11, rpnTokens->GetCount());
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(0).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(1).Type);
		ASSERT_EQ(RPN_TT_OP_BinaryPlus, rpnTokens->GetAt(2).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(3).Type);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(4).Type);
		ASSERT_EQ(RPN_TT_OP_Multiply, rpnTokens->GetAt(5).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(6).Type);	ASSERT_EQ(1, rpnTokens->GetAt(6).ElementCount);
		ASSERT_EQ(RPN_TT_NumericLitaral_Int32, rpnTokens->GetAt(7).Type);
		ASSERT_EQ(RPN_TT_OP_UnaryPlus, rpnTokens->GetAt(8).Type);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(9).Type);	ASSERT_EQ(2, rpnTokens->GetAt(9).ElementCount);
		ASSERT_EQ(RPN_TT_OP_FuncCall, rpnTokens->GetAt(10).Type);	ASSERT_EQ(1, rpnTokens->GetAt(10).ElementCount);
	}
}
