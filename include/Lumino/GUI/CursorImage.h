
#pragma once
#include <Lumino/Graphics/Painter.h>

namespace Lumino
{
namespace GUI
{

/**
	@brief	マウスカーソルの形状を表すクラスです。
*/
class CursorImage
	: public CoreObject
{
public:
	CursorImage(GUIManager* manager);
	virtual ~CursorImage();

	void SetTexture(Graphics::Texture* texture) { m_texture = texture; }
	void SetSourceRect(const Rect& rect) { m_srcRect = rect; }
	void SetPatternCount(int count) { m_patternCount = count; }
	void SetPatternDuration(float time) { m_patternDuration = time; }	// アニメーションが1ループするのにかかる時間
	float GetPatternDuration() const { return m_patternDuration; }

	void Draw(Graphics::Painter* painter, const PointF& point, float time);

private:
	GUIManager*					m_manager;
	RefPtr<Graphics::Texture>	m_texture;
	Rect						m_srcRect;
	int							m_patternCount;
	float						m_patternDuration;
};

} // namespace GUI
} // namespace Lumino
