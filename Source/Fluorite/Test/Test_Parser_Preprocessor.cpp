#include "TestConfig.h"
#include "../src/Parser/Context.h"
#include "../src/Parser/EntityDatabase.h"
#include "../src/Parser/Frontend/Cpp/Preprocessor.h"

class Test_Parser_Preprocessor : public ::testing::Test
{
protected:
	Context m_context;
	ByteBuffer m_buffer;
	CppLexer m_lex;
	Preprocessor m_prepro;
	RefPtr<CompileUnitFile> m_fileCache;
	CompileOptions m_compileOptions;
	DiagnosticsItemSet m_diag;
	TokenListPtr m_tokens;

	Array<PathName> m_additionalIncludePaths;
	MacroMapContainer m_definedMacros;

	virtual void SetUp() 
	{
		m_additionalIncludePaths.Add(LN_LOCALFILE("TestData"));
	}
	virtual void TearDown() {}


	void Preprocess(const char* code)
	{
		bool r = TryPreprocess(code);
		if (!r) {
			LN_THROW(0, InvalidOperationException);
		}
	}

	bool TryPreprocess(const char* code)
	{
		m_context.Clear();
		m_diag.ClearItems();
		m_fileCache = RefPtr<CompileUnitFile>::MakeRef();
		m_fileCache->Initialize(LN_LOCALFILE("test.c"));
		m_buffer = ByteBuffer(code);
		m_tokens = m_lex.Tokenize(m_buffer, &m_diag);

		EntityDatabase db;
		ReferenceTracker rt;
		rt.Initialize(&db);
		bool r = m_prepro.BuildPreprocessedTokenList(&m_context, m_fileCache, m_tokens, m_fileCache, &m_additionalIncludePaths, m_definedMacros, &rt, &m_diag) == ResultState::Success;
		return r;
	}
};
//#error aaa
//#error "aaa"
/*
	defined()

	#defnie
	#error
	#endif
	#else
	#elif
	#include
	#ifndef
	#ifdef
	#line
	#pragma
	#undef
	#warning
	#
*/


//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Basic)
{
	{
		const char* code = "#define AAA";
		Preprocess(code);
	}
	{
		const char* code = "#define AAA 1";
		Preprocess(code);
	}
	{
		const char* code = "#define AAA 1 ";
		Preprocess(code);
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Basic_define)
{
#if 0
	// <Test> 関数形式マクロの定義 (引数無し)
	{
		const char* code = "#define AAA() 1";
		Preprocess(code);
		m_fileCache->outputMacroMap;
	}
	// <Test> 関数形式マクロの定義 (引数1つ)
	{
		const char* code = "#define AAA(x) x+1";
		Preprocess(code);
	}
	// <Test> 関数形式マクロの定義 (引数2つ以上)
	{
		const char* code = "#define AAA(x, y) x+y";
		Preprocess(code);
	}
	// <Test> 関数形式マクロの定義 (可変長引数)
	{
		const char* code = "#define AAA(...) __VA_ARGS__";
		Preprocess(code);
	}
	// <Test> 関数形式マクロの定義 (引数1つ + 可変長引数)
	{
		const char* code = "#define AAA(x, ...) x+__VA_ARGS__";
		Preprocess(code);
	}
#endif
}

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Basic_if)
{
	// <Test> #if 1
	{
		const char* code =
			"#if 1\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(5).IsValid());	// '1'
		ASSERT_EQ(true, m_tokens->GetAt(6).IsValid());	// '\n'
	}
	// <Test> #if 0
	{
		const char* code =
			"#if 0\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(5).IsValid());	// '1'
		ASSERT_EQ(false, m_tokens->GetAt(6).IsValid());	// '\n'
	}
	// <Test> #if true
	{
		const char* code =
			"#if true\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(5).IsValid());	// '1'
	}
	// <Test> #if false
	{
		const char* code =
			"#if false\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(5).IsValid());	// '1'
	}
	// <Test> #if 式 (結果 0 以外)
	{
		const char* code =
			"#if 1*2-1\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(9).IsValid());	// '1'
	}
	// <Test> #if 式 (結果 0)
	{
		const char* code =
			"#if 1-1\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(7).IsValid());	// '1'
	}


	// <Test> #if defined
	{
		const char* code =
			"#define AAA\n"
			"#if defined AAA\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(7).IsValid());	// '1'
	}
	// <Test> #if defined
	{
		const char* code =
			"#if defined AAA\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(7).IsValid());	// '1'
	}
	// <Test> #if defined()
	{
		const char* code =
			"#define AAA\n"
			"#if defined(AAA)\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(13).IsValid());
	}
	// <Test> #if defined()
	{
		const char* code =
			"#if !defined(AAA)\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(9).IsValid());
	}


	// <Test> #if マクロ
	{
		const char* code =
			"#define AAA 1\n"
			"#if AAA\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(12).IsValid());
	}
	// <Test> #if 未定義マクロは 0 になる
	{
		const char* code =
			"#if AAA\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(5).IsValid());
	}
	// <Test> #if マクロを含む式
	{
		const char* code =
			"#define AAA 1\n"
			"#if AAA-1\n"
			"1\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(14).IsValid());
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Basic_elif)
{
	// <Test> #if が有効なら #elif は常に無効
	{
		const char* code =
			"#if 1\n"
			"1\n"
			"#elif 1\n"
			"2\n"
			"#endif";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(12).IsValid());	// '2'
	}
	// <Test> #if が無効なら #elif は条件を確認する (有効)
	{
		const char* code =
			"#if 0\n"
			"1\n"
			"#elif 1\n"
			"2\n"
			"#endif";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(12).IsValid());	// '2'
	}
	// <Test> #if が無効なら #elif は条件を確認する (無効)
	{
		const char* code =
			"#if 0\n"
			"1\n"
			"#elif 0\n"
			"2\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(12).IsValid());	// '2'
	}

	// <Test> 複数の #elif (#if が有効となるパターン)
	{
		const char* code =
			"#if 1\n"
			"1\n"
			"#elif 0\n"
			"2\n"
			"#elif 0\n"
			"3\n"
			"#else\n"
			"4\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(5).IsValid());		// '1'
		ASSERT_EQ(false, m_tokens->GetAt(12).IsValid());	// '2'
		ASSERT_EQ(false, m_tokens->GetAt(19).IsValid());	// '3'
		ASSERT_EQ(false, m_tokens->GetAt(24).IsValid());	// '4'
	}

	// <Test> 複数の #elif (1つめの #elif が有効となるパターン)
	{
		const char* code =
			"#if 0\n"
			"1\n"
			"#elif 1\n"
			"2\n"
			"#elif 0\n"
			"3\n"
			"#else\n"
			"4\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(5).IsValid());		// '1'
		ASSERT_EQ(true, m_tokens->GetAt(12).IsValid());		// '2'
		ASSERT_EQ(false, m_tokens->GetAt(19).IsValid());	// '3'
		ASSERT_EQ(false, m_tokens->GetAt(24).IsValid());	// '4'
	}

	// <Test> 複数の #elif (2つめの #elif が有効となるパターン)
	{
		const char* code =
			"#if 0\n"
			"1\n"
			"#elif 0\n"
			"2\n"
			"#elif 1\n"
			"3\n"
			"#else\n"
			"4\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(5).IsValid());		// '1'
		ASSERT_EQ(false, m_tokens->GetAt(12).IsValid());	// '2'
		ASSERT_EQ(true, m_tokens->GetAt(19).IsValid());		// '3'
		ASSERT_EQ(false, m_tokens->GetAt(24).IsValid());	// '4'
	}

	// <Test> 複数の #elif (#else が有効となるパターン)
	{
		const char* code =
			"#if 0\n"
			"1\n"
			"#elif 0\n"
			"2\n"
			"#elif 0\n"
			"3\n"
			"#else\n"
			"4\n"
			"#endif\n";
		Preprocess(code);
		ASSERT_EQ(false, m_tokens->GetAt(5).IsValid());		// '1'
		ASSERT_EQ(false, m_tokens->GetAt(12).IsValid());	// '2'
		ASSERT_EQ(false, m_tokens->GetAt(19).IsValid());	// '3'
		ASSERT_EQ(true, m_tokens->GetAt(24).IsValid());		// '4'
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Basic_ifdef)
{
	// <Test> 無効領域
	{
		const char* code =
			"#define AAA\n"
			"#ifdef AAAB\n"
			"1\n"
			"#endif\n";
		Preprocess(code);

		// 数値 1 は無効領域
		ASSERT_EQ(CommonTokenType::ArithmeticLiteral, m_tokens->GetAt(10).GetCommonType());
		ASSERT_EQ(false, m_tokens->GetAt(10).IsValid());
	}
	// <Test> 有効領域
	{
		const char* code =
			"#define AAA\n"
			"#ifdef AAA\n"
			"1\n"
			"#endif\n";
		Preprocess(code);

		// 数値 1 は有効領域
		ASSERT_EQ(CommonTokenType::ArithmeticLiteral, m_tokens->GetAt(10).GetCommonType());
		ASSERT_EQ(true, m_tokens->GetAt(10).IsValid());
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Basic_ifndef)
{
	// <Test> 無効領域
	{
		const char* code =
			"#define AAA\n"
			"#ifndef AAAB\n"
			"1\n"
			"#endif\n";
		Preprocess(code);

		// 数値 1 は有効領域
		ASSERT_EQ(CommonTokenType::ArithmeticLiteral, m_tokens->GetAt(10).GetCommonType());
		ASSERT_EQ(true, m_tokens->GetAt(10).IsValid());
	}
	// <Test> 有効領域
	{
		const char* code =
			"#define AAA\n"
			"#ifndef AAA\n"
			"1\n"
			"#endif\n";
		Preprocess(code);

		// 数値 1 は無効領域
		ASSERT_EQ(CommonTokenType::ArithmeticLiteral, m_tokens->GetAt(10).GetCommonType());
		ASSERT_EQ(false, m_tokens->GetAt(10).IsValid());
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Basic_empty)
{
	// <Test> 空のディレクティブ (#だけ)
	{
		const char* code =
			"#\n"	// 末尾がNewLine
			"#";	// 末尾がEOF
		Preprocess(code);
		// 無視されるので、エラーや警告が無ければOK
	}
}

#define AAAAAA
#undef AAAAAA

#if AAAAAA

printf;

#endif


#define DDD(x, y) x + y
#if DDD(1, -1)
printf;
#endif

#define FFF(x, y) x+y
#define EEE FFF(1, 1)
#undef FFF
#define FFF(x, y) x-y	// こっち優先
#if EEE
printf;
#endif

#define HHH 1
#define GGG defined(HHH)
#if GGG
printf;
#endif

//#define CCC 1+1
//#define CCC 1+1 
//#define CCC 1/**/+1
//#define CCC 1 + 1


#if 0
#pm_include
#if 0
#else
#endif
#endif


//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Basic_else)
{
	// <Test> 有効 → 無効
	{
		const char* code =
			"#define AAA\n"
			"#ifdef AAA\n"
			"1\n"
			"#else\n"
			"2\n"
			"#endif\n";
		Preprocess(code);

		// 1 は有効領域
		ASSERT_EQ(CommonTokenType::ArithmeticLiteral, m_tokens->GetAt(10).GetCommonType());
		ASSERT_EQ(true, m_tokens->GetAt(10).IsValid());
		// 2 は無効領域
		ASSERT_EQ(CommonTokenType::ArithmeticLiteral, m_tokens->GetAt(15).GetCommonType());
		ASSERT_EQ(false, m_tokens->GetAt(15).IsValid());
	}
	// <Test> 無効 → 有効
	{
		const char* code =
			"#ifdef AAA\n"
			"1\n"
			"#else\n"
			"2\n"
			"#endif";
		Preprocess(code);

		// 1 は無効領域
		ASSERT_EQ(CommonTokenType::ArithmeticLiteral, m_tokens->GetAt(5).GetCommonType());
		ASSERT_EQ(false, m_tokens->GetAt(5).IsValid());
		// 2 は有効領域
		ASSERT_EQ(CommonTokenType::ArithmeticLiteral, m_tokens->GetAt(10).GetCommonType());
		ASSERT_EQ(true, m_tokens->GetAt(10).IsValid());

	}
}

#define AAA 1-

#if AAA 1
#error AA
#endif


//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Unit_include)
{
	// <Test> include ファイル内のマクロ定義が取り出せる。
	//{
	//	const char* code =
	//		"#include \"test.h\"\n"
	//		"#ifdef TEST\n"
	//		"1\n"
	//		"#endif";
	//	Preprocess(code);
	//	ASSERT_EQ(true, m_tokens->GetAt(10).IsValid());
	//}
	// <Test> 同じ include ファイルが複数回 include され、それぞれ定義マクロが異なる場合
	{
		const char* code =
			"#include \"IncludeTest1.h\"\n"
			"#ifdef CCC\n"
			"1\n"
			"#endif\n"
			"#ifdef BBB\n"
			"1\n"
			"#endif\n"
			"#define AAA\n"
			"#include \"IncludeTest1.h\"\n"
			"#ifdef BBB\n"
			"1\n"
			"#endif";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(10).IsValid());
		ASSERT_EQ(false, m_tokens->GetAt(20).IsValid());
		ASSERT_EQ(true, m_tokens->GetAt(40).IsValid());		// AAA が定義されたので .h 内で BBB が定義される
	}
}


//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Unit_pragma_once)
{
	// <Test> #pragma once
	{
		const char* code =
			"#include \"pragma_once.h\"\n"
			"#ifdef BBB\n"
			"1\n"
			"#endif\n"
			"#define AAA"
			"#include \"pragma_once.h\"\n"
			"#ifdef BBB\n"
			"1\n"
			"#endif";
		Preprocess(code);
		ASSERT_EQ(true, m_tokens->GetAt(10).IsValid());	// 1 は有効領域
		ASSERT_EQ(true, m_tokens->GetAt(29).IsValid());	// 1 は有効領域
		
	}
}

//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Illigal)
{
	// どう並べ替えても一番したが偉い
#define BBB2	AAA2
#define AAA2	100
	printf("%d\n", BBB2);
#undef AAA2
#define AAA2	200
	printf("%d\n", BBB2);

	// <Illigal> #if の定数式が無い
	{
		const char* code = "#if";
		ASSERT_EQ(false, TryPreprocess(code));
		ASSERT_EQ(DiagnosticsCode::Preprocessor_InvalidConstantExpression, m_diag.GetItems()->GetAt(0).GetCode());
	}
	// <Illigal> #if の定数式が整数ではない
	{
		const char* code = "#if 1.5 + 2";
		ASSERT_EQ(false, TryPreprocess(code));
		ASSERT_EQ(DiagnosticsCode::Preprocessor_InvalidConstantExpression, m_diag.GetItems()->GetAt(0).GetCode());
	}
	// <Illigal> #if のまえに #else が来た
	{
		const char* code = "#else\n";
		ASSERT_EQ(false, TryPreprocess(code));
		ASSERT_EQ(DiagnosticsCode::Preprocessor_UnexpectedElse, m_diag.GetItems()->GetAt(0).GetCode());
	}
	// <Illigal> defined の後ろが識別子か ( 以外
	{
		const char* code = "#if defined + AAA\n";
		ASSERT_EQ(false, TryPreprocess(code));
		ASSERT_EQ(DiagnosticsCode::Preprocessor_ExpectedDefinedId, m_diag.GetItems()->GetAt(0).GetCode());
	}
	// <Illigal> #else が連続で来た
	{
		const char* code =
			"#ifdef AAA\n"
			"#else\n"
			"#else\n";
		ASSERT_EQ(false, TryPreprocess(code));
		ASSERT_EQ(DiagnosticsCode::Preprocessor_UnexpectedElse, m_diag.GetItems()->GetAt(0).GetCode());
	}
	// <Illigal> #if の前に #endif が来た
	{
		const char* code =
			"#endif";
		ASSERT_EQ(false, TryPreprocess(code));
		ASSERT_EQ(DiagnosticsCode::Preprocessor_UnexpectedEndif, m_diag.GetItems()->GetAt(0).GetCode());
	}
	// <Illigal> #if の前に #elif が来た
	{
		const char* code = "#elif\n";
		ASSERT_EQ(false, TryPreprocess(code));
		ASSERT_EQ(DiagnosticsCode::Preprocessor_UnexpectedElif, m_diag.GetItems()->GetAt(0).GetCode());
	}
	// <Illigal> #if 系に対応する #endif が見つからなかった。
	{
		const char* code = "#if 1\n";
		ASSERT_EQ(false, TryPreprocess(code));
		ASSERT_EQ(DiagnosticsCode::Preprocessor_NoExistsEndif, m_diag.GetItems()->GetAt(0).GetCode());
	}
}



//-----------------------------------------------------------------------------
TEST_F(Test_Parser_Preprocessor, Example)
{

}
