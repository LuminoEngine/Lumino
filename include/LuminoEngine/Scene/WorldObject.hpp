
#pragma once
#include "Common.hpp"

namespace ln {
class World;
class Component;

class WorldObject
	: public Object
{
public:
    void addComponent(Component* component);

protected:
    // 物理演算・衝突判定の前
    virtual void onPreUpdate();

    // フレーム更新
    virtual void onUpdate(float elapsedSeconds);

    virtual void onRender();

LN_CONSTRUCT_ACCESS:
	WorldObject();
	virtual ~WorldObject();
	void initialize();

public: // TODO:
    void preUpdateFrame();
    void updateFrame(float elapsedSeconds);
    void render();

    World* m_world;
    Ref<List<Ref<Component>>> m_components;

    friend class World;
};

} // namespace ln
