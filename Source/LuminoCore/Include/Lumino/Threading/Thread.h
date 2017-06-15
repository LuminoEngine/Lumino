
#pragma once
#include "../Base/Common.h"
#include "../Base/Delegate.h"
#include "ConditionFlag.h"

LN_NAMESPACE_BEGIN
namespace detail { class ThreadImpl; }
class Exception;

/**
	@brief		スレッドクラスの基底クラス
	@details	使い方
				①このクラスを継承した新しいクラスを作成し、Execute() を実装します。
				②Start() でスレッドを開始します。
				③wait() で終了を待ちます。
				  終了した際に Execute() で発生した例外がある場合、wait() から際 throw されます。
				④インスタンスを破棄します。
*/
class LUMINO_EXPORT Thread
{
public:
    Thread();
    virtual ~Thread();

public:

	/**
		@brief	スレッド関数 (これを自身で実装する)
	*/
	virtual void execute() = 0;

public:

	/**
		@brief	実行開始
	*/
	void start();

	/**
		@brief		スレッドの終了を待つ
		@details	スレッドがハンドルされない例外で終了した場合、
					この関数はその例外を再throwします。
	*/
	void wait();

	/**
		@brief	終了しているかどうかの判定
	*/
	bool isFinished();

	/**
		@brief	スレッド識別子の取得 (start() 後に呼ぶこと)
	*/
	intptr_t getThreadId() const;

public:

	/**
		@brief	指定した時間だけ現在のスレッドをスリープ
		@param	milliseconds	: 待機時間 (ミリ秒)
	*/
	static void sleep(int milliseconds);

	/**
		@brief	現在実行中のスレッドのIDを取得
	*/
	static intptr_t getCurrentThreadId();

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Thread);

	friend class detail::ThreadImpl;
	void ExecuteInternal();
	void reset();

	detail::ThreadImpl*	m_impl;
	ConditionFlag		mFinished;
	Exception*			mLastException;
};


/**
	@brief	Delegate をスレッド関数として実行するスレッド
	@code
			class Test
			{
			private:
				DelegateThread	m_Thread;
				void ThreadFunc()
				{
					// 別のスレッドで実行されるメンバ関数
				}
			public:
				void run()
				{
					// スレッド起動
					m_Thread.Start(CreateDelegate(this, &Test::ThreadFunc));
				}
			};
	@endcode
*/
class LUMINO_EXPORT DelegateThread
    : public Thread
{
#ifdef LN_CPP11
public:
	void start(Delegate<void()> func);

protected:
	virtual void execute();

private:
	Delegate<void()>	m_ThreadFunc;
#else
public:
	void start(Delegate00 func);

protected:
	virtual void Execute();
	
private:
	Delegate00	m_ThreadFunc;
#endif
};

LN_NAMESPACE_END
