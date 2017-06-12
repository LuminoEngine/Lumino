
#pragma once
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Vertex.h>
#include <Lumino/Graphics/ContextInterface.h>

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
		Cmd_DrawBoxBorder2,
		Cmd_DrawBoxShadow,
	};

	void AddDrawBoxBackground(const Rect& rect, const CornerRadius& cornerRadius);
	void AddDrawBoxBorder(float x, float y, float w, float h, float l, float t, float r, float b, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, float ltRad, float rtRad, float lbRad, float rbRad, const Color& shadowColor, float shadowBlur, float shadowWidth, bool shadowInset, bool borderInset);
	void AddDrawBoxBorder2(const Rect& rect, const ThicknessF& thickness, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, const CornerRadius& cornerRadius, bool borderInset);
	void AddDrawBoxShadow(const Rect& rect, const CornerRadius& cornerRadius, const Color& color, float blur, float width, bool inset);
};

class ShapesRendererCommandListCache
	: public SimpleOneTimeObjectCache<ShapesRendererCommandList>
{
protected:
	virtual RefPtr<ShapesRendererCommandList> createObject() override;
};

class ShapesRendererCore
	: public RefObject
{
public:
	ShapesRendererCore();
	virtual ~ShapesRendererCore();
	void initialize(GraphicsManager* manager);

	void RenderCommandList(ShapesRendererCommandList* commandList, detail::BrushRawData* fillBrush);

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

	void ReleaseCommandList(ShapesRendererCommandList* commandList);
	void RequestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex);
	Path* AddPath(PathType type, const Color& color, PathWinding winding = PathWinding::CCW, PathAttribute attribute = PathAttribute::None);
	void EndPath(Path* path);
	void ExtractBasePoints(ShapesRendererCommandList* commandList);
	void MakeBasePointsAndBorderComponent(const Rect& rect, const ThicknessF& thickness, const CornerRadius& cornerRadius, BorderComponent components[4]);
	void CalcExtrudedDirection();
	void ExpandVertices(const Path& path);
	void ExpandFill(const Path& path);
	void ExpandStrip2PointStroke(const Path& path);
	void ExpandStrip3PointStroke(const Path& path);
	void ExpandAntiAliasStroke(const Path& path, int startIndex);
	void PlotCornerBasePointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center);
	Vector2 GetExtPos(const BasePoint& pt, float borderExtSign, float widht) const { return (pt.exDirect) ? pt.pos + pt.exDir * borderExtSign : pt.pos + pt.exDir * borderExtSign * widht; }
	Vector2 GetAAExtDir(const BasePoint& pt) const { return (pt.enabledAA) ? pt.exDir : Vector2::Zero; }

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

	void ExecuteCommand(ShapesRendererCommandList* commandList);

protected:
	virtual bool IsStandaloneShader() const { return false; }
	virtual void flush() override {}
	virtual void OnActivated() override {}
	virtual void OnDeactivated() override { flush(); }
	virtual void OnSetState(const DrawElementBatch* state);

private:
	GraphicsManager*				m_manager;
	RefPtr<ShapesRendererCore>		m_core;
	detail::BrushRawData			m_fillBrush;
};

} // namespace detail
LN_NAMESPACE_END
