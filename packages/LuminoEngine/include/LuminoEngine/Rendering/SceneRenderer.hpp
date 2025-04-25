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
    enum class DrawElementType {
        Sprite,
    };

    // SceneNode と考えてOK。Zソートの対象となる。
    struct DrawElement {
        DrawElementType type;
        DrawElement* next;
        float zDistance;
        bool visible;
        DrawElement(DrawElementType t)
            : type(t)
            , next(nullptr)
            , zDistance(0.0f)
            , visible(true) {}
        virtual ~DrawElement() {}
        virtual const Matrix& worldMatrix() = 0;
        virtual void onRender() = 0;
    };

    SceneRenderer(detail::RenderingManager* manager);
    MaybeResult init();
    void dispose();
    void destructDrawElementList();
    void addDrawElement(DrawElement* instance);
    void setupElement(DrawElement* instance);

    // Not call destructor.
    template<class T, class... TArgs>
    T* newDrawElement() {
        void* buffer = m_dataAllocator->allocate(sizeof(T));
        T* data = new (buffer) T(std::forward<TArgs>(args)...);
        addDrawElement(data);
        return data;
    }

    detail::RenderingManager* m_manager;
    Ref<detail::LinearAllocator> m_dataAllocator;
    DrawElement* m_headDrawElement;
    DrawElement* m_tailDrawElement;
    int m_visibleDrawElementCount;
    const RenderViewPoint* m_currentViewPoint;
    detail::ZSortDistanceBase m_zSortDistanceBase;

};


} // namespace ln

