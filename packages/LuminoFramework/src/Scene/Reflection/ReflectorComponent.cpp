
#include "../Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/PrimitiveMeshRenderer.hpp>
#include <LuminoFramework/Scene/World.hpp>
#include <LuminoFramework/Scene/WorldObject.hpp>
#include <LuminoFramework/Scene/Reflection/OffscreenWorldRenderView.hpp>
#include <LuminoFramework/Scene/Reflection/ReflectorComponent.hpp>

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

    m_renderTarget = makeObject_deprecated<RenderTargetTexture>(512, 512, TextureFormat::RGBA8, false, false);

    m_offscreenRenderView = makeObject_deprecated<OffscreenWorldRenderView>();
    m_offscreenRenderView->setRenderTarget(m_renderTarget);

    auto shader = Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/Rendering/Resource/Reflector.fx"));
    m_material = makeObject_deprecated<Material>();
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
    //detail::CameraInfo virtualCamera;
    RenderViewPoint* virtualViewPoint = m_offscreenRenderView->m_viewPoint;
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

        virtualViewPoint->viewPosition = view;
        virtualViewPoint->viewDirection = Vector3::normalize(target - view);
#ifdef LN_COORD_RH
        virtualViewPoint->viewMatrix = Matrix::makeLookAtRH(view, target, up);
#else
        virtualViewPoint->viewMatrix = Matrix::makeLookAtLH(view, target, up);
#endif
        //virtualViewPoint->up.set(0, 1, 0);
        //virtualViewPoint->up.applyMatrix4(rotationMatrix);
        //virtualViewPoint->up.reflect(normal);
        //virtualViewPoint->lookAt(target);

        //virtualViewPoint->far = camera.far; // Used in WebGLBackground

        //virtualViewPoint->updateMatrixWorld();
        //virtualViewPoint->projectionMatrix.copy(camera.projectionMatrix);

        {
            virtualViewPoint->viewPixelSize = Size(m_renderTarget->width(), m_renderTarget->height());
            //virtualViewPoint->viewPosition = viewPoint->viewPosition;
            //virtualViewPoint->viewDirection = viewPoint->viewDirection;
            //virtualViewPoint->viewMatrix = viewPoint->viewMatrix;
            virtualViewPoint->projMatrix = viewPoint->projMatrix;
            virtualViewPoint->viewProjMatrix = Matrix::multiply(virtualViewPoint->viewMatrix, virtualViewPoint->projMatrix);
            virtualViewPoint->viewFrustum = ViewFrustum(viewPoint->viewProjMatrix);
            virtualViewPoint->fovY = viewPoint->fovY;
            virtualViewPoint->nearClip = viewPoint->nearClip;
            virtualViewPoint->farClip = viewPoint->farClip;
            virtualViewPoint->dpiScale = 1.0f; // TODO: dpiScale
        }

        // TODO: RenderView::onUpdateViewPoint で実装
        LN_NOTIMPLEMENTED();
        //m_offscreenRenderView->makeViewProjections(virtualViewPoint);



        Matrix virtualCameraWorldMatrix = Matrix::makeAffineLookAtLH(
            virtualViewPoint->viewPosition,
            virtualViewPoint->viewPosition + virtualViewPoint->viewDirection,
            up);
        virtualCameraWorldMatrix.translate(virtualViewPoint->viewPosition);

        // Update the texture matrix
        //Matrix textureMatrix(
        //    0.5, 0.0, 0.0, 0.5,
        //    0.0, 0.5, 0.0, 0.5,
        //    0.0, 0.0, 0.5, 0.5,
        //    0.0, 0.0, 0.0, 1.0
        //);
        //textureMatrix.multiply(virtualViewPoint->projectionMatrix);
        //textureMatrix.multiply(virtualViewPoint->matrixWorldInverse);
        //textureMatrix.multiply(scope.matrixWorld);
        //↑逆順注意。
        // まず scope.matrixWorld で普通に座標変換。
        // virtualViewPoint->matrixWorldInverse でカメラのローカル空間に変換 (普通に viewMatrix?)
        // 最後に proj 変換。
#if 0
        Matrix textureMatrix(
            0.5, 0.0, 0.0, 0.5,
            0.0, 0.5, 0.0, 0.5,
            0.0, 0.0, 0.5, 0.5,
            0.0, 0.0, 0.0, 1.0
        );
        textureMatrix = Matrix::multiply(virtualViewPoint->projMatrix, textureMatrix);
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
        //textureMatrix = Matrix::multiply(textureMatrix, virtualViewPoint->projMatrix);
        //auto virtualCameraWorldMatrixInverse = Matrix::makeInverse(virtualCameraWorldMatrix);
        //Matrix textureMatrix = scope->worldMatrix() * virtualCameraWorldMatrixInverse * virtualViewPoint->projMatrix
        //* Matrix (
        //        0.5, 0.0, 0.0, 0.0,
        //        0.0, 0.5, 0.0, 0.0,
        //        0.0, 0.0, 0.5, 0.0,
        //        0.5, 0.5, 0.5, 1.0
        //    );
        Matrix textureMatrix = scope->worldMatrix() * virtualViewPoint->viewMatrix * virtualViewPoint->projMatrix;
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
        const auto uv1 = Vector2(1.0, 1.0);
        PrimitiveMeshRenderer::drawPlane(context, m_material, m_size.x, m_size.y, Vector2::Zero, uv1, Color::White);
    }
}

} // namespace ln

