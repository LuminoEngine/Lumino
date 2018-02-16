/*
	clang -Xclang -ast-dump Test.cpp
*/
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
	//Ref<DocumentSymbol> m_lastDocument;
	::TypeSymbol* m_currentRecord;

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

	Ref<DocumentSymbol> parseDocument(Decl* decl)
	{
		if (const FullComment *Comment = decl->getASTContext().getLocalCommentForDeclUncached(decl))
			return HeaderParser::parseDocument(getSourceText(Comment->getSourceRange()));
		return nullptr;
	}

	::AccessLevel tlanslateAccessLevel(AccessSpecifier ac)
	{
		switch (ac)
		{
		case clang::AS_public:
			return ::AccessLevel::Public;
		case clang::AS_protected:
			return ::AccessLevel::Protected;
		case clang::AS_private:
			return ::AccessLevel::Private;
		default:
			LN_UNREACHABLE();
			return ::AccessLevel::Private;
		}
	}

	//std::string getQualTypeNameSymple(QualType* type)
	//{
	//	type->getAsString()
	//	type->dump()
	//	PrintingPolicy PrintPolicy;

	//	SplitQualType T_split = type->split();
	//	std::string name = QualType::getAsString(T_split/*, PrintPolicy*/);

	//	if (Desugar && !T.isNull()) {
	//		// If the type is sugared, also dump a (shallow) desugared type.
	//		SplitQualType D_split = T.getSplitDesugaredType();
	//		if (T_split != D_split)
	//			OS << ":'" << QualType::getAsString(D_split, PrintPolicy) << "'";
	//	}
	//}

	String getRawTypeFullName(const QualType& type)
	{
		// type.getAsString() だと完全週修飾名になる。"struct ln::Vector3" など。
		auto name = String::fromStdString(type.getAsString());
		name = name.replace(_T("struct"), "");
		name = name.replace(_T("class"), "");
		name = name.replace(_T("const"), "");
		name = name.replace(_T("*"), "");
		name = name.replace(_T("&"), "");

		if (name == _T("_Bool"))
		{
			return _T("bool");
		}

		return name.trim();
		//SplitQualType st = type.split();
		//if (st.Ty->isRecordType())
		//{
		//	// type.getAsString() だと完全週修飾名になる。"struct ln::Vector3" など。
		//	// Decl から定義名をとると、"Vector3" などが取れる。
		//	CXXRecordDecl* rd = st.Ty->getAsCXXRecordDecl();
		//	DeclarationName name = rd->getDeclName();
		//	return String::fromStdString(name.getAsString());
		//}
		//else
		//{
		//	return String::fromStdString(type.getAsString());
		//}
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

			//errs() << "TopLevel : " << D->getDeclKindName();
			//if (NamedDecl *N = dyn_cast<NamedDecl>(D))  errs() << " " << N->getNameAsString();
			//errs() << "\n";

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

				auto info = Ref<::TypeSymbol>::makeRef();
				info->setRawFullName(getRawTypeFullName(QualType(decl->getTypeForDecl(), 0)));

				// documentation
				info->document = parseDocument(decl);

				// metadata
				info->metadata = HeaderParser::parseMetadata(attr->name, attr->args);

				// base classes
				auto bases = decl->getDefinition()->bases();
				for (auto& base : bases)
				{
					//auto cxx = base.getType()->getAsCXXRecordDecl();
					//info->baseClassRawName = String::fromStdString(cxx->getNameAsString());
					info->baseClassRawName = getRawTypeFullName(base.getType());
				}

				if (decl->getDefinition()->isStruct())
				{
					info->isStruct = true;
					m_parser->getDB()->structs.add(info);
				}
				else
				{
					m_parser->getDB()->classes.add(info);
				}



				m_currentRecord = info;
				EnumerateDecl(decl);
				m_currentRecord = nullptr;
			}
		}

		return true;
	}

	// メンバ関数
	bool VisitCXXMethodDecl(CXXMethodDecl* decl)
	{
		if (unsigned offset = getOffsetOnRootFile(m_sm, decl->getLocation()))
		{
			auto* attr = m_parser->findUnlinkedAttrMacro(offset);
			if (attr)
			{
				attr->linked = true;

				auto info = Ref<MethodSymbol>::makeRef();
				info->owner = m_currentRecord;
				info->name = String::fromStdString(decl->getNameAsString());
				info->accessLevel = tlanslateAccessLevel(decl->getAccess());

				// documentation
				info->document = parseDocument(decl);

				// metadata
				info->metadata = HeaderParser::parseMetadata(attr->name, attr->args);

				// return type
				info->returnTypeRawName = getRawTypeFullName(decl->getReturnType());

				// qualifiers
				info->isVirtual = decl->isVirtual();
				info->isStatic = decl->isStatic();
				info->isConst = decl->isConst();

				info->owner->declaredMethods.add(info);

				for (unsigned int iParam = 0; iParam < decl->getNumParams(); iParam++)
				{
					ParmVarDecl* paramDecl = decl->getParamDecl(iParam);
					QualType& type = paramDecl->getType();
					bool hasConst = type.getQualifiers().hasConst();
					SplitQualType sp = type.split();
					//PointerType
					
					auto paramInfo = Ref<ParameterSymbol>::makeRef();
					paramInfo->name = String::fromStdString(paramDecl->getNameAsString());
					paramInfo->typeRawName = getRawTypeFullName(type);
					paramInfo->isIn = hasConst;
					paramInfo->isOut = (!hasConst && sp.Ty->isPointerType());
					info->parameters.add(paramInfo);
				}
			}
		}

		return true;
	}

	// メンバ変数
	bool VisitFieldDecl(FieldDecl* decl)
	{
		if (unsigned offset = getOffsetOnRootFile(m_sm, decl->getLocation()))
		{
			if (auto* attr = m_parser->findUnlinkedAttrMacro(offset))
			{
				attr->linked = true;

				auto info = Ref<FieldSymbol>::makeRef();
				info->name = String::fromStdString(decl->getNameAsString());
				info->document = parseDocument(decl);
				info->typeRawName = getRawTypeFullName(decl->getType());
				m_currentRecord->declaredFields.add(info);
			}
		}

		return true;
	}

	bool VisitEnumDecl(EnumDecl* decl)
	{
		if (unsigned offset = getOffsetOnRootFile(m_sm, decl->getLocation()))
		{
			if (auto* attr = m_parser->findUnlinkedAttrMacro(offset))
			{
				attr->linked = true;

				auto ss = decl->getNameAsString();
				auto s2 = decl->getQualifiedNameAsString();

				decl->dump();

				auto symbol = Ref<TypeSymbol>::makeRef();
				symbol->setRawFullName(getRawTypeFullName(QualType(decl->getTypeForDecl(), 0)));

				// documentation
				symbol->document = parseDocument(decl);

				// metadata
				symbol->metadata = HeaderParser::parseMetadata(attr->name, attr->args);
			}
		}
		return true;
	}

	// typedef
	bool VisitTypedefDecl(TypedefDecl *aTypedefDecl)
	{
		return true;
	}

	// namespace
	bool VisitNamespaceDecl(NamespaceDecl* decl)
	{
		EnumerateDecl(decl);
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
				name == "LN_STRUCT" ||
				name == "LN_FIELD" ||
				name == "LN_METHOD" ||
				name == "LN_ENUM")
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


	Path tempFilePath(filePath.getString() + _T(".cpp"));
	FileSystem::copyFile(filePath, tempFilePath, true);



	// TODO: Path から直接 toLocalPath
	std::string localFilePath = tempFilePath.getString().toStdString();

	std::vector<std::string> args;
	args.push_back("");	// program name
	args.push_back(localFilePath.c_str());
	args.push_back("--");

	for (auto& path : m_includePathes)
	{
		args.push_back("-I");
		args.push_back(path.getString().toStdString());
	}

	
	args.push_back("-include");
	args.push_back("C:/Proj/LN/Lumino/Source/LuminoEngine/Source/LuminoEngine.PCH.h");

	
	args.push_back("-std=c++11");
	args.push_back("-fsyntax-only");
	args.push_back("-fms-compatibility");		// Enable full Microsoft Visual C++ compatibility
	args.push_back("-fms-extensions");			// Enable full Microsoft Visual C++ compatibility
	args.push_back("-fmsc-version=1900");		// Microsoft compiler version number to report in _MSC_VER (0 = don't define it (default))

	//const char* argv[] =
	//{
	//	"",	
	//	localFilePath.c_str(),
	//	"--",
	//	//"-I", "C:/Proj/LN/Lumino/Source/LuminoCore/Include",
	//	//"-I", "C:/Proj/LN/Lumino/Source/LuminoEngine/Include",
	//	"-D", "LN_NAMESPACE_BEGIN",
	//	"-D", "LN_NAMESPACE_END",
	//	"-fms-compatibility",
	//	"-fms-extensions",
	//	"-fmsc-version=1900",
	//};
	std::vector<const char*> argv;
	for (auto& arg : args)
	{
		argv.push_back(arg.c_str());
	}
	int argc = argv.size();

	::clang::tooling::CommonOptionsParser op(argc, argv.data(), ::llvm::cl::GeneralCategory);
	::clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());
	int result = Tool.run(local::NewLocalFrontendActionFactory(this).get());

	FileSystem::deleteFile(tempFilePath);
	return result;
}

HeaderParser::AttrMacro* HeaderParser::findUnlinkedAttrMacro(unsigned offset)
{
	for (size_t i = 0; i < lnAttrMacros.size(); i++)
	{
		if (!lnAttrMacros[i].linked)
		{
			unsigned o1 = lnAttrMacros[i].offset;
			unsigned o2 = (i < lnAttrMacros.size() - 1) ? lnAttrMacros[i + 1].offset : UINT32_MAX;
			if (o1 <= offset && offset < o2)
			{
				return &lnAttrMacros[i];
			}
		}
	}
	return nullptr;
}

Ref<DocumentSymbol> HeaderParser::parseDocument(const std::string& comment)
{
	auto info = Ref<DocumentSymbol>::makeRef();


	String doc = String::fromStdString(comment, Encoding::getUTF8Encoding());

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
					auto paramInfo = Ref<ParameterDocumentSymbol>::makeRef();
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

Ref<MetadataSymbol> HeaderParser::parseMetadata(std::string name, const std::string& args)
{
	auto metadata = Ref<MetadataSymbol>::makeRef();
	metadata->name = String::fromStdString(name);
	auto argEntries = String::fromStdString(args).split(_T(","), StringSplitOptions::RemoveEmptyEntries);
	for (auto& arg : argEntries)
	{
		String key, value;
		auto tokens = arg.split(_T("="), StringSplitOptions::RemoveEmptyEntries);
		key = tokens[0].trim();
		if (tokens.getCount() >= 2)
		{
			value = tokens[1].trim();
		}
		metadata->AddValue(String(key), String(value));
	}
	return metadata;
}
