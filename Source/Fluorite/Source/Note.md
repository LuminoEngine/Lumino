

InputFile
	SourceTokens	: 元のトークンリスト

SourceManager : CompileUnit と 1:1
	AdditionalTokens	: プリプロセスでマクロ展開されたトークン
	Tokens			: SourceTokens と AdditionalTokens を参照する SourceLocation(+Valid) の配列




clang
--------------------
Lex
↓
Parser		… AST 作る。
↓
Sema		… AST を検査する。未定義関数のエラーはここ


Roslyn
--------------------
Lex

SyntaxFactory


SymbolKind と MethodKind は別。
SymbolKind.Method、MethodKind.GetProperty など。
http://source.roslyn.io/#Microsoft.CodeAnalysis/Symbols/SymbolKind.cs,30fd9c0834bef6ff

