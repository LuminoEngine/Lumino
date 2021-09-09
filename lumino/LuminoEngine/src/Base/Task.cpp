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
    
//==============================================================================
// Task

Ref<Task> Task::create(const std::function<void()>& action)
{
    // コンストラクタを private にしてあるので new で入れる
    return Ref<Task>(LN_NEW Task(action), false);
}

Ref<Task> Task::run(const std::function<void()>& action)
{
    auto t = Ref<Task>(LN_NEW Task(action), false);
    t->start();
    return t;
}

Task::Task(const std::function<void()>& action)
	: m_action(action)
	, m_status(TaskStatus::Created)
	, m_exception(nullptr)
	, m_waiting(false)
{
}

Task::~Task()
{
	LN_SAFE_DELETE(m_exception);
}

void Task::start()
{
	LN_SAFE_DELETE(m_exception);
	m_waiting.lock();
	TaskScheduler::get()->queueTask(this);
}

void Task::wait()
{
	m_waiting.wait();
}

//Task* Task::then(const std::function<void()>& action, Dispatcher* dispatcher)
//{
//    NextTask n;
//    n.task = create(action);
//    n.scheduler = nullptr;
//    n.dispatcher = (dispatcher) ? dispatcher : Dispatcher::mainThread();
//	m_nextTasks.push_back(n);
//    return n.task;
//}

//Task* Task::awaitThen(const std::function<void()>& action, Dispatcher* dispatcher)
//{
//    NextTask n;
//    n.task = create(action);
//    n.scheduler = nullptr;
//    n.dispatcher = (dispatcher) ? dispatcher : Dispatcher::mainThread();
//    return n.task;
//}

TaskStatus Task::status() const
{
	return m_status;
}

bool Task::isCompleted() const
{
	return m_status == TaskStatus::Completed;
}

bool Task::isFaulted() const
{
	return m_status == TaskStatus::Faulted;
}

Exception* Task::exception() const
{
	return m_exception;
}

void Task::execute()
{
	m_status = TaskStatus::Running;
	try
	{
		m_action();
		m_status = TaskStatus::Completed;

        //// post next tasks
        //for (auto& n : m_nextTasks) {
        //    if (n.dispatcher) {
        //        n.dispatcher->post(n.task);
        //    }
        //}
	}
	catch (Exception& e)
	{
		m_exception = e.copy();
		m_status = TaskStatus::Faulted;
	}
	catch (...)
	{
		m_exception = LN_NEW Exception();
		m_status = TaskStatus::Faulted;
	}
	m_waiting.unlock();
}

//==============================================================================
// GenericTask


//==============================================================================
// Dispatcher

//Dispatcher* Dispatcher::mainThread()
//{
//    // TODO: ほんとは外部からもらった方がいい
//    static Dispatcher instance;
//    return &instance;
//}

void Dispatcher::post(Task* task)
{
    std::lock_guard<std::mutex> lock(m_taskQueueMutex);
    m_taskQueue.push_back(task);
}

void Dispatcher::post(const std::function<void()>& action)
{
    post(Task::create(action));
}

void Dispatcher::executeTasks(uint32_t maxCount)
{
    for (uint32_t i = 0; !m_taskQueue.empty() && i < maxCount; i++)
    {
        // キューから1つ取り出す
        Ref<Task> task = nullptr;
        {
            std::lock_guard<std::mutex> lock(m_taskQueueMutex);
            task = m_taskQueue.front();
            m_taskQueue.pop_front();
        }

        // 実行。状態変化は内部で行う
        task->execute();
        task.reset();
    }
}

void Dispatcher::dispose()
{
	executeTasks();
}

//==============================================================================
// TaskScheduler

//TaskScheduler* TaskScheduler::getDefault()
//{
//    static TaskScheduler instance(4);
//    return &instance;
//}

TaskScheduler* TaskScheduler::s_instance = nullptr;

void TaskScheduler::init()
{
	if (!s_instance) {
		s_instance = LN_NEW TaskScheduler(4);
	}
}

void TaskScheduler::finalizeInternal()
{
	if (s_instance) {
        s_instance->dispose();
		RefObjectHelper::release(s_instance);
		s_instance = nullptr;
	}
}

TaskScheduler* TaskScheduler::get()
{
	return s_instance;
}

TaskScheduler::TaskScheduler(int threadCount)
    : m_threadList()
    , m_semaphore()
    , m_endRequested(false)
{
    m_threadList.reserve(threadCount);
    for (int i = 0; i < threadCount; ++i)
    {
        m_threadList.add(std::make_shared<std::thread>(std::bind(&TaskScheduler::executeThread, this)));
    }
}

TaskScheduler::~TaskScheduler()
{
    LN_CHECK(m_threadList.isEmpty());
}

void TaskScheduler::dispose()
{
    m_endRequested = true;

    for (int i = 0; i < m_threadList.size(); i++)	// スレッドの数だけセマフォ増やして全部起こして、
    {
        m_semaphore.notify();
    }
    for (auto& thr : m_threadList)	// 全部終わるまで待つ
    {
        thr->join();
    }
    m_threadList.clear();

    LN_CHECK(m_taskQueue.isEmpty());
}

int TaskScheduler::maxConcurrencyLevel() const
{
    return m_threadList.size();
}

void TaskScheduler::queueTask(Task* task)
{
    if (LN_REQUIRE(task != nullptr)) return;

    std::lock_guard<std::mutex> lock(m_taskQueueMutex);
    m_taskQueue.add(task);

    m_semaphore.notify();
}

void TaskScheduler::executeThread()
{
    bool queueEmpty = true;

    while (true)
    {
        if (queueEmpty) {
            m_semaphore.wait();	// キューに何か追加されるまで待つ。または終了要求まで。
        }
        else {
            Thread::sleep(1);
        }

        // 終了要求がきていたらおしまい
        if (m_endRequested) {
            break;
        }

        // キューから1つ取り出す
        Ref<Task> task = nullptr;
        {
            std::lock_guard<std::mutex> lock(m_taskQueueMutex);



            for (auto itr = m_taskQueue.begin(); itr < m_taskQueue.end();) {
                bool pop = false;
                bool executale = false;
                auto& t = *itr;
                if (t->m_prevTask) {
                    if (t->m_prevTask->isCompleted()) {
                        pop = true;
                        executale = true;
                    }
                    else if (t->m_prevTask->isFaulted()) {
                        pop = true;
                        executale = false;
                    }
                }
                else {
                    pop = true;
                    executale = true;
                }

                if (executale) {
                    task = t;
                }
                if (pop) {
                    itr = m_taskQueue.erase(itr);
                }
                else {
                    ++itr;
                }
            }

            queueEmpty = m_taskQueue.isEmpty();
        }

        // 実行。状態変化は内部で行う
        if (task) {
            task->execute();
            task.reset();
        }
    }
}

} // namespace ln

