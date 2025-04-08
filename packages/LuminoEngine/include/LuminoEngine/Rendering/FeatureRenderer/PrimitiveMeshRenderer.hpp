#pragma once
#include "../Common.hpp"

namespace ln {

class PrimitiveMeshRenderer : public Object {
public:

    void begin(RenderingContext_deprecated* renderingContext, Material* material);
    void begin(CommandList* commandList, Material* material);
    void end();

    void drawPlane(float width, float depth, const Color& color = Color::White);
    void drawPlane(float width, float depth, const Vector2& uv1, const Vector2& uv2, const Color& color = Color::White);
    void drawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform = Matrix());
    void drawBox(const Box& box, const Color& color = Color::White, const Matrix& localTransform = Matrix());


private:
    PrimitiveMeshRenderer();
    Result_deprecated<> init();

    CommandList* m_commandList;
    Material* m_material;
    Array<detail::MeshGenerater*> m_generators;

    friend class detail::RenderingManager;
};

} // namespace ln

