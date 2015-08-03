
#pragma once

#include <Lumino/Base/Rect.h>
#include <Lumino/Math/Matrix.h>

namespace Lumino
{
namespace Graphics
{
class GraphicsManager;
class GeometryRendererCore;

/**
	@brief		基本的な形状の描画クラス
*/
class GeometryRenderer
	: public RefObject
{
public:
	static GeometryRenderer* Create(GraphicsManager* manager);

public:

	/// 描画パスの開始 (呼び出しは任意。呼び出さない場合、Draw～ 系はただ頂点バッファを DrawPrimitive するだけになる)
	void BeginPass();

	/// 描画パスの終了
	void EndPass();

	/// 座標変換行列の設定
	void SetTransform(const Matrix& matrix);

	/// 座標変換行列の設定
	void SetViewProjTransform(const Matrix& matrix);

	/// テクスチャの設定 (NULL でダミーテクスチャを使用する)
	void SetTexture(Device::ITexture* texture);

	/// 線分の描画
	void DrawLine(const Vector3& from, const Vector3& to, const ColorF& fromColor, const ColorF& toColor);

	/// 四角形の描画
	void DrawSquare(
		float x1, float y1, float z1, float u1, float v1, const ColorF& c1,
		float x2, float y2, float z2, float u2, float v2, const ColorF& c2,
		float x3, float y3, float z3, float u3, float v3, const ColorF& c3,
		float x4, float y4, float z4, float u4, float v4, const ColorF& c4);

	/// 矩形の描画     
	void DrawRect(const RectF& destRect, const RectF& texUVRect, const ColorF& color);

protected:
	GeometryRenderer(GraphicsManager* manager);
	virtual ~GeometryRenderer();
	GraphicsManager*		m_manager;
	GeometryRendererCore*	m_internal;
};

} // namespace Graphics
} // namespace Lumino
