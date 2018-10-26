
#pragma once
#include "Common.hpp"

namespace ln {
class World;

class WorldObject
	: public Object
{
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

private:
    void preUpdateFrame();
    void updateFrame(float elapsedSeconds);
    void render();

    friend class World;
};

} // namespace ln
