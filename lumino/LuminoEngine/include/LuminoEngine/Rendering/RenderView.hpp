#pragma once
#include <LuminoGraphics/Common.hpp>
#include <LuminoGraphics/Texture.hpp>
#include <LuminoGraphics/DepthBuffer.hpp>
#include <LuminoGraphics/ShaderInterfaceFramework.hpp>
#include "../UI/UIEvents.hpp"
#include "Common.hpp"

namespace ln {
class UIEventArgs;
class UIStyleContext;
class UILayoutContext;
class UIViewport;
namespace detail {
class RenderingManager;
class FrameBufferCache;
class DrawElementList;
class SceneRenderer;
class UIStyleInstance;


}

// RenderView は別の RenderingContext の描画コマンド構築中に、レンダリングターゲットを生成する目的で render を実行することがある。
// そのため render の実装は RenderingContext や GraphicsCommandList の状態に依存しないようにしなければならない。
// TODO: onUpdateUILayout() など、少し UI に依存してきている。UI モジュールにもっていくのが正しい？

/**
 * RenderView
 */
LN_CLASS()
class RenderView
	: public Object
{
	LN_OBJECT;
public:
	RenderView();
	virtual ~RenderView();
	void init();

	UIViewport* viewport() const { return m_parentViewport; }

    virtual void render(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) = 0;

	// TODO: internal
	//detail::CameraInfo mainCameraInfo;
	const detail::CameraInfo& viewProjection(RenderPart kind) const { return m_viewProjections[static_cast<int>(kind)]; }
	void makeViewProjections(const detail::CameraInfo& base, float dpiScale);
	Vector3 transformProjection(const Vector3& pos, RenderPart from, RenderPart to) const;

	SceneClearMode clearMode() const { return m_clearMode; }
	void setClearMode(SceneClearMode value) { m_clearMode = value; }

	const Color& backgroundColor() const { return m_backgroundColor; }
	void setBackgroundColor(const Color& value) { m_backgroundColor = value; }

    // フレーム開始時に決定 (unit:dp)
	//const Point& actualScreenOffset() const { return m_actualScreenOffset; }
    const Size& actualSize() const { return m_actualSize; }

    //LN_METHOD(Event)
	Ref<EventConnection> connectOnUIEvent(Ref<UIGeneralEventHandler> handler);

	detail::SceneRenderingPipeline* m_sceneRenderingPipeline = nullptr;	// for PostEffect
	RenderTargetTexture* gbuffer(GBuffer kind) const;


public: // TODO: protected
	void updateFrame(float elapsedSeconds);
	void updateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle);
	void updateUILayout(UILayoutContext* layoutContext);
	virtual void onUpdateFrame(float elapsedSeconds);
	virtual void onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle);
	virtual void onUpdateUILayout(UILayoutContext* layoutContext);
    virtual UIElement* onLookupMouseHoverElement(const Point& framewindowClientPoint);
    virtual void onRoutedEvent(UIEventArgs* e);

LN_INTERNAL_ACCESS:
	//void setActualScreenOffset(const Point& offset) { m_actualScreenOffset = offset; }
	void setActualSize(const Size& size);

private:
	detail::RenderingManager* m_manager;
	UIViewport* m_parentViewport = nullptr;

	SceneClearMode m_clearMode;
	Color m_backgroundColor;
	Point m_actualScreenOffset;
    Size m_actualSize;

	std::array<detail::CameraInfo, (int)RenderPart::_Count> m_viewProjections;

    Event<UIGeneralEventHandler> m_onUIEvent;

	friend class UIViewport;
};


// RenderingContext を使った描画の起点となった視点情報。
// 特に Partcle や Tilemap で使用する。これらは Camera が視点となるが、onRender() から Camera までの参照はかなり遠い。
// また UI でも使うことがあるため、単に Camera ではなく、もう一段抽象化したデータ構造を用意しておく。(UI では Camera という概念はイメージしづらい)
class RenderViewPoint
    : public Object
{
public:
    Matrix worldMatrix;
    Size		viewPixelSize;
    Vector3		viewPosition;
    Vector3		viewDirection;
    Matrix		viewMatrix;
    Matrix		projMatrix;
    Matrix		viewProjMatrix;
    ViewFrustum	viewFrustum;

    float fovY = 1.0f;
    float		nearClip = 0;
    float		farClip = 0;
	float		dpiScale = 1.0;
};

} // namespace ln

