#include <memory>
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


class LWIGVisitor : public DeclVisitor<LWIGVisitor, bool>
{
private:
	const SourceManager& m_sourceManager;
	::SymbolDatabase* m_db;

public:
	LWIGVisitor(CompilerInstance* CI, ::SymbolDatabase* db)
		: m_sourceManager(CI->getASTContext().getSourceManager())
		, m_db(db)
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
	bool VisitCXXRecordDecl(CXXRecordDecl *aCXXRecordDecl/*, bool aForce = false*/)
	{
		// 参照用(class foo;のような宣言)なら追跡しない
		if (!aCXXRecordDecl->isCompleteDefinition()) {
			return true;
		}

		auto attrs = getAnnotation(aCXXRecordDecl);

		//// 名前無しなら表示しない(ただし、強制表示されたら表示する:Elaborated用)
		//if (!aCXXRecordDecl->getIdentifier() && !aForce) {
		//	return true;
		//}


		// クラス定義の処理
		//errs() << "CXXRecordDecl : " << aCXXRecordDecl->getNameAsString() << " {\n";
		{
			//INDENTATION;

			//// 型の種類
			//errs() << indentation << "Kind : ";
			//switch (aCXXRecordDecl->TagDecl::getTagKind()) {
			//case TTK_Struct:    errs() << "struct\n";       break;
			//case TTK_Interface: errs() << "__interface\n";  break;
			//case TTK_Union:     errs() << "union\n";        break;
			//case TTK_Class:     errs() << "class\n";        break;
			//case TTK_Enum:      errs() << "enum\n";         break;
			//default:            errs() << "unknown!!\n";    break;
			//}

			//// アノテーション(in アトリビュート)
			//printAnnotation(aCXXRecordDecl, "  Annotation : ", "\n");

			//// 各種特徴フラグ
			//if (aCXXRecordDecl->isUsed())           errs() << indentation << "  Used\n";
			//if (aCXXRecordDecl->isReferenced())     errs() << indentation << "  Referenced\n";
			//if (aCXXRecordDecl->isPolymorphic())    errs() << indentation << "  Polymorphic\n";
			//if (aCXXRecordDecl->isAbstract())       errs() << indentation << "  Abstract\n";
			//if (aCXXRecordDecl->isEmpty())          errs() << indentation << "  isEmpty\n";
			//if (isa<ClassTemplatePartialSpecializationDecl>(aCXXRecordDecl)) {
			//	errs() << indentation << "  This is a Template-Parcial-Specialization.\n";
			//}
			//else if (isa<ClassTemplateSpecializationDecl>(aCXXRecordDecl)) {
			//	errs() << indentation << "  This is a Template-Specialization.\n";
			//}

			//// 基底クラスの枚挙処理
			//for (CXXRecordDecl::base_class_iterator Base = aCXXRecordDecl->bases_begin(), BaseEnd = aCXXRecordDecl->bases_end();
			//	Base != BaseEnd;
			//	++Base) {                                          // ↓型名を取り出す(例えば、Policy.Bool=0の時、bool型は"_Bool"となる)
			//	errs() << indentation << "Base : ";
			//	printAccessSpec(Base->getAccessSpecifier());
			//	if (Base->isVirtual()) errs() << "virtual ";
			//	errs() << Base->getType().getAsString(Policy) << "\n";
			//}

			// メンバーの枚挙処理
			EnumerateDecl(aCXXRecordDecl);
		}
		//errs() << indentation << "}\n";
		//errs() << indentation << "====================================>>>\n";
		return true;
	}

	// メンバ関数
	bool VisitCXXMethodDecl(CXXMethodDecl* D)
	{
		auto attrs = getAnnotation(D);
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

//------------------------------------------------------------------------------
// 以下、決まり文句

// このインタフェースは、プリプロセッサの動作を観察する方法を提供します。
// https://clang.llvm.org/doxygen/classclang_1_1PPCallbacks.html
// #XXXX を見つけたときや、マクロ展開が行われたときに呼ばれるコールバックを定義したりする。
class LocalPPCallbacks : public PPCallbacks
{
public:
	LocalPPCallbacks(Preprocessor& pp, CompilerInstance* ci)
		: m_pp(pp)
		, m_ci(ci)
	{}

	virtual void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD, SourceRange Range, const MacroArgs *Args) override
	{
		auto stringRef = Lexer::getSourceText(
			CharSourceRange::getTokenRange(Range),
			m_ci->getSourceManager(),
			m_ci->getLangOpts());


		// Token の種類の名前を返す。 indentifer など。
		//auto* name = MacroNameTok.getName();
		//printf("%s %d\n", stringRef.str().c_str(), Range.getBegin().getRawEncoding());
		auto loc = Range.getBegin();
		std::cout << "  " << m_ci->getSourceManager().getFileID(loc).getHashValue() << " " << loc.getRawEncoding() << "\n";

		const MacroInfo* MI = MD.getMacroInfo();

		MI->dump();

		for (unsigned ArgNo = 0U; ArgNo < MI->getNumArgs(); ++ArgNo)
		{
			const IdentifierInfo *Arg = *(MI->arg_begin() + ArgNo);
			const Token *ResultArgToks = Args->getUnexpArgument(ArgNo);
		}
	}

private:
	Preprocessor & m_pp;
	CompilerInstance*	m_ci;
};

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
	explicit LocalASTConsumer(CompilerInstance* CI, ::SymbolDatabase* db)
		: m_visitor(std::make_unique<LWIGVisitor>(CI, db))
	{
		Preprocessor &PP = CI->getPreprocessor();
		PP.addPPCallbacks(llvm::make_unique<LocalPPCallbacks>(PP, CI));
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
	::SymbolDatabase * m_db;

	LocalFrontendAction(::SymbolDatabase* db)
		: m_db(db)
	{}

	virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, llvm::StringRef file)
	{
		return llvm::make_unique<LocalASTConsumer>(&CI, m_db);
	}
};


// SilClangAnalyzer のポインタを ↑のクラスたちにわたすためのファクトリ
std::unique_ptr<FrontendActionFactory> NewLocalFrontendActionFactory(::SymbolDatabase* db)
{
	class SimpleFrontendActionFactory : public FrontendActionFactory
	{
	public:
		::SymbolDatabase * m_db;

		SimpleFrontendActionFactory(::SymbolDatabase* db)
			: m_db(db)
		{}

		clang::FrontendAction *create() override { return new LocalFrontendAction(m_db); }
	};

	return std::unique_ptr<FrontendActionFactory>(new SimpleFrontendActionFactory(db));
}

} // namespace

//------------------------------------------------------------------------------

int HeaderParser::parse(const Path& filePath, ::SymbolDatabase* db)
{
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
	return Tool.run(local::NewLocalFrontendActionFactory(db).get());
}


