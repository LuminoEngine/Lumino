#pragma once
#include <Lumino/Base/EnumFlags.hpp>
#include <Lumino/Rendering/RenderFeature.hpp>
#include <Lumino/Graphics/GeometryStructs.hpp>
#include <Lumino/Graphics/ColorStructs.hpp>
#include <Lumino/Graphics/VertexDeclaration.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"

namespace ln {
namespace detail {

/** スプライトのソート方法 */
enum class SpriteSortMode
{
	None = 0x00,			/**< ソートしない */
	DepthBackToFront = 0x01,			/**< Z値が大きいものが先に描画されるようにソートする (アルファブレンド使用時の推奨) */
	DepthFrontToBack = 0x02,			/**< Z値が小さいものが先に描画されるようにソートする (SpriteSortMode_DepthBackToFront と同時には使えない) */
	//Texture = 0x04,			/**< テクスチャを優先してソートする (同じテクスチャを持つスプライトが多数あるとき用。ただし、アルファブレンドが有効な場合は描画が不自然になることがある) */
};
//LN_FLAGS_OPERATORS(SpriteSortFlags);

/// オブジェクトのソートの基準
enum SortingDistanceBasis
{
	SortingDistanceBasis_RawZ = 0,		///< オブジェクトの Z 値
	SortingDistanceBasis_ViewPont,		///< オブジェクトの位置と視点との距離
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
		Size viewPixelSize;
		Ref<ITexture*> texture;
		SpriteSortMode sortMode;
		SortingDistanceBasis sortingBasis;
	};

	InternalSpriteRenderer();
	void initialize(GraphicsManager* manager);

	void setState(const State& state);

	void drawRequest(
		const Matrix& transform,
		const Vector2& size,
		const Vector2& anchorRatio,
		ITexture* texture,
		const Rect& srcRect,
		const Color& color,
		SpriteBaseDirection baseDir,
		BillboardType billboardType);

	void flush();
	void clear();

private:
	class SpriteCmpDepthBackToFront;		// Z 値の大きい方から小さい方へソートする比較
	class SpriteCmpDepthFrontToBack;		// Z 値の小さい方から大きい方へソートする比較
	class SpriteCmpTexDepthBackToFront;		// Z 値の大きい方から小さい方へソートする比較 (テクスチャ優先)
	class SpriteCmpTexDepthFrontToBack;		// Z 値の小さい方から大きい方へソートする比較 (テクスチャ優先)

	struct SpriteVertex
	{
		Vector3 position;  
		Color color;
		Vector2 uv;
	};

	struct SpriteData
	{
		SpriteVertex vertices[4];
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

private:
};

} // namespace detail
} // namespace ln

