// 以下の ifdef ブロックは DLL からのエクスポートを容易にするマクロを作成するための 
// 一般的な方法です。この DLL 内のすべてのファイルは、コマンド ラインで定義された LUMINOHSP_EXPORTS
// シンボルを使用してコンパイルされます。このシンボルは、この DLL を使用するプロジェクトでは定義できません。
// ソースファイルがこのファイルを含んでいる他のプロジェクトは、 
// LUMINOHSP_API 関数を DLL からインポートされたと見なすのに対し、この DLL は、このマクロで定義された
// シンボルをエクスポートされたと見なします。
#ifdef LUMINOHSP_EXPORTS
#define LUMINOHSP_API __declspec(dllexport)
#else
#define LUMINOHSP_API __declspec(dllimport)
#endif

bool Commands_cmdfunc(int cmd, int* retVal);
bool Structs_reffunc(int cmd, int* typeRes, void** retValPtr);
void RegisterStructTypes(HSP3TYPEINFO *info);
