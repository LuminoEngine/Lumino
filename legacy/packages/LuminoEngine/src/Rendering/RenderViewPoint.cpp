#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderViewPoint.hpp>

namespace ln {

void RenderViewPoint::resetUnproject(const Size& viewPixelSize_) {
    worldMatrix = Matrix::Identity;
    viewPixelSize = viewPixelSize_;
    viewPosition = Vector3::Zero;
    viewDirection = Vector3::UnitZ;
    viewMatrix = Matrix::Identity;
    projMatrix = Matrix::Identity;
    viewProjMatrix = Matrix::Identity;
    viewFrustum = ViewFrustum();
    fovY = 1.0f;
    nearClip = 0.0f;
    farClip = 1.0f;
    dpiScale = 1.0f;
}

void RenderViewPoint::resetPerspective(
    const Vector3& viewPos,
    const Vector3& viewDir,
    float fovY,
    const Size& size,
    float n,
    float f) {
    worldMatrix = Matrix::Identity; // TODO: Lookat
    viewPixelSize = size;
    viewPosition = viewPos;
    viewDirection = viewDir;
#ifdef LN_COORD_RH
    viewMatrix = Matrix::makeLookAtRH(viewPos, viewPos + viewDir, Vector3::UnitY);
    // projMatrix = Matrix::makeOrthoRH(size.width, size.height, n, f);//Matrix::makePerspectiveFovLH(fovY, size.width / size.height, n, f);
    projMatrix = Matrix::makePerspectiveFovRH(fovY, size.width / size.height, n, f);
#else
    viewMatrix = Matrix::makeLookAtLH(viewPos, viewPos + viewDir, Vector3::UnitY);
    // projMatrix = Matrix::makeOrthoLH(size.width, size.height, n, f);//Matrix::makePerspectiveFovLH(fovY, size.width / size.height, n, f);
    projMatrix = Matrix::makePerspectiveFovLH(fovY, size.width / size.height, n, f);
#endif
    viewProjMatrix = viewMatrix * projMatrix;
    viewFrustum = ViewFrustum(viewProjMatrix);
    // zSortDistanceBase = ZSortDistanceBase::CameraScreenDistance;
    this->fovY = fovY;
    nearClip = n;
    farClip = f;
    dpiScale = 1.0f;
}

void RenderViewPoint::resetPerspective2DLH(const Vector3& viewPos, const Size& size, float n, float f) {
    worldMatrix = Matrix::Identity; // TODO: Lookat
    viewPixelSize = size;
    viewPosition = viewPos;
    viewDirection = Vector3(0, 0, -1);
    viewMatrix = Matrix::makeTranslation(viewPos);
    projMatrix = Matrix::makePerspective2DLH(size.width, size.height, n, f);
    // LH なのは godot 参考。(godot は ZIndex だが)
    // なお godot は 3D 空間は RH.

    viewProjMatrix = viewMatrix * projMatrix;
    viewFrustum = ViewFrustum(viewProjMatrix);
    this->fovY = 0.5f; // dummy
    nearClip = n;
    farClip = f;
    dpiScale = 1.0f;
}

void RenderViewPoint::resetPerspective2DRH(const Vector3& viewPos, const Size& size, float n, float f) {
    worldMatrix = Matrix::Identity; // TODO: Lookat
    viewPixelSize = size;
    viewPosition = viewPos;
    viewDirection = Vector3(0, 0, 1);
    viewMatrix = Matrix::makeTranslation(viewPos);
    projMatrix = Matrix::makePerspective2DRH(size.width, size.height, n, f);
    // RH なのは godot 参考。(godot は ZIndex だが)
    // なお godot は 3D 空間は RH.
    viewProjMatrix = viewMatrix * projMatrix;
    viewFrustum = ViewFrustum(viewProjMatrix);
    this->fovY = 0.5f; // dummy
    nearClip = n;
    farClip = f;
    dpiScale = 1.0f;
}

// 2.5D 用
void RenderViewPoint::resetPerspectiveOrthoLH(
    const Vector3& viewPos,
    const Vector3& lookPos,
    const Size& size,
    float n,
    float f) {
    worldMatrix = Matrix::Identity;
    viewPixelSize = size;
    viewPosition = viewPos;
    viewDirection = Vector3::normalize(lookPos - viewPos);
    viewMatrix = Matrix::makeLookAtLH(viewPos, lookPos, Vector3::UnitY);
    projMatrix = Matrix::makeOrthoLH(size.width, size.height, n, f);
    viewProjMatrix = viewMatrix * projMatrix;
    viewFrustum = ViewFrustum(viewProjMatrix);
    fovY = 0.5f; // dummy
    nearClip = n;
    farClip = f;
    dpiScale = 1.0f;
}

void RenderViewPoint::copyFrom(const RenderViewPoint* other) {
    LN_DCHECK(other);
    worldMatrix = other->worldMatrix;
    viewPixelSize = other->viewPixelSize;
    viewPosition = other->viewPosition;
    viewDirection = other->viewDirection;
    viewMatrix = other->viewMatrix;
    projMatrix = other->projMatrix;
    viewProjMatrix = other->viewProjMatrix;
    viewFrustum = other->viewFrustum;
    fovY = other->fovY;
    nearClip = other->nearClip;
    farClip = other->farClip;
    dpiScale = other->dpiScale;
}

void RenderViewPoint::makeCameraInfo(detail::CameraInfo* cameraInfo) const {
    LN_DCHECK(cameraInfo);
    cameraInfo->viewPixelSize = viewPixelSize;
    cameraInfo->viewPosition = viewPosition;
    cameraInfo->viewDirection = viewDirection;
    cameraInfo->viewMatrix = viewMatrix;
    cameraInfo->projMatrix = projMatrix;
    cameraInfo->viewProjMatrix = viewProjMatrix;
    cameraInfo->viewFrustum = viewFrustum;
    cameraInfo->nearClip = nearClip;
    cameraInfo->farClip = farClip;
    cameraInfo->dpiScale = dpiScale;
}


} // namespace ln
