/*
	Note:

	Lumino の Task モジュールは、C# のタスクベースの非同期パターン https://docs.microsoft.com/ja-jp/dotnet/standard/asynchronous-programming-patterns/task-based-asynchronous-pattern-tap?view=netframework-4.7.2
	に似たプログラミングモデルを提供するためのクラス群です。

	C++ では非同期処理のための機能として std::async や std::future などのクラスが提供されていますが、
	タスクが実行されるスレッドのコントロールや、実行終了通知を送るスレッドを指定することは困難です。

	Task モジュールがカバーする代表的なストーリーは次の通りです。
	- UI スレッドを持つ C# の await のように、タスク実行終了後はメインスレッドに通知を送る (後処理を実行する)
	- Emscripten やその他のスレッドを利用することが困難なスクリプト言語向けに、ローディングなどの処理をメインスレッドで行うようにスイッチできるようにする
*/
#include "Internal.hpp"
#include <LuminoEngine/Base/Task.hpp>


namespace ln {


} // namespace ln

