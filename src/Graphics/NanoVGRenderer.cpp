
#include "Internal.h"
#include "../../external/nanovg/nanovg.h"
#include <Lumino/Graphics/Texture.h>
#include "Device/GraphicsDriverInterface.h"
#include "GraphicsManager.h"
#include "NanoVGRenderer.h"

LN_NAMESPACE_BEGIN
namespace detail
{

class IndexStack
{
public:
	IndexStack()
		: m_indexStack()
		, m_capacity()
	{
	}

	void Reserved(int size)
	{
		m_indexStack.Clear();
		m_capacity = 0;
		Grow(size);
	}

	int Pop()
	{
		int top;
		m_indexStack.Pop(&top);
		return top;
	}

	bool IsEmpty() const
	{
		return m_indexStack.IsEmpty();
	}

	void Grow(int size)
	{
		for (int i = 0; i < size; i++)
		{
			m_indexStack.Push(m_capacity + i);
		}
		m_capacity += size;
	}

private:
	ln::Stack<int>	m_indexStack;
	int				m_capacity;
};

enum NVGcreateFlags
{
	// Flag indicating if geometry based anti-aliasing is used (may not be needed when using MSAA).
	NVG_ANTIALIAS = 1 << 0,
	// Flag indicating if strokes should be drawn using stencil buffer. The rendering will be a little
	// slower, but path overlaps (i.e. self-intersecting or sharp turns) will be drawn just once.
	NVG_STENCIL_STROKES = 1 << 1,
	// Flag indicating that additional debug checks are done.
	NVG_DEBUG = 1 << 2,
};

struct LNNVGcontext
{
	int	flags = 0;
	float view[2];

	GraphicsManager*	manager;
	IndexStack			textureIndexStack;
	List<TexturePtr>	textureList;
};

//------------------------------------------------------------------------------
static int glnvg__renderCreate(void* uptr)
{
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;


	return 1;
}

//------------------------------------------------------------------------------
static int glnvg__renderCreateTexture(void* uptr, int type, int w, int h, int imageFlags, const unsigned char* data)
{
	// 今のところこのフラグは使われていないようだ
	assert((imageFlags & NVG_IMAGE_REPEATX) == 0);
	assert((imageFlags & NVG_IMAGE_REPEATY) == 0);
	assert((imageFlags & NVG_IMAGE_FLIPY) == 0);
	assert((imageFlags & NVG_IMAGE_PREMULTIPLIED) == 0);

	LNNVGcontext* lnc = (LNNVGcontext*)uptr;

	//TextureFormat format;
	//if ((type & NVG_TEXTURE_ALPHA) != 0)
	//	format = TextureFormat::A8;
	//else if ((type & NVG_TEXTURE_RGBA) != 0)
	//	format = TextureFormat::R8G8B8A8;

	bool mipmap = ((imageFlags & NVG_IMAGE_GENERATE_MIPMAPS) != 0);

	//auto tex = Texture2DPtr::MakeRef();
	//tex->Initialize(lnc->manager, SizeI(w, h), format, mipmap);

	//NVG_TEXTURE_ALPHA = 0x01,
	//NVG_TEXTURE_RGBA = 0x02,

	//NVG_IMAGE_GENERATE_MIPMAPS = 1 << 0,     // Generate mipmaps during creation of the image.
	//NVG_IMAGE_REPEATX = 1 << 1,		// Repeat image in X direction.
	//NVG_IMAGE_REPEATY = 1 << 2,		// Repeat image in Y direction.
	//NVG_IMAGE_FLIPY = 1 << 3,		// Flips (inverses) image in Y direction when rendered.
	//NVG_IMAGE_PREMULTIPLIED = 1 << 4,		// Image data has premultiplied alpha.

	

	return 9;//tex->id;
}

//------------------------------------------------------------------------------
static int glnvg__renderDeleteTexture(void* uptr, int image)
{
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;
	return 1;
}

//------------------------------------------------------------------------------
static int glnvg__renderUpdateTexture(void* uptr, int image, int x, int y, int w, int h, const unsigned char* data)
{
	// テキスト描画機能を使うときに必要になる
	assert(0);
	return 0;
}

//------------------------------------------------------------------------------
static int glnvg__renderGetTextureSize(void* uptr, int image, int* w, int* h)
{
	// テキスト描画機能を使うときに必要になる
	assert(0);
	return 0;
}

//------------------------------------------------------------------------------
// レンダリング開始時に呼び出される
static void glnvg__renderViewport(void* uptr, int width, int height, float devicePixelRatio)
{
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;
	lnc->view[0] = (float)width;
	lnc->view[1] = (float)height;
}

//------------------------------------------------------------------------------
// nvgCancelFrame から呼び出される
static void glnvg__renderCancel(void* uptr)
{
	assert(0);
}

//------------------------------------------------------------------------------
static void glnvg__renderFlush(void* uptr, NVGcompositeOperationState compositeOperation)
{
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;
}

//------------------------------------------------------------------------------
static void glnvg__renderFill(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe,
	const float* bounds, const NVGpath* paths, int npaths)
{
}

//------------------------------------------------------------------------------
static void glnvg__renderStroke(void* uptr, NVGpaint* paint, NVGscissor* scissor, float fringe,
	float strokeWidth, const NVGpath* paths, int npaths)
{
}

//------------------------------------------------------------------------------
static void glnvg__renderTriangles(void* uptr, NVGpaint* paint, NVGscissor* scissor,
	const NVGvertex* verts, int nverts)
{
}

//------------------------------------------------------------------------------
static void glnvg__renderDelete(void* uptr)
{
	LNNVGcontext* lnc = (LNNVGcontext*)uptr;
	if (lnc == NULL) return;

	LN_SAFE_DELETE(lnc);
}

//------------------------------------------------------------------------------
NVGcontext* nvgCreateLNContext(GraphicsManager* manager, int flags)
{
	LNNVGcontext* lnc = LN_NEW LNNVGcontext();
	lnc->manager = manager;

	NVGparams params;
	memset(&params, 0, sizeof(params));
	params.renderCreate = glnvg__renderCreate;
	params.renderCreateTexture = glnvg__renderCreateTexture;
	params.renderDeleteTexture = glnvg__renderDeleteTexture;
	params.renderUpdateTexture = glnvg__renderUpdateTexture;
	params.renderGetTextureSize = glnvg__renderGetTextureSize;
	params.renderViewport = glnvg__renderViewport;
	params.renderCancel = glnvg__renderCancel; 
	params.renderFlush = glnvg__renderFlush;
	params.renderFill = glnvg__renderFill;
	params.renderStroke = glnvg__renderStroke;
	params.renderTriangles = glnvg__renderTriangles;
	params.renderDelete = glnvg__renderDelete;
	params.userPtr = lnc;
	params.edgeAntiAlias = flags & NVG_ANTIALIAS ? 1 : 0;

	lnc->flags = flags;

	NVGcontext* ctx = nvgCreateInternal(&params);
	if (ctx == NULL) goto error;

	return ctx;

error:
	if (ctx != NULL) nvgDeleteInternal(ctx);
	return NULL;
}

//------------------------------------------------------------------------------
NanoVGRenderer::NanoVGRenderer()
{
}

//------------------------------------------------------------------------------
NanoVGRenderer::~NanoVGRenderer()
{
}

//------------------------------------------------------------------------------
void NanoVGRenderer::Initialize(GraphicsManager* manager)
{
}


} // namespace detail
LN_NAMESPACE_END

