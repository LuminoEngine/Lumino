
#pragma once
#include "WorldObject.hpp"

namespace ln {
class VisualComponent;

/** VisualComponent がアタッチされた WorldObject のプレハブクラスです。 */
class VisualObject
	: public WorldObject
{
    LN_OBJECT;
public:
	/** 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true) */
    LN_METHOD(Property)
    void setVisible(bool value);

	/** 可視状態を取得します。*/
    LN_METHOD(Property)
    bool isVisible() const;

LN_CONSTRUCT_ACCESS:
    VisualObject();
	virtual ~VisualObject();
	void initialize();

protected:
    void setMainVisualComponent(VisualComponent* component);
    const Ref<VisualComponent>& mainVisualComponent() const { return m_component; }

private:
    Ref<VisualComponent> m_component;
};

} // namespace ln
