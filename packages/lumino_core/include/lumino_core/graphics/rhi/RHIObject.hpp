#pragma once

#include <functional>
#include <vector>
#include <lumino_base/RefObject.hpp>

namespace ln::rhi {

/**
 * すべてのRHIリソースオブジェクトの基底クラス。
 * finalize()が呼び出されたとき（参照カウントがゼロになったとき）、登録済みの破棄コールバックを呼び出します。
 * PipelineCacheなどのキャッシュが古いエントリを自動的に削除できるようにします。
 */
class RHIObject : public RefObject {
public:
    using DestroyCallback = std::function<void(RHIObject*)>;

    void addDestroyCallback(DestroyCallback cb) {
        m_destroyCallbacks.push_back(std::move(cb));
    }

protected:
    void finalize() override {
        for (auto& cb : m_destroyCallbacks) {
            cb(this);
        }
        m_destroyCallbacks.clear();
        RefObject::finalize();
    }

private:
    std::vector<DestroyCallback> m_destroyCallbacks;
};

} // namespace ln::rhi
