#pragma once
#include "../Graphics/Common.hpp"
#include "../Graphics/Texture.hpp"
#include "../Graphics/DepthBuffer.hpp"
#include "../Shader/ShaderInterfaceFramework.hpp"
#include "../UI/UIEvents.hpp"
#include "Common.hpp"

namespace ln {
class GraphicsContext;
class UIEventArgs;
class UIStyleContext;
namespace detail {
class RenderingManager;
class FrameBufferCache;
class DrawElementList;
class DrawElementListCollector;
class SceneRenderer;
class UIStyleInstance;
}

// RenderView は別の RenderingContext の描画コマンド構築中に、レンダリングターゲットを生成する目的で render を実行することがある。
// そのため render の実装は RenderingContext や GraphicsContext の状態に依存しないようにしなければならない。
// TODO: onUpdateUILayout() など、少し UI に依存してきている。UI モジュールにもっていくのが正しい？
class RenderView
	: public Object
{
public:
	RenderView();
	virtual ~RenderView();
	void init();

	// World の描画などでは RenderView のインスタンスを持ち回り、描画してほしい人が Manager を ここに add していく。
	void clearDrawElementListManagers();
	void addDrawElementListManager(detail::DrawElementListCollector* elementListManager);

    // index:0 にセットされている RT と depthBuffer へ描画
    virtual void render(GraphicsContext* graphicsContext) = 0;

	// TODO: internal
	//detail::CameraInfo mainCameraInfo;

	RenderViewClearMode clearMode() const { return m_clearMode; }
	void setClearMode(RenderViewClearMode value) { m_clearMode = value; }

	const Color& backgroundColor() const { return m_backgroundColor; }
	void setBackgroundColor(const Color& value) { m_backgroundColor = value; }

    // フレーム開始時に決定 (unit:dp)
	const Point& actualScreenOffset() const { return m_actualScreenOffset; }
    const Size& actualSize() const { return m_actualSize; }

    LN_METHOD(Event)
    EventConnection connectOnUIEvent(UIEventHandler handler);

public: // TODO: protected
	void updateFrame(float elapsedSeconds);
	void updateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* parentFinalStyle);
	void updateUILayout(const Rect& parentFinalGlobalRect);
	virtual void onUpdateFrame(float elapsedSeconds);
	virtual void onUpdateUIStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle);
	virtual void onUpdateUILayout(const Rect& finalGlobalRect);
    virtual UIElement* onLookupMouseHoverElement(const Point& framewindowClientPoint);
    virtual void onRoutedEvent(UIEventArgs* e);

LN_INTERNAL_ACCESS:
	const List<detail::DrawElementListCollector*>& elementListManagers() const { return m_elementListManagers; }
	void setActualScreenOffset(const Point& offset) { m_actualScreenOffset = offset; }
    void setActualSize(const Size& size) { m_actualSize = size; }

private:
	detail::RenderingManager* m_manager;

    // TODO: これ List じゃなくていい気がする、というか、List じゃないほうが安全
	List<detail::DrawElementListCollector*> m_elementListManagers;

	RenderViewClearMode m_clearMode;
	Color m_backgroundColor;
	Point m_actualScreenOffset;
    Size m_actualSize;

    Event<UIEventHandler> m_onUIEvent;
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

    float		nearClip = 0;
    float		farClip = 0;
	float		dpiScale = 1.0;
};

} // namespace ln

