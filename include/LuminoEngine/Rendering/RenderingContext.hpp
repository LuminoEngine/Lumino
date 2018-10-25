#pragma once
#include "../Graphics/ColorStructs.hpp"
#include "../Graphics/GeometryStructs.hpp"
#include "../Graphics/RenderState.hpp"

namespace ln {
class Texture;
class RenderTargetTexture;
class DepthBuffer;
class AbstractMaterial;
class MeshContainer;
namespace detail {
class RenderingManager;
class DrawElementList;
class DrawElementListBuilder;
} // namespace detail

class RenderingContext
	: public RefObject
{
public:
	//--------------------------------------------------------------------------
	/** @name render targets */
	/** @{ */

	/** レンダリングターゲットを設定します。index 0 のレンダリングターゲットを変更すると、ビューポート領域とシザー領域がリセットされます。 */
	void setRenderTarget(int index, RenderTargetTexture* value);

	/** 現在設定されているレンダリングターゲットを取得します。*/
	//RenderTargetTexture* getRenderTarget(int index) const;

	/** 深度バッファを設定します。*/
	void setDepthBuffer(DepthBuffer* value);

	/** 現在設定されている深度バッファを取得します。*/
	//DepthBuffer* getDepthBuffer() const;

	/** ビューポート領域を設定します。*/
	void setViewportRect(const RectI& value);	// これも optional でいい気がする

	/** 現在設定されている ビューポート領域を取得します。*/
	//const RectI& getViewportRect() const;

	/** シザー領域を設定します。*/
	void setScissorRect(const RectI& value);

	/** @} */

	//--------------------------------------------------------------------------
	/** @name render status */
	/** @{ */

	/** BlendMode を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setBlendMode(Optional<BlendMode> value);

	/** CullMode を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setCullingMode(Optional<CullMode> value);

	/** 深度テストの有無を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setDepthTestEnabled(Optional<bool> value);

	/** 深度書き込みの有無を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setDepthWriteEnabled(Optional<bool> value);

	/** @} */

	//--------------------------------------------------------------------------
	/** @name drawing */
	/** @{ */

	/** スプライトを描画します。 */
	void drawSprite(
		const Matrix& transform,
		const Size& size,
		const Vector2& anchor,
		const Rect& srcRect,
		const Color& color,
		SpriteBaseDirection baseDirection,
		BillboardType billboardType,
		AbstractMaterial* material);

	void drawMesh(MeshContainer* meshContainer, int sectionIndex);

	/** @} */


	//--------------------------------------------------------------------------
	/** @name light */
	/** @{ */

	void addAmbientLight(const Color& color, float intensity);

	void addHemisphereLight(const Color& skyColor, const Color& groundColor, float intensity);

	void addDirectionalLight(const Color& color, float intensity, const Vector3& direction);

	void addPointLight(const Color& color, float intensity, const Vector3& position, float range, float attenuation);

	void addSpotLight(const Color& color, float intensity, const Vector3& position, const Vector3& direction, float range, float attenuation, float spotAngle, float spotPenumbra);

	/** @} */

LN_PROTECTED_INTERNAL_ACCESS:
	RenderingContext();
	virtual ~RenderingContext();
	void setDrawElementList(detail::DrawElementList* list);
	void reset();

private:
	detail::RenderingManager* m_manager;
	Ref<detail::DrawElementListBuilder> m_builder;
};

namespace detail {

// TODO: Test
class WorldSceneGraphRenderingContext
	: public RenderingContext
{
public:
	WorldSceneGraphRenderingContext();

	void reset();

	// 本番では、World が持っていたりする。
	Ref<detail::DrawElementList> m_elementList;

private:
};

} // namespace detail
} // namespace ln

