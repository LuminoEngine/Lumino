#pragma once
#include <LuminoEngine/Common.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/GPU/DepthBuffer.hpp>
#include <LuminoEngine/GPU/ShaderInterfaceFramework.hpp>
#include "Common.hpp"

namespace ln {

/**
 * RenderView
 */
LN_CLASS()
class RenderView : public Object {
    LN_OBJECT;
public:
    RenderView();
    virtual ~RenderView();
    void init(RenderingContext* renderingContext = nullptr);


    // TODO: internal
    // detail::CameraInfo mainCameraInfo;
    const RenderViewPoint* viewProjection(RenderPart kind) const { return m_viewProjections[static_cast<int>(kind)]; }
    Vector3 transformProjection(const Vector3& pos, RenderPart from, RenderPart to) const;

    SceneClearMode clearMode() const { return m_clearMode; }
    void setClearMode(SceneClearMode value) { m_clearMode = value; }

    const Color& backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const Color& value) { m_backgroundColor = value; }

    RenderingPipeline* renderingPipeline() const;
    void setRenderingPipeline(RenderingPipeline* value);

    // フレーム開始時に決定 (unit:dp)
    // const Point& actualScreenOffset() const { return m_actualScreenOffset; }
    const Size& actualSize() const { return m_actualSize; }

    //SceneRenderingPipeline* m_sceneRenderingPipeline = nullptr; // for PostEffect
    RenderTargetTexture* builtinRenderTexture(BuiltinRenderTextureType type) const { return m_builtinRenderTextures[(size_t)type]; }
    void setBuiltinRenderTexture(BuiltinRenderTextureType type, RenderTargetTexture* value) { m_builtinRenderTextures[(size_t)type] = value; } // TODO: internal
    void clearBuiltinRenderTextures(); // TODO: internal

    RenderingContext* getContext() const;

    virtual void render(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget) { LN_UNREACHABLE(); } //= 0;
    void renderPipeline(GraphicsCommandList* graphicsContext, /*RenderingContext* renderingContext, */RenderTargetTexture* renderTarget);


public: // TODO: intenral
    void updateFrame(float elapsedSeconds);
    RenderingContext* beginRenderPipeline(GraphicsCommandList* graphicsContext, RenderTargetTexture* renderTarget);
    void endRenderPipeline();

protected:
    const Ref<RenderViewPoint>& viewPoint() const { return m_viewPoint; }

    virtual void onUpdateFrame(float elapsedSeconds);

    // ViewPoint のインスタンスは RenderView で持つが、それをどのように更新するかはサブクラスに任せたい。
    // 例えば WorldRenderView であれば Camera と同期したり、UIRenderView であれば 2D 用の固定となる。
    // そのため ViewPoint のインスタンスは RenderView からは公開しない。
    virtual void onUpdateViewPoint(RenderViewPoint* viewPoint, RenderTargetTexture* renderTarget) = 0;
    
    virtual void onRender(GraphicsCommandList* graphicsContext, RenderingContext* renderingContext, RenderTargetTexture* renderTarget) = 0;

public: // TODO: internal
    // void setActualScreenOffset(const Point& offset) { m_actualScreenOffset = offset; }
    void setActualSize(const Size& size);

private:
    void makeViewProjections(const RenderViewPoint* base);

    detail::RenderingManager* m_manager;
    SceneClearMode m_clearMode;
    Color m_backgroundColor;
    Point m_actualScreenOffset;
    Size m_actualSize;
    Ref<RenderViewPoint> m_viewPoint;
    Ref<RenderingContext> m_renderingContext;
    Ref<RenderingPipeline> m_renderingPipeline;

    GraphicsCommandList* m_currentCommandList;
    RenderTargetTexture* m_currentRenderTarget;

    std::array<Ref<RenderViewPoint>, (size_t)RenderPart::_Count> m_viewProjections;
    std::array<RenderTargetTexture*, (size_t)BuiltinRenderTextureType::_Count> m_builtinRenderTextures;

};

} // namespace ln
