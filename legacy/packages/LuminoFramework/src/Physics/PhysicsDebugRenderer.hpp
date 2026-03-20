#pragma once

namespace ln {
namespace detail {

// 3D/ 2D 共通の描画キャッシュ。
class PhysicsDebugRenderer
{
public:
	static const size_t MaxVertexCount = 8192;

	void init();
	void render(RenderingContext* context);
	void addLineVertex(const Vector3& v, const Color& c);
	void addTriangleVertex(const Vector3& v, const Color& c);
	void drawLine(const Vector3& p1, const Vector3& p2, const Color& color1, const Color& color2);

private:

    Ref<VertexBuffer> m_linesBuffer;
    Ref<VertexBuffer> m_trianglesBuffer;
    size_t m_linesVertexCount;
    size_t m_trianglesVertexCount;
};

} // namespace detail
} // namespace ln

