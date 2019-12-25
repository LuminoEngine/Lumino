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


class ShapesRenderFeature
	: public RenderFeature
{
public:
	ShapesRenderFeature();
    void init(RenderingManager* manager);
	RenderingManager* manager() const { return m_manager; }

	RequestBatchResult requestDrawCommandList(GraphicsContext* context, ShapesRendererCommandList* commandList/*, detail::BrushRawData* fillBrush*/);

protected:
	virtual void beginRendering() override;
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;
	virtual bool drawElementTransformNegate() const override { return true; }

private:
	struct BatchData
	{
		int indexOffset;
		int indexCount;
	};

	class Batch : public RenderFeatureBatch
	{
	public:
		BatchData data;
	};

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

	//void prepareBuffers(GraphicsContext* context, int triangleCount);
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

	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
	size_t m_vertexUsedCount = 0;
	size_t m_indexUsedCount = 0;
	//int m_buffersReservedTriangleCount = 0;
	BatchData m_batchData;


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

//class ShapesRenderFeature
//	: public RenderFeature
//{
//public:
//	ShapesRenderFeature();
//	void init(RenderingManager* manager);
//
//    void renderCommandList(GraphicsContext* context, const ShapesRendererCommandList& commandList);
//
//protected:
//	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
//	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;
//    virtual bool drawElementTransformNegate() const override { return true; }
//
//private:
//	Ref<InternalShapesRenderer> m_internal;
//};




// 必須情報。border の情報が含まれているが、Shadow を正しく描画するために必要となる
struct BoxElementShapeBaseStyle
{
	Matrix transform;
	Rect baseRect;
	CornerRadius cornerRadius;
	Thickness borderThickness;
	bool borderInset = false;
	bool aligndLineAntiAlias = false;	// 軸に平行な辺にも AA を適用するか (回転させたい場合に使用)
};

struct BoxElementShapeBackgroundStyle
{
	Color color;
};

struct BoxElementShapeBorderStyle
{
	Color borderLeftColor;
	Color borderTopColor;
	Color borderRightColor;
	Color borderBottomColor;
};

struct BoxElementShapeShadowStyle
{
	Vector2 shadowOffset;
	Color shadowColor;
	float shadowWidth = 0.0f;
	float shadowBlur = 0.0f;
	bool shadowInset = false;
};


// Shape ひとつ分の構築を担当する。
// Shape(Box、Border, Shadow など) は例えば Box と Shadow を分けて考えてもいいし、addXXXX で、ひとつの Shape として扱ってもよい。
// このクラスでは RHI の VertexBuffer は扱わない。インデックスは生成するが、ひとつの Shape の中で閉じた 0 スタートで生成する。
//（このクラスの呼び出し側で RHI の IndexBuffer に転送するときにオフセットを付ける必要がある）
class BoxElementShapeBuilder
{
public:
	BoxElementShapeBuilder();
	void init();

	void reset();
	void setBaseRect(const BoxElementShapeBaseStyle& style);
	void setFillBox(const BoxElementShapeBackgroundStyle& style);
	void setBoxBorderLine(const BoxElementShapeBorderStyle& style);
	void setBoxShadow(const BoxElementShapeShadowStyle& style);
	//void setBaseRect(const Matrix& transform, const Rect& rect, const CornerRadius& cornerRadius);
	//void setFillBox(const Color& color);
	//void setBoxBorderLine(const Thickness& thickness, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, bool borderInset);
	//void setBoxShadow(const Vector2& offset, const Color& color, float blur, float width, bool inset);
	void build();

	int vertexCount() const;
	int indexCount() const;
	void writeToBuffer(Vertex* vertexBuffer, uint16_t* indexBuffer, uint16_t indexOffset);

private:
	struct BaselinePoint
	{
		Vector2	pos;
		Vector2	infrateDir;	// AA や Shadow のための押し出し方向。Shape に対して外向きのベクトル。必ずしも放射状であるとは限らない
		float cornerRatio;		// corner の始点～終点のどこに位置している点か。0.0 は始点、1.0 は終点。外周を時計回りで考える
		
		
		//Vector2	front;	// Path 内の次の点への向き
	};

	struct BaselinePath
	{
		int startPoint;	// index of m_baselinePointBuffer
		int pointCount;
	};

	// Baselineの[上][左][下][右]、計4つセットの基本要素。
	// Border は上下左右で個別に Color を設定できるが、その単位となる。
	struct BorderComponent
	{
		int	startPoint;
		int pointCount;

		// 外周のうち、カーブの部分
		int outerCornerStart1;	// 丸めない場合は始点を指す（通常 startPoint と同値）
		int outerCornerStart2;	// 丸めない場合は終点を指す
		int outerCornerCount1() const { return outerCornerStart2 - outerCornerStart1; }
		int outerCornerCount2() const { return pointCount - outerCornerCount1(); }

		// 内周全体
		int	innterPointStart;
		int innterPointCount;
	};

	enum Side
	{
		Top = 0,
		Right = 1,
		Bottom = 2,
		Left = 3,
	};

	void makeBasePointsAndBorderComponent(const Rect& shapeOuterRect, const CornerRadius& cornerRadius, BorderComponent components[4]);
	void plotCornerBasePointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center);
	//void calculateBasePointsDirection();

	// Input infomation
	BoxElementShapeBaseStyle m_baseStyle;
	BoxElementShapeBackgroundStyle m_backgroundStyle;
	BoxElementShapeBorderStyle m_borderStyle;
	BoxElementShapeShadowStyle m_shadowStyle;
	//Matrix m_transform;
	//Rect m_baseRect;
	//CornerRadius m_cornerRadius;
	//Color m_boxColor;
	//Thickness m_borderThickness;
	//Color m_borderLeftColor;
	//Color m_borderTopColor;
	//Color m_borderRightColor;
	//Color m_borderBottomColor;
	//bool m_borderInset = false;
	//Vector2 m_shadowOffset;
	//Color m_shadowColor;
	//float m_shadowWidth = 0.0f;
	//float m_shadowBlur = 0.0f;
	//bool m_shadowInset = false;
	//bool m_aligndLineAntiAlias = false;	// 軸に平行な辺にも AA を適用するか (回転させたい場合に使用)
	bool m_backgroundEnabled;
	bool m_borderEnabled;
	bool m_shadowEnabled;

	// Working data
	Rect m_shapeOuterRect;
	BorderComponent m_borderComponents[4];

	CacheBuffer<BaselinePoint> m_baselinePointBuffer;
	BaselinePath m_outerBaselinePath;


//List<BaselinePath> m_baselinePaths;

};


// TODO: name:BoxElementRenderFeature
class ShapesRenderFeature2
	: public RenderFeature
{
public:
	ShapesRenderFeature2();
	void init(RenderingManager* manager);

	RequestBatchResult requestDrawCommandList(ShapesRendererCommandList* commandList);

protected:
	virtual void beginRendering() override;
	virtual void submitBatch(GraphicsContext* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsContext* context, RenderFeatureBatch* batch) override;
	virtual bool drawElementTransformNegate() const override { return true; }

private:
	struct BatchData
	{
		int indexOffset;
		int indexCount;
	};

	class Batch : public RenderFeatureBatch
	{
	public:
		BatchData data;
	};

	RenderingManager* m_manager;
	BatchData m_batchData;
	BoxElementShapeBuilder m_shapeBuilder;
	
	// Rendering resource
	Ref<VertexLayout> m_vertexLayout;
	Ref<VertexBuffer> m_vertexBuffer;
	Ref<IndexBuffer> m_indexBuffer;
	size_t m_vertexUsedCount = 0;
	size_t m_indexUsedCount = 0;
};




class DrawShapesElement : public RenderDrawElement
{
public:
	ShapesRendererCommandList commandList;

	virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsContext* context, RenderFeature* renderFeature, const detail::SubsetInfo* subsetInfo) override
	{
#ifdef LN_BOX_ELEMENT_RENDER_FEATURE_TEST
		return static_cast<detail::ShapesRenderFeature2*>(renderFeature)->requestDrawCommandList(&commandList);
#else
		return static_cast<detail::ShapesRenderFeature*>(renderFeature)->requestDrawCommandList(context, &commandList);
#endif
	}

private:
	//SizeI m_srcTextureSize;
};

} // namespace detail
} // namespace ln

