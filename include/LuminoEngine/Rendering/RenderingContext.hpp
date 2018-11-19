#pragma once
#include "Common.hpp"
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
class BuiltinEffectData;
} // namespace detail

class RenderingContext
	: public RefObject  // TODO: Object
{
public:
	//--------------------------------------------------------------------------
	/** @name render targets */
	/** @{ */

	/** レンダリングターゲットを設定します。index 0 のレンダリングターゲットを変更すると、ビューポート領域とシザー領域がリセットされます。 */
	void setRenderTarget(int index, RenderTargetTexture* value);

	/** 現在設定されているレンダリングターゲットを取得します。*/
	//RenderTargetTexture* getRenderTarget(int index) const;
    RenderTargetTexture* renderTarget(int index) const;

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

    // このあたりの設定は Material の設定をオーバーライドする。
    // 主な用途としては、Mesh のように複数のマテリアルを含むオブジェクトを描画する際、
    // サブセット全体の設定をオーバーライドするようなケースを想定している。
    // (特に、ヒットエフェクトのための BlendColor や、エディタでの両面表示設定など、Material 自体のパラメータを変更したくない場合)

	/** BlendMode を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setBlendMode(Optional<BlendMode> value);

	/** CullMode を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setCullingMode(Optional<CullMode> value);

	/** 深度テストの有無を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setDepthTestEnabled(Optional<bool> value);

	/** 深度書き込みの有無を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setDepthWriteEnabled(Optional<bool> value);

	/** @} */


    // BuiltinEffectData
    //void setTransfrom(const Matrix& value);
    void setOpacity(float value);
    void setColorScale(const Color& value);
    void setBlendColor(const Color& value);
    void setTone(const ToneF& value);

    void resetState();
    void pushState(bool reset = true);  // TODO: enum, scoped_guard
    void popState();

	//--------------------------------------------------------------------------
	/** @name drawing */
	/** @{ */

    // これは主に Post effect の実装で使用します。
    // 実際に処理が行われるのはレンダリングパイプラインの ImageEffect フェーズです。
    // 通常、drawMesh や drawSprite とは実行されるタイミングが異なるため、Post effect の実装のみを目的として使用してください。
    //void blit(AbstractMaterial* material);
    void blit(RenderTargetTexture* source, RenderTargetTexture* destination);
    void blit(RenderTargetTexture* source, RenderTargetTexture* destination, AbstractMaterial* material);

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

    // TODO: internal
    void setBaseTransfrom(const Optional<Matrix>& value);
    void setBaseBuiltinEffectData(const Optional<detail::BuiltinEffectData>& value);

LN_PROTECTED_INTERNAL_ACCESS:
	RenderingContext();
	virtual ~RenderingContext();
	void setDrawElementList(detail::DrawElementList* list);
	void resetForBeginRendering();

private:
	detail::RenderingManager* m_manager;
	Ref<detail::DrawElementListBuilder> m_builder;
};

} // namespace ln

