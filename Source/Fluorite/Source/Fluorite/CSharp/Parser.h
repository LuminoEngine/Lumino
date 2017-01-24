
/*
	Parser の基本ルール
	
	Parse～() は、呼び出し側でその文法が確定したら呼び出す。
	例えば、ParseStmt() で、現在のトークンが "return" なら ParseReturnStmt() を呼び出す、など。


	Statement
		→ AssignmentExpr ;
		
	
	
	
	### 主要要素
	
	
	Decl
		関数外の定義すべて。以下のようなものがある。(C#)
		- namespace-declaration
		- type-declaration
			- class-declaration
			- struct-declaration
			- interface-declaration
			- enum-declaration
			- delegate-declaration
		- class-member-declaration:
			- constant-declaration
			- field-declaration
			- method-declaration
			- property-declaration
			- event-declaration
			- indexer-declaration
			- operator-declaration
			- constructor-declaration
			- destructor-declaration
			- static-constructor-declaration
			- type-declaration
			
	TranslationUnitDecl < Decl
		compilation-unit:
			namespace-member-declarations(opt)
	
	Stmt
		method-body の中身。
	
	Expr < Stmt
		式文 (expression-statement) の中身。

*/

#pragma once

namespace fl {

/** */
class SyntaxNode
{
public:
};

/**
	@note
	C++ ヘッダファイルもこれをルート要素とする。
	ステートメントが途中で切れるようなコードは対象外。
*/
class CompilationUnitSyntax : public SyntaxNode
{
public:
};

} // namespace fl




