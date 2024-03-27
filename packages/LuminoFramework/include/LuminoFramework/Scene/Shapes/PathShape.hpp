
#pragma once
#include "../../Visual/VisualComponent.hpp"
#include "../VisualObject.hpp"

namespace ln {
class Material;
class MeshPrimitive;
namespace detail {
struct PathShape_NVGcontext;
}

enum class PathPlacementSpace
{
	/** 図形を XY 平面上に生成します。Z+ を正面とします。 */
	XY,

	/** 図形を XZ 平面上に生成します。Y+ を正面とします。 */
	XZ,
};
	
class PathShapeComponent
    : public VisualComponent
{
    LN_OBJECT;
public:
	/** @copydoc PathShape::beginCanvas */
	void beginCanvas();
	/** @copydoc PathShape::endCanvas */
	void endCanvas(PathPlacementSpace space);
	/** @copydoc PathShape::beginPath */
	void beginPath();
	/** @copydoc PathShape::moveTo */
	void moveTo(float x, float y);
	/** @copydoc PathShape::lineTo */
	void lineTo(float x, float y);
	/** @copydoc PathShape::fill */
	void fill();
	/** @copydoc PathShape::stroke */
	void stroke();
	/** @copydoc PathShape::closePath */
	void closePath();
	/** @copydoc PathShape::setFillColor */
	void setFillColor(const Color& value);
	/** @copydoc PathShape::setStrokeColor */
	void setStrokeColor(const Color& value);
	/** @copydoc PathShape::setStrokeWidth */
	void setStrokeWidth(float value);

protected:
	void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    PathShapeComponent();
    bool init();

private:
	Ref<Material> m_material;
	Ref<MeshPrimitive> m_mesh;
	detail::PathShape_NVGcontext* m_pathContext;
};

/**
 * 
 */
LN_CLASS()
class PathShape
	: public VisualObject
{
	LN_OBJECT;
public:

	/** パスレンダリングを開始します。 */
	void beginCanvas() { m_component->beginCanvas(); }

	/** パスレンダリングを終了し、メッシュを構築します。 */
	void endCanvas(PathPlacementSpace space = PathPlacementSpace::XY) { m_component->endCanvas(space); }

	// Paths

	/** 新しいパスを開始します。 */
	void beginPath() { m_component->beginPath(); }

	/** 指定したポイントを最初のポイントとして新しいサブパスを開始します。 */
	void moveTo(float x, float y) { m_component->moveTo(x, y); }

	/** パスの最後のポイントから指定されたポイントまでの線分を追加します。 */
	void lineTo(float x, float y) { m_component->lineTo(x, y); }

	/** 始点と終点を結んでパスを閉じます。 */
	void closePath() { m_component->closePath(); }

	/** 現在のパスを現在の塗りつぶしスタイルで塗りつぶします。 */
	void fill() { m_component->fill(); }

	/** 現在のパスを現在のストロークスタイルで塗りつぶします。 */
	void stroke() { m_component->stroke(); }

	// Render styles

	/** 現在の塗りつぶしスタイルを、指定した単色に設定します。 */
	void setFillColor(const Color& value) { m_component->setFillColor(value); }

	/** 現在のストロークスタイルを、指定した単色に設定します。 */
	void setStrokeColor(const Color& value) { m_component->setStrokeColor(value); }

	/** 現在のストロークスタイルの幅を設定します。 */
	void setStrokeWidth(float value) { m_component->setStrokeWidth(value); }

LN_CONSTRUCT_ACCESS:
	PathShape();
	bool init();

private:
	Ref<PathShapeComponent> m_component;
};

} // namespace ln
