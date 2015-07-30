
#include "../Internal.h"
#include <Lumino/Graphics/Painter.h>
#include "PainterEngine.h"
#include "RenderingCommand.h"

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// Brush
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Brush::Brush()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Brush::~Brush()
{
}


//=============================================================================
// TextureBrush
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextureBrush::TextureBrush()
	: m_srcRect(0, 0, INT_MAX, INT_MAX)
	, m_wrapMode(BrushWrapMode_Stretch)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
TextureBrush::~TextureBrush()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void TextureBrush::Create(const TCHAR* filePath, GraphicsManager* manager)
{
	m_texture.Attach(Texture::Create(filePath, TextureFormat_R8G8B8A8, 1, manager));
}

//=============================================================================
// Painter
//=============================================================================


//=============================================================================
class SetProjectionCommand : public RenderingCommand
{
	PainterEngine* engine;
	Matrix matrix;
	//Size viewSize;

public:
	static void Create(CmdInfo& cmd, PainterEngine* engine, const Matrix& matrix/*, const Size& size*/)
	{
		HandleCast<SetProjectionCommand>(cmd)->engine = engine;
		HandleCast<SetProjectionCommand>(cmd)->matrix = matrix;
		//HandleCast<SetProjectionCommand>(cmd)->viewSize = size;
	}

private:
	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		engine->SetViewProjMatrix(matrix);
		//engine->SetViewPixelSize(viewSize);
	}
};

//=============================================================================
class DrawFillRectangleCommand : public RenderingCommand
{
	PainterEngine* engine;
	RectF rect;
	Device::ITexture* srcTexture;
	Rect srcRect;
	BrushWrapMode wrapMode;

public:
	static void Create(CmdInfo& cmd, PainterEngine* engine, const RectF& rect, Device::ITexture* srcTexture, const Rect& srcRect, BrushWrapMode wrapMode)
	{
		cmd.m_commandList->MarkGC(srcTexture);
		HandleCast<DrawFillRectangleCommand>(cmd)->engine = engine;
		HandleCast<DrawFillRectangleCommand>(cmd)->rect = rect;
		HandleCast<DrawFillRectangleCommand>(cmd)->srcTexture = srcTexture;
		HandleCast<DrawFillRectangleCommand>(cmd)->srcRect = srcRect;
		HandleCast<DrawFillRectangleCommand>(cmd)->wrapMode = wrapMode;
	}

	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		engine->DrawFillRectangle(rect, srcTexture, srcRect, wrapMode);
	}
};

//=============================================================================
class DrawFrameRectangleCommand : public RenderingCommand
{
	PainterEngine* engine;
	RectF rect;
	float frameWidth;
	Device::ITexture* srcTexture;
	Rect srcRect;

public:
	static void Create(CmdInfo& cmd, PainterEngine* engine, const RectF& rect, float frameWidth, Device::ITexture* srcTexture, const Rect& srcRect)
	{
		cmd.m_commandList->MarkGC(srcTexture);
		HandleCast<DrawFrameRectangleCommand>(cmd)->engine = engine;
		HandleCast<DrawFrameRectangleCommand>(cmd)->rect = rect;
		HandleCast<DrawFrameRectangleCommand>(cmd)->frameWidth = frameWidth;
		HandleCast<DrawFrameRectangleCommand>(cmd)->srcTexture = srcTexture;
		HandleCast<DrawFrameRectangleCommand>(cmd)->srcRect = srcRect;
	}

	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		engine->DrawFrameRectangle(rect, frameWidth, srcTexture, srcRect);
	}
};


//=============================================================================
class DrawGlyphRunCommand : public RenderingCommand
{
	PainterEngine* engine;
	size_t dataList;
	int dataCount;
	Device::ITexture* glyphsTexture;
	Device::ITexture* strokesTexture;
	ColorF foreColor;
	ColorF strokeColor;

public:
	static void Create(CmdInfo& cmd, 
		PainterEngine* engine, 
		PainterEngine::GlyphRunData* dataList,
		int dataCount,
		Device::ITexture* glyphsTexture,
		Device::ITexture* strokesTexture,
		const ColorF& foreColor, 
		const ColorF& strokeColor)
	{
		// ※以前はこの中でキャッシュからグリフを読み取っていたりしたが、その中で Texture.Lock が呼ばれたため
		//   Alloc() の再帰が起こってしまった。
		size_t dataHandle = Alloc(cmd, sizeof(PainterEngine::GlyphRunData) * dataCount, dataList);
		cmd.m_commandList->MarkGC(glyphsTexture);
		if (strokesTexture != NULL) { cmd.m_commandList->MarkGC(strokesTexture); }
		HandleCast<DrawGlyphRunCommand>(cmd)->engine = engine;
		HandleCast<DrawGlyphRunCommand>(cmd)->dataList = dataHandle;
		HandleCast<DrawGlyphRunCommand>(cmd)->dataCount = dataCount;
		HandleCast<DrawGlyphRunCommand>(cmd)->glyphsTexture = glyphsTexture;
		HandleCast<DrawGlyphRunCommand>(cmd)->strokesTexture = strokesTexture;
		HandleCast<DrawGlyphRunCommand>(cmd)->foreColor = foreColor;
		HandleCast<DrawGlyphRunCommand>(cmd)->strokeColor = strokeColor;
	}

	virtual void Execute(RenderingCommandList* commandList, Device::IRenderer* renderer)
	{
		engine->DrawGlyphRun((PainterEngine::GlyphRunData*)commandList->GetBuffer(dataList), dataCount, glyphsTexture, strokesTexture, foreColor, strokeColor);
	}
};


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Painter::Painter(GraphicsManager* manager)
	: m_manager(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Painter::~Painter()
{
}

/// ピクセル単位の2D描画に使う射影行列の作成
static void perspective2DLH(Matrix* out_, float width_, float height_, float near_, float far_)
{
	// ※このオフセット調整が必要なのは DX9 のみ。本来であればシェーダ内でたいおうするものなので、
	// 後々この処理は削除する。
#if 1
	out_->Set(
		2.0f / width_, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height_, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (far_ - near_), 0.0f,
		-1.0f, 1.0f, near_ / (near_ - far_), 1.0f);
#else
	// 平行移動成分は、画面半分 + 0.5 ピクセルの調整
	out_->set(
		2.0f / width_, 0.0f, 0.0f, 0.0f,
		0.0f, -2.0f / height_, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (far_ - near_), 0.0f,
		-1.0f - (0.5f / width_), 1.0f + (0.5f / height_), near_ / (near_ - far_), 1.0f);
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::SetProjection(const Size& viewSize, float nearZ, float farZ)
{
	Matrix mat;
	perspective2DLH(&mat, (float)viewSize.Width, (float)viewSize.Height, nearZ, farZ);
	m_manager->GetPrimaryRenderingCommandList()->AddCommand<SetProjectionCommand>(m_manager->GetPainterEngine(), mat/*, viewSize*/);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::SetBrush(Brush* brush)
{
	m_currentBrush = brush;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawRectangle(const RectF& rect)
{
	if (m_currentBrush->GetType() == BrushType_Texture)
	{
		TextureBrush* b = static_cast<TextureBrush*>(m_currentBrush.GetObjectPtr());
		m_manager->GetPrimaryRenderingCommandList()->AddCommand<DrawFillRectangleCommand>(
			m_manager->GetPainterEngine(), rect, b->GetTexture()->GetDeviceObject(), b->GetSourceRect(), b->GetWrapMode());
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawFrameRectangle(const RectF& rect, float frameWidth)
{
	Device::ITexture* srcTexture;
	Rect srcRect(0, 0, INT_MAX, INT_MAX);
	if (m_currentBrush != NULL && m_currentBrush->GetType() == BrushType_Texture)
	{
		TextureBrush* tb = static_cast<TextureBrush*>(m_currentBrush.GetObjectPtr());
		if (tb->GetTexture() != NULL)
		{
			srcTexture = tb->GetTexture()->GetDeviceObject();
			srcRect = tb->GetSourceRect();
		}
	}

	m_manager->GetPrimaryRenderingCommandList()->AddCommand<DrawFrameRectangleCommand>(
		m_manager->GetPainterEngine(), rect, frameWidth, srcTexture, srcRect);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawTexture(const RectF& dstRect, Texture* texture, const Rect& srcRect)
{
	m_manager->GetPrimaryRenderingCommandList()->AddCommand<DrawFillRectangleCommand>(
		m_manager->GetPainterEngine(), dstRect, texture->GetDeviceObject(), srcRect, BrushWrapMode_Stretch);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Painter::DrawGlyphRun(GlyphRun* glyphRun)
{
	if (glyphRun == NULL) { return; }

	// 一時メモリ確保
	m_tempBuffer.Resize(sizeof(PainterEngine::GlyphRunData) * glyphRun->m_glyphData.Items.GetCount());
	auto data = (PainterEngine::GlyphRunData*)m_tempBuffer.GetData();

	// 確保したメモリにテクスチャ描画情報を作っていく
	Texture* tex1 = NULL;
	Texture* tex2 = NULL;	// TODO: ストローク
	int count = glyphRun->m_glyphData.Items.GetCount();
	for (int i = 0; i < count; ++i)
	{
		Rect srcRect;
		Imaging::TextLayoutResultItem& item = glyphRun->m_glyphData.Items[i];
		glyphRun->m_glyphTextureCache->LookupGlyph(item.Char, &tex1, &srcRect);

		data[i].Position.Set((float)item.Location.OuterTopLeftPosition.X, (float)item.Location.OuterTopLeftPosition.Y);
		data[i].SrcPixelRect.Set(srcRect.X, srcRect.Y, srcRect.Width, srcRect.Height);
	}

	// コマンド化
	m_manager->GetPrimaryRenderingCommandList()->AddCommand<DrawGlyphRunCommand>(
		m_manager->GetPainterEngine(), data, count, tex1->GetDeviceObject(), (tex2) ? tex2->GetDeviceObject() : NULL, ColorF::Black, ColorF::Blue);	// TODO: 色

}

} // namespace Graphics
} // namespace Lumino
