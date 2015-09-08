// 以下の ifdef ブロックは DLL からのエクスポートを容易にするマクロを作成するための 
// 一般的な方法です。この DLL 内のすべてのファイルは、コマンド ラインで定義された LNOTE_HSP_EXPORTS
// シンボルを使用してコンパイルされます。このシンボルは、この DLL を使用するプロジェクトでは定義できません。
// ソースファイルがこのファイルを含んでいる他のプロジェクトは、 
// LNOTE_HSP_API 関数を DLL からインポートされたと見なすのに対し、この DLL は、このマクロで定義された
// シンボルをエクスポートされたと見なします。
#ifdef LNOTE_HSP_EXPORTS
#define LNOTE_HSP_API __declspec(dllexport)
#else
#define LNOTE_HSP_API __declspec(dllimport)
#endif

// このクラスは lnote_hsp.dll からエクスポートされました。
class LNOTE_HSP_API Clnote_hsp {
public:
	Clnote_hsp(void);
	// TODO: メソッドをここに追加してください。
};

extern LNOTE_HSP_API int nlnote_hsp;

LNOTE_HSP_API int fnlnote_hsp(void);
