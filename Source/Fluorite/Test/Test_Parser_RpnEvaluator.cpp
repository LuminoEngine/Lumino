#include "TestConfig.h"
#include <algorithm>

class Test_Parser_RpnEvaluator : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}



	ByteBuffer m_buffer;
	CppLexer m_lex;
	DiagnosticsItemSet m_diag;
	RpnEvaluator m_eval;
	RpnParser m_parser;
	RpnOperand m_value;
	void Eval(const char* exp)
	{
		bool r = TryEval(exp);
		if (!r || !m_diag.GetItems()->IsEmpty()) {
			LN_THROW(0, InvalidOperationException);
		}
	}
	bool TryEval(const char* exp)
	{
		m_diag.ClearItems();
		m_buffer = ByteBuffer(exp);
		auto tokens = m_lex.Tokenize(m_buffer, &m_diag);
		m_parser.ParseCppConstExpression2(tokens->cbegin(), tokens->cend(), &m_diag);
		return m_eval.TryEval(m_parser.GetTokenList(), &m_diag, &m_value) == ResultState::Success;
	}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_RpnEvaluator, Basic)
{
	// <Test> ƒŠƒeƒ‰ƒ‹1‚Â‚¾‚¯
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
		ASSERT_EQ(1, m_diag.GetItems()->GetCount());
		ASSERT_EQ(DiagnosticsCode::RpnEvaluator_OperatorInvalidType, m_diag.GetItems()->GetAt(0).GetCode());
		ASSERT_EQ("Float", m_diag.GetItems()->GetAt(0).GetOptions().GetAt(0));
	}
}
