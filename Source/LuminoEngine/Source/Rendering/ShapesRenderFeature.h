
#pragma once
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Vertex.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Rendering/Common.h>

LN_NAMESPACE_BEGIN
namespace detail {

// なぜコマンドが必要なのか？DrawList や RenderingCommand ではだめなのか？
//		LineTo を考えてみればわかりやすい。
//		点を1つ打つのに、DrawElement を１つ作りたいか？No.
//		このコマンドは、ShapesRenderFeature 内でプリミティブをキャッシュするためのものである。
//		ちなみに、RenderingCommand は描画スレッド有効でしか働かないので論外。
class ShapesRendererCommandList
	: public CommandDataCache
{
public:
	enum Command
	{
		Cmd_DrawBoxBackground,
		Cmd_DrawBoxBorder,
		Cmd_DrawBoxBorderLine,
		Cmd_DrawBoxShadow,
	};

	void addDrawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius);
	void addDrawBoxBorder(const Rect& rect, const Thickness& thickness, const CornerRadius& cornerRadius, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const Color& shadowColor, float shadowBlur, float shadowWidth, bool shadowInset, bool borderInset);
	void drawBoxBorderLine(const Rect& rect, const Thickness& thickness, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const CornerRadius& cornerRadius, bool borderInset);
	void addDrawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Color& color, float blur, float width, bool inset);
};

class ShapesRendererCommandListCache
	: public SimpleOneTimeObjectCache<ShapesRendererCommandList>
{
protected:
	virtual Ref<ShapesRendererCommandList> createObject() override;
};

class ShapesRendererCore
	: public RefObject
{
public:
	ShapesRendererCore();
	virtual ~ShapesRendererCore();
	void initialize(GraphicsManager* manager);

	void renderCommandList(ShapesRendererCommandList* commandList, detail::BrushRawData* fillBrush);

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
	};

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

	void releaseCommandList(ShapesRendererCommandList* commandList);
	void requestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex);
	Path* addPath(PathType type, const Color& color, PathWinding winding = PathWinding::CCW, PathAttribute attribute = PathAttribute::None);
	void endPath(Path* path);
	void extractBasePoints(ShapesRendererCommandList* commandList);
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

	GraphicsManager*			m_manager;
	Driver::IVertexBuffer*		m_vertexBuffer;
	Driver::IIndexBuffer*		m_indexBuffer;
	CacheBuffer<BasePoint>		m_basePoints;
	CacheBuffer<OutlinePoint>	m_outlinePoints;
	List<Path>					m_pathes;
	CacheBuffer<Vertex>			m_vertexCache;
	CacheBuffer<uint16_t>		m_indexCache;
};

class ShapesRenderFeature
	: public RefObject
	, public detail::IRenderFeature
{
public:
	ShapesRenderFeature();
	virtual ~ShapesRenderFeature();
	void initialize(GraphicsManager* manager);

	void executeCommand(ShapesRendererCommandList* commandList);

protected:
	virtual bool isStandaloneShader() const { return false; }
	virtual void flush() override {}
	virtual void onActivated() override {}
	virtual void onDeactivated() override { flush(); }
	virtual void onSetState(const DrawElementBatch* state);

private:
	GraphicsManager*				m_manager;
	Ref<ShapesRendererCore>		m_core;
	detail::BrushRawData			m_fillBrush;
};

} // namespace detail
LN_NAMESPACE_END
