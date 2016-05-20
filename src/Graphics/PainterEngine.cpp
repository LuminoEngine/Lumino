/*
	ユーザーのシェーダとの共存の仕方。
	
	ストローク付きのテキストをテクスチャブラシ描画しようとしたら、
	テクスチャ2枚をレジスタに送るか、2パス描画が必須。
	また、テクスチャブラシのタイル配置のためには特殊な意味を持つUVを
	頂点バッファに持たせなければならない。

	ユーザーシェーダ側をシンプルにするなら、WPFのように
	1度別のRTに描画し、それを矩形で描画する2パスで行うのが良い。
	このままだとメモリ使用量やドローコールが増えるが、
	ユーザーシェーダを使わない場合はこの処理を行わないようにしたり、
	一時RTはできるだけ使いまわすようにすれば少しはましかも。

*/
#include "../Internal.h"
#include <math.h>
#include <Lumino/Graphics/BitmapPainter.h>
#include <Lumino/Graphics/GraphicsException.h>
#include "PainterEngine.h"
#include "GraphicsHelper.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//==============================================================================
// PainterEngine
//==============================================================================

static const byte_t g_Painter_fx_Data[] =
{
#include "Resource/Painter.fx.h"
};
static const size_t g_Painter_fx_Len = LN_ARRAY_SIZE_OF(g_Painter_fx_Data);
	

//------------------------------------------------------------------------------
PainterEngine::PainterEngine()
{
}

//------------------------------------------------------------------------------
PainterEngine::~PainterEngine()
{
	while (!m_sectionStack.IsEmpty())
	{
		m_sectionStack.GetTop().CurrentState.ReleaseObjects();
		m_sectionStack.Pop();
	}
}

//------------------------------------------------------------------------------
void PainterEngine::Create(GraphicsManager* manager)
{
	GraphicsResourceObject::Initialize(manager);
	CreateInternal();

	//memset(&m_currentState.Brush, 0, sizeof(m_currentState.Brush));
	//m_currentState.Opacity = 1.0f;
	m_currentInternalGlyphMask = m_dummyTexture;
}

//------------------------------------------------------------------------------
void PainterEngine::CreateInternal()
{
	auto* device = m_manager->GetGraphicsDevice();

	const int DefaultFaceCount = 1024;

	m_vertexBuffer.Attach(device->CreateVertexBuffer(
		PainterVertex::Elements(), PainterVertex::ElementCount, DefaultFaceCount * 4, NULL, DeviceResourceUsage_Dynamic));
	m_indexBuffer.Attach(device->CreateIndexBuffer(
		DefaultFaceCount * 6, NULL, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic));

	m_vertexCache.Reserve(1024);
	m_indexCache.Reserve(1024);

	//RefPtr<ByteBuffer> code(FileUtils::ReadAllBytes(_T("D:/Proj/Lumino/src/Graphics/Resource/Painter.fx")));
	ShaderCompileResult r;
	m_shader.Shader.Attach(device->CreateShader(g_Painter_fx_Data, g_Painter_fx_Len, &r));
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);

	m_shader.Technique = m_shader.Shader->GetTechnique(0);
	m_shader.Pass = m_shader.Technique->GetPass(0);
	m_shader.varWorldMatrix = m_shader.Shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.Shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTone = m_shader.Shader->GetVariableByName(_T("g_tone"));
	m_shader.varTexture = m_shader.Shader->GetVariableByName(_T("g_texture"));
	m_shader.varGlyphMaskSampler = m_shader.Shader->GetVariableByName(_T("g_glyphMaskTexture"));
	m_shader.varViewportSize = m_shader.Shader->GetVariableByName(_T("g_viewportSize"));

	m_renderer = device->GetRenderer();

	m_shader.varWorldMatrix->SetMatrix(Matrix::Identity);
	m_shader.varViewProjMatrix->SetMatrix(Matrix::Identity);


	//-----------------------------------------------------
	// ダミーテクスチャ

	m_dummyTexture.Attach(device->CreateTexture(Size(32, 32), 1, TextureFormat::R8G8B8A8, NULL), false);
	Driver::IGraphicsDevice::ScopedLockContext lock(device);
	BitmapPainter painter(m_dummyTexture->Lock());
	painter.Clear(Color::White);
	m_dummyTexture->Unlock();
}

//------------------------------------------------------------------------------
void PainterEngine::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL)
	{
		// TODO: TextureBrush の復元は困難。End で Detach する仕様にしたい。
		while (!m_sectionStack.IsEmpty())
		{
			m_sectionStack.GetTop().CurrentState.ReleaseObjects();
			m_sectionStack.Pop();
		}

		m_vertexBuffer.SafeRelease();
		m_indexBuffer.SafeRelease();
		m_shader.Shader.SafeRelease();
		m_renderer = NULL;
		m_dummyTexture.SafeRelease();
		m_currentInternalGlyphMask.SafeRelease();
	}
	else
	{
		CreateInternal();
	}
}

//------------------------------------------------------------------------------
//void PainterEngine::SetTransform(const Matrix& matrix)
//{
//
//}

//------------------------------------------------------------------------------
void PainterEngine::Begin()
{
	Flush();
	m_sectionStack.Push(PainterEngineSection());

	m_shader.varWorldMatrix->SetMatrix(Matrix::Identity);
	//m_shader.varViewProjMatrix->SetMatrix(Matrix::Identity);

	GetCurrentState().ReleaseObjects();
	GetCurrentState().Brush.Type = BrushType_Unknown;
	GetCurrentState().Opacity = 1.0f;
	GetCurrentState().ForeColor = ColorF::Black;
	m_currentInternalGlyphMask = m_dummyTexture;
}

//------------------------------------------------------------------------------
void PainterEngine::End()
{
	Flush();
	m_sectionStack.GetTop().CurrentState.ReleaseObjects();
	m_sectionStack.Pop();
}

//------------------------------------------------------------------------------
void PainterEngine::Flush()
{
	if (m_indexCache.GetCount() == 0) { return; }

	Driver::ITexture* srcTexture = NULL;
	PainterEngineState& state = GetCurrentState();
	if (state.Brush.Type == BrushType_Texture) {
		srcTexture = state.Brush.TextureBrush.Texture;
	}
	else if (state.Brush.Type == BrushType_FrameTexture) {
		srcTexture = state.Brush.FrameTextureBrush.Texture;
	}

	if (srcTexture == NULL) {
		srcTexture = m_dummyTexture;
	}

	// 描画する
	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	m_shader.varTone->SetVector((Vector4&)state.Tone);
	m_shader.varTexture->SetTexture(srcTexture);
	m_shader.varGlyphMaskSampler->SetTexture(m_currentInternalGlyphMask);
	m_shader.Pass->Apply();
	m_renderer->DrawPrimitiveIndexed(m_vertexBuffer, m_indexBuffer, PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);

	// キャッシュクリア
	m_vertexCache.Clear();
	m_indexCache.Clear();
}

//------------------------------------------------------------------------------
void PainterEngine::SetTransform(const Matrix& matrix)
{
	// TODO: 遅延
	m_shader.varWorldMatrix->SetMatrix(matrix);
}

//------------------------------------------------------------------------------
void PainterEngine::SetViewProjMatrix(const Matrix& matrix)
{
	m_shader.varViewProjMatrix->SetMatrix(matrix);
}

//------------------------------------------------------------------------------
//void PainterEngine::SetViewPixelSize(const Size& size)
//{
//	if (m_shader.varViewportSize != NULL) {
//		m_shader.varViewportSize->SetVector(Vector4((float)size.Width, (float)size.Height, 0, 0));
//	}
//}

//------------------------------------------------------------------------------
void PainterEngine::SetState(const PainterEngineState& state)
{
	Flush();	// 描画設定が変わるのでここでフラッシュ
	GetCurrentState().Copy(state);
	UpdateCurrentForeColor();
}

//------------------------------------------------------------------------------
//void PainterEngine::SetBrush(const BrushData& data)
//{
//	Flush();	// 描画設定が変わるのでここでフラッシュ
//	m_currentState.ReleaseObjects();
//	memcpy(&m_currentState.Brush, &data, sizeof(m_currentState.Brush));
//	AttachBrushData();
//	UpdateCurrentForeColor();
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void PainterEngine::SetOpacity(float opacity)
//{
//	Flush();	// 描画設定が変わるのでここでフラッシュ
//	m_currentState.Opacity = opacity;
//	UpdateCurrentForeColor();
//}
//
//void SetTone(const ToneF& tone)
//{
//}

//------------------------------------------------------------------------------
void PainterEngine::DrawRectangle(const RectF& rect)
{
	// DrawGlyphRun() 以外は NULL で呼び出しておく
	SetInternalGlyphMaskTexture(NULL);

	PainterEngineState& state = GetCurrentState();
	if (state.Brush.Type == BrushType_Texture)
	{
		Driver::ITexture* srcTexture = state.Brush.TextureBrush.Texture;
		Rect& srcRect = *((Rect*)state.Brush.TextureBrush.SourceRect);

		SizeF texSize((float)srcTexture->GetRealSize().width, (float)srcTexture->GetRealSize().height);
		texSize.width = 1.0f / texSize.width;
		texSize.height = 1.0f / texSize.height;
		RectF uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);


		if (state.Brush.TextureBrush.WrapMode == BrushWrapMode_Stretch)
		{
			InternalDrawRectangleStretch(rect, uvSrcRect);
		}
		else {
			LN_THROW(0, NotImplementedException);
		}
	}
	else if (state.Brush.Type == BrushType_FrameTexture)
	{
		auto& brush = state.Brush.FrameTextureBrush;

		// 枠
		{
			//Rect& srcRect = *((Rect*)brush.SourceRect);
			DrawFrameRectangle(rect, (float)brush.FrameThicness);
		}

		// Inner
		{
			Rect& srcRect = *((Rect*)brush.InnerSourceRect);
			Driver::ITexture* srcTexture = brush.Texture;
			SizeF texSize((float)srcTexture->GetRealSize().width, (float)srcTexture->GetRealSize().height);
			texSize.width = 1.0f / texSize.width;
			texSize.height = 1.0f / texSize.height;
			RectF uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

			if (brush.WrapMode == BrushWrapMode_Stretch)
			{
				RectF tr = rect;
				tr.Inflate((float)-brush.FrameThicness, (float)-brush.FrameThicness);
				InternalDrawRectangleStretch(tr, uvSrcRect);
			}
			else {
				LN_THROW(0, NotImplementedException);
			}

		}
	}
	else
	{
		uint16_t i = m_vertexCache.GetCount();
		m_indexCache.Add(i + 0);
		m_indexCache.Add(i + 1);
		m_indexCache.Add(i + 2);
		m_indexCache.Add(i + 2);
		m_indexCache.Add(i + 1);
		m_indexCache.Add(i + 3);

		PainterVertex v;
		v.Color = state.ForeColor;
		v.Position.Set(rect.GetLeft(), rect.GetTop(), 0);		v.UVOffset.Set(0, 0, 0, 0); v.UVTileUnit.Set(0, 0);	// 左上
		m_vertexCache.Add(v);
		v.Position.Set(rect.GetRight(), rect.GetTop(), 0);		v.UVOffset.Set(0, 0, 0, 0); v.UVTileUnit.Set(0, 0);	// 右上
		m_vertexCache.Add(v);
		v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0);	v.UVOffset.Set(0, 0, 0, 0); v.UVTileUnit.Set(0, 0);	// 左下
		m_vertexCache.Add(v);
		v.Position.Set(rect.GetRight(), rect.GetBottom(), 0);	v.UVOffset.Set(0, 0, 0, 0); v.UVTileUnit.Set(0, 0);	// 右下
		m_vertexCache.Add(v);
	}
}


#if 0
//------------------------------------------------------------------------------
void PainterEngine::DrawFillRectangle(const RectF& rect, Driver::ITexture* srcTexture, const Rect& srcRect, BrushWrapMode wrapMode)
{
	m_vertexCache.Clear();
	m_indexCache.Clear();

	SizeF texSizeInv(1.0f / srcTexture->GetSize().Width, 1.0f / srcTexture->GetSize().Height);
	RectF uvSrcRect(srcRect);
	uvSrcRect.X			*= texSizeInv.Width;
	uvSrcRect.Width		*= texSizeInv.Width;
	uvSrcRect.Y			*= texSizeInv.Height;
	uvSrcRect.Height	*= texSizeInv.Height;

	// 拡縮モード
	if (wrapMode == BrushWrapMode_Stretch) 
	{
		InternalDrawRectangleStretch(rect, uvSrcRect);

		m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
		m_renderer->SetVertexBuffer(m_vertexBuffer);
		m_shader.varTexture->SetTexture(srcTexture);
		m_shader.Pass->Apply();
		m_renderer->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);
	}
	// タイリングモード
	else if (wrapMode == BrushWrapMode_Tile) {
		LN_THROW(0, NotImplementedException);
	}
	else {
		LN_THROW(0, NotImplementedException);
	}
}
#endif

//------------------------------------------------------------------------------
void PainterEngine::DrawFrameRectangle(const RectF& rect, float frameWidth/*, Driver::ITexture* srcTexture, const Rect& srcRect_*/)
{
	Driver::ITexture* srcTexture = NULL;
	Rect srcRect;
	PainterEngineState& state = GetCurrentState();
	if (state.Brush.Type == BrushType_Texture)
	{
		srcTexture = state.Brush.TextureBrush.Texture;
		srcRect = *((Rect*)state.Brush.TextureBrush.SourceRect);
	}
	else if (state.Brush.Type == BrushType_FrameTexture)
	{
		srcTexture = state.Brush.FrameTextureBrush.Texture;
		srcRect = *((Rect*)state.Brush.FrameTextureBrush.SourceRect);
	}
	else {
		// テクスチャブラシ以外で書くことはできない
		// TODO: 黒で矩形を書いた方が、間違っているとわかりやすいかも
		return;
	}

	// DrawGlyphRun() 以外は NULL で呼び出しておく
	SetInternalGlyphMaskTexture(NULL);

	if (srcTexture == NULL) {
		srcTexture = m_dummyTexture;
	}

	if (srcRect.width == INT_MAX) {
		srcRect.width = srcTexture->GetSize().width;
	}
	if (srcRect.height == INT_MAX) {
		srcRect.height = srcTexture->GetSize().height;
	}

	m_vertexCache.Clear();
	m_indexCache.Clear();

	SizeF texSize((float)srcTexture->GetRealSize().width, (float)srcTexture->GetRealSize().height);
	texSize.width = 1.0f / texSize.width;
	texSize.height = 1.0f / texSize.height;
	RectF uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);
	

	float frameWidthH = frameWidth;
	float frameWidthV = frameWidth;
	float uvFrameWidthH = frameWidth * texSize.width;
	float uvFrameWidthV = frameWidth * texSize.height;
	int frameWidthHI = (int)frameWidth;	// 型変換回数を減らすため、あらかじめ int 化しておく
	int frameWidthVI = (int)frameWidth;

	// 横幅が小さいため、枠幅も狭めなければならない
	if (rect.width < frameWidthH * 2)
	{
		float ratio = rect.width / (frameWidthH * 2);	// 元の幅から何 % になるか
		frameWidthH *= ratio;
		uvFrameWidthH *= ratio;
		frameWidthHI = (int)ceil( ratio * frameWidthHI);
	}
	// 縦幅が小さいため、枠幅も狭めなければならない
	if (rect.height < frameWidthV * 2)
	{
		float ratio = rect.height / (frameWidthV * 2);	// 元の幅から何 % になるか
		frameWidthV *= ratio;
		uvFrameWidthV *= ratio;
		frameWidthVI = (int)ceil(ratio * frameWidthVI);
	}

	RectF outerRect = rect;
	RectF innerRect(outerRect.x + frameWidthH, outerRect.y + frameWidthV, outerRect.width - frameWidthH * 2, outerRect.height - frameWidthV * 2);
	RectF outerUVRect = uvSrcRect;
	RectF innerUVRect(outerUVRect.x + uvFrameWidthH, outerUVRect.y + uvFrameWidthV, outerUVRect.width - uvFrameWidthH * 2, outerUVRect.height - uvFrameWidthV * 2);
	Rect  outerSrcRect = srcRect;
	Rect  innerSrcRect(outerSrcRect.x + frameWidthHI, outerSrcRect.y + frameWidthVI, outerSrcRect.width - frameWidthHI * 2, outerSrcRect.height - frameWidthVI * 2);

	// 左上	■□□
	//		□　□
	//		□□□
	InternalDrawRectangleTiling(
		RectF(outerRect.GetLeft(),		outerRect.GetTop(),		frameWidthH,	frameWidthV),
		Rect(outerSrcRect.GetLeft(),	outerSrcRect.GetTop(),	frameWidthHI,	frameWidthVI),
		RectF(outerUVRect.GetLeft(),	outerUVRect.GetTop(),	uvFrameWidthH,	uvFrameWidthV),
		srcTexture);

	// 上	□■□
	//		□　□
	//		□□□
	InternalDrawRectangleTiling(
		RectF(innerRect.GetLeft(),		outerRect.GetTop(),		innerRect.width,	frameWidth),
		Rect(innerSrcRect.GetLeft(),	outerSrcRect.GetTop(),	innerSrcRect.width,	frameWidthVI),
		RectF(innerUVRect.GetLeft(),	outerUVRect.GetTop(),	innerUVRect.width,	uvFrameWidthV),
		srcTexture);

	// 右上	□□■
	//		□　□
	//		□□□
	InternalDrawRectangleTiling(
		RectF(innerRect.GetRight(),		outerRect.GetTop(),		frameWidthH,	frameWidthV),
		Rect(innerSrcRect.GetRight(),	outerSrcRect.GetTop(),	frameWidthHI,	frameWidthVI),
		RectF(innerUVRect.GetRight(),	outerUVRect.GetTop(),	uvFrameWidthH,	uvFrameWidthV),
		srcTexture);

	// 右	□□□
	//		□　■
	//		□□□
	InternalDrawRectangleTiling(
		RectF(innerRect.GetRight(),		innerRect.GetTop(),		frameWidthH,		innerRect.height),
		Rect(innerSrcRect.GetRight(),	innerSrcRect.GetTop(),	frameWidthHI,	innerSrcRect.height),
		RectF(innerUVRect.GetRight(),	innerUVRect.GetTop(),	uvFrameWidthH,	innerUVRect.height),
		srcTexture);

	// 右下	□□□
	//		□　□
	//		□□■
	InternalDrawRectangleTiling(
		RectF(innerRect.GetRight(),		innerRect.GetBottom(),		frameWidthH,	frameWidthV),
		Rect(innerSrcRect.GetRight(),	innerSrcRect.GetBottom(),	frameWidthHI,	frameWidthVI),
		RectF(innerUVRect.GetRight(),	innerUVRect.GetBottom(),	uvFrameWidthH,	uvFrameWidthV),
		srcTexture);

	// 下	□□□
	//		□　□
	//		□■□
	InternalDrawRectangleTiling(
		RectF(innerRect.GetLeft(),		innerRect.GetBottom(),		innerRect.width,	frameWidthV),
		Rect(innerSrcRect.GetLeft(),	innerSrcRect.GetBottom(),	innerSrcRect.width,	frameWidthVI),
		RectF(innerUVRect.GetLeft(),	innerUVRect.GetBottom(),	innerUVRect.width,	uvFrameWidthV),
		srcTexture);

	// 左下	□□□
	//		□　□
	//		■□□
	InternalDrawRectangleTiling(
		RectF(outerRect.GetLeft(),		innerRect.GetBottom(),		frameWidthH,	frameWidthV),
		Rect(outerSrcRect.GetLeft(),	innerSrcRect.GetBottom(),	frameWidthHI,	frameWidthVI),
		RectF(outerUVRect.GetLeft(),	innerUVRect.GetBottom(),	uvFrameWidthH,	uvFrameWidthV),
		srcTexture);

	// 左	□□□
	//		■　□
	//		□□□
	InternalDrawRectangleTiling(
		RectF(outerRect.GetLeft(),		innerRect.GetTop(),		frameWidthH,	innerRect.height),
		Rect(outerSrcRect.GetLeft(),	innerSrcRect.GetTop(),	frameWidthHI,	innerSrcRect.height),
		RectF(outerUVRect.GetLeft(),	innerUVRect.GetTop(),	uvFrameWidthH,	innerUVRect.height),
		srcTexture);

	//m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	//m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	//m_renderer->SetVertexBuffer(m_vertexBuffer);
	//m_renderer->SetIndexBuffer(m_indexBuffer);
	//m_shader.varTexture->SetTexture(srcTexture);
	//m_shader.Pass->Apply();
	//m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, 16);
}

//------------------------------------------------------------------------------
void PainterEngine::DrawGlyphRun(const PointF& position, const GlyphRunData* dataList, int dataCount, Driver::ITexture* glyphsTexture, Driver::ITexture* strokesTexture/*, const ColorF& foreColor, const ColorF& strokeColor*/)
{
	SetInternalGlyphMaskTexture(glyphsTexture);

	SizeF texSizeInv(1.0f / glyphsTexture->GetRealSize().width, 1.0f / glyphsTexture->GetRealSize().height);
	for (int i = 0; i < dataCount; ++i)
	{
		RectF uvSrcRect = dataList[i].SrcPixelRect;
		uvSrcRect.x *= texSizeInv.width;
		uvSrcRect.width *= texSizeInv.width;
		uvSrcRect.y *= texSizeInv.height;
		uvSrcRect.height *= texSizeInv.height;

		RectF dstRect(dataList[i].Position, dataList[i].SrcPixelRect.GetSize());
		dstRect.x += position.x;
		dstRect.y += position.y;
		InternalDrawRectangleStretch(dstRect, uvSrcRect);
	}

	//m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	//m_indexBuffer->SetSubData(0, m_indexCache.GetBuffer(), m_indexCache.GetBufferUsedByteCount());
	//m_renderer->SetVertexBuffer(m_vertexBuffer);
	//m_renderer->SetIndexBuffer(m_indexBuffer);
	//m_shader.varTexture->SetTexture(glyphsTexture);
	//m_shader.Pass->Apply();
	//m_renderer->DrawPrimitiveIndexed(PrimitiveType_TriangleList, 0, m_indexCache.GetCount() / 3);
}

//------------------------------------------------------------------------------
void PainterEngine::InternalDrawRectangleStretch(const RectF& rect, const RectF& srcUVRect)
{
	if (rect.IsEmpty()) { return; }		// 矩形がつぶれているので書く必要はない

	float lu = srcUVRect.GetLeft();
	float tv = srcUVRect.GetTop();
	float uvWidth = srcUVRect.width;
	float uvHeight = srcUVRect.height;

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);

	PainterVertex v;
	v.Color = GetCurrentState().ForeColor;
	v.Position.Set(rect.GetLeft(),  rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1);	// 左上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(2, 1);	// 右上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(),  rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 2);	// 左下
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(2, 2);	// 右下
	m_vertexCache.Add(v);
}

//------------------------------------------------------------------------------
//　Note: srcRect は、rect の中にいくつのタイルを並べられるかを計算するために使用する
//------------------------------------------------------------------------------
void PainterEngine::InternalDrawRectangleTiling(const RectF& rect, const Rect& srcRect, const RectF& srcUVRect, Driver::ITexture* srcTexture)
{
	if (rect.IsEmpty()) { return; }		// 矩形がつぶれているので書く必要はない

	float uvX = srcUVRect.x;
	float uvY = srcUVRect.y;
	float uvWidth = srcUVRect.width;
	float uvHeight = srcUVRect.height;
	float blockCountW = (rect.width / srcRect.width);	// 横方向にいくつのタイルを並べられるか (端数も含む)
	float blockCountH = (rect.height / srcRect.height);	// 縦方向にいくつのタイルを並べられるか (端数も含む)

	float lu = uvX;
	float ru = uvX + uvWidth * blockCountW;
	float tv = uvY;
	float bv = uvY + uvHeight * blockCountH;

	uint16_t i = m_vertexCache.GetCount();
	m_indexCache.Add(i + 0);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 2);
	m_indexCache.Add(i + 1);
	m_indexCache.Add(i + 3);

	PainterVertex v;
	v.Color = GetCurrentState().ForeColor;
	v.Position.Set(rect.GetLeft(),  rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1);	// 左上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1);	// 右上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(),  rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1.0f + blockCountH);	// 左下
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1.0f + blockCountH);	// 右下
	m_vertexCache.Add(v);
}

//------------------------------------------------------------------------------
//void PainterEngine::AttachBrushData()
//{
//	m_currentState.ForeColor = ColorF::White;
//	if (m_currentState.Brush.Type == BrushType_SolidColor) {
//		m_currentState.ForeColor.R = m_currentState.Brush.SolidColorBrush.Color[0];
//		m_currentState.ForeColor.G = m_currentState.Brush.SolidColorBrush.Color[1];
//		m_currentState.ForeColor.B = m_currentState.Brush.SolidColorBrush.Color[2];
//		m_currentState.ForeColor.A = m_currentState.Brush.SolidColorBrush.Color[3];
//	}
//	else if (m_currentState.Brush.Type == BrushType_Texture) {
//		LN_SAFE_ADDREF(m_currentState.Brush.TextureBrush.Texture);
//	}
//	else if (m_currentState.Brush.Type == BrushType_FrameTexture) {
//		LN_SAFE_ADDREF(m_currentState.Brush.FrameTextureBrush.Texture);
//	}
//}

//------------------------------------------------------------------------------
//void PainterEngine::DetachBrushData()
//{
//	if (m_currentState.Brush.Type == BrushType_Texture) {
//		LN_SAFE_RELEASE(m_currentState.Brush.TextureBrush.Texture);
//	}
//	else if (m_currentState.Brush.Type == BrushType_FrameTexture) {
//		LN_SAFE_RELEASE(m_currentState.Brush.FrameTextureBrush.Texture);
//	}
//}

//------------------------------------------------------------------------------
void PainterEngine::SetInternalGlyphMaskTexture(Driver::ITexture* mask)
{
	if (m_currentInternalGlyphMask.Get() != mask)
	{
		Flush();
		if (mask != NULL) {
			m_currentInternalGlyphMask = mask;
		}
		else {
			m_currentInternalGlyphMask = m_dummyTexture;
		}
	}
}

//------------------------------------------------------------------------------
void PainterEngine::UpdateCurrentForeColor()
{
	PainterEngineState& state = GetCurrentState();
	if (state.Brush.Type == BrushType_SolidColor)
	{
		state.ForeColor.a = state.Opacity * state.Brush.SolidColorBrush.Color[3];
	}
	else {
		state.ForeColor.a = state.Opacity;
	}
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
