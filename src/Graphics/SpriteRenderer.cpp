﻿/*
    ソートする要素は、
        ・プライオリティ
        ・Z座標 or ビュー行列の位置成分からの距離
        ・テクスチャ


    行列計算のテスト…

        頂点毎にワールド行列を覚えておいて (Vector3 の Right Up Front Position の4x3)、
        シェーダ内で計算するのと、CPU 側で4頂点に対して transform() するのとどっちが早いのかテスト。

        方法…
            drawRequest() を 6400 回実行。
            そのループと、シーン全体の描画処理の時間を計測する。

            Intel(R) Core(TM)2 Duo CPU     E8200  @ 2.66GHz (2 CPUs), ~2.7GHz
            NVIDIA Quadro FX 1700

        結果…

            １．CPU 側で座標変換

                drawRequest : 約 6500ns
                Render      : 約 3500ns

            ２．シェーダ内で計算

                drawRequest : 約 11000ns
                Render      : 約 5000ns

            ３．２の環境で、行列の代入を切ってみる

                drawRequest : 約 5000ns
                Render      : 約 5000ns

            おまけ.
                シェーダ内の mul 回数を 1 回だけにしてみた。(↑のは行列作成→mul→ViewProj mul の2回)

                drawRequest : 約 5000ns
                Render      : 約 5000ns

                あんまり変わらない。

        考察…

            行列の計算よりも、値コピー×4 (4頂点分) のほうがはるかに重い。
            シェーダの方は渡す頂点データのサイズが一番影響大きかったみたい。
                
*/

/*

◆ 設計とか

・Sprite クラスの drawSubset() は、サブセット番号 0 のみが渡されてくる。

・SceneObject を継承することでユーザー任意の描画をできるようにする
	・SpriteRenderer を使う限り、drawPrimitive() 使うわけじゃないのでこれまで通りっていうわけにはいかない。

・SceneOjbect には onRequestBatchDraw() を持たせておく。
  この関数は、SpriteRenderer や BillboardRenderer のように、データをバッファに溜めてから
  後で一度に描画するものの描画リクエスト送信を実装する。

・ISprite には２つの種類がある。
  ひとつは、単一の汎用性のあるスプライトっていうイメージ。
  これは以前までと同様、onDrawSubset() で setVertexBuffer()、drawPrimitive() の順に実行して描画する。
  バッチ処理の対象ではない。
  もうひとつは、バッチ処理されるもの。ひとつめのよりも高速に描画できる
  (描画中、setVertexBuffer() で頂点バッファの設定を変えない等)が、
  スプライト以外のオブジェクトとのZソートがうまくいかない。
  （2Dであるスプライトなら頑張ればいけると思うけど、3D空間に配置されるビルボードはかなりキツイ）
  そのため、バッチ処理は以下のものを対象とする。
    ・半透明要素の無いオブジェクト(アルファテストでカットされるものはOK(アルファ値が完全に0))
    ・半透明要素があり、かつ、半透明要素の描画パスで描画するもの(通常のオブジェクトの後に描画するべきもの)
  一応、バッチ処理の想定している使い方は主にエフェクト関係なので、この方向で問題はないかも。
  作成に関しては createSprite() createBatchSprite() の関数を使い、
  返るインターフェイスは両方とも ISprite。





Selene ではスプライトひとつ毎に drawPrimitive 読んでたけど…？

検証
    頂点 60000
    トライアングルリスト
    視点の近くで描画

    １．四角形ひとつにつき 回、DrawPrimitive() を呼ぶ
        奇数フレーム目が平均 1000ms
        偶数フレーム目が平均 500ms

    ２．全体を1度の DrawPrimitive() で描画
        １．とほとんど同じ。

    DrawPrimitive() のところだけ比べてみると、
    １．では 0～2ms、２．では 0ms。


    速度的にはほとんど変わらなかった。ちなみに速度が落ちてるのは Present()。
    誤差は関数呼び出しの分の時間って感じで、描画にはほとんど関係ないかも。




◆ 複数の四角形を格納した頂点バッファに対して、
   四角形ひとつにつき一度 drawPrimitive() を呼んで描画する


検証するコード

	lnU32 n = (VERTEX_NUM / 3) / 2;
	for ( lnU32 i = 0; i < n; ++i )
	{
	    renderer_->setVertexBuffer( mVertexBuffer, true );
	    renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );
	}


頂点数 60000
トライアングルリスト
一度の drawPrimitive() で2つのプリミティブ (四角形)を描画する


● 動的な頂点バッファとして作成した場合
	80ms～90ms
	
● 静的な頂点バッファとして作成した場合
	40ms～50ms
	倍くらい早い。

● lock() unlock() の重さ
	静的な頂点バッファとして作成後、
	上記コードの直前で
		mVertexBuffer->lock();
		mVertexBuffer->unlock();
	の２行を呼ぶ。
	結果、80ms～90ms。
	動的な頂点バッファとほぼ同じ。
	
● lock() unlock() の重さ  100回
	静的な頂点バッファとして作成後、
	上記コードの直前で
	for ( lnU32 i = 0; i < 100; ++i )
    {
        mVertexBuffer->lock();
        mVertexBuffer->unlock();
    }
    を実行する。
    結果、80ms～90ms。
    ほとんど変わらなかった。
    
● renderer_->setVertexBuffer( mVertexBuffer, true ); を
	ループの外に出して、１度だけ実行するようにする
	
	・動的の場合
		80ms～90ms。
		あまり変わらない。
		
	・静的の場合
		40ms～50ms
		あまり変わらない。
		
	・静的で１００回lock()
		80ms～90ms。
		あまり変わらない。
		
		
◆ もうひとつ頂点バッファを作って、以下のコードを実行

	lnU32 n = (VERTEX_NUM / 3) / 2;
	n /= 2;
    for ( lnU32 i = 0; i < n; ++i )
    {
        renderer_->setVertexBuffer( mVertexBuffer, true );
        renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );

        renderer_->setVertexBuffer( mVertexBuffer2, true );
        renderer_->drawPrimitive( LN_PRIMITIVE_TRIANGLELIST, i * 3, 2 );
    }
    
    ・動的の場合
		200ms～220ms。
		
		
    ・静的の場合
    	90～110ms
    	
    ・静的・100 lock() の場合
    	90～110ms
    	
	・静的・100 lock() 両方の場合
    	90～110ms



*/

#include "Internal.h"
#include <math.h>
#include <Lumino/Graphics/GraphicsException.h>
#include "SpriteRenderer.h"
#include "RenderingCommand.h"
#include "GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//==============================================================================
// SpriteRenderer
//==============================================================================

//------------------------------------------------------------------------------
SpriteRenderer* SpriteRenderer::Create(int maxSpriteCount, GraphicsManager* manager)
{
	return LN_NEW SpriteRenderer(manager, maxSpriteCount);
}

//------------------------------------------------------------------------------
SpriteRenderer::SpriteRenderer(GraphicsManager* manager, int maxSpriteCount)
	: m_manager(manager)
	, m_internal(nullptr)
	, m_spriteSortMode(SpriteSortMode::Texture | SpriteSortMode::DepthBackToFront)
{
	m_internal = LN_NEW SpriteRendererImpl();
	m_internal->Initialize(manager, maxSpriteCount);
}

//------------------------------------------------------------------------------
SpriteRenderer::~SpriteRenderer()
{
	LN_SAFE_RELEASE(m_internal);
}

//------------------------------------------------------------------------------
void SpriteRenderer::SetTransform(const Matrix& matrix)
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		Matrix, matrix,
		{
			m_internal->SetTransform(matrix);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::SetState(const RenderState& renderState, const Matrix& view, const Matrix& proj)
{
	LN_ENQUEUE_RENDER_COMMAND_4(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		RenderState, renderState,
		Matrix, view,
		Matrix, proj,
		{
			m_internal->SetRenderState(renderState);
			m_internal->SetViewProjMatrix(view, proj);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::SetViewPixelSize(const SizeI& size)
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		SizeI, size,
		{
			m_internal->SetViewPixelSize(size);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::SetSortMode(SpriteSortMode flags, SortingDistanceBasis basis)
{
	m_spriteSortMode = flags;
}

//------------------------------------------------------------------------------
void SpriteRenderer::DrawRequest2D(
	const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Texture* texture,
	const RectF& srcRect,
	const Color* colorTable)
{
	SpriteColorTable ct = { {Color::White, Color::White, Color::White, Color::White} };
	if (colorTable != nullptr)
	{
		ct.colors[0] = colorTable[0];
		ct.colors[1] = colorTable[1];
		ct.colors[2] = colorTable[2];
		ct.colors[3] = colorTable[3];
	}

	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_7(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector3, center,
		const Vector2, size,
		RefPtr<Driver::ITexture>, deviceTexture,
		const RectF, srcRect,
		SpriteColorTable, ct,
		{
			m_internal->DrawRequest2D(position, center, size, deviceTexture, srcRect, ct);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::DrawRequest2D(
	const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Texture* texture,
	const RectF& srcRect,
	const Color& color)
{
	SpriteColorTable ct = { { color, color, color, color } };
	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_7(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector3, center,
		const Vector2, size,
		RefPtr<Driver::ITexture>, deviceTexture,
		const RectF, srcRect,
		SpriteColorTable, ct,
		{
			m_internal->DrawRequest2D(position, center, size, deviceTexture, srcRect, ct);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::DrawRequest3D(
	const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Texture* texture,
	const RectF& srcRect,
	const Color* colorTable,
	AxisDirection front)
{
	SpriteColorTable ct = { { Color::White, Color::White, Color::White, Color::White } };
	if (colorTable != nullptr)
	{
		ct.colors[0] = colorTable[0];
		ct.colors[1] = colorTable[1];
		ct.colors[2] = colorTable[2];
		ct.colors[3] = colorTable[3];
	}

	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_8(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector3, center,
		const Vector2, size,
		RefPtr<Driver::ITexture>, deviceTexture,
		const RectF, srcRect,
		SpriteColorTable, ct,
		AxisDirection, front,
		{
			m_internal->DrawRequest3D(position, center, size, deviceTexture, srcRect, ct, front);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::DrawRequest3D(
	const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Texture* texture,
	const RectF& srcRect,
	const Color& color,
	AxisDirection front)
{
	SpriteColorTable ct = { { color, color, color, color } };
	Driver::ITexture* deviceTexture = (texture != nullptr) ? texture->GetDeviceObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_8(
		SpriteRenderer_SetTransform, m_manager,
		SpriteRendererImpl*, m_internal,
		const Vector3, position,
		const Vector3, center,
		const Vector2, size,
		RefPtr<Driver::ITexture>, deviceTexture,
		const RectF, srcRect,
		SpriteColorTable, ct,
		AxisDirection, front,
		{
			m_internal->DrawRequest3D(position, center, size, deviceTexture, srcRect, ct, front);
		});
}

//------------------------------------------------------------------------------
void SpriteRenderer::Flush()
{
	LN_ENQUEUE_RENDER_COMMAND_2(
		SpriteRenderer_Flush, m_manager,
		SpriteRendererImpl*, m_internal,
		SpriteSortMode, m_spriteSortMode,
		{
			m_internal->Flush(m_spriteSortMode);
		});
}


//==============================================================================
// SpriteRendererImpl
//==============================================================================
	
static const byte_t g_SpriteRenderer_fx_Data[] =
{
#include "Resource/SpriteRenderer.lfx.h"
};
static const size_t g_SpriteRenderer_fx_Len = LN_ARRAY_SIZE_OF(g_SpriteRenderer_fx_Data);

//------------------------------------------------------------------------------
SpriteRendererImpl::SpriteRendererImpl()
	: m_vertexBuffer()
	, m_indexBuffer()
	, m_maxSprites(0)
	, m_spriteRequestList()
	, m_spriteRequestListUsedCount(0)
	, m_spriteIndexList()
	, m_renderStateList()
	, m_currentRenderStateIndex(0)
	, m_attributeList()
	, m_transformMatrix()
	, m_viewDirection()
	, m_viewInverseMatrix()
	, m_viewPosition()
	, m_viewProjMatrix()
	, m_viewPixelSize()
	, m_sortingBasis(SortingDistanceBasis_RawZ)
{
	//CreateInternal();
}

//------------------------------------------------------------------------------
SpriteRendererImpl::~SpriteRendererImpl()
{
}

////------------------------------------------------------------------------------
//void SpriteRendererImpl::OnChangeDevice(Driver::IGraphicsDevice* device)
//{
//	if (device == NULL)
//	{
//		m_vertexBuffer.SafeRelease();
//		m_indexBuffer.SafeRelease();
//		m_shader.Shader.SafeRelease();
//	}
//	else
//	{
//		CreateInternal();
//	}
//}

//------------------------------------------------------------------------------
void SpriteRendererImpl::Initialize(GraphicsManager* manager, int maxSpriteCount)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
	m_maxSprites = maxSpriteCount;
	auto* device = manager->GetGraphicsDevice();

	//-----------------------------------------------------
	// 頂点バッファとインデックスバッファ
	m_vertexDeclaration.Attach(device->CreateVertexDeclaration(BatchSpriteVertex::Elements(), BatchSpriteVertex::ElementCount));
	m_vertexBuffer.Attach(device->CreateVertexBuffer(sizeof(BatchSpriteVertex) * m_maxSprites * 4, NULL, ResourceUsage::Dynamic));

#if 1
	ByteBuffer indexBuf(sizeof(uint16_t) * m_maxSprites * 6, false);
	uint16_t* ib = (uint16_t*)indexBuf.GetData();
	int idx = 0;
	int i2 = 0;
	for (int i = 0; i < m_maxSprites; ++i)
	{
		i2 = i * 6;
		idx = i * 4;
		ib[i2 + 0] = idx;
		ib[i2 + 1] = idx + 1;
		ib[i2 + 2] = idx + 2;
		ib[i2 + 3] = idx + 2;
		ib[i2 + 4] = idx + 1;
		ib[i2 + 5] = idx + 3;
	}
	m_indexBuffer.Attach(device->CreateIndexBuffer(
		m_maxSprites * 6, ib, IndexBufferFormat_UInt16, ResourceUsage::Dynamic));
#else
	m_indexBuffer.Attach(device->CreateIndexBuffer(
		m_maxSprites * 6, NULL, IndexBufferFormat_UInt16, DeviceResourceUsage_Dynamic));

	// インデックスバッファの値は最初から固定でOKなのでここで書き込んでしまう
	uint16_t* ib;
	size_t lockedSize;	// dummy
	m_indexBuffer->Lock((void**)&ib, &lockedSize);
	int idx = 0;
	int i2 = 0;
	for (int i = 0; i < m_maxSprites; ++i)
	{
		i2 = i * 6;
		idx = i * 4;
		ib[i2 + 0] = idx;
		ib[i2 + 1] = idx + 1;
		ib[i2 + 2] = idx + 2;
		ib[i2 + 3] = idx + 2;
		ib[i2 + 4] = idx + 1;
		ib[i2 + 5] = idx + 3;
	}
	m_indexBuffer->Unlock();
#endif

	//-----------------------------------------------------
	// シェーダ

	ShaderCompileResult r;
	m_shader.Shader.Attach(device->CreateShader(g_SpriteRenderer_fx_Data, g_SpriteRenderer_fx_Len, &r));
	LN_THROW(r.Level != ShaderCompileResultLevel_Error, CompilationException, r);

	m_shader.varTexture = m_shader.Shader->GetVariableByName(_T("gMaterialTexture"));
	m_shader.varViewProjMatrix = m_shader.Shader->GetVariableByName(_T("gViewProjMatrix"));
	m_shader.varViewPixelSize = m_shader.Shader->GetVariableByName(_T("gViewportSize"));
	m_shader.varTexture = m_shader.Shader->GetVariableByName(_T("gMaterialTexture"));
	m_shader.techMainDraw = m_shader.Shader->GetTechnique(0);

	//-----------------------------------------------------
	// メモリ確保と各種初期値

	m_spriteRequestList.Resize(m_maxSprites);
	m_spriteIndexList.Resize(m_maxSprites);

	// ステートにはデフォルトをひとつ詰めておく
	m_renderStateList.Add(RenderState());

	//if (m_3DSystem) {
	//	// 視点からの距離が大きいものを先に描画する
	//	SetSortMode(SpriteSortMode_Texture | SpriteSortMode_DepthBackToFront, SortingDistanceBasis_ViewPont);
	//}
	//else {
	//	// スプライトの Z 値が小さいものを先に描画する
	//	SetSortMode(SpriteSortMode_Texture | SpriteSortMode_DepthBackToFront, SortingDistanceBasis_RawZ);
	//}

	m_spriteRequestListUsedCount = 0;
	m_currentRenderStateIndex = 0;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetTransform(const Matrix& matrix)
{
	m_transformMatrix = matrix;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetViewProjMatrix(const Matrix& view, const Matrix& proj)
{
	m_viewDirection.Set(view.m[0][2], view.m[1][2], view.m[2][2]);
	m_viewInverseMatrix = Matrix::MakeInverse(view);
	m_viewPosition = m_viewInverseMatrix.GetPosition();
	m_viewProjMatrix = (view * proj);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetViewPixelSize(const SizeI& size)
{
	m_viewPixelSize.Set((float)size.width, (float)size.height);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetRenderState(const RenderState& state)
{
	// 同じものがあればカレントに
	size_t count = m_renderStateList.GetCount();
	for (size_t i = 0; i < count; ++i)
	{
		if (state == m_renderStateList[i]) {
			m_currentRenderStateIndex = i;
			return;
		}
	}

	// 見つからなかったら登録
	m_renderStateList.Add(state);
	m_currentRenderStateIndex = count;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::SetSortMode(uint32_t flags, SortingDistanceBasis basis)
{
	m_sortingBasis = basis;
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::DrawRequest2D(
    const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Driver::ITexture* texture,
	const RectF& srcRect,
	const SpriteColorTable& colorTable)
{
	DrawRequest3DInternal(
		position,
		center,
		size,
		texture,
		srcRect,
		colorTable,
		AxisDirection_RZ,
		false);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::DrawRequest3D(
	const Vector3& position,
	const Vector3& center,
	const Vector2& size,
	Driver::ITexture* texture,
	const RectF& srcRect,
	const SpriteColorTable& colorTable,
	AxisDirection front)
{
	DrawRequest3DInternal(position, center, size, texture, srcRect, colorTable, front, true);
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::DrawRequest3DInternal(
    const Vector3& position,
    const Vector3& center,
    const Vector2& size,
	Driver::ITexture* texture,
    const RectF& srcRect,
	const SpriteColorTable& colorTable,
	AxisDirection front,
	bool is3D)
{
	LN_THROW(m_spriteRequestListUsedCount < m_maxSprites, InvalidOperationException);

	BatchSpriteData& sprite = m_spriteRequestList[m_spriteRequestListUsedCount];

	// 3D の場合の頂点座標
	if (is3D)
	{
		Vector3 origin(-center);
		Vector2 harf_size(size * 0.5f);
		float l, t, r, b;
		r = harf_size.x;
		b = -harf_size.y;
		l = -r;
		t = -b;

#define LN_WRITE_V3( x_, y_, z_ ) origin.x + x_, origin.y + y_, origin.z + z_

		switch (front)
		{
		case AxisDirection_X:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(0, t, l));     // 左上
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(0, b, l));     // 左下
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(0, t, r));     // 右上
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(0, b, r));     // 右下
			break;
		case AxisDirection_Y:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(l, 0, t));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(l, 0, b));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(r, 0, t));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(r, 0, b));
			break;
		case AxisDirection_Z:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(r, t, 0));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(r, b, 0));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(l, t, 0));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(l, b, 0));
			break;
		case AxisDirection_RX:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(0, t, r));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(0, b, r));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(0, t, l));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(0, b, l));
			break;
		case AxisDirection_RY:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(r, 0, t));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(r, 0, b));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(l, 0, t));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(l, 0, b));
			break;
		case AxisDirection_RZ:
			sprite.Vertices[0].Position.Set(LN_WRITE_V3(l, t, 0));
			sprite.Vertices[1].Position.Set(LN_WRITE_V3(l, b, 0));
			sprite.Vertices[2].Position.Set(LN_WRITE_V3(r, t, 0));
			sprite.Vertices[3].Position.Set(LN_WRITE_V3(r, b, 0));
			break;
		}
#undef LN_WRITE_V3
	}
	// 2D の場合の頂点座標
	else
	{
		Vector3 origin(-center);
		sprite.Vertices[0].Position.Set(origin.x, origin.y, origin.z);
		sprite.Vertices[1].Position.Set(origin.x, origin.y + size.y, origin.z);
		sprite.Vertices[2].Position.Set(origin.x + size.x, origin.y, origin.z);
		sprite.Vertices[3].Position.Set(origin.x + size.x, origin.y + size.y, origin.z);
	}

	Matrix mat = m_transformMatrix.GetRotationMatrix();


	// ビルボード (Scene から使う場合は SceneNode が面倒見てるので、Scene 以外で必要になるまで保留…)
	if (0)
	{
		// TODO:
		//mat.setMul3x3( mViewInverseMatrix );
	}
	// ビルボード・Y 軸のみに適用
	else if (0)
	{
		if (m_viewDirection.x > 0.0f)
		{
			mat.RotateY(-atanf(m_viewDirection.z / m_viewDirection.x) + Math::PI / 2);
		}
		else if (m_viewDirection.x == 0.0f)
		{
			//D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
		}
		else
		{
			mat.RotateY(-atanf(m_viewDirection.z / m_viewDirection.x) - Math::PI / 2);
		}

	}
	// ビルボードではない
	else
	{
	}

	mat.Translate(position);
	mat.Translate(m_transformMatrix.GetPosition());

	// 座標変換
	sprite.Vertices[0].Position.TransformCoord(mat);
	sprite.Vertices[1].Position.TransformCoord(mat);
	sprite.Vertices[2].Position.TransformCoord(mat);
	sprite.Vertices[3].Position.TransformCoord(mat);

	// 色
	sprite.Vertices[0].Color = colorTable.colors[0];
	sprite.Vertices[1].Color = colorTable.colors[1];
	sprite.Vertices[2].Color = colorTable.colors[2];
	sprite.Vertices[3].Color = colorTable.colors[3];

	// テクスチャ
	if (texture != nullptr)
	{
		// テクスチャ座標
		const SizeI& texSize = texture->GetRealSize();
		Vector2 texSizeInv(1.0f / texSize.width, 1.0f / texSize.height);
		RectF sr(srcRect);
		float l = sr.x * texSizeInv.x;
		float t = sr.y * texSizeInv.y;
		float r = (sr.x + sr.width) * texSizeInv.x;
		float b = (sr.y + sr.height) * texSizeInv.y;
		sprite.Vertices[0].TexUV.x = l;
		sprite.Vertices[0].TexUV.y = t;
		sprite.Vertices[1].TexUV.x = l;
		sprite.Vertices[1].TexUV.y = b;
		sprite.Vertices[2].TexUV.x = r;
		sprite.Vertices[2].TexUV.y = t;
		sprite.Vertices[3].TexUV.x = r;
		sprite.Vertices[3].TexUV.y = b;

		// テクスチャ
		sprite.Texture = texture;	// TOOD: AddRef するべきかも？
	}
	else
	{
		sprite.Vertices[0].TexUV.x = 0;
		sprite.Vertices[0].TexUV.y = 0;
		sprite.Vertices[1].TexUV.x = 0;
		sprite.Vertices[1].TexUV.y = 1;
		sprite.Vertices[2].TexUV.x = 1;
		sprite.Vertices[2].TexUV.y = 0;
		sprite.Vertices[3].TexUV.x = 1;
		sprite.Vertices[3].TexUV.y = 1;
		sprite.Texture = m_manager->GetDummyDeviceTexture();
	}

	// カメラからの距離をソート用Z値にする場合
	if (m_sortingBasis == SortingDistanceBasis_ViewPont) {
		sprite.Depth = (m_viewPosition - position).GetLengthSquared();
	}
	else {
		sprite.Depth = position.z;
	}

	sprite.Visible = true;
	sprite.Assigned = false;
	sprite.Priority = 0;
	sprite.RenderStateIndex = m_currentRenderStateIndex;

	++m_spriteRequestListUsedCount;
}


//------------------------------------------------------------------------------
// Z 値の大きい方から小さい方へソートする比較
class SpriteRendererImpl::SpriteCmpDepthBackToFront
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Depth > rsp.Depth)
				{
					return true;
				}
				if (lsp.Depth == rsp.Depth)
				{
					return lsp.Texture < rsp.Texture;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};

// Z 値の小さい方から大きい方へソートする比較
class SpriteRendererImpl::SpriteCmpDepthFrontToBack
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Depth < rsp.Depth)
				{
					return true;
				}
				if (lsp.Depth == rsp.Depth)
				{
					return lsp.Texture < rsp.Texture;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};

// Z 値の大きい方から小さい方へソートする比較 (テクスチャ優先)
class SpriteRendererImpl::SpriteCmpTexDepthBackToFront
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Texture < rsp.Texture)
				{
					return true;
				}
				if (lsp.Texture == rsp.Texture)
				{
					// Depth 降順
					return lsp.Depth > rsp.Depth;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};

// Z 値の小さい方から大きい方へソートする比較 (テクスチャ優先)
class SpriteRendererImpl::SpriteCmpTexDepthFrontToBack
{
public:
	SpriteRequestList* spriteList;

	bool operator()(int l_, int r_)
	{
		const BatchSpriteData& lsp = spriteList->GetAt(l_);
		const BatchSpriteData& rsp = spriteList->GetAt(r_);

		if (lsp.Priority == rsp.Priority)
		{
			if (lsp.RenderStateIndex == rsp.RenderStateIndex)
			{
				if (lsp.Texture < rsp.Texture)
				{
					return true;
				}
				if (lsp.Texture == rsp.Texture)
				{
					return lsp.Depth < rsp.Depth;
				}
				return false;
			}
			return lsp.RenderStateIndex < rsp.RenderStateIndex;
		}
		return lsp.Priority < rsp.Priority;
	}
};


//------------------------------------------------------------------------------
void SpriteRendererImpl::Flush(SpriteSortMode sortFlags)
{
	int spriteCount = m_spriteRequestListUsedCount;	// 描画するスプライトの数
	if (spriteCount == 0) { return; }				// 0 個ならなにもしない

	// 描画するスプライトのインデックスリストを初期化する
	for (int i = 0; i < spriteCount; ++i) {
		m_spriteIndexList[i] = i;
	}
	//memcpy(m_spriteIndexList, mSpriteIndexArraySource, sizeof(*m_spriteIndexList) * mLastSpriteNum);

	// インデックスを並び替える
	if (sortFlags & SpriteSortMode::Texture)
	{
		if (sortFlags & SpriteSortMode::DepthBackToFront)
		{
			SpriteCmpDepthBackToFront cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
		else if (sortFlags & SpriteSortMode::DepthFrontToBack)
		{
			SpriteCmpDepthFrontToBack cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
	}
	else
	{
		if (sortFlags & SpriteSortMode::DepthBackToFront)
		{
			SpriteCmpTexDepthBackToFront cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
		else if (sortFlags & SpriteSortMode::DepthFrontToBack)
		{
			SpriteCmpTexDepthFrontToBack cmp;
			cmp.spriteList = &m_spriteRequestList;
			std::stable_sort(m_spriteIndexList.begin(), m_spriteIndexList.begin() + spriteCount, cmp);
		}
	}

	int si = 0;     // Sprite Index (m_spriteIndexList)
	int vi = 0;     // Vertex Index
	int ri = 0;     // Request Index (m_spriteRequestList)
	int start_idx = 0;
	int prim_num = 0;
	Driver::ITexture* current_tex = m_spriteRequestList[m_spriteIndexList[0]].Texture;
	int currnetRenderStateIndex = m_spriteRequestList[m_spriteIndexList[0]].RenderStateIndex;

	//-----------------------------------------------------
	// 属性リストを作る

	m_attributeList.Clear();
	while (true)
	{
		while (true)
		{
			++si;
			++prim_num;
#if 1
			// 次のスプライトのテクスチャが、処理中のテクスチャと異なる場合は次の属性作成に移る
			if (si >= spriteCount ||
				m_spriteRequestList[m_spriteIndexList[si]].Texture != current_tex ||
				m_spriteRequestList[m_spriteIndexList[si]].RenderStateIndex != currnetRenderStateIndex)
			{
				break;
			}
#else
			// 次のスプライトのテクスチャが、処理中のテクスチャと異なる場合は次の属性作成に移る
			if (si >= spriteCount || m_spriteRequestList[m_spriteIndexList[si]].Texture != current_tex)
			{
				break;
			}
#endif
		}

		Attribute attr;
		attr.StartIndex = start_idx;
		attr.PrimitiveNum = prim_num * 2;
		attr.Texture = current_tex;
		attr.RenderStateIndex = currnetRenderStateIndex;
		m_attributeList.Add(attr);

		if (si >= spriteCount)
		{
			break;
		}

		start_idx = si * 6;
		prim_num = 0;
		current_tex = m_spriteRequestList[m_spriteIndexList[si]].Texture;
		currnetRenderStateIndex = m_spriteRequestList[m_spriteIndexList[si]].RenderStateIndex;
	}

	//-----------------------------------------------------
	// 頂点データをコピー

	BatchSpriteVertex* vb = static_cast< BatchSpriteVertex* >(m_vertexBuffer->Lock());
	si = 0;
	vi = 0;
	for (; si < spriteCount; ++si)
	{
		ri = m_spriteIndexList[si];
		memcpy(&vb[vi], m_spriteRequestList[ri].Vertices, sizeof(m_spriteRequestList[0].Vertices));
		vi += 4;
	}
	m_vertexBuffer->Unlock();

	//-----------------------------------------------------
	// 描画

	auto* r = m_manager->GetGraphicsDevice()->GetRenderer();
	m_shader.varViewProjMatrix->SetMatrix(m_viewProjMatrix);
	m_shader.varViewPixelSize->SetVector(Vector4(m_viewPixelSize.x, m_viewPixelSize.y, 0, 0));

	Driver::IShaderPass* pass = m_shader.techMainDraw->GetPass(0);

	AttributeList::iterator itr = m_attributeList.begin();
	AttributeList::iterator end = m_attributeList.end();
	for (; itr != end; ++itr)
	{
		r->SetRenderState(m_renderStateList[itr->RenderStateIndex]);
		m_shader.varTexture->SetTexture(itr->Texture);
		pass->Apply();
		r->SetVertexDeclaration(m_vertexDeclaration);
		r->SetVertexBuffer(0, m_vertexBuffer);
		r->SetIndexBuffer(m_indexBuffer);
		r->DrawPrimitiveIndexed(PrimitiveType_TriangleList, itr->StartIndex, itr->PrimitiveNum);
	}

	//-----------------------------------------------------
	// クリーンアップ

	Clear();
}

//------------------------------------------------------------------------------
void SpriteRendererImpl::Clear()
{
	m_renderStateList.Clear();
	m_renderStateList.Add(RenderState());
	m_currentRenderStateIndex = 0;
	m_spriteRequestListUsedCount = 0;
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
