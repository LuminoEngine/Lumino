
#pragma once
#include "Common.hpp"
#include "../Rendering/RenderingContext.hpp"
#include "../Scene/Component.hpp"

namespace ln {

class VisualComponent
	: public Component
{
    LN_OBJECT;
public:
	/** 可視状態を設定します。false の場合、コンポーネントの描画は行われません。(default: true) */
	LN_METHOD(Property)
	void setVisible(bool value) { m_isVisible = value; }

	/** 可視状態を取得します。*/
	LN_METHOD(Property)
	bool isVisible() const { return m_isVisible; }

protected:

LN_CONSTRUCT_ACCESS:
    VisualComponent();
	virtual ~VisualComponent();
	void initialize();

private:
    virtual void render(RenderingContext* context) override;

    bool m_isVisible;
};

} // namespace ln
