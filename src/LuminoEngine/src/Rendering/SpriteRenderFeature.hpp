#pragma once
#include <Lumino/Base/EnumFlags.hpp>
#include <Lumino/Graphics/GeometryStructs.hpp>
#include <Lumino/Graphics/ColorStructs.hpp>
#include <Lumino/Graphics/VertexDeclaration.hpp>
#include <Lumino/Rendering/RenderFeature.hpp>
#include <Lumino/Rendering/Vertex.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"

namespace ln {
class Texture;
namespace detail {

// スプライトのソート方法
enum class SpriteSortMode
{
	None,				// ソートしない
	DepthBackToFront,	// Z値が大きいものが先に描画されるようにソートする (アルファブレンド使用時の推奨)
	DepthFrontToBack,	// Z値が小さいものが先に描画されるようにソートする (SpriteSortMode_DepthBackToFront と同時には使えない)
};

// オブジェクトのソートの基準
enum class SortingDistanceBasis
{
	RawZ,		// オブジェクトの Z 値
	ViewPont,	// オブジェクトの位置と視点との距離
};

// 実際の描画を行う内部クラス。
// レンダリングスレッド上で動作する。
class InternalSpriteRenderer
	: public RefObject
{
public:
	struct State
	{
		Matrix viewMatrix;
		Matrix projMatrix;
		Size textureRealSize;
		SpriteSortMode sortMode = SpriteSortMode::DepthBackToFront;
		SortingDistanceBasis sortingBasis = SortingDistanceBasis::ViewPont;
	};

	InternalSpriteRenderer();
	void initialize(RenderingManager* manager);

	void setState(const State& state);

	void drawRequest(
		const Matrix& transform,
		const Vector2& size,
		const Vector2& anchorRatio,
		const Rect& srcRect,
		const Color& color,
		SpriteBaseDirection baseDir,
		BillboardType billboardType);

	void flush();
	void clear();

private:
	class SpriteCmpDepthBackToFront;		// Z 値の大きい方から小さい方へソートする比較
	class SpriteCmpDepthFrontToBack;		// Z 値の小さい方から大きい方へソートする比較

	struct SpriteData
	{
		Vertex vertices[4];
		int priority;	// 優先度 (大きい方が後から描画される =手前)
		float depth;	// ソートに使われる Z 値 (大きいほど遠い)
	};

	void prepareBuffers(int spriteCount);

	State m_state;
	Matrix m_viewInverseMatrix;
	Vector3 m_viewDirection;
	Vector3 m_viewPosition;
	List<SpriteData> m_spriteDataList;
	List<int> m_spriteIndexList;

	IGraphicsDeviceContext* m_device;
	Ref<IVertexDeclaration> m_vertexDeclaration;
	Ref<IVertexBuffer> m_vertexBuffer;
	Ref<IIndexBuffer> m_indexBuffer;
	int m_buffersReservedSpriteCount;
};

class SpriteRenderFeature
	: public RenderFeature
{
public:
	void setSortInfo(
		SpriteSortMode sortMode,
		SortingDistanceBasis sortingBasis);

	void setState(
		const Matrix& viewMatrix,
		const Matrix& projMatrix,
		const Size& textureRealSize);

	void drawRequest(
		const Matrix& transform,
		const Vector2& size,
		const Vector2& anchorRatio,
		const Rect& srcRect,
		const Color& color,
		SpriteBaseDirection baseDirection,
		BillboardType billboardType);

	virtual void flush() override;

LN_CONSTRUCT_ACCESS:
	SpriteRenderFeature();
	void initialize(RenderingManager* manager);

private:
	RenderingManager* m_manager;
	InternalSpriteRenderer::State m_state;
	Ref<InternalSpriteRenderer> m_internal;
};

} // namespace detail
} // namespace ln

