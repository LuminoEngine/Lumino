
#pragma once
#include "../../Visual/VisualComponent.hpp"
#include "../VisualObject.hpp"

namespace ln {
class Material;
class MeshPrimitive;

enum class PathPlacementSpace
{
	/** 図形を XY 平面上に生成します。Z+ を正面、Y+ を上とします。 */
	XY,

	/** 図形を XZ 平面上に生成します。Y+ 正面、Z- を上とします。 */
	XZ,
};
	
class PathShapeComponent
    : public VisualComponent
{
    LN_OBJECT;
public:

protected:
	void onRender(RenderingContext* context) override;

LN_CONSTRUCT_ACCESS:
    PathShapeComponent();
    bool init();

private:
	Ref<Material> m_material;
	Ref<MeshPrimitive> m_mesh;
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
	void beginPath();
	void endPath();

	// Paths

	/** 指定したポイントを最初のポイントとして新しいサブパスを開始します。 */
	void moveTo(float x);

	/** パスの最後のポイントから指定されたポイントまでの線分を追加します。 */
	void lineTo(float x, float y);

	/** 現在のパスを現在の塗りつぶしスタイルで塗りつぶします。 */
	void fill();

	// Render styles

	/** 現在の塗りつぶしスタイルを、指定した単色に設定します。 */
	void setFillColor(const Color& color);

LN_CONSTRUCT_ACCESS:
	PathShape();
	bool init();

private:
	Ref<PathShapeComponent> m_component;
};

} // namespace ln
