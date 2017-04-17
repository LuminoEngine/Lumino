
#pragma once
#include <Lumino/Graphics/Brush.h>
#include <Lumino/Graphics/Vertex.h>
#include <Lumino/Graphics/ContextInterface.h>

LN_NAMESPACE_BEGIN
namespace detail {

// なぜコマンドが必要なのか？DrawList や RenderingCommand ではだめなのか？
//		LineTo を考えてみればわかりやすい。
//		点を1つ打つのに、DrawElement を１つ作りたいか？No.
//		このコマンドは、ShapesRenderer 内でプリミティブをキャッシュするためのものである。
//		ちなみに、RenderingCommand は描画スレッド有効でしか働かないので論外。
class ShapesRendererCommandList
	: public CommandDataCache
{
public:

	enum Command
	{
		Cmd_DrawBoxBorder,
		Cmd_DrawBoxShadow,
	};

	void AddDrawBoxBorder(float x, float y, float w, float h, float l, float t, float r, float b, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, float ltRad, float rtRad, float lbRad, float rbRad, const Color& shadowColor, float shadowBlur, float shadowWidth, bool shadowInset);
	void AddDrawBoxShadow(float x, float y, float w, float h, const Color& color, float blur, float width, bool inset);
};

class ShapesRendererCommandListCache
	: public SimpleOneTimeObjectCache<ShapesRendererCommandList>
{
protected:
	virtual RefPtr<ShapesRendererCommandList> CreateObject() override;
};

class ShapesRendererCore
	: public RefObject
{
public:
	ShapesRendererCore();
	virtual ~ShapesRendererCore();
	void Initialize(GraphicsManager* manager);

	void RenderCommandList(ShapesRendererCommandList* commandList, detail::BrushRawData* fillBrush);

private:
	enum class PathType
	{
		Convex,
		Strip2Point,	// TODO: 未使用になりそう
		Strip3Point,	// これは AA できない。シャドウ用
	};

	struct Path
	{
		PathType	type;
		int			pointStart;
		int			pointCount;
		Color		color;
	};

	struct BasePoint
	{
		Vector2	pos;
		Vector2	exDir;	// 押し出し方向 (right-dir)
	};

	struct OutlinePoint
	{
		Vector2	pos;
		Vector2	exDir;	// 押し出し方向 (AntiAlias 用)
		float	alpha;
	};

	void ReleaseCommandList(ShapesRendererCommandList* commandList);
	void RequestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex);
	Path* AddPath(PathType type, const Color& color);
	void EndPath(Path* path);
	void ExtractBasePoints(ShapesRendererCommandList* commandList);
	void CalcExtrudedDirection();
	void ExpandVertices(const Path& path);
	void ExpandFill(const Path& path);
	void ExpandStrip2PointStroke(const Path& path);
	void ExpandStrip3PointStroke(const Path& path);
	void ExpandAntiAliasStroke(const Path& path, int startIndex);
	void PlotCornerBasePointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT, const Vector2& center);

	GraphicsManager*			m_manager;
	Driver::IVertexBuffer*		m_vertexBuffer;
	Driver::IIndexBuffer*		m_indexBuffer;
	CacheBuffer<BasePoint>		m_basePoints;
	CacheBuffer<OutlinePoint>	m_outlinePoints;
	List<Path>					m_pathes;
	CacheBuffer<Vertex>			m_vertexCache;
	CacheBuffer<uint16_t>		m_indexCache;
};

class ShapesRenderer
	: public RefObject
	, public detail::IRendererPloxy
{
public:
	ShapesRenderer();
	virtual ~ShapesRenderer();
	void Initialize(GraphicsManager* manager);

	void ExecuteCommand(ShapesRendererCommandList* commandList);

protected:
	virtual bool IsStandaloneShader() const { return false; }
	virtual void Flush() override {}
	virtual void OnActivated() override {}
	virtual void OnDeactivated() override { Flush(); }
	virtual void OnSetState(const DrawElementBatch* state);

private:
	GraphicsManager*				m_manager;
	RefPtr<ShapesRendererCore>		m_core;
	detail::BrushRawData			m_fillBrush;
};

} // namespace detail
LN_NAMESPACE_END
