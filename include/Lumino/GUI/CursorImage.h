
#pragma once
#include <Lumino/Graphics/Painter.h>
#include "Common.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN

/**
	@brief	マウスカーソルの形状を表すクラスです。
*/
class CursorImage
	: public CoreObject
{
public:
	CursorImage(GUIManagerImpl* manager);
	virtual ~CursorImage();

	void SetTexture(Texture* texture) { m_texture = texture; }
	void SetSourceRect(const Rect& rect) { m_srcRect = rect; }
	void SetPatternCount(int count) { m_patternCount = count; }
	void SetPatternDuration(float time) { m_patternDuration = time; }	// アニメーションが1ループするのにかかる時間
	float GetPatternDuration() const { return m_patternDuration; }

	void Draw(Painter* painter, const PointF& point, float time);

private:
	GUIManagerImpl*				m_manager;
	RefPtr<Texture>	m_texture;
	Rect						m_srcRect;
	int							m_patternCount;
	float						m_patternDuration;
};

LN_NAMESPACE_GUI_END
} // namespace Lumino
