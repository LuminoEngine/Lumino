
#pragma once
#include "Common.hpp"

namespace ln {
class Level;
class World;
class WorldObject;
class RenderingContext;
namespace detail {
    class WorldObjectTransform;
}

LN_CLASS()
class Component
	: public Object
    , public IWorldRenderingElement
{
    LN_OBJECT;
public:

    WorldObject* worldObject() const { return m_object; }

protected:
    // アタッチされた WorldObject の transform へのポインタ
    detail::WorldObjectTransform* transrom() const;

	/** オブジェクトが World に追加された後、最初の onUpdate の前に呼び出されます。onStart() が呼び出された時点では、オブジェクトは必ず何らかの World に属しています。 */
	virtual void onStart();

    // 以下、すべて空実装
    virtual void onAttached(WorldObject* owner);
    virtual void onDetaching(WorldObject* owner);
    virtual void onPreUpdate(float elapsedSeconds);
    virtual void onUpdate(float elapsedSeconds);
    virtual void onApplyPhysicsFeedForward();
    virtual void onPrepareRender(RenderingContext* context);
    virtual void onRender(RenderingContext* context);

    /**
     * 調整やデバッグのためのコンテンツを描画するべき時に呼び出されます。
     * 
     * RenderView によりデバッグ描画が要求されている場合等に呼び出されます。
     * あくまで開発者にとって有用な情報を描画する際に利用するべきです。
     * キャラクターのネームプレートなど、エンドユーザーに示したい情報は onRender() で描画してください。
     */
    virtual void onRenderGizmo(RenderingContext* context);

LN_CONSTRUCT_ACCESS:
	Component();
	virtual ~Component();
	Result<> init();

public:
    WorldObject* m_object;
    //bool m_mainVisualComponent = false; // TODO: flags

private:
    void render(RenderingContext* context) override;
    void renderGizmo(RenderingContext* context);

    friend class Level;
    friend class World;
    friend class WorldObject;
};

} // namespace ln
