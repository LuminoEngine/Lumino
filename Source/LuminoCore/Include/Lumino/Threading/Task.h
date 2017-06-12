
#pragma once
#include "../Base/Exception.h"
#include "../Base/RefObject.h"
#include "../Base/Delegate.h"
#include "ConditionFlag.h"

LN_NAMESPACE_BEGIN
namespace tr
{
class Task;
typedef RefPtr<Task>	TaskPtr;

enum class TaskStatus
{
	Created,	/**< Task オブジェクトは生成されているが、スケジューラに追加されていない。*/

	Waiting,	/**< スケジューラに追加され、実行されるのを待機中。*/

	Running,	/**< 実行中。*/

	Completed,	/**< 処理は正常に完了した。*/

	Faulted,	/**< 実行中にハンドルされない例外が発生して終了した。*/
};

/**
	@brief	
*/
class Task
	: public RefObject
{
public:

	static TaskPtr create(const Delegate<void()>& action);

	static TaskPtr Run(const Delegate<void()>& action);

	void start();

	void Wait();

	/** この Task の現在の状態を取得します。*/
	TaskStatus GetStatus() const;

	/** この Task が完了したかどうかを確認します。*/
	bool IsCompleted() const;

	/** 実行中にハンドルされない例外が発生したことが原因で Task が終了したかを確認します。*/
	bool IsFaulted() const;

	/** 実行中に発生しハンドルされなかった例外を返します。例外が発生していなければ nullptr です。*/
	Exception* GetException() const;

LN_INTERNAL_ACCESS:
	void execute();

private:
	Task(const Delegate<void()>& action);	// 今は Run からだけ考える
	~Task();

	Delegate<void()>	m_action;
	TaskStatus			m_status;
	Exception*			m_exception;
	ConditionFlag		m_waiting;
};

} // namespace tr
LN_NAMESPACE_END
