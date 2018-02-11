#include <memory>
#include "SymbolDatabase.h"
#include "Parser.h"


#ifdef _MSC_VER     // start of disabling MSVC warnings
#pragma warning(push)
#pragma warning(disable:4146 4180 4244 4258 4267 4291 4345 4351 4355 4456 4457 4458 4459 4503 4624 4722 4800 4996)
#endif
#include <iostream>
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/DeclVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/MacroArgs.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"

#include "clang/AST/TypeLoc.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "llvm/Support/raw_ostream.h"
#include "clang/AST/Comment.h"
#include "clang/AST/CommentVisitor.h"

#ifdef _MSC_VER     // end of disabling MSVC warnings
#pragma warning(pop)
#endif


class SymbolDatabase;

namespace local
{
using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace clang::comments;

#if 0
class LWIGCommentVisitor : public ConstCommentVisitor<LWIGCommentVisitor>
{
public:
	const CompilerInstance& m_ci;

	LWIGCommentVisitor(const CompilerInstance& ci)
		: m_ci(ci)
	{}

	void dumpFullComment(const FullComment *C)
	{
		ConstCommentVisitor<LWIGCommentVisitor>::visitFullComment(C);
		for (Comment::child_iterator I = C->child_begin(), E = C->child_end(); I != E; ++I)
		{
			const Comment *Child = *I;
			visit(Child);
		}


		//if (!C)
		//	return;

		//FC = C;
		//dumpComment(C);
		//FC = nullptr;
	}

	// Comments.
	const char *getCommandName(unsigned CommandID)
	{
		const CommandTraits *Traits = nullptr;


		if (Traits)
			return Traits->getCommandInfo(CommandID)->Name;
		const CommandInfo *Info = CommandTraits::getBuiltinCommandInfo(CommandID);
		if (Info)
			return Info->Name;
		return "<not a builtin command>";
	}

	//void dumpComment(const Comment *C)
	//{
	//	dumpChild([=] {
	//		if (!C) {
	//			ColorScope Color(*this, NullColor);
	//			OS << "<<<NULL>>>";
	//			return;
	//		}

	//		{
	//			ColorScope Color(*this, CommentColor);
	//			OS << C->getCommentKindName();
	//		}
	//		dumpPointer(C);
	//		dumpSourceRange(C->getSourceRange());
	//		ConstCommentVisitor<ASTDumper>::visit(C);
	//		for (Comment::child_iterator I = C->child_begin(), E = C->child_end();
	//			I != E; ++I)
	//			dumpComment(*I);
	//	});
	//}

	// Inline comments.
	void visitTextComment(const TextComment *C)
	{
		LN_LOG_INFO << "visitTextComment";
	}
	void visitInlineCommandComment(const InlineCommandComment *C)
	{
		LN_LOG_INFO << "visitInlineCommandComment";
	}
	void visitHTMLStartTagComment(const HTMLStartTagComment *C)
	{
		LN_LOG_INFO << "visitHTMLStartTagComment";
	}
	void visitHTMLEndTagComment(const HTMLEndTagComment *C)
	{
		LN_LOG_INFO << "visitHTMLEndTagComment";
	}

	// Block comments.
	void visitBlockCommandComment(const BlockCommandComment *C)
	{
		auto aa = getCommandName(C->getCommandID());
		LN_LOG_INFO << "visitBlockCommandComment";
	}
	void visitParamCommandComment(const ParamCommandComment *C)
	{
		auto s = C->getArgText(0);
		auto ss = s.str();
		LN_LOG_INFO << "visitParamCommandComment";

	}
	void visitTParamCommandComment(const TParamCommandComment *C)
	{
		LN_LOG_INFO << "visitTParamCommandComment";
	}
	void visitVerbatimBlockComment(const VerbatimBlockComment *C)
	{
		LN_LOG_INFO << "visitVerbatimBlockComment";
	}
	void visitVerbatimBlockLineComment(const VerbatimBlockLineComment *C)
	{
		LN_LOG_INFO << "visitVerbatimBlockLineComment";
	}
	void visitVerbatimLineComment(const VerbatimLineComment *C)
	{
		LN_LOG_INFO << "visitVerbatimLineComment";
	}
	void visitParagraphComment(const ParagraphComment* comment)
	{
		auto cct = comment->getCommentKindName();
		comment->dump();
		//auto aa = getCommandName(comment->getCommandID());

		for (Comment::child_iterator I = comment->child_begin(), E = comment->child_end(); I != E; ++I)
		{
			cct = (*I)->getCommentKindName();

			auto stringRef = Lexer::getSourceText(
				CharSourceRange::getTokenRange((*I)->getLocation()),
				m_ci.getSourceManager(),
				m_ci.getLangOpts());

			printf("");
		}

		LN_LOG_INFO << "visitParagraphComment";
	}
};
#endif

class LWIGVisitor : public DeclVisitor<LWIGVisitor, bool>
{
private:
	CompilerInstance * m_ci;
	const SourceManager& m_sm;
	::HeaderParser* m_parser;
	//Ref<DocumentInfo> m_lastDocument;

public:
	LWIGVisitor(CompilerInstance* CI, ::HeaderParser* parser)
		: m_ci(CI)
		, m_sm(CI->getASTContext().getSourceManager())
		, m_parser(parser)
	{
	}

	std::vector<std::string> getAnnotation(Decl* decl)
	{
		std::vector<std::string> attrs;
		if (decl->hasAttrs())
		{
			for (auto& attr : decl->getAttrs())
			{
				auto* anno = dyn_cast<AnnotateAttr>(attr);
				if (anno != nullptr)
				{
					attrs.push_back(anno->getAnnotation().str());
				}
			}
		}
		return attrs;
	}

	static unsigned getOffsetOnRootFile(const SourceManager& sm, SourceLocation loc)
	{
		auto ploc = sm.getPresumedLoc(loc);
		if (ploc.getIncludeLoc().isInvalid())
		{
			auto eloc = sm.getDecomposedExpansionLoc(loc);
			return eloc.second;
		}
		return 0;
	}


	std::string getSourceText(SourceRange range) const
	{
		auto stringRef = Lexer::getSourceText(
			CharSourceRange::getTokenRange(range),
			m_ci->getSourceManager(),
			m_ci->getLangOpts());
		return stringRef.str();
	}

	void EnumerateDecl(DeclContext* aDeclContext)
	{
		for (DeclContext::decl_iterator i = aDeclContext->decls_begin(), e = aDeclContext->decls_end(); i != e; i++)
		{
			Decl *D = *i;
			//if (indentation.IndentLevel == 0) {
			//	errs() << "TopLevel : " << D->getDeclKindName();                                    // Declの型表示
			//	if (NamedDecl *N = dyn_cast<NamedDecl>(D))  errs() << " " << N->getNameAsString();  // NamedDeclなら名前表示
			//	errs() << " (" << D->getLocation().printToString(SM) << ")\n";                      // ソース上の場所表示
			//}

			errs() << "TopLevel : " << D->getDeclKindName();
			if (NamedDecl *N = dyn_cast<NamedDecl>(D))  errs() << " " << N->getNameAsString();
			errs() << "\n";

			Visit(D);
		}
	}


	// class/struct/unionの処理
	bool VisitCXXRecordDecl(CXXRecordDecl* decl)
	{
		if (!decl->isCompleteDefinition())
		{
			// 宣言
			return true;
		}


		if (unsigned offset = getOffsetOnRootFile(m_sm, decl->getLocation()))
		{
			auto* attr = m_parser->findUnlinkedAttrMacro(offset);
			if (attr)
			{

				attr->linked = true;


				auto info = Ref<::TypeInfo>::makeRef();
				info->name = String::fromStdString(decl->getNameAsString());

				// documentation
				Ref<DocumentInfo> doc;
				if (const FullComment *Comment = decl->getASTContext().getLocalCommentForDeclUncached(decl))
				{
					info->document = HeaderParser::parseDocument(getSourceText(Comment->getSourceRange()));
				}

				// metadata
				auto metadata = Ref<MetadataInfo>::makeRef();
				auto args = String::fromStdString(attr->args).split(_T(","), StringSplitOptions::RemoveEmptyEntries);
				for (auto& arg : args)
				{
					String key, value;
					auto tokens = String::fromStdString(attr->args).split(_T("="), StringSplitOptions::RemoveEmptyEntries);
					key = tokens[0].trim();
					if (tokens.getCount() >= 2)
					{
						value = tokens[1].trim();
					}
					metadata->AddValue(String(key), String(value));
				}
				info->metadata = metadata;

				// base classes
				auto bases = decl->getDefinition()->bases();
				for (auto& base : bases)
				{
					auto cxx = base.getType()->getAsCXXRecordDecl();
					info->baseClassRawName = String::fromStdString(cxx->getNameAsString());
				}


				//info->metadata = MoveLastMetadata();
				//if (!classHeader.baseClassNames.isEmpty()) info->baseClassRawName = String(classHeader.baseClassNames[0].name);
				m_parser->getDB()->classes.add(info);
			}
		}





		//auto attrs = getAnnotation(aCXXRecordDecl);

		//auto op = m_ci->getLangOpts();
		//auto stringRef = Lexer::getSourceText(
		//	CharSourceRange::getTokenRange(aCXXRecordDecl->getLocation()),
		//	m_ci->getSourceManager(),
		//	m_ci->getLangOpts());
		//auto ss = stringRef.str();

		//// 名前無しなら表示しない(ただし、強制表示されたら表示する:Elaborated用)
		//if (!aCXXRecordDecl->getIdentifier() && !aForce) {
		//	return true;
		//}

		// クラス定義の処理
		//errs() << "CXXRecordDecl : " << aCXXRecordDecl->getNameAsString() << " {\n";
		{

			EnumerateDecl(decl);
		}

		
		//errs() << indentation << "}\n";
		//errs() << indentation << "====================================>>>\n";
		return true;
	}

	// メンバ関数
	bool VisitCXXMethodDecl(CXXMethodDecl* decl)
	{
		if (unsigned offset = getOffsetOnRootFile(m_sm, decl->getLocation()))
		{
			auto* attr = m_parser->findUnlinkedAttrMacro(offset);
			attr->linked = true;


			if (const FullComment *Comment = decl->getASTContext().getLocalCommentForDeclUncached(decl))
			{
				auto stringRef = Lexer::getSourceText(
					CharSourceRange::getTokenRange(Comment->getSourceRange()),
					m_ci->getSourceManager(),
					m_ci->getLangOpts());
				auto ss = stringRef.str();

				//LWIGCommentVisitor cv(*m_ci);
				//cv.dumpFullComment(Comment);
				//std::cout << "Comment:" << Comment->getCommentKindName() << "\n";

			}
		}

		return true;
	}

	// メンバ変数
	bool VisitFieldDecl(FieldDecl *aFieldDecl)
	{
		return true;
	}

	// typedef
	bool VisitTypedefDecl(TypedefDecl *aTypedefDecl)
	{
		return true;
	}

	// namespace
	bool VisitNamespaceDecl(NamespaceDecl *aNamespaceDecl)
	{
		return true;
	}

	// using
	bool VisitUsingDirectiveDecl(UsingDirectiveDecl *aUsingDirectiveDecl)
	{
		return true;
	}
};

// このインタフェースは、プリプロセッサの動作を観察する方法を提供します。
// https://clang.llvm.org/doxygen/classclang_1_1PPCallbacks.html
// #XXXX を見つけたときや、マクロ展開が行われたときに呼ばれるコールバックを定義したりする。
// ★C++のクラスの属性構文は、class キーワードと名前の間に書く。Lumino のは class の前に書くスタイルなので、
//   clang と属性構文の機能を使うことができない。そのためマクロを自分で解析する。
class LocalPPCallbacks : public PPCallbacks
{
public:
	LocalPPCallbacks(Preprocessor& pp, CompilerInstance* ci, ::HeaderParser* parser)
		: m_pp(pp)
		, m_ci(ci)
		, m_parser(parser)
	{
	}

	virtual void MacroExpands(const Token& MacroNameTok, const MacroDefinition& MD, SourceRange range, const MacroArgs* args) override
	{
		const SourceManager& sm = m_ci->getSourceManager();
		const LangOptions& opts = m_ci->getLangOpts();
		const MacroInfo* macroInfo = MD.getMacroInfo();

		// マクロが書かれている場所は input のルートであるか？ (include ファイルは解析したくない)
		auto ploc = sm.getPresumedLoc(range.getBegin());
		if (ploc.getIncludeLoc().isInvalid())
		{
			std::string name = Lexer::getSourceText(CharSourceRange::getTokenRange(range.getBegin()), sm, opts).str();
			if (name == "LN_CLASS" ||
				name == "LN_METHOD")
			{
				::HeaderParser::AttrMacro attrMacro;
				attrMacro.name = name;

				std::string args = Lexer::getSourceText(CharSourceRange::getTokenRange(range), sm, opts).str();
				args = args.substr(args.find('(') + 1);
				args = args.substr(0, args.find(')'));
				attrMacro.args = args;

				//
				//for (int iArg = 0; iArg < args->getNumArguments(); iArg++)
				//{
				//	auto a = args->getPreExpArgument(iArg, macroInfo, m_pp);
				//	//const Token* tok = args->getPreExpArgument(iArg);
				//	//if (tok->is(tok::eof))
				//	//{
				//	//	break;
				//	//}

				//	attrMacro.args.push_back(Lexer::getSourceText(CharSourceRange::getTokenRange(tok->getLocation()), sm, opts));
				//}

				//int a2 = args->getArgLength();

				//for (int iArg = 0; i < )

				//std::string tokens = 

				//for (int iArg = 0U; iArg < macroInfo->getNumArgs(); iArg++)
				//{
				//	const IdentifierInfo* param = *(macroInfo->arg_begin() + iArg);	// params がほしいときはこっち
				//	const Token* argToks = args->getUnexpArgument(iArg);				// args がほしいときはこっち

				//	if (argToks->is(tok::eof))
				//	{
				//		// マクロを使っている側の実引数の数が少ない。
				//		// あるいは #define AAA(...) のような定義で、実引数が省略されている。
				//	}

				//	attrMacro.args.push_back(Lexer::getSourceText(CharSourceRange::getTokenRange(argToks->getLocation()), sm, opts));
				//}

				auto eloc = sm.getDecomposedExpansionLoc(range.getBegin());
				attrMacro.offset = eloc.second;
				m_parser->lnAttrMacros.push_back(attrMacro);
			}
		}
	}

private:
	Preprocessor & m_pp;
	CompilerInstance* m_ci;
	::HeaderParser* m_parser;
};

//------------------------------------------------------------------------------
// 以下、決まり文句

// ASTConsumer は、AST のエントリポイントとなる何らかの要素を見つけたときにそれを通知する。
// 通常は HandleTranslationUnit() だけ実装すればよい。
// https://clang.llvm.org/docs/RAVFrontendAction.html
// clang は色々な AST の作成方法を持っているらしく、HandleTranslationUnit() 以外は
// 通常の翻訳単位以外の解析で何かしたいときに使うようだ。
class LocalASTConsumer : public ASTConsumer
{
private:
	std::unique_ptr<LWIGVisitor> m_visitor;

public:
	explicit LocalASTConsumer(CompilerInstance* CI, ::HeaderParser* parser)
		: m_visitor(std::make_unique<LWIGVisitor>(CI, parser))
	{
		Preprocessor &PP = CI->getPreprocessor();
		PP.addPPCallbacks(llvm::make_unique<LocalPPCallbacks>(PP, CI, parser));
	}

	virtual void HandleTranslationUnit(ASTContext& Context)
	{
		m_visitor->EnumerateDecl(Context.getTranslationUnitDecl());
	}
};

// FrontendAction は、コンパイラフロントエンドの共通のエントリポイント。具体的に何したいの？を表すために使う。
// https://clang.llvm.org/docs/RAVFrontendAction.html
// https://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html
// 標準だと ASTFrontendAction の派生として、単に AST をダンプしたり、HTML に変換したりといったアクションが用意されている。
// 今回は AST を全部自分でトラバースしたいので ASTFrontendAction を使う。
class LocalFrontendAction : public ASTFrontendAction
{
public:
	::HeaderParser* m_parser;

	LocalFrontendAction(::HeaderParser* parser)
		: m_parser(parser)
	{}

	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef file)
	{
		return llvm::make_unique<LocalASTConsumer>(&CI, m_parser);
	}
};


// SilClangAnalyzer のポインタを ↑のクラスたちにわたすためのファクトリ
std::unique_ptr<FrontendActionFactory> NewLocalFrontendActionFactory(::HeaderParser* parser)
{
	class SimpleFrontendActionFactory : public FrontendActionFactory
	{
	public:
		::HeaderParser* m_parser;

		SimpleFrontendActionFactory(::HeaderParser* parser)
			: m_parser(parser)
		{}

		clang::FrontendAction *create() override { return new LocalFrontendAction(m_parser); }
	};

	return std::unique_ptr<FrontendActionFactory>(new SimpleFrontendActionFactory(parser));
}

} // namespace

//------------------------------------------------------------------------------

int HeaderParser::parse(const Path& filePath, ::SymbolDatabase* db)
{
	LN_CHECK(db);
	m_db = db;

	// TODO: Path から直接 toLocalPath
	std::string localFilePath = filePath.getString().toStdString();

	const char* argv[] =
	{
		"",	// program name
		localFilePath.c_str(),
		"--",
		//"-I", "C:/Proj/LN/Lumino/Source/LuminoCore/Include",
		//"-I", "C:/Proj/LN/Lumino/Source/LuminoEngine/Include",
		"-D", "LN_NAMESPACE_BEGIN",
		"-D", "LN_NAMESPACE_END",
		"-fms-compatibility",		// Enable full Microsoft Visual C++ compatibility
		"-fms-extensions",			// Enable full Microsoft Visual C++ compatibility
		"-fmsc-version=1900",		// Microsoft compiler version number to report in _MSC_VER (0 = don't define it (default))
	};
	int argc = sizeof(argv) / sizeof(const char*);

	::clang::tooling::CommonOptionsParser op(argc, argv, ::llvm::cl::GeneralCategory);
	::clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());
	return Tool.run(local::NewLocalFrontendActionFactory(this).get());
}

HeaderParser::AttrMacro* HeaderParser::findUnlinkedAttrMacro(unsigned offset)
{
	for (size_t i = 0; lnAttrMacros.size(); i++)
	{
		if (!lnAttrMacros[i].linked && lnAttrMacros[i].offset > offset)
		{
			if (i == 0)
				return nullptr;
			else
				return &lnAttrMacros[i - 1];
		}
	}
	return nullptr;
}

Ref<DocumentInfo> HeaderParser::parseDocument(const std::string& comment)
{
	auto info = Ref<DocumentInfo>::makeRef();


	String doc = String::fromStdString(comment);

	// 改行コード統一し、コメント開始終了を削除する
	doc = doc
		.replace(_T("\r\n"), _T("\n"))
		.replace(_T("\r"), _T("\n"))
		.replace(_T("/**"), _T(""))
		.replace(_T("*/"), _T(""));


	List<String> lines = doc.split(_T("\n"));
	String* target = &info->summary;
	for (String line : lines)
	{
		line = line.trim();

		MatchResult result;
		if (Regex::search(line, _T("@(\\w+)"), &result))
		{
			if (result[1] == _T("brief"))
			{
				target = &info->summary;
				line = line.substring(result.getLength());
			}
			else if (result[1] == _T("param"))
			{
				String con = line.substring(result.getLength());
				if (Regex::search(con, _T(R"(\[(\w+)\]\s+(\w+)\s*\:\s*)"), &result))
				{
					auto paramInfo = Ref<ParameterDocumentInfo>::makeRef();
					info->params.add(paramInfo);
					paramInfo->io = result[1];
					paramInfo->name = result[2];
					target = &paramInfo->description;
					line = con.substring(result.getLength());
				}
			}
			else if (result[1] == _T("return"))
			{
				target = &info->returns;
				line = line.substring(result.getLength());
			}
			else if (result[1] == _T("details"))
			{
				target = &info->details;
				line = line.substring(result.getLength());
			}
			else if (result[1] == _T("copydoc"))
			{
				String con = line.substring(result.getLength());
				if (Regex::search(con, _T(R"((\w+)(.*))"), &result))
				{
					info->copydocMethodName = result[1];
					info->copydocSignature = result[2];
					info->copydocSignature = info->copydocSignature.remove('(').remove(')').remove(' ').remove('\t');
					target = &info->details;
					line.clear();
				}
			}
		}

		(*target) += line.trim();
	}

	return info;
}
