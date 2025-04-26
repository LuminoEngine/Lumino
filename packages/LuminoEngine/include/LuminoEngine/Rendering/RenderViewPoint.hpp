#pragma once
#include "Common.hpp"

namespace ln {

// RenderingContext を使った描画の起点となった視点情報。
// 特に Partcle や Tilemap で使用する。これらは Camera が視点となるが、onRender() から Camera までの参照はかなり遠い。
// また UI でも使うことがあるため、単に Camera ではなく、もう一段抽象化したデータ構造を用意しておく。(UI では Camera という概念はイメージしづらい)
// Mirror を作るときも、わざわざ CameraObject をひとつ作るのはおおげさ。
// というより、SceneGraph のモジュールの一部である Camera というクラスに依存したくない。
//
// RenderView と 1:1 ではない点に注意。
// RenderView は RenderViewPoint を持つが、RenderViewPoint は 後段の RenderPart でも複数個のインスタンスを使う。
// そちらを CameraInfo にするのもアリだが、そうすると他言語に公開ができなくなる。
class RenderViewPoint : public Object {
public:
    Matrix worldMatrix;
    Size viewPixelSize;
    Vector3 viewPosition;
    Vector3 viewDirection;
    Matrix viewMatrix;
    Matrix projMatrix;
    Matrix viewProjMatrix;
    ViewFrustum viewFrustum;

    float fovY = 1.0f;
    float nearClip = 0;
    float farClip = 0;
    float dpiScale = 1.0;

    void resetUnproject(const Size& viewPixelSize);
    void
    resetPerspective(const Vector3& viewPos, const Vector3& viewDir, float fovY, const Size& size, float n, float f);
    void resetPerspectiveOrthoLH(const Vector3& viewPos, const Vector3& lookPos, const Size& size, float n, float f);
    void resetPerspective2DLH(const Vector3& viewPos, const Size& size, float n, float f);
    void resetPerspective2DRH(const Vector3& viewPos, const Size& size, float n, float f);
    void copyFrom(const RenderViewPoint* other);
    void makeCameraInfo(detail::CameraInfo* cameraInfo) const;
};

} // namespace ln
