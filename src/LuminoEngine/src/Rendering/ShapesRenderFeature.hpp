#pragma once
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderFeature.hpp>
#include "RenderStage.hpp"
#include "RenderingManager.hpp"

namespace ln {
namespace detail {

	
template<typename T>
class CacheBuffer
{
public:
	CacheBuffer()
		: m_buffer()
		, m_capacity(0)
		, m_count(0)
	{
	}

	void clearAndReserve(int count)
	{
		m_buffer.resize(sizeof(T) * count);
		m_capacity = count;
		m_count = 0;
	}

	void add(const T& value)
	{
		tryGlow(m_count + 1);
		memcpy(m_buffer.data() + (sizeof(T) * m_count), &value, sizeof(T));
		m_count++;
	}

	T* request(int count)
	{
		tryGlow(m_count + count);
		size_t begin = getBufferUsedByteCount();
		m_count += count;
		return reinterpret_cast<T*>(m_buffer.data() + begin);
	}

	void clear()
	{
		m_count = 0;
	}

	T& getAt(int index) { return ((T*)(m_buffer.data()))[index]; }
	T& getLast() { return getAt(m_count - 1); }

	int getCount() const { return m_count; }
	byte_t* getBuffer() { return m_buffer.data(); }
	size_t getBufferUsedByteCount() { return m_count * sizeof(T); }

private:
	void tryGlow(int requestCount)
	{
		if (/*m_count + */requestCount > m_capacity)
		{
			clearAndReserve(m_capacity * 2);
		}
	}

	ByteBuffer	m_buffer;
	int			m_capacity;
	int			m_count;
};

// なぜコマンドが必要なのか？DrawList や RenderingCommand ではだめなのか？
//		LineTo を考えてみればわかりやすい。
//		点を1つ打つのに、DrawElement を１つ作りたくはない。
//		このコマンドは、ShapesRenderFeature 内でプリミティブをキャッシュするためのものである。
//		ちなみに、RenderingCommand は描画スレッド有効でしか働かないので論外。
class ShapesRendererCommandList
{
public:
	enum CommandType
	{
		Cmd_DrawBoxBackground,
		//Cmd_DrawBoxBorder,
		Cmd_DrawBoxBorderLine,
		Cmd_DrawBoxShadow,
	};

	struct ListNode
	{
		CommandType type;
		ListNode* next;
		Matrix transform;
	};

	struct DrawBoxBackgroundCommand : public ListNode
	{
		Rect rect;
		CornerRadius cornerRadius;
		Color color;
	};

	//struct DrawBoxBorderCommand : public ListNode
	//{
	//	Rect rect;
	//	Thickness thickness;
	//	CornerRadius cornerRadius;
	//	Color leftColor;
	//	Color topColor;
	//	Color rightColor;
	//	Color bottomColor;
	//	Color shadowColor;
	//	float shadowBlur;
	//	float shadowWidth;
	//	bool shadowInset;
	//	bool borderInset;
	//};
	
	struct DrawBoxBorderLineCommand : public ListNode
	{
		Rect rect;
		Thickness thickness;
		Color leftColor;
		Color topColor;
		Color rightColor;
		Color bottomColor;
		CornerRadius cornerRadius;
		bool borderInset;
	};

	struct DrawBoxShadowCommand : public ListNode
	{
		Rect rect;
		CornerRadius cornerRadius;
		Vector2 offset;
		Color color;
		float blur;		// ↓の次に、太らせた外周から、外側と内側へこの値の分だけぼかしをかける
		float width;	// まずはこの値まで Shape を太らせる
		bool inset;
	};

	void addDrawBoxBackground(LinearAllocator* allocator, const Matrix& transform, const Rect& rect, const CornerRadius& cornerRadius, const Color& color);
	//void addDrawBoxBorder(LinearAllocator* allocator, const Matrix& transform, const Rect& rect, const Thickness& thickness, const CornerRadius& cornerRadius, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const Color& shadowColor, float shadowBlur, float shadowWidth, bool shadowInset, bool borderInset);
	void drawBoxBorderLine(LinearAllocator* allocator, const Matrix& transform, const Rect& rect, const Thickness& thickness, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const CornerRadius& cornerRadius, bool borderInset);
	void addDrawBoxShadow(LinearAllocator* allocator, const Matrix& transform, const Rect& rect, const CornerRadius& cornerRadius, const Vector2& offset, const Color& color, float blur, float width, bool inset);

	ListNode* head = nullptr;
	ListNode* tail = nullptr;

private:
	void addCommandNode(ListNode* cmd, CommandType type, const Matrix& transform);
};

class InternalShapesRenderer
    : public RefObject
{
public:
	InternalShapesRenderer();
    void init(RenderingManager* manager);
	RenderingManager* manager() const { return m_manager; }

	void renderCommandList(ICommandList* context, ShapesRendererCommandList* commandList/*, detail::BrushRawData* fillBrush*/);

private:
	enum class PathType
	{
		Convex,
		Strip2Point,	// TODO: 未使用になりそう
		Strip3Point,	// これは AA できない。シャドウ用
	};

	enum class PathWinding
	{
		CCW,	// 反時計回り (基本)
		CW,
	};

	enum class PathAttribute
	{
		None,
		Background,		// ブラシの色を適用するかどうか
	};

	struct Path
	{
		PathType		type;
		int				pointStart;
		int				pointCount;
		Color			color;
		PathWinding		winding;
		PathAttribute	attribute;
		const Matrix*	transform;	// 実行中のコマンド構造体に乗っている transform への参照
	};


	// ベースラインの上下左右、計4つセットの基本要素。
	// Border は上下左右で個別に Color を設定できるが、その単位となる。
	struct BorderComponent
	{
		int	firstPoint;
		int lastPoint;
	};

	struct BasePoint
	{
		Vector2	pos;
		Vector2	exDir;	// 押し出し方向 (right-dir)	※radius=0の場合は 長さ1ではない (√2)
		bool	enabledAA;
		bool	exDirect;
	};

	struct OutlinePoint
	{
		Vector2	pos;
		Vector2	exDirAA;	// 押し出し方向 (AntiAlias 用)
		float	alpha;
	};

	//void releaseCommandList(ShapesRendererCommandList* commandList);
	void requestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex);
	Path* addPath(PathType type, const Matrix* transform, const Color& color, PathWinding winding = PathWinding::CCW, PathAttribute attribute = PathAttribute::None);
	void endPath(Path* path);
	void extractBasePoints(ShapesRendererCommandList* commandList);
	void extractBasePoints(ShapesRendererCommandList::ListNode* command);
	void makeBasePointsAndBorderComponent(const Rect& rect, const Thickness& thickness, const CornerRadius& cornerRadius, BorderComponent components[4]);
	void calcExtrudedDirection();
	void expandVertices(const Path& path);
	void expandFill(const Path& path);
	void expandStrip2PointStroke(const Path& path);
	void expandStrip3PointStroke(const Path& path);
	void expandAntiAliasStroke(const Path& path, int startIndex);
	void plotCornerBasePointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center);
	Vector2 getExtPos(const BasePoint& pt, float borderExtSign, float widht) const { return (pt.exDirect) ? pt.pos + pt.exDir * borderExtSign : pt.pos + pt.exDir * borderExtSign * widht; }
	Vector2 getAAExtDir(const BasePoint& pt) const { return (pt.enabledAA) ? pt.exDir : Vector2::Zero; }

	RenderingManager* m_manager;
	Ref<IVertexBuffer>		m_vertexBuffer;
	Ref<IIndexBuffer>		m_indexBuffer;
	CacheBuffer<BasePoint>		m_basePoints;
	CacheBuffer<OutlinePoint>	m_outlinePoints;
	List<Path>					m_pathes;
	CacheBuffer<Vertex>			m_vertexCache;
	CacheBuffer<uint16_t>		m_indexCache;
};

// 特に state とかないので不要なのだが、実装を他と合わせてイメージを持ちやすいようにしている。
// TODO: 後で消す。
class ShapesRenderFeatureStageParameters
	: public RenderFeatureStageParameters
{
public:
	ShapesRenderFeatureStageParameters()
		: RenderFeatureStageParameters(CRCHash::compute("ShapesRenderFeatureStageParameters"))
	{
	}

	virtual bool equals(const RenderFeatureStageParameters* other) override
	{
		if (typeId() != other->typeId()) return false;
		if (this == other) return true;
		return true;
	}

	virtual void copyTo(RenderFeatureStageParameters* params) override
	{
		LN_CHECK(typeId() == params->typeId());
	}

private:
};

class ShapesRenderFeature
	: public RenderFeature
{
public:
	ShapesRenderFeature();
	void init(RenderingManager* manager);

    void renderCommandList(GraphicsContext* context, const ShapesRendererCommandList& commandList);

protected:
	virtual RenderFeatureBatch* submitBatch(GraphicsContext* context) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;
    virtual bool drawElementTransformNegate() const override { return true; }

private:
	Ref<InternalShapesRenderer> m_internal;
};

class DrawShapesElement : public RenderDrawElement
{
public:
	ShapesRendererCommandList commandList;

    virtual void onDraw(GraphicsContext* context, RenderFeature* renderFeatures, const SubsetInfo* subsetInfo) override
    {
		m_srcTextureSize.width = subsetInfo->materialTexture->width();
		m_srcTextureSize.height = subsetInfo->materialTexture->height();

		static_cast<detail::ShapesRenderFeature*>(renderFeatures)->renderCommandList(context, commandList);
    }

private:
    SizeI m_srcTextureSize;
};

} // namespace detail
} // namespace ln

