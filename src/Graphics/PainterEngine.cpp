
#include "../Internal.h"
#include <Lumino/Imaging/BitmapPainter.h>
#include "PainterEngine.h"

namespace Lumino
{
namespace Graphics
{

//=============================================================================
// PainterEngine
//=============================================================================

	

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PainterEngine::PainterEngine()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PainterEngine::~PainterEngine()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::Create(GraphicsManager* manager)
{
	m_vertexBuffer.Attach(manager->GetGraphicsDevice()->GetDeviceObject()->CreateVertexBuffer(
		PainterVertex::Elements(), PainterVertex::ElementCount, 1024, NULL, DeviceResourceUsage_Dynamic));

	m_vertexCache.Reserve(1024);

	RefPtr<ByteBuffer> code(FileUtils::ReadAllBytes(_T("D:/Proj/Lumino/src/Graphics/Resource/Painter.fx")));
	ShaderCompileResult r;
	m_shader.Shader.Attach(manager->GetGraphicsDevice()->GetDeviceObject()->CreateShader(code->GetData(), code->GetSize(), &r));
	m_shader.Technique = m_shader.Shader->GetTechnique(0);
	m_shader.Pass = m_shader.Technique->GetPass(0);
	m_shader.varWorldMatrix = m_shader.Shader->GetVariableByName(_T("g_worldMatrix"));
	m_shader.varViewProjMatrix = m_shader.Shader->GetVariableByName(_T("g_viewProjMatrix"));
	m_shader.varTexture = m_shader.Shader->GetVariableByName(_T("g_texture"));

	m_renderer = manager->GetGraphicsDevice()->GetDeviceObject()->GetRenderer();

	m_shader.varWorldMatrix->SetMatrix(Matrix::Identity);
	m_shader.varViewProjMatrix->SetMatrix(Matrix::Identity);


	//-----------------------------------------------------
	// ダミーテクスチャ

	m_dummyTexture.Attach(manager->GetGraphicsDevice()->GetDeviceObject()->CreateTexture(Size(32, 32), 1, TextureFormat_R8G8B8A8), false);
	Device::IGraphicsDevice::ScopedLockContext lock(manager->GetGraphicsDevice()->GetDeviceObject());
	Imaging::BitmapPainter painter(m_dummyTexture->Lock());
	painter.Clear(Color::White);
	m_dummyTexture->Unlock();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void PainterEngine::SetTransform(const Matrix& matrix)
//{
//
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::SetViewProjMatrix(const Matrix& matrix)
{
	m_shader.varViewProjMatrix->SetMatrix(matrix);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::DrawFrameRectangle(const RectF& rect, float frameWidth, Device::ITexture* srcTexture, const Rect& srcRect_)
{
	if (srcTexture == NULL) {
		srcTexture = m_dummyTexture;
	}

	Rect srcRect = srcRect_;
	if (srcRect.Width == INT_MAX) {
		srcRect.Width = srcTexture->GetSize().Width;
	}
	if (srcRect.Height == INT_MAX) {
		srcRect.Height = srcTexture->GetSize().Height;
	}

	m_vertexCache.Clear();

	SizeF texSize(srcTexture->GetSize().Width, srcTexture->GetSize().Height);
	texSize.Width = 1.0f / texSize.Width;
	texSize.Height = 1.0f / texSize.Height;
	RectF uvSrcRect(srcRect.X * texSize.Width, srcRect.Y * texSize.Height, srcRect.Width * texSize.Width, srcRect.Height * texSize.Height);
	float uvFrameWidthW = frameWidth * texSize.Width;
	float uvFrameWidthH = frameWidth * texSize.Height;

	RectF outerRect = rect;
	RectF innerRect(outerRect.X + frameWidth, outerRect.Y + frameWidth, outerRect.Width - frameWidth * 2, outerRect.Height - frameWidth * 2);
	RectF outerUVRect = uvSrcRect;
	RectF innerUVRect(outerUVRect.X + uvFrameWidthW, outerUVRect.Y + uvFrameWidthH, outerUVRect.Width - uvFrameWidthW * 2, outerUVRect.Height - uvFrameWidthH * 2);
	// ↑外側に広げるモードがあってもいいかも

	/* 上
		□■□
		□　□
		□□□
	*/
	InternalDrawRectangle(
		RectF(innerRect.X, outerRect.Y, innerRect.Width, innerRect.Y - outerRect.Y),
		srcTexture,
		Rect(srcRect.X + frameWidth, srcRect.Y, srcRect.Width - frameWidth * 2, frameWidth),
		RectF(innerUVRect.X, outerUVRect.Y, innerUVRect.Width, innerUVRect.Y - outerUVRect.Y));

	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	m_renderer->SetVertexBuffer(m_vertexBuffer);
	m_shader.varTexture->SetTexture(srcTexture);
	m_shader.Pass->Apply();
	m_renderer->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);

#if 0
	//TextureBrush* textureBrush = static_cast<TextureBrush*>(brush);
	SizeF texSize(srcTexture->GetSize().Width, srcTexture->GetSize().Height);
	texSize.Width = 1.0f / texSize.Width;
	texSize.Height = 1.0f / texSize.Height;
	RectF uvSrcRect(srcRect.X * texSize.Width, srcRect.Y * texSize.Height, srcRect.Width * texSize.Width, srcRect.Height * texSize.Height);
	float uvFrameWidthW = frameWidth * texSize.Width;
	float uvFrameWidthH = frameWidth * texSize.Height;

	// 位置も UV もローカル座標系上で作る。複雑な図形でも、境界ボックスの左上が原点になる。



	// 上
	/*
		□■■□
		□　　□
		□　　□
		□□□□
	*/

	float width = innerRect.Width;
	float height = innerRect.Y;
	float uvX = innerUVRect.X;
	float uvY = outerUVRect.Y;
	float uvWidth = innerUVRect.Width;
	float uvHeight = innerUVRect.Y;

	float blockCountW = (width / uvWidth) + fmod(width, uvWidth);		// 横方向にいくつのタイルを並べられるか (端数も含む)
	float blockCountH = (height / uvHeight) + fmod(height, uvHeight);	// 縦方向にいくつのタイルを並べられるか (端数も含む)


	float lu = uvX;
	float ru = uvX + uvWidth * blockCountW;
	float tv = uvY;
	float bv = uvY + uvHeight * blockCountH;

	PainterVertex v;
	v.Color.Set(1, 1, 1, 1);
	v.Position.Set(innerRect.GetLeft(),  outerRect.GetTop(), 0); v.UVOffset.Set(lu, tv, lu - lu, tv - tv); v.UVTileUnit.Set(0, 0);	// 左上
	m_vertexCache.Add(v);
	v.Position.Set(innerRect.GetRight(), outerRect.GetTop(), 0); v.UVOffset.Set(ru, tv, ru - lu, tv - tv); v.UVTileUnit.Set(blockCountW, 0);	// 右上
	m_vertexCache.Add(v);
	v.Position.Set(innerRect.GetLeft(),  innerRect.GetTop(), 0); v.UVOffset.Set(lu, bv, lu - lu, bv - tv); v.UVTileUnit.Set(0, 0);	// 左下
	m_vertexCache.Add(v);
	v.Position.Set(innerRect.GetRight(), innerRect.GetTop(), 0); v.UVOffset.Set(ru, bv, ru - lu, bv - tv); v.UVTileUnit.Set(blockCountW, blockCountH);	// 右下
	m_vertexCache.Add(v);


	m_vertexBuffer->SetSubData(0, m_vertexCache.GetBuffer(), m_vertexCache.GetBufferUsedByteCount());
	


	m_renderer->SetVertexBuffer(m_vertexBuffer);

	m_shader.varTexture->SetTexture(srcTexture);
	m_shader.Pass->Apply();
	m_renderer->DrawPrimitive(PrimitiveType_TriangleStrip, 0, 2);
#endif
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void PainterEngine::InternalDrawRectangle(const RectF& rect, Device::ITexture* srcTexture, const Rect& srcRect, const RectF& srcUVRect)
{

	float width = rect.Width;
	float height = rect.Height;
	float uvX = srcUVRect.X;
	float uvY = srcUVRect.Y;
	float uvWidth = srcUVRect.Width;
	float uvHeight = srcUVRect.Height;

	float blockCountW = (width / srcRect.Width)/* + fmod(width, srcRect.Width)*/;		// 横方向にいくつのタイルを並べられるか (端数も含む)
	float blockCountH = (height / srcRect.Height)/* + fmod(height, srcRect.Height)*/;	// 縦方向にいくつのタイルを並べられるか (端数も含む)


	float lu = uvX;
	float ru = uvX + uvWidth * blockCountW;
	float tv = uvY;
	float bv = uvY + uvHeight * blockCountH;

	PainterVertex v;
	v.Color.Set(1, 1, 1, 1);
	v.Position.Set(rect.GetLeft(),  rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1);	// 左上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1);	// 右上
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetLeft(),  rect.GetBottom(), 0); v.UVOffset.Set(lu, bv, uvWidth, uvHeight); v.UVTileUnit.Set(1, 1);	// 左下
	m_vertexCache.Add(v);
	v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(lu, bv, uvWidth, uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1.0f + blockCountH);	// 右下
	m_vertexCache.Add(v);

	//PainterVertex v;
	//v.Color.Set(1, 1, 1, 1);
	//v.Position.Set(rect.GetLeft(), rect.GetTop(), 0);    v.UVOffset.Set(lu, tv, lu - lu, tv - tv); v.UVTileUnit.Set(1, 1);	// 左上
	//m_vertexCache.Add(v);
	//v.Position.Set(rect.GetRight(), rect.GetTop(), 0);    v.UVOffset.Set(ru, tv, ru - uvWidth, tv - tv); v.UVTileUnit.Set(1.0f + blockCountW, 1);	// 右上
	//m_vertexCache.Add(v);
	//v.Position.Set(rect.GetLeft(), rect.GetBottom(), 0); v.UVOffset.Set(lu, bv, lu - lu, bv - uvHeight); v.UVTileUnit.Set(1, 1);	// 左下
	//m_vertexCache.Add(v);
	//v.Position.Set(rect.GetRight(), rect.GetBottom(), 0); v.UVOffset.Set(ru, bv, ru - uvWidth, bv - uvHeight); v.UVTileUnit.Set(1.0f + blockCountW, 1.0f + blockCountH);	// 右下
	m_vertexCache.Add(v);
}

} // namespace Graphics
} // namespace Lumino
