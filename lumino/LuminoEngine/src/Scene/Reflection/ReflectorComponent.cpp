
#include "../Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Scene/Reflection/OffscreenWorldRenderView.hpp>
#include <LuminoEngine/Scene/Reflection/ReflectorComponent.hpp>

namespace ln {

//==============================================================================
// ReflectorComponent

LN_OBJECT_IMPLEMENT(ReflectorComponent, VisualComponent) {}

ReflectorComponent::ReflectorComponent()
    : m_material(nullptr)
    , m_size(10.0f, 10.0f)
{
}

bool ReflectorComponent::init()
{
	if (!VisualComponent::init()) return false;

    m_renderTarget = makeObject<RenderTargetTexture>(512, 512, TextureFormat::RGBA8, false, false);

    m_offscreenRenderView = makeObject<OffscreenWorldRenderView>();
    m_offscreenRenderView->setRenderTarget(m_renderTarget);

    auto shader = Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Reflector.fx"));
    m_material = makeObject<Material>();
    m_material->setShader(shader);
    //m_material->setColor(Color::Red);
    m_material->setMainTexture(m_renderTarget);

	return true;
}

void ReflectorComponent::onPrepareRender(RenderingContext* context)
{
    // デフォルトの描画モデルとして、Y+ 方向を向く Plane を使っているので、それに合わせる。
    // もし他のジオメトリを使いたい場合、デフォルト姿勢の時の鏡面の法線を指定する。
    const Vector3 localNormal = Vector3(0, 1, 0);

    const auto* viewPoint = context->viewPoint();
    //auto& virtualCamera = m_offscreenRenderView->m_cameraInfo;
    detail::CameraInfo virtualCamera;
    {

        WorldObject* scope = worldObject();


        Vector3 reflectorWorldPosition = scope->position(); //reflectorWorldPosition.setFromMatrixPosition(scope.matrixWorld);
        Vector3 cameraWorldPosition = viewPoint->viewPosition;  //cameraWorldPosition.setFromMatrixPosition(camera.matrixWorld);

        Matrix rotationMatrix = Matrix::extractRotation(scope->worldMatrix());
        //rotationMatrix.transpose();

        Vector3 normal = Vector3::transformCoord(localNormal, rotationMatrix); //  normal.applyMatrix4(rotationMatrix);
       

        Vector3 view;
        view = reflectorWorldPosition - cameraWorldPosition;// camera->scope へ   //view.subVectors(reflectorWorldPosition, cameraWorldPosition);

        // Avoid rendering when reflector is facing away

        if (Vector3::dot(view, normal) >= 0)
            return; // TODO: onRender も行わないようにしたい

        // 反射させて反転。それに reflectorWorldPosition を足すと、
        // view は鏡世界の中の視点位置となる。
        view = -Vector3::reflect(view, normal); // view.reflect(normal).negate();
        view += reflectorWorldPosition;

        //Vector3 lookAtPosition = Vector3::transformCoord(Vector3(0, 0, -1), rotationMatrix) + cameraWorldPosition;
        Vector3 lookAtPosition = viewPoint->viewPosition + viewPoint->viewDirection;
        //lookAtPosition.set(0, 0, -1);
        //lookAtPosition.applyMatrix4(rotationMatrix);
        //lookAtPosition.add(cameraWorldPosition);
        Vector3 target = reflectorWorldPosition - lookAtPosition;// cameraLookat->scope へ 
        target = -Vector3::reflect(target, normal);
        target += reflectorWorldPosition;
        //target.subVectors(reflectorWorldPosition, lookAtPosition);
        //target.reflect(normal).negate();
        //target.add(reflectorWorldPosition);

        // 上方向が反転するので、左右逆のイメージができる。
        // 面反転する必要はない。
        Vector3 up = Vector3::transformCoord(Vector3(0, 1, 0), rotationMatrix);
        up = Vector3::reflect(up, normal);
        //Vector3 up = Vector3(0, 1, 0);

        virtualCamera.viewPosition = view;
        virtualCamera.viewDirection = Vector3::normalize(target - view);
#ifdef LN_COORD_RH
        virtualCamera.viewMatrix = Matrix::makeLookAtRH(view, target, up);
#else
        virtualCamera.viewMatrix = Matrix::makeLookAtLH(view, target, up);
#endif
        //virtualCamera.up.set(0, 1, 0);
        //virtualCamera.up.applyMatrix4(rotationMatrix);
        //virtualCamera.up.reflect(normal);
        //virtualCamera.lookAt(target);

        //virtualCamera.far = camera.far; // Used in WebGLBackground

        //virtualCamera.updateMatrixWorld();
        //virtualCamera.projectionMatrix.copy(camera.projectionMatrix);

        {
            virtualCamera.viewPixelSize = Size(m_renderTarget->width(), m_renderTarget->height());
            //virtualCamera.viewPosition = viewPoint->viewPosition;
            //virtualCamera.viewDirection = viewPoint->viewDirection;
            //virtualCamera.viewMatrix = viewPoint->viewMatrix;
            virtualCamera.projMatrix = viewPoint->projMatrix;
            virtualCamera.viewProjMatrix = Matrix::multiply(virtualCamera.viewMatrix, virtualCamera.projMatrix);
            virtualCamera.viewFrustum = ViewFrustum(viewPoint->viewProjMatrix);
            virtualCamera.nearClip = viewPoint->nearClip;
            virtualCamera.farClip = viewPoint->farClip;
        }

        m_offscreenRenderView->makeViewProjections(virtualCamera, 1.0f);    // TODO: dpiScale



        Matrix virtualCameraWorldMatrix = Matrix::makeAffineLookAtLH(
            virtualCamera.viewPosition,
            virtualCamera.viewPosition + virtualCamera.viewDirection,
            up);
        virtualCameraWorldMatrix.translate(virtualCamera.viewPosition);

        // Update the texture matrix
        //Matrix textureMatrix(
        //    0.5, 0.0, 0.0, 0.5,
        //    0.0, 0.5, 0.0, 0.5,
        //    0.0, 0.0, 0.5, 0.5,
        //    0.0, 0.0, 0.0, 1.0
        //);
        //textureMatrix.multiply(virtualCamera.projectionMatrix);
        //textureMatrix.multiply(virtualCamera.matrixWorldInverse);
        //textureMatrix.multiply(scope.matrixWorld);
        //↑逆順注意。
        // まず scope.matrixWorld で普通に座標変換。
        // virtualCamera.matrixWorldInverse でカメラのローカル空間に変換 (普通に viewMatrix?)
        // 最後に proj 変換。
#if 0
        Matrix textureMatrix(
            0.5, 0.0, 0.0, 0.5,
            0.0, 0.5, 0.0, 0.5,
            0.0, 0.0, 0.5, 0.5,
            0.0, 0.0, 0.0, 1.0
        );
        textureMatrix = Matrix::multiply(virtualCamera.projMatrix, textureMatrix);
        textureMatrix = Matrix::multiply(Matrix::makeInverse(virtualCameraWorldMatrix), textureMatrix);
        textureMatrix = Matrix::multiply(scope->worldMatrix(), textureMatrix);

#else
        //Matrix textureMatrix(
        //    0.5, 0.0, 0.0, 0.0,
        //    0.0, 0.5, 0.0, 0.0,
        //    0.0, 0.0, 0.5, 0.0,
        //    0.5, 0.5, 0.5, 1.0
        //);
        //textureMatrix = Matrix::multiply(textureMatrix, scope->worldMatrix());
        //textureMatrix = Matrix::multiply(textureMatrix, Matrix::makeInverse(virtualCameraWorldMatrix));
        //textureMatrix = Matrix::multiply(textureMatrix, virtualCamera.projMatrix);
        //auto virtualCameraWorldMatrixInverse = Matrix::makeInverse(virtualCameraWorldMatrix);
        //Matrix textureMatrix = scope->worldMatrix() * virtualCameraWorldMatrixInverse * virtualCamera.projMatrix
        //* Matrix (
        //        0.5, 0.0, 0.0, 0.0,
        //        0.0, 0.5, 0.0, 0.0,
        //        0.0, 0.0, 0.5, 0.0,
        //        0.5, 0.5, 0.5, 1.0
        //    );
        Matrix textureMatrix = scope->worldMatrix() * virtualCamera.viewMatrix * virtualCamera.projMatrix;
#endif

        m_material->setMatrix(_TT("_TextureMatrix"), textureMatrix);
    }

    context->world->enqueueOffscreenRenderView(m_offscreenRenderView);
}

void ReflectorComponent::onRender(RenderingContext* context)
{
    //if (context->currentRenderView == m_offscreenRenderView) {
    if (dynamic_cast<OffscreenWorldRenderView*>(context->currentRenderView)) {
    }
    else {

        context->setMaterial(m_material);

        auto uv1 = Vector2(1.0, 1.0);

        context->drawPlane(m_size.x, m_size.y, Vector2::Zero, uv1, Color::White);
    }
}

} // namespace ln

