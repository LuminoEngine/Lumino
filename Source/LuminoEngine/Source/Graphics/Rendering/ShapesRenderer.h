
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

	void AddDrawBoxBorder(float x, float y, float w, float h, float l, float t, float r, float b, const Color& leftColor, const Color& topColor, const Color& rightColor, const Color& bottomColor, float ltRad, float rtRad, float lbRad, float rbRad);
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
	struct Path
	{
		int		pointStart;
		int		pointCount;
		Color	color;
		bool	close;
	};

	struct BasePoint
	{
		Vector3	pos;
	};

	void ReleaseCommandList(ShapesRendererCommandList* commandList);
	void RequestBuffers(int vertexCount, int indexCount, Vertex** vb, uint16_t** ib, uint16_t* outBeginVertexIndex);
	Path* AddPath();
	void EndPath(Path* path, bool close);
	void ExtractBasePoints(ShapesRendererCommandList* commandList);
	void CalcExtrudedDirection();
	void ExpandFill();
	void PlotBasePointsBezier(const Vector2& first, const Vector2& firstCpDir, const Vector2& last, const Vector2& lastCpDir, float firstT, float lastT);

	GraphicsManager*		m_manager;
	Driver::IVertexBuffer*	m_vertexBuffer;
	Driver::IIndexBuffer*	m_indexBuffer;
	CacheBuffer<BasePoint>	m_points;
	List<Path>				m_pathes;
	CacheBuffer<Vertex>		m_vertexCache;
	CacheBuffer<uint16_t>	m_indexCache;
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
