#pragma once

namespace local {
using namespace clang;
using namespace clang::tooling;
using namespace llvm;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace clang::comments;

class ClangVisitorRunner12;

#if 0
// このインタフェースは、プリプロセッサの動作を観察する方法を提供します。
// https://clang.llvm.org/doxygen/classclang_1_1PPCallbacks.html
// #XXXX を見つけたときや、マクロ展開が行われたときに呼ばれるコールバックを定義したりする。
// ★C++のクラスの属性構文は、class キーワードと名前の間に書く。Lumino のは class の前に書くスタイルなので、
//   clang と属性構文の機能を使うことができない。そのためマクロを自分で解析する。
class LocalPPCallbacks : public PPCallbacks {
public:
    LocalPPCallbacks(Preprocessor& pp, CompilerInstance* ci, ClangVisitorRunner12* runner)
        : m_pp(pp)
        , m_ci(ci) {
    }

    virtual void MacroExpands(const Token& MacroNameTok, const MacroDefinition& MD, SourceRange range, const MacroArgs* args) override {
        return PPCallbacks::MacroExpands(MacroNameTok, MD, range, args);
    }

private:
    Preprocessor& m_pp;
    CompilerInstance* m_ci;
};
#endif

//------------------------------------------------------------------------------
// 以下、決まり文句

// ASTConsumer は、AST のエントリポイントとなる何らかの要素を見つけたときにそれを通知する。
// 通常は HandleTranslationUnit() だけ実装すればよい。
// https://clang.llvm.org/docs/RAVFrontendAction.html
// clang は色々な AST の作成方法を持っているらしく、HandleTranslationUnit() 以外は
// 通常の翻訳単位以外の解析で何かしたいときに使うようだ。
class LocalASTConsumer : public ASTConsumer {
private:
    CompilerInstance* m_compilerInstance;
    ClangVisitorRunner12* m_runner;

public:
    explicit LocalASTConsumer(CompilerInstance* CI, ClangVisitorRunner12* runner);

    void HandleTranslationUnit(ASTContext& astContext) override;
};

// FrontendAction は、コンパイラフロントエンドの共通のエントリポイント。具体的に何したいの？を表すために使う。
// https://clang.llvm.org/docs/RAVFrontendAction.html
// https://clang.llvm.org/doxygen/classclang_1_1ASTFrontendAction.html
// 標準だと ASTFrontendAction の派生として、単に AST をダンプしたり、HTML に変換したりといったアクションが用意されている。
// 今回は AST を全部自分でトラバースしたいので ASTFrontendAction を使う。
class LocalFrontendAction : public ASTFrontendAction {
public:
    ClangVisitorRunner12* m_runner;

    LocalFrontendAction(ClangVisitorRunner12* runner)
        : m_runner(runner) {
    }

    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance& CI, llvm::StringRef file) {
        return std::make_unique<LocalASTConsumer>(&CI, m_runner);
    }
};

// SilClangAnalyzer のポインタを ↑のクラスたちにわたすためのファクトリ
std::unique_ptr<FrontendActionFactory> NewLocalFrontendActionFactory(ClangVisitorRunner12* runner) {
    class SimpleFrontendActionFactory : public FrontendActionFactory {
    public:
        ClangVisitorRunner12* m_runner;

        SimpleFrontendActionFactory(ClangVisitorRunner12* runner)
            : m_runner(runner) {}

        std::unique_ptr<clang::FrontendAction> create() override { return std::make_unique<LocalFrontendAction>(m_runner); }
    };

    return std::unique_ptr<FrontendActionFactory>(new SimpleFrontendActionFactory(runner));
}


class ClangVisitorRunner12 {
public:

    virtual std::shared_ptr<clang::DiagnosticConsumer> onCreateDiagnosticConsumer(clang::DiagnosticOptions* options) = 0;

    virtual std::unique_ptr<PPCallbacks> onCreatePPCallbacks(CompilerInstance* CI, Preprocessor& PP) = 0;

    virtual void onHandleTranslationUnit(CompilerInstance* CI, ASTContext* astContext) = 0;

    int run(const std::vector<const char*>& argv) {

        
    // clang 内部で発生した様々なエラーを受け取れるようにする。
        // clang::DiagnosticOptions は TextDiagnosticPrinter の中で delete されるので自分で delete してはならない。
        clang::DiagnosticOptions* diagnosticOptions = new clang::DiagnosticOptions();
        diagnosticOptions->ShowColors = 1;
        diagnosticOptions->ShowOptionNames = 1;

        // ここで指定する Category は -help 用なので特に気にしなくてOK。

        /*
		コマンドライン引数について [12.0.0]
		----------
		解析本体は llvm::cl::CommonOptionsParser。
		clang::tooling::CommonOptionsParser は、Tooling 用のユーティリティクラスで、与えられたコマンドライン引数を解析して以下の分割を行う。
		- Clang 本体に入力する引数
		- カスタムツールで使う用の引数

		CommonOptionsParser はコンストラクタでコマンドライン引数解析を実行し、--help が与えられていれば標準出力したのちプロセスを強制終了する。

		受け入れる書式は次の通り。

		```
		ツールEXE名 [options] <source0> [... <sourceN>] [-- [... <ClangArgs>]]
		```

		もっと簡単に示すと次のようになる。

		```
		ツールEXE名 <カスタムツールで使う引数> -- <Clang本体に渡す引数>
		```

		<カスタムツールで使う引数> は、CommonOptionsParser の引数 Category で指定できる。
		ここでは GeneralCategory を指定しており、これは「カスタムツールを作る時の共通オプション」を定義する。
		オプションの内容は --help を与えると一覧を見ることができる。

		<カスタムツールで使う引数> は、CommonOptionsParser を実行するとそれぞれが対応するグローバル変数に結果が格納される。
		例えば `--abort-on-max-devirt-iterations-reached` というコマンドライン引数を与えると、llvm::AbortOnMaxDevirtIterationsReached というグローバル変数でオプションの有無を知ることができる。

		<カスタムツールで使う引数> は、1つ以上のソースファイルの入力が必須となっているため、
		ここではエラー回避のためソースファイルのパスを指定している。
		なおこのファイルリストは CommonOptionsParser::getSourcePathList() で取得できるリストに格納される。

		<Clang本体に渡す引数> は CommonOptionsParser を通すと、CommonOptionsParser::getCompilations() で取得できるリストに格納される。
	*/
#if 1
        //std::unique_ptr<clang::TextDiagnosticPrinter> textDiagnosticPrinter =
        //	std::make_unique<clang::TextDiagnosticPrinter>(llvm::outs(),
        //	&diagnosticOptions);
        auto diag = onCreateDiagnosticConsumer(diagnosticOptions);
        if (!diag) {
            diag = std::make_shared<clang::TextDiagnosticPrinter>(llvm::outs(), diagnosticOptions);
        }

        llvm::IntrusiveRefCntPtr<clang::DiagnosticIDs> diagIDs;
        //std::unique_ptr<clang::DiagnosticsEngine> diagnosticsEngine =
        //    std::make_unique<clang::DiagnosticsEngine>(diagIDs, diagnosticOptions, diag.get() /*textDiagnosticPrinter.get()*/);
        auto diagnosticsEngine = new clang::DiagnosticsEngine(diagIDs, diagnosticOptions, diag.get());

        std::unique_ptr<clang::CompilerInstance> compilerInstance(new clang::CompilerInstance());
        auto& compilerInvocation = compilerInstance->getInvocation();

        // 引数エラーがある場合はここで出力される
        clang::CompilerInvocation::CreateFromArgs(compilerInvocation, argv, *diagnosticsEngine);


        compilerInstance->createDiagnostics(diag.get() /*textDiagnosticPrinter.get()*/, false);

        // Debug
        auto* languageOptions = compilerInvocation.getLangOpts();
        auto& preprocessorOptions = compilerInvocation.getPreprocessorOpts();
        auto& targetOptions = compilerInvocation.getTargetOpts();
        auto& frontEndOptions = compilerInvocation.getFrontendOpts();
        auto& codeGenOptions = compilerInvocation.getCodeGenOpts();

        auto actionFactory = local::NewLocalFrontendActionFactory(this);
        auto action = actionFactory->create();
        //std::unique_ptr<clang::CodeGenAction> action = std::make_unique<clang::EmitLLVMOnlyAction>(&context);

        int result = 0;
        if (!compilerInstance->ExecuteAction(*action)) {
            result = 1;
        }
#else
        // ClangTool を使用すると、cc1 ではなく GCC 互換モードで動く。
        // このとき -triplet も使えなくなる。
        // cc1 にする方法が見つからなかった。

        auto diag = std::make_shared<local::LocalDiagnosticConsumer>(diagnosticOptions);

        // args のパース処理
        ::clang::tooling::CommonOptionsParser op(argc, argv.data(), ::llvm::cl::GeneralCategory);

        const auto AllCompileCommands = op.getCompilations().getAllCompileCommands();

        // パースした情報 (CDB=CompilationDataBase) を使って、ClangTool を作る
        ::clang::tooling::ClangTool Tool(op.getCompilations(), op.getSourcePathList());
        Tool.setDiagnosticConsumer(diag.get());

        // ClangTool を実行する
        int result = Tool.run(local::NewLocalFrontendActionFactory(nullptr).get());
#endif

        return result;
    }

private:

};

LocalASTConsumer::LocalASTConsumer(CompilerInstance* CI, ClangVisitorRunner12* runner)
    : m_compilerInstance(CI)
    , m_runner(runner) {
    Preprocessor& PP = CI->getPreprocessor();
    auto PPCallbacks = runner->onCreatePPCallbacks(CI, PP);
    if (PPCallbacks) {
        PP.addPPCallbacks(std::move(PPCallbacks));
    }
}

void LocalASTConsumer::HandleTranslationUnit(ASTContext& astContext) {
    m_runner->onHandleTranslationUnit(m_compilerInstance, &astContext);
}
//
} // namespace local
