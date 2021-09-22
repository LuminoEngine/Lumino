#pragma once
#include <LuminoEngine/Graphics/RenderState.hpp>
#include "Common.hpp"
#include "../Font/Common.hpp"
#include "../Graphics/ColorStructs.hpp"

namespace ln {




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

	void setTransfrom(const Matrix& value);

	//--------------------------------------------------------------------------
	/** @name render status */
	/** @{ */

    // このあたりの設定は Material の設定をオーバーライドする。
    // 主な用途としては、Mesh のように複数のマテリアルを含むオブジェクトを描画する際、
    // サブセット全体の設定をオーバーライドするようなケースを想定している。
    // (特に、ヒットエフェクトのための BlendColor や、エディタでの両面表示設定など、Material 自体のパラメータを変更したくない場合)

	/** BlendMode を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setBlendMode(Optional<BlendMode> value);

    /** ShadingModel を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
    void setShadingModel(Optional<ShadingModel> value);

	/** CullMode を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setCullingMode(Optional<CullMode> value);

	/** 深度テストの有無を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setDepthTestEnabled(Optional<bool> value);

	/** 深度書き込みの有無を設定します。 シーンの既定値を使用する場合は nullptr を指定します。 */
	void setDepthWriteEnabled(Optional<bool> value);

	/** @} */

    // デフォルトは nullptr。この場合、既定のマテリアルが使用される。
    void setMaterial(Material* material);



    void setRenderPhase(RenderPart value);

    // BuiltinEffectData
    //void setTransfrom(const Matrix& value);
    void setOpacity(float value);
    void setColorScale(const Color& value);
    void setBlendColor(const Color& value);
    void setTone(const ColorTone& value);

	// Text
	void setFont(Font* value);
	void setTextColor(const Color& value);

    void resetState();
    void pushState(bool reset = true);  // TODO: enum, scoped_guard
    void popState();

	//--------------------------------------------------------------------------
	/** @name drawing */
	/** @{ */

	// TODO: deprecated. シャドウパスなど、マルチパスの SceneRenderer を実行するとき、メイン以外のパスでもクリアしてしまうので、対策するか、いっそ無くす。
	void clear(Flags<ClearFlags> flags = ClearFlags::All, const Color& color = Color::Zero, float z = 1.0f, uint8_t stencil = 0x00);

    void drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);
    void drawPlane(float width, float depth, const Color& color = Color::White);
	void drawPlane(float width, float depth, const Vector2& uv1, const Vector2& uv2, const Color& color = Color::White);
    void drawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform = Matrix());
	void drawBox(const Box& box, const Color& color = Color::White, const Matrix& localTransform = Matrix());

    /** 四辺の位置が -1.0~1.0 の矩形を描画します。座標変換行列に単位行列を使用することで、スクリーン全体を覆う矩形を描画することができます。 */
    void drawScreenRectangle();

    // これは主に Post effect の実装で使用します。
    // 実際に処理が行われるのはレンダリングパイプラインの PostEffect フェーズです。
    // 通常、drawMesh や drawSprite とは実行されるタイミングが異なるため、Post effect の実装のみを目的として使用してください。
    //void blit(Material* material);
    //void blit(RenderTargetTexture* source, RenderTargetTexture* destination);
    //void blit(RenderTargetTexture* source, RenderTargetTexture* destination, Material* material);
	void blit(Material* source, RenderTargetTexture* destination, RenderPart phase = RenderPart::PostEffect);

	/** スプライトを描画します。 */
	void drawSprite(
		const Matrix& transform,
		const Size& size,
		const Vector2& anchor,
		const Rect& srcRect,
		const Color& color,
		SpriteBaseDirection baseDirection,
		BillboardType billboardType,
        const Flags<detail::SpriteFlipFlags>& flipFlags,
		Material* material);

	// TODO: GraphicsのRenderPass内の map 禁止に伴い。Rendering で直接 VertexBuffer や IndexBuffer を扱うことを禁止したほうがいいかも。
	// 代わりに、Mesh を使う。
	// VertexBuffer::map の意味が直接転送に変わるので、(mapImmediatly() とかにしたい) まずこれを使うわけにはいかない。
	// そうすると GraphicsContext::map を使ってコマンドバッファに乗せて転送することになるけど、GraphicsContext は RenderingContext の内側に隠蔽されているので
	// 直接触ることはできない。（やってしまうと、コマンド順序が間違う）
	void drawPrimitive(VertexLayout* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveTopology topology, int startVertex, int primitiveCount);

    void drawMesh(MeshResource* meshResource, int sectionIndex);
	//void drawMesh(MeshContainer* meshContainer, int sectionIndex);
    void drawMesh(MeshPrimitive* mesh, int sectionIndex);
	void drawSkinnedMesh(MeshPrimitive* mesh, int sectionIndex, detail::SkeletonInstance* skeleton, detail::MorphInstance* morph);

	void drawMeshInstanced(InstancedMeshList* list);

	void drawTextSprite(const StringRef& text, const Color& color, const Vector2& anchor, SpriteBaseDirection baseDirection, detail::FontRequester* font);


    // font が nullptr の場合は defaultFont
    void drawText(const StringRef& text, const Rect& area = Rect(), TextAlignment alignment = TextAlignment::Forward/*, TextCrossAlignment crossAlignment = TextCrossAlignment::Forward*//*, const Color& color, Font* font = nullptr*/);
	void drawChar(uint32_t codePoint, const Color& color, Font* font = nullptr, const Matrix& transform = Matrix::Identity);

	void drawFlexGlyphRun(detail::FlexGlyphRun* glyphRun);

    void invokeExtensionRendering(INativeGraphicsExtension* extension);


	// デバッグ2D用 AA無しプリミティブ

	// 線分または塗りつぶしで描かれる正多角形。
	// XY 平面上に、Z-正面で作成する。
	void drawRegularPolygonPrimitive(int vertexCount, float radius, const Color& color, bool fill, const Matrix& localTransform = Matrix());


	/** @} */

	//--------------------------------------------------------------------------
	/** @name commands */
	/** @{ */



	CanvasContext* beginPath();
	void endPath();

	/** @} */

	//--------------------------------------------------------------------------
	/** @name light */
	/** @{ */

	void addAmbientLight(const Color& color, float intensity);

	void addHemisphereLight(const Color& skyColor, const Color& groundColor, float intensity);

	void addEnvironmentLightInfo(const Color& color, const Color& ambientColor, const Color& skyColor, const Color& groundColor, float intensity, const Vector3& direction, bool mainLight, float shadowCameraZFar, float shadowLightZFar);

	void addDirectionalLight(const Color& color, float intensity, const Vector3& direction, bool mainLight, float shadowCameraZFar, float shadowLightZFar);

	void addPointLight(const Color& color, float intensity, const Vector3& position, float range, float attenuation);

	void addSpotLight(const Color& color, float intensity, const Vector3& position, const Vector3& direction, float range, float attenuation, float spotAngle, float spotPenumbra);

	/** @} */



	/** 指定した文字列を描画する際のサイズを計算します。(unit: dp) */
	Size measureTextSize(Font* font, const StringRef& text) const;
	Size measureTextSize(Font* font, uint32_t codePoint) const;



	CommandList* getCommandList(RenderPart index);



	World* world = nullptr;


    // TODO: internal
    RenderViewPoint* viewPoint() const;
    void setViewPoint(RenderViewPoint* value);
    void setBaseTransfrom(const Optional<Matrix>& value);
	const Matrix& baseTransform() const;
	void setRenderPriority(int value);
    void setBaseBuiltinEffectData(const Optional<detail::BuiltinEffectData>& value);
   	void setAdditionalElementFlags(detail::RenderDrawElementTypeFlags value);
	void setObjectId(int value);

	const Ref<detail::DrawElementListBuilder>& builder() const;
	const Ref<CommandList>& commandList() const { return m_commandList; }
	const Ref<detail::CommandListServer>& commandListServer() const { return m_listServer; }
	
	RenderView* baseRenderView = nullptr;	// for PostEffect
    GraphicsContext* m_frameWindowRenderingGraphicsContext = nullptr;
	//detail::RenderDrawElement* lastRenderDrawElement() const;
	void collectPostEffect(PostEffect* effect) { m_imageEffects.add(effect); }
	const List<PostEffect*>& imageEffects() const { return m_imageEffects; }
	void clearPostEffects() { m_imageEffects.clear(); }
	//detail::SceneRenderingPipeline* m_sceneRenderingPipeline = nullptr;	// for PostEffect
	//RenderTargetTexture* gbuffer(GBuffer kind) const;
	RenderView* currentRenderView = nullptr;	// Offscreen の場合はそれ

	void addDynamicLightInfo(const detail::DynamicLightInfo& info) { return m_dynamicLightInfoList.add(info); }

	const List<detail::DynamicLightInfo>& dynamicLightInfoList() const { return m_dynamicLightInfoList; }

LN_PROTECTED_INTERNAL_ACCESS:
	RenderingContext();
	void resetForBeginRendering();

protected:  // TODO:
	detail::RenderingManager* m_manager;
	List<PostEffect*> m_imageEffects;
	Ref<CommandList> m_commandList;
	Ref<detail::CommandListServer> m_listServer;

private:
	Ref<CanvasContext> m_pathContext;
	bool m_pathBegan;
	List<detail::DynamicLightInfo> m_dynamicLightInfoList;
};

} // namespace ln

