#pragma once
#include <LuminoCore/Runtime/Object.hpp>

namespace ln {
class AbstractObjectPoolManager;

class AbstractObjectPoolPage : public RefObject {
public:
    RefObject* get(int32_t index) const { return m_objects[index].get(); }

protected:
    virtual Result<> onCreateObjects(int32_t count, Array<Ref<RefObject>>* result) = 0;

private:
    Array<Ref<RefObject>> m_objects;
    friend class AbstractObjectPoolManager;
};

class AbstractObjectPoolManager : public RefObject {
public:
    AbstractObjectPoolManager(int32_t pageSize);

    int32_t pageSize() const { return m_pageSize; }
    AbstractObjectPoolPage* requestPage();
    void discardPage(AbstractObjectPoolPage* page);

protected:
    virtual Ref<AbstractObjectPoolPage> onCreateNewPage() = 0;

private:
    Ref<AbstractObjectPoolPage> createNewPage(size_t size);

    int32_t m_pageSize;
    Array<Ref<AbstractObjectPoolPage>> m_pagePool;  // page instances
    std::deque<AbstractObjectPoolPage*> m_freePages; // page references
};


class ObjectPoolManager : public AbstractObjectPoolManager {
public:
    ObjectPoolManager(int32_t pageSize);

protected:
    virtual Result<> onCreateObjects(int32_t count, Array<Ref<RefObject>>* result) = 0;

    Ref<AbstractObjectPoolPage> onCreateNewPage() override;

private:
    class ObjectPoolPage : public AbstractObjectPoolPage {
    public:
        ObjectPoolPage(ObjectPoolManager* manager) : m_manager(manager) {}
        Result<> onCreateObjects(int32_t count, Array<Ref<RefObject>>* result) override {
            return m_manager->onCreateObjects(count, result);
        }

    private:
        ObjectPoolManager* m_manager;
    };
};


class ObjectPool : public RefObject {
public:
    ObjectPool(AbstractObjectPoolManager* manager);

    /**
     * Pool からひとつオブジェクトを取り出して返す。
     * 処理速度を重視するためコンストラクタ呼び出しは行われない。
     * 呼び出し側で必要に応じて初期化すること。
     */
    RefObject* allocate();

private:
    AbstractObjectPoolManager* m_manager;
    int32_t m_usedCount;
    AbstractObjectPoolPage* m_currentPage;
    List<AbstractObjectPoolPage*> m_retiredPages;
};

} // namespace ln
