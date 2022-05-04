#pragma once
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/RenderFeature.hpp>
#include "../RenderElement.hpp"
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>
#include "../RLIs/RLIBatchList.hpp"

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
        clearAndReserve(54);
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
	const T& getAt(int index) const { return ((T*)(m_buffer.data()))[index]; }
	T& getLast() { return getAt(m_count - 1); }

	int getCount() const { return m_count; }
	byte_t* getBuffer() { return m_buffer.data(); }
	size_t getBufferUsedByteCount() { return m_count * sizeof(T); }

private:
	void tryGlow(int requestCount)
	{
		if (/*m_count + */requestCount > m_capacity)
		{
            m_capacity = m_capacity * 2;
            m_buffer.resize(sizeof(T) * m_capacity);
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


#ifdef LN_BOX_ELEMENT_RENDER_FEATURE_TEST
#else
class ShapesRenderFeature
	: public RenderFeature
{
public:
	ShapesRenderFeature();
    void init(RenderingManager* manager);
	RenderingManager* manager() const { return m_manager; }

	RequestBatchResult requestDrawCommandList(GraphicsCommandList* context, ShapesRendererCommandList* commandList/*, detail::BrushRawData* fillBrush*/);

protected:
	virtual void beginRendering() override;
	virtual void submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch) override;
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

	//void prepareBuffers(GraphicsCommandList* context, int triangleCount);
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
#endif

class BoxElementShapeCommandList
{
public:
    enum CommandType
    {
        Cmd_Reset,
        Cmd_Base,
        Cmd_Background,
        Cmd_Border,
        Cmd_Shadow,
        Cmd_Submit,
    };

    struct ListNode
    {
        CommandType type;
        ListNode* next;
    };

    struct BaseCommand : public ListNode
    {
        BoxElementShapeBaseStyle style;
    };

    struct BackgroundCommand : public ListNode
    {
        BoxElementShapeBackgroundStyle style;
    };

    struct BorderCommand : public ListNode
    {
        BoxElementShapeBorderStyle style;
    };

    struct ShadowCommand : public ListNode
    {
        BoxElementShapeShadowStyle style;
    };

    void addResetCommand(LinearAllocator* allocator);
    void addBaseCommand(LinearAllocator* allocator, const BoxElementShapeBaseStyle& style);
    void addBackgroundCommand(LinearAllocator* allocator, const BoxElementShapeBackgroundStyle& style);
    void addBorderCommand(LinearAllocator* allocator, const BoxElementShapeBorderStyle& style);
    void addShadowCommand(LinearAllocator* allocator, const BoxElementShapeShadowStyle& style);
    void addSubmitCommand(LinearAllocator* allocator);

    ListNode* head = nullptr;
    ListNode* tail = nullptr;

private:
    void addCommandNode(ListNode* cmd, CommandType type);
};


class BoxElementShapeBuilderCommon
{
public:
    void setBaseRect(const BoxElementShapeBaseStyle& style);
    void setFillBox(const BoxElementShapeBackgroundStyle& style);
    void setBoxBorderLine(const BoxElementShapeBorderStyle& style);
    void setBoxShadow(const BoxElementShapeShadowStyle& style);

    int vertexCount() const;
    int indexCount() const;
    void writeToBuffer(Vertex* vertexBuffer, uint16_t* indexBuffer, uint16_t indexOffset);
    void writeToBuffer32(Vertex* vertexBuffer, uint32_t* indexBuffer);

protected:
    enum Side
    {
        Top = 0,
        Right = 1,
        Bottom = 2,
        Left = 3,
    };

    enum Corner
    {
        TopLeft = 0,
        TopRight = 1,
        BottomRight = 2,
        BottomLeft = 3,
    };

    struct GuideArea
    {
        Rect rect;
        CornerRadius corner;

        bool verify() const;
    };

    struct GlobalComponent
    {
        float width;
        Color color;
    };

    struct OutlinePoint
    {
        Vector2 pos;
        Color color;
        Vector2 antiAliasDir[2];	// AA を作るときの押し出し方向。
        Vector2 rightDir;			// 軸と平行な辺に対して AA を作るかどうかの判断に使う
        float cornerRatio;		// corner の始点～終点のどこに位置している点か。0.0 は始点、1.0 は終点。外周を時計回りで考える。curve を生成しない場合でも、Component の始点は 0.0, 終点は 1.0
        Corner cornerGroup;
    };

    enum class OutlinePathType
    {
        Convex,         // 始点と終点を結んだ領域を面張りする
        Stripe,    // AA用
    };

    enum class PathWinding
    {
        CW,	// 時計回り (基本)
        CCW,
    };

    struct OutlinePath
    {
        OutlinePathType type;
        int indexStart; // Index of PointId
        int indexCount; // Index of PointId
        PathWinding winding;
        bool stripeClosing = false;
        int antiAliasCount = 1;

        // 頂点id の index
        int beginIndex() const { return indexStart; }
        int endIndex() const { return indexCount + indexCount; }
    };

    void resetComon();
    void setupGuideline();
    bool outerShadowEnabled() const { return m_shadowEnabled && !m_shadowStyle.shadowInset; }
    bool insetShadowEnabled() const { return m_shadowEnabled && m_shadowStyle.shadowInset; }

    static void ajustGuidelineCorners(GuideArea* guide);

    int addOutlinePoint(const Vector2& pos, const Vector2& infrateDir, float cornerRatio, Corner cornerGroup);
    OutlinePath* beginOutlinePath(OutlinePathType type, PathWinding winding = PathWinding::CW);
    void endOutlinePath(OutlinePath* path);
    int addOutlineIndex(int pointId);
    OutlinePoint& outlinePoint(int index) { return m_outlinePointBuffer.getAt(index); }
    int outlineIndex(int index) const { return m_outlineIndices.getAt(index); }
    void makeOutlineAntiAlias(const OutlinePath* path/*, int start, int count*/);

    void expandPathes();
    void expandConvex(const OutlinePath& path);
    void expandStripeStroke(const OutlinePath& path);

    // Input infomation
    BoxElementShapeBaseStyle m_baseStyle;
    BoxElementShapeBackgroundStyle m_backgroundStyle;
    BoxElementShapeBorderStyle m_borderStyle;
    BoxElementShapeShadowStyle m_shadowStyle;
    bool m_backgroundEnabled;
    bool m_borderEnabled;
    bool m_shadowEnabled;
    bool m_shapeAAEnabled;

    // Guideline
    GuideArea m_shapeOuterGuide;	    // shape の外周。Background または、outsetBorder の外側。
    GuideArea m_shapeInnerGuide;	    // shape の内周。insetBorder の境界。insetShadow の接合部。border が無い場合は Background の外周と等しい。
    GuideArea m_outerShadowNearGuide;   // shape の外側 Shadow の、Blur 中間点 (alpha 1.0)
    GuideArea m_outerShadowFarGuide;    // shape の外側 Shadow の、Blur 消失点 (alpha 0.0)
    GuideArea m_innerShadowNearGuide;   // shape の外側 Shadow の、Blur 中間点 (alpha 1.0)
    GuideArea m_innerShadowFarGuide;    // shape の外側 Shadow の、Blur 消失点 (alpha 0.0)
    GlobalComponent m_commonComponents[4];

    // Outline
    CacheBuffer<OutlinePoint> m_outlinePointBuffer;
    List<OutlinePath> m_outlinePaths;
    CacheBuffer<uint16_t> m_outlineIndices;

    // TODO: 以下、いらないかも。見積もり立てて、外部からもらったBufferに直接書くこともできる
    CacheBuffer<Vertex> m_vertexCache;
    CacheBuffer<uint16_t> m_indexCache;
};

#if 0
// Shape ひとつ分の構築を担当する。
// Shape(Box、Border, Shadow など) は例えば Box と Shadow を分けて考えてもいいし、addXXXX で、ひとつの Shape として扱ってもよい。
// このクラスでは RHI の VertexBuffer は扱わない。インデックスは生成するが、ひとつの Shape の中で閉じた 0 スタートで生成する。
//（このクラスの呼び出し側で RHI の IndexBuffer に転送するときにオフセットを付ける必要がある）
class BoxElementShapeBuilder2
    : public BoxElementShapeBuilderCommon
{
public:
	BoxElementShapeBuilder2();
	void init();

	void reset();
	void build();


private:
	
	// Baseline の作成基準となる、Round 情報などを持った矩形領域
    using BaseRect = GuideArea;

	struct BasePath
	{
		int start;
		int count;

		int begin() const { return start; }
		int end() const { return start + count; }
	};

	// Baselineの[上][左][下][右]、計4つセットの基本要素。
	// Border は上下左右で個別に Color を設定できるが、その単位となる。
	struct BorderComponent
	{
		float width;	// 厚さ
		Color color;
		int	outerPointStart;
		int outerPointCount;

		int beginOuter() const { return outerPointStart; }
		int endOuter() const { return outerPointStart + outerPointCount; }

		// 外周のうち、カーブの部分
		int outerCornerStart1;	// 丸めない場合は始点を指す（通常 startPoint と同値）
		int outerCornerStart2;	// 丸めない場合は終点を指す
		int outerCornerCount1() const { return outerCornerStart2 - outerCornerStart1; }
		int outerCornerCount2() const { return outerPointCount - outerCornerCount1(); }

        int beginIds1() const { return outerCornerStart1; }
        int endIds1() const { return outerCornerStart1 + outerCornerCount1(); }
        int beginIds2() const { return outerCornerStart2; }
        int endIds2() const { return outerCornerStart2 + outerCornerCount2(); }


		// 内周全体
		int	innterPointStart;
		int innterPointCount;
	};

	struct OutlinePoint
	{
		Vector2 pos;
        Color color;
		Vector2 antiAliasDir[2];	// AA を作るときの押し出し方向。
		Vector2 rightDir;			// 軸と平行な辺に対して AA を作るかどうかの判断に使う
        float cornerRatio;		// corner の始点～終点のどこに位置している点か。0.0 は始点、1.0 は終点。外周を時計回りで考える。curve を生成しない場合でも、Component の始点は 0.0, 終点は 1.0
    
        int stripePairPointId = -1;  // (outsetの場合) ひとつ内側の Path 上の、対応する頂点Id
    };

	struct OutlinePath
	{
		OutlinePathType type;
		int pointStart; // TODO: id
		int pointCount; // TODO: id
		Color color;
		PathWinding winding;
		bool stripeClosing = false;
        int antiAliasCount = 1;

		// 頂点index. IndexBuffer に書き込める値。
		int begin() const { return pointStart; }
		int end() const { return pointStart + pointCount; }
	};

	void setupBaseRects();
	int addOutlinePoint(const Vector2& pos, const Vector2& infrateDir, float cornerRatio);
	void makeBaseOuterPointsAndBorderComponent(const BaseRect& baseRect, float dirSign, BorderComponent components[4], BasePath* outBasePath);
	
	OutlinePath* beginOutlinePath(OutlinePathType type, const Color& color, PathWinding winding = PathWinding::CW);
	void endOutlinePath(OutlinePath* path);
	int addOutlineIndex(int index);
	OutlinePoint& outlinePoint(int index) { return m_outlinePointBuffer.getAt(index); }
	int outlineIndex(int index) const { return m_outlineIndices.getAt(index); }
    void makeStripePointPair(BorderComponent* outerCmps, const BorderComponent* innerCmps);
    void makeOutlineAntiAlias(const OutlinePath* path/*, int start, int count*/);
    void makeStripePath(const BorderComponent* outerCmps, const BorderComponent* innerCmps, PathWinding winding = PathWinding::CW);
    void applyColorToShadowComponents(BorderComponent components[4], float alpha);

	void expandPathes();
	void expandConvex(const OutlinePath& path);
	void expandStripeStroke(const OutlinePath& path);


	// Working data
    //CornerRadius m_cornerRatios;
	BaseRect m_shapeOuterRect;	// shape の外周。Background または、outsetBorder の外側。
	BaseRect m_shapeInnerRect;	// shape の内周。insetBorder の境界。insetShadow の接合部。border が無い場合は Background の外周と等しい。
	BaseRect m_shadowBaseRect;	// shadow の外周。inset/outset共有。shadowBlur はこの時点では考慮しない。
	BasePath m_shapeOuterBasePath;
    BasePath m_shapeInnerBasePath;
	BorderComponent m_shapeOuterComponents[4];
	CacheBuffer<OutlinePoint> m_outlinePointBuffer;

	List<OutlinePath> m_outlinePaths;
	CacheBuffer<uint16_t> m_outlineIndices;
};
#endif

// Shape ひとつ分の構築を担当する。
// Shape(Box、Border, Shadow など) は例えば Box と Shadow を分けて考えてもいいし、addXXXX で、ひとつの Shape として扱ってもよい。
// このクラスでは RHI の VertexBuffer は扱わない。インデックスは生成するが、ひとつの Shape の中で閉じた 0 スタートで生成する。
//（このクラスの呼び出し側で RHI の IndexBuffer に転送するときにオフセットを付ける必要がある）
class BoxElementShapeBuilder3
    : public BoxElementShapeBuilderCommon
{
public:
    BoxElementShapeBuilder3();
    void init();
    void reset();
    void build();

private:
    
    // Baselineの[上][左][下][右]、計4つセットの基本要素。
	// Border は上下左右で個別に Color を設定できるが、その単位となる。
    struct Component
    {
        float width;	// 厚さ
        Color color;
        int	pointIdStart;
        int pointIdCount;

        int beginIds() const { return pointIdStart; }
        int endIds() const { return pointIdStart + pointIdCount; }

        // 外周のうち、カーブの部分
        int cornerStart1;	// 丸めない場合は始点を指す（通常 startPoint と同値）
        int cornerStart2;	// 丸めない場合は終点を指す
        int cornerCount1() const { return cornerStart2 - cornerStart1; }
        int cornerCount2() const { return pointIdCount - cornerCount1(); }

        int beginIds1() const { return cornerStart1; }
        int endIds1() const { return cornerStart1 + cornerCount1(); }
        int beginIds2() const { return cornerStart2; }
        int endIds2() const { return cornerStart2 + cornerCount2(); }
    };

    struct ComponentSet
    {
        Component components[4];
        int startId;
        int countId;

        int beginIds() const { return startId; }
        int endIds() const { return startId + countId; }
    };

    struct PointIdRange
    {
        int startId;
        int countId;

        int beginIds() const { return startId; }
        int endIds() const { return startId + countId; }
    };

    void makeBaseOuterPointsAndBorderComponent(const GuideArea& baseRect, float dirSign, ComponentSet* outSet);
    void makeShadowoutlinePoints(const GuideArea& baseRect, float dirSign, PointIdRange* outRange);

    void makeStripePath(const PointIdRange& outer, const PointIdRange& inner);

    void applyColors(const PointIdRange& range, const Color& color);
    void applyColorsAndOffset(const PointIdRange& range, const Color& color, const Vector2& offset);

    // 0 は Curve 無しを表す
    int m_shadowtess[4];
};

// TODO: name:BoxElementRenderFeature
class ShapesRenderFeature2
	: public RenderFeature
{
public:
	ShapesRenderFeature2();
	void init(RenderingManager* manager);

	RequestBatchResult requestDrawCommandList(
		RenderFeatureBatchList* batchList,
		const RLIBatchState& batchState,
		ShapesRendererCommandList* commandList);

    RequestBatchResult requestDrawCommandList(
		RenderFeatureBatchList* batchList,
		const RLIBatchState& batchState,
		BoxElementShapeCommandList* commandList);

protected:
	virtual void beginRendering() override;
	virtual void submitBatch(GraphicsCommandList* context, detail::RenderFeatureBatchList* batchList) override;
	virtual void renderBatch(GraphicsCommandList* context, RenderFeatureBatch* batch) override;
	
    // モノによってはかなりの頂点数となるため、CPU 側での頂点変換はコストが高すぎる
    virtual bool drawElementTransformNegate() const override { return false; }

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

	Batch* acquireBatch(RenderFeatureBatchList* batchList, const RLIBatchState& batchState);

	RenderingManager* m_manager;
	//BatchData m_batchData;
	//BoxElementShapeBuilder2 m_shapeBuilder;
    BoxElementShapeBuilder3 m_shapeBuilder;
	
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

	virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const RLIBatchState* state) override
	{
#ifdef LN_BOX_ELEMENT_RENDER_FEATURE_TEST
		return static_cast<detail::ShapesRenderFeature2*>(renderFeature)->requestDrawCommandList(batchList, *state, &commandList);
#else
		return static_cast<detail::ShapesRenderFeature*>(renderFeature)->requestDrawCommandList(context, &commandList);
#endif
	}

private:
	//SizeI m_srcTextureSize;
};

class DrawBoxElementShape : public RenderDrawElement
{
public:
    BoxElementShapeCommandList commandList;

    virtual RequestBatchResult onRequestBatch(detail::RenderFeatureBatchList* batchList, GraphicsCommandList* context, RenderFeature* renderFeature, const RLIBatchState* state) override
    {
        return static_cast<detail::ShapesRenderFeature2*>(renderFeature)->requestDrawCommandList(batchList, *state, &commandList);
    }
};

} // namespace detail
} // namespace ln

