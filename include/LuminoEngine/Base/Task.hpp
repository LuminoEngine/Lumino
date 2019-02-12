
#pragma once

namespace ln {

enum class TaskStatus
{
	Created,	/**< Task オブジェクトは生成されているが、スケジューラに追加されていない。*/

	Waiting,	/**< スケジューラに追加され、実行されるのを待機中。*/

	Running,	/**< 実行中。*/

	Completed,	/**< 処理は正常に完了した。*/

	Faulted,	/**< 実行中にハンドルされない例外が発生して終了した。*/
};

class Task
{
public:

private:
};

} // namespace ln
