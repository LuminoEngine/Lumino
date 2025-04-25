#pragma once
#include "Common.hpp"

namespace ln {

class SceneRenderer final : public Object {
    // NOTE: 以前の RenderingContext のような、ハイレベル描画クラス。
    //   SpriteRenderer を自分でコントロールして begin/end する方法だと、
    //   それさえまたぐ状況でのZソートが出来なくなるため、改めて用意した。
    //   RenderPass の begin~end の間で使用できる。
public:
    void reset(const RenderViewPoint* currentViewPoint);
    void render();
    void drawSprite(const SpriteData& data);

private:
    enum class DrawInstanceType {
        Sprite,
    };

    // SceneNode と考えてOK。Zソートの対象となる。
    struct DrawInstance {
        DrawInstanceType type;
        DrawInstance* next;
        float zDistance;
        bool visible;
        DrawInstance(DrawInstanceType t)
            : type(t)
            , next(nullptr)
            , zDistance(0.0f)
            , visible(true) {}
        virtual ~DrawInstance() {}
        virtual const Matrix& worldMatrix() = 0;
        virtual void onRender() = 0;
    };

    SceneRenderer(detail::RenderingManager* manager);
    MaybeResult init();
    void dispose();
    void destructDrawInstanceList();
    void addDrawInstance(DrawInstance* instance);
    void setupInstance(DrawInstance* instance);

    // Not call destructor.
    template<class T, class... TArgs>
    T* newDrawInstance() {
        void* buffer = m_dataAllocator->allocate(sizeof(T));
        T* data = new (buffer) T(std::forward<TArgs>(args)...);
        addDrawInstance(data);
        return data;
    }


    detail::RenderingManager* m_manager;
    Ref<detail::LinearAllocator> m_dataAllocator;
    DrawInstance* m_headDrawInstance;
    DrawInstance* m_tailDrawInstance;
    int m_drawInstanceCount;
    const RenderViewPoint* m_currentViewPoint;
    detail::ZSortDistanceBase m_zSortDistanceBase;

};


} // namespace ln

