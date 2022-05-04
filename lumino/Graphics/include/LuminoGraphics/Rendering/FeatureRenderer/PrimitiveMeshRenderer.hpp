#pragma once
#include "../Common.hpp"

namespace ln {

class PrimitiveMeshRenderer : public Object {
public:
    static PrimitiveMeshRenderer* get();

    void begin(RenderingContext* renderingContext, Material* material);
    void begin(CommandList* commandList, Material* material);
    void end();

    void drawPlane(float width, float depth, const Color& color = Color::White);
    void drawPlane(float width, float depth, const Vector2& uv1, const Vector2& uv2, const Color& color = Color::White);
    void drawSphere(float radius, int slices, int stacks, const Color& color, const Matrix& localTransform = Matrix());
    void drawBox(const Box& box, const Color& color = Color::White, const Matrix& localTransform = Matrix());

    
    static void drawPlane(RenderingContext* context, Material* material, float width, float depth, const Color& color = Color::White) {
        auto* r = get();
        r->begin(context, material);
        r->drawPlane(width, depth, color);
        r->end();
    }
    static void drawPlane(RenderingContext* context, Material* material, float width, float depth, const Vector2& uv1, const Vector2& uv2, const Color& color = Color::White) {
        auto* r = get();
        r->begin(context, material);
        r->drawPlane(width, depth, uv1, uv2, color);
        r->end();
    }
    static void drawSphere(RenderingContext* context, Material* material, float radius, int slices, int stacks, const Color& color, const Matrix& localTransform = Matrix()) {
        auto* r = get();
        r->begin(context, material);
        r->drawSphere(radius, slices, stacks, color, localTransform);
        r->end();
    }
    static void drawBox(RenderingContext* context, Material* material, const Box& box, const Color& color = Color::White, const Matrix& localTransform = Matrix()) {
        auto* r = get();
        r->begin(context, material);
        r->drawBox(box, color, localTransform);
        r->end();
    }

private:
    PrimitiveMeshRenderer();
    Result init();

    CommandList* m_commandList;
    Material* m_material;
    Array<detail::MeshGenerater*> m_generators;

    friend class detail::RenderingManager;
};

} // namespace ln

