
#pragma once
#include <functional>

namespace ln {
class TaskScheduler;
class Dispatcher;

enum class TaskStatus
{
	Created,	/**< Task オブジェクトは生成されているが、スケジューラに追加されていない。*/

	Waiting,	/**< スケジューラに追加され、実行されるのを待機中。*/

	Running,	/**< 実行中。*/

	Completed,	/**< 処理は正常に完了した。*/

	Faulted,	/**< 実行中にハンドルされない例外が発生して終了した。*/
};

// TODO: deprecated
class Task
    : public RefObject
{
public:
	//static Promise invoke(const std::function<void()>& action);
	//// ↓以下、いろいろあるけど UI では↑をメインに使う。



    static Ref<Task> create(const std::function<void()>& action);

    static Ref<Task> run(const std::function<void()>& action);

    void start();

    void wait();




	// 正常終了した場合の後続の処理を追加する。
 //   Task* then(const std::function<void()>& action, Dispatcher* dispatcher = nullptr);

	//Task* rejected(const std::function<void()>& action, Dispatcher* dispatcher = nullptr);


    //Task* awaitThen(const std::function<void()>& action, Dispatcher* dispatcher = nullptr);

    /** この Task の現在の状態を取得します。*/
    TaskStatus status() const;

    /** この Task が正常に完了したかどうかを確認します。*/
    bool isCompleted() const;

    /** 実行中にハンドルされない例外が発生したことが原因で Task が終了したかを確認します。*/
    bool isFaulted() const;

    /** 実行中に発生しハンドルされなかった例外を返します。例外が発生していなければ nullptr です。*/
    Exception* exception() const;


    void then(const std::function<void()>& action)
    {
        if (LN_REQUIRE(!this->m_prevTask)) return;
        auto continuationsTask = Task::create(action);
        continuationsTask->m_prevTask = this;
        continuationsTask->start();
    }

protected:
    Task(const std::function<void()>& action);
    ~Task();
    void execute();

    //struct NextTask
    //{
    //    Ref<Task> task;
    //    TaskScheduler* scheduler;
    //    Dispatcher* dispatcher;
    //};

    std::function<void()> m_action;
    std::atomic<TaskStatus> m_status;
    //std::vector<NextTask> m_nextTasks;
    Exception* m_exception;
    ConditionEvent m_waiting;

    Ref<Task> m_prevTask = nullptr;

    friend class TaskScheduler;
    friend class Dispatcher;
};

template<class TResult>
class GenericTask
    : public Task
{
public:
    static Ref<GenericTask<TResult>> create(const std::function<TResult()>& action);

    TResult result()
    {
        wait();
        return m_result;
    }

private:
    GenericTask(const std::function<TResult()>& action);

    //std::function<TResult()> m_action;
    TResult m_result;
};

template<class TResult>
Ref<GenericTask<TResult>> GenericTask<TResult>::create(const std::function<TResult()>& action)
{
    return Ref<GenericTask<TResult>>(LN_NEW GenericTask<TResult>(action), false);
}

template<class TResult>
GenericTask<TResult>::GenericTask(const std::function<TResult()>& action)
    : Task([this, action]() {
        m_result = action();
    }),
    m_result{}
{
}

namespace detail {

class semaphore
{
private:
    std::mutex mutex_;
    std::condition_variable condition_;
    unsigned long count_ = 0; // Initialized as locked.

public:
    void notify() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        ++count_;
        condition_.notify_one();
    }

    void wait() {
        std::unique_lock<decltype(mutex_)> lock(mutex_);
        while (!count_) // Handle spurious wake-ups.
            condition_.wait(lock);
        --count_;
    }

    bool try_wait() {
        std::lock_guard<decltype(mutex_)> lock(mutex_);
        if (count_) {
            --count_;
            return true;
        }
        return false;
    }
};
} // namespace detail


class Dispatcher
    : public RefObject
{
public:
    //static Dispatcher* mainThread();

    void post(Task* task);
    void post(const std::function<void()>& action);

	// これを、Task を実行したいスレッドで呼び出す。
    void executeTasks(uint32_t maxCount = UINT32_MAX);

	void dispose();

private:
    std::deque<Ref<Task>> m_taskQueue;
    std::mutex m_taskQueueMutex;
};

class TaskScheduler
    : public RefObject
{
public:
	static void init();
	static void finalizeInternal();
    static TaskScheduler* get();

    // この TaskScheduler が同時に並列実行できる Task の数を取得します。
    int maxConcurrencyLevel() const;

private:
    TaskScheduler(int threadCount);
    ~TaskScheduler();
    void dispose();

    void queueTask(Task* task);
    void executeThread();

    List<std::shared_ptr<std::thread>> m_threadList;
    List<Ref<Task>> m_taskQueue;
    //std::deque<Ref<Task>> m_taskQueue;
    detail::semaphore m_semaphore;
    std::mutex m_taskQueueMutex;
    std::atomic<bool> m_endRequested;

	static TaskScheduler* s_instance;

    friend class Task;
};


} // namespace ln
