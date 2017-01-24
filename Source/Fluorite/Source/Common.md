
解析
--------------------
### 字句解析

### 構文解析 (SyntaxParser)
AST を作る。
この時点ではまだ Symbol は作らない。

Roslyn なら CSharp.Syntax.MethodDeclarationSyntax とかが作られる。

### 意味解析 (SemanticAnalyzer)
ここまできてようやく Symbol を作っていく。



ユースケース
--------------------
### ファイルを登録して解析する
```
AnalyzerContext* ac = AnalyzerContext::Create();
ac->RegisterSourceFile("test.cpp");
ac->AnalyzeAll();
```

### ある行を含んでいる関数定義を探す (Symbol から)
```
SymbolDatabase* db = ac->GetSymbolDatabase();
Enumerable<Symbol*> sources = db->GetSourceFileSymbols();
Symbol* source = sources.First([](Symbol* s){ s->GetName() == "C:\\test.cpp"; });
Enumerable<Reference*> refs = source.GetReferences();
Enumerable<Reference*> funcDecls = refs.Where([](Reference* r){ r->GetKind() == ReferenceDecl::FunctionDeclaration; });
Reference* funcDeclRef  = refs.First([](Reference* r){ r->GetSourceRange().ContainsLineNumber(100); });
Symbol* func = funcDeclRef->GetSymbol();
```



Token
--------------------

### ポインタコピーか、実態コピーか
トークンの配列は Array<Token*> とするか、Array<Token> とするか、という問題。

以前は Token は実態、コードの String は参照だった。
ただ、これだとオリジナルのソースコードバッファはずっと生きていなければならず、管理的にめんどくさい。

これは clang あたりを参考にしてみたものだが、構造解析エンジンとしては Token 1つにかなりたくさんの情報をもたせる必要がある。
そのうちコピーコストも大きくなりそう。
ちなみに und は Token 参照である。CppCheck も。

Fluorite としては以下のようにしてみる。
- Token は参照
- コードの String は実態 (Lex 後、オリジナルのバッファは捨てても良いようにする)

Reference
--------------------
ASTNode(Syntax) とは別物。それらだけだと CallBy を表現できない。



Kind
--------------------
複数の言語を同時に解析するとき、似ている種別だから使いまわす、はナシ。
例えば、C++ の FuncDecl と C# の MethodDecl は別にする。ほかにも FuncPtr とか Delegate とか。



リソース管理・シリアライズ
--------------------
シリアライズはイテレーションを効率的に行うために必要。
つまり、include ファイルがみつからない → include パス設定して再解析とか、
解析エラーのとき、ファイルちょっとだけ直して再解析とかのキャッシュ。


部分的な再解析
--------------------
まず、解析したいファイルに含まれている全ての Reference を削除する。
削除したら、その参照先の Entity の参照カウントを1つ減らす。
0になったら Entity を削除する。



その他メモ
--------------------
構造解析が終わった段階ではマクロ展開などはしない。
いつでも展開できるように情報をそろえておくだけ。



