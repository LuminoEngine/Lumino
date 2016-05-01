
#include <windows.h>
#include "EntryPoint.h"

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//	_In_opt_ HINSTANCE hPrevInstance,
//	_In_ LPWSTR    lpCmdLine,
//	_In_ int       nCmdShow)
int APIENTRY WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
	)
{
#if 0
	char* c = GetCommandLineA();

	// 件数を調べる
	int argc = 0;
	for (; *c; ++argc)
	{
		// 先頭や途中や最後に使われている空白をスキップする
		while (*c == ' ') { ++c; }

		// コマンドライン最後の空白だった場合，ループから抜ける
		if (!(*c)) { break; }

		// "" で囲まれた文字列を処理
		if (*c == '\"')
		{
			// " をスキップ
			++c;

			// 終わりの " を探す
			while (*c && *c != '\"') { ++c; }

			// この時点で c は " を指しているので、次の文字に移動
			++c;
		}
		// スペースで囲まれた文字列を処理
		else
		{
			// 次の空白まで移動
			while (*c && *c != ' ') { ++c; }
		}
	}

	c = GetCommandLineA();
	char** argv = static_cast< char** >(malloc(sizeof(char*) * argc));
	int i = 0;

	while (*c)
	{
		// 先頭や途中や最後に使われている空白をスキップする
		while (*c == ' ') { ++c; }

		// コマンドライン最後の空白だった場合，ループから抜ける
		if (!(*c)) { break; }

		// "" で囲まれた文字列を処理
		if (*c == '\"')
		{
			// " をスキップ
			++c;

			argv[i] = c;
			++i;

			// 終わりの " を探す
			while (*c && *c != '\"') { ++c; }

			*c = '\0';	// " を \0 に変えて区切る

			++c;
		}
		// スペースで囲まれた文字列を処理
		else
		{
			// 次の空白まで移動
			while (*c && *c != ' ') { ++c; }
		}
	}
#endif

	int r = ln::EntryPoint();

	//SAFE_FREE(argv);

	return r;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
int main(int argc_, char* argv_[])
{
	return ln::EntryPoint();
}
