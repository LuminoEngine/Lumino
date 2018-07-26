﻿#include "Common.hpp"

class Test_Graphics_LowLevelRendering : public ::testing::Test
{
public:
	virtual void SetUp()
	{
		auto shader1 = Shader::create(LN_ASSETFILE("MultiStreamVertexBuffer-1.vsh"), LN_ASSETFILE("MultiStreamVertexBuffer-1.psh"));


		//m_shader1 = Shader::create(LN_ASSETFILE("simple.vert"), LN_ASSETFILE("simple.frag"));
		//m_shader1 = Shader::create(LN_ASSETFILE("simple.vsh"), LN_ASSETFILE("test.psh"));
		m_shader1 = Shader::create(LN_ASSETFILE("simple.vsh"), LN_ASSETFILE("simple.psh"));

		m_vertexDecl1 = newObject<VertexDeclaration>();
		m_vertexDecl1->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Position, 0);
	}

	virtual void TearDown() {}

	Ref<Shader> m_shader1;
	Ref<VertexDeclaration> m_vertexDecl1;
};

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, BasicTriangle)
{
	// # 時計回り (左ねじ) で描画できること
	{
		m_shader1->findConstantBuffer("ConstBuff")->findParameter("g_color")->setVector(Vector4(1, 0, 0, 1));

		Vector4 v[] = {
			Vector4(0, 0.5, 0, 1),
			Vector4(0.5, -0.25, 0, 1),
			Vector4(-0.5, -0.25, 0, 1),
		};
		auto vertexBuffer = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

		auto ctx = Engine::graphicsContext();
		ctx->setVertexDeclaration(m_vertexDecl1);
		ctx->setVertexBuffer(0, vertexBuffer);
		ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-BasicTriangle-1.png"));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, VertexBuffer)
{
	auto buffer1 = m_shader1->findConstantBuffer("ConstBuff");
	buffer1->findParameter("g_color")->setVector(Vector4(1, 0, 0, 1));

	struct Param
	{
		GraphicsResourceUsage usage;
		GraphicsResourcePool pool;
	};
	Param params[] =
	{
		{ GraphicsResourceUsage::Static, GraphicsResourcePool::None },
		{ GraphicsResourceUsage::Static, GraphicsResourcePool::Managed },
		{ GraphicsResourceUsage::Dynamic, GraphicsResourcePool::None },
		{ GraphicsResourceUsage::Dynamic, GraphicsResourcePool::Managed },
	};

	// test static and dynamic
	for (int i = 0; i < 4; i++)
	{
		auto usage = params[i].usage;
		auto pool = params[i].pool;

		auto vb1 = newObject<VertexBuffer>(sizeof(Vector4) * 3, usage);
		auto vb2 = newObject<VertexBuffer>(sizeof(Vector4) * 3, usage);
		vb1->setResourcePool(pool);
		vb2->setResourcePool(pool);

		auto ctx = Engine::graphicsContext();
		ctx->setVertexDeclaration(m_vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);

		// * [ ] まだ一度もレンダリングに使用されていないバッファを、更新できること
		{
			Vector4 v1[] = {
				Vector4(0, 0.5, 0, 1),
				Vector4(0.5, -0.25, 0, 1),
				Vector4(-0.5, -0.25, 0, 1),
			};
			memcpy(vb1->map(MapMode::Write), v1, vb1->size());

			ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

			ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-VertexBuffer-2.png"));
		}

		// * [ ] 一度レンダリングに使用されたバッファを、再更新できること
		{
			Vector4 v2[] = {
				Vector4(0, 1, 0, 1),
				Vector4(1, -1, 0, 1),
				Vector4(-1, -1, 0, 1),
			};
			memcpy(vb1->map(MapMode::Write), v2, vb1->size());

			ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

			ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-VertexBuffer-3.png"));
		}

		// * [ ] まだ一度もレンダリングに使用されていないバッファを、拡張できること
		{
			Vector4 v2[] = {
				Vector4(-0.5, 0.5, 0, 1),
				Vector4(0.5, 0.5, 0, 1),
				Vector4(-0.5, -0.5, 0, 1),
				Vector4(0.5, -0.5, 0, 1),
			};

			vb2->resize(sizeof(Vector4) * 4);
			ASSERT_EQ(sizeof(Vector4) * 4, vb2->size());

			memcpy(vb2->map(MapMode::Write), v2, vb2->size());

			ctx->setVertexBuffer(0, vb2);
			ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);

			ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-VertexBuffer-4.png"));
		}

		// * [ ] 一度レンダリングに使用されたバッファを、拡張できること
		{
			Vector4 v2[] = {
				Vector4(-0.5, 0.5, 0, 1),
				Vector4(0.5, 0.5, 0, 1),
				Vector4(-0.5, -0.5, 0, 1),
				Vector4(0.5, -0.5, 0, 1),
				Vector4(-0.5, -1, 0, 1),
			};

			vb2->resize(sizeof(Vector4) * 5);
			ASSERT_EQ(sizeof(Vector4) * 5, vb2->size());

			memcpy(vb2->map(MapMode::Write), v2, vb2->size());

			ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 3);

			ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-VertexBuffer-5.png"));
		}
	}


	// TODO: 部分 lock
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, MultiStreamVertexBuffer)
{
	auto shader1 = Shader::create(LN_ASSETFILE("MultiStreamVertexBuffer-1.vsh"), LN_ASSETFILE("MultiStreamVertexBuffer-1.psh"));

	struct PosColor
	{
		Vector3 pos;
		Vector4 color;
	};
	PosColor v1[3] = { 
		{ { -1, 1, 0 }, { 0.5, 0, 0, 0 } }, 
		{ {0, 1, 0}, { 0, 1., 0, 0 } },
		{ {-1, 0, 0}, { 0, 0, 0.5, 0 } }
	};
	Vector3 uv1[3] = { { 0.5, 0, 0 },{ 0, -0.5, 0 }, { 0, 0, 0.5 } };
	Vector4 uv2[3] = { { 0, 0, 0, 1 },{ 0, 0, 0, 1 },{ 0, 0, 0, 1 } };

	auto vb1 = newObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);
	auto vb2 = newObject<VertexBuffer>(sizeof(uv1), uv1, GraphicsResourceUsage::Static);
	auto vb3 = newObject<VertexBuffer>(sizeof(uv2), uv2, GraphicsResourceUsage::Static);
	auto vd1 = newObject<VertexDeclaration>();
	vd1->addVertexElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
	vd1->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);
	vd1->addVertexElement(1, VertexElementType::Float3, VertexElementUsage::TexCoord, 0);
	vd1->addVertexElement(2, VertexElementType::Float4, VertexElementUsage::TexCoord, 1);

	auto ctx = Engine::graphicsContext();
	ctx->setVertexBuffer(0, vb1);
	ctx->setVertexBuffer(1, vb2);
	ctx->setVertexBuffer(2, vb3);
	ctx->setVertexDeclaration(vd1);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

	ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
	ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);

	ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-MultiStreamVertexBuffer-1.png"));
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, IndexBuffer)
{
	auto buffer1 = m_shader1->findConstantBuffer("ConstBuff");
	buffer1->findParameter("g_color")->setVector(Vector4(0, 0, 1, 1));

	struct Param
	{
		GraphicsResourceUsage usage;
		GraphicsResourcePool pool;
	};
	Param params[] =
	{
		{ GraphicsResourceUsage::Static, GraphicsResourcePool::None },
		{ GraphicsResourceUsage::Static, GraphicsResourcePool::Managed },
		{ GraphicsResourceUsage::Dynamic, GraphicsResourcePool::None },
		{ GraphicsResourceUsage::Dynamic, GraphicsResourcePool::Managed },
	};

	// test static and dynamic
	for (int i = 0; i < 4; i++)
	{
		auto usage = params[i].usage;
		auto pool = params[i].pool;

		Vector4 vertices[] = {
			Vector4(0, 0.5, 0, 1),
			Vector4(0, 0, 0, 1),
			Vector4(0.5, -0.25, 0, 1),
			Vector4(0, 0, 0, 1),
			Vector4(-0.5, -0.25, 0, 1),
		};
		auto vb1 = newObject<VertexBuffer>(sizeof(Vector4) * 5, vertices, usage);
		auto ib1 = newObject<IndexBuffer>(3, IndexBufferFormat::UInt16, usage);
		ib1->setResourcePool(pool);

		auto ctx = Engine::graphicsContext();
		ctx->setVertexDeclaration(m_vertexDecl1);
		ctx->setVertexBuffer(0, vb1);
		ctx->setIndexBuffer(ib1);
		ctx->setShaderPass(m_shader1->techniques()[0]->passes()[0]);

		// * [ ] まだ一度もレンダリングに使用されていないバッファを、更新できること
		{
			uint16_t indices[] = { 0, 2, 4 };
			memcpy(ib1->map(MapMode::Write), indices, ib1->bytesSize());

			ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->drawPrimitiveIndexed(PrimitiveType::TriangleList, 0, 1);

			ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-IndexBuffer-1.png"));
		}

		// * [ ] 一度レンダリングに使用されたバッファを、再更新できること
		{
			uint16_t indices[] = { 1, 2, 4 };
			memcpy(ib1->map(MapMode::Write), indices, ib1->bytesSize());

			ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->drawPrimitiveIndexed(PrimitiveType::TriangleList, 0, 1);

			ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-IndexBuffer-2.png"));
		}

		// * [ ] フォーマット変更 16 -> 32
		{
			if (usage == GraphicsResourceUsage::Static && pool == GraphicsResourcePool::None) {
				// un supported
			}
			else
			{
				ib1->setFormat(IndexBufferFormat::UInt32);

				ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
				ctx->drawPrimitiveIndexed(PrimitiveType::TriangleList, 0, 1);

				ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-IndexBuffer-2.png"));	// ↑と同じ結果
			}
		}
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, ConstantBuffer)
{
	auto shader1 = Shader::create(LN_ASSETFILE("simple.vsh"), LN_ASSETFILE("ConstantBufferTest-1.psh"));
	auto buffer1 = shader1->findConstantBuffer("ConstBuff");
	auto buffer2 = shader1->findConstantBuffer("ConstBuff2");

	Vector4 v[] = {
		Vector4(-1, 1, 0, 1),
		Vector4(0, 1, 0, 1),
		Vector4(-1, 0, 0, 1),
	};
	auto vertexBuffer = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);
	auto ctx = Engine::graphicsContext();
	ctx->setVertexDeclaration(m_vertexDecl1);
	ctx->setVertexBuffer(0, vertexBuffer);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

	auto renderAndCapture = [&]() {
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleList, 0, 1);
		return TestEnv::capture()->getPixel32(0, 0);
	};

	// * [ ] float
	{
		buffer1->findParameter("g_type")->setInt(1);
		buffer1->findParameter("g_color1")->setFloat(0.5);	// 赤っぽくする
		ASSERT_EQ(true, renderAndCapture().r > 100);		// 赤っぽくなっているはず
	}
	// * [ ] float2
	{
		buffer1->findParameter("g_type")->setInt(2);
		buffer1->findParameter("g_color2")->setVector(Vector4(1, 0, 0, 1));
		ASSERT_EQ(true, renderAndCapture().r > 200);
	}
	// * [ ] float3
	{
		buffer1->findParameter("g_type")->setInt(3);
		buffer1->findParameter("g_color3")->setVector(Vector4(0, 1, 0, 1));
		ASSERT_EQ(true, renderAndCapture().g > 200);
	}
	// * [ ] float4
	{
		buffer1->findParameter("g_type")->setInt(4);
		buffer1->findParameter("g_color4")->setVector(Vector4(0, 0, 1, 1));
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}

	float ary1[3] = { 0, 0.5, 0 };
	Vector4 ary2[3] = { {1, 0, 0, 1}, {0, 1, 0, 1}, {0, 0, 1, 1} };

	// * [ ] float[]
	{
		buffer1->findParameter("g_type")->setInt(11);
		buffer1->findParameter("g_float1ary3")->setFloatArray(ary1, 3);
		ASSERT_EQ(true, renderAndCapture().r > 100);
	}
	// * [ ] float2[]
	{
		buffer1->findParameter("g_type")->setInt(12);
		buffer1->findParameter("g_float2ary3")->setVectorArray(ary2, 3);
		ASSERT_EQ(true, renderAndCapture().g > 200);
	}
	// * [ ] float3[]
	{
		buffer1->findParameter("g_type")->setInt(13);
		buffer1->findParameter("g_float3ary3")->setVectorArray(ary2, 3);
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}
	// * [ ] float4[]
	{
		buffer1->findParameter("g_type")->setInt(14);
		buffer1->findParameter("g_float4ary3")->setVectorArray(ary2, 3);
		ASSERT_EQ(true, renderAndCapture().g > 200);
	}


	//* [ ] 座標変換したときに一般的な使い方ができるか
	{
		auto pos = Vector4(1, 0, 0, 1);
		auto mat = Matrix::makeRotationY(-Math::PIDiv2);
		auto r = Vector4::transform(pos, mat);	// (0, 0, 1)
		buffer1->findParameter("g_type")->setInt(99);
		buffer1->findParameter("g_color4")->setVector(pos);
		buffer2->findParameter("g_mat44")->setMatrix(mat);
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}
	//* [ ] 座標変換したときに一般的な使い方ができるか (array)
	{
		auto pos = Vector4(1, 0, 0, 1);
		auto mat = Matrix::makeRotationY(-Math::PIDiv2);
		auto r = Vector4::transform(pos, mat);	// (0, 0, 1)
		Matrix ary[3] = { {}, mat, {} };
		buffer1->findParameter("g_type")->setInt(100);
		buffer1->findParameter("g_color4")->setVector(pos);
		buffer2->findParameter("g_mat44ary3")->setMatrixArray(ary, 3);
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}
	//* [ ] array にすると転置される？
	{
		/* シェーダコードは↓
			float4x4 m = g_mat44ary3[1];
			return float4(g_mat44[0][2], m[0][2], m[2][0], 1);

			この場合 R と G が同じ値となり黄色が出てくるはずだが、
			実際にやってみると R と B が出ている。

			このブロックのテストは、↑の2つのブロックのテストの後に行われるため
			2つの行列は同じ値になっているはずだが・・・行列配列だけ、中身が転置されてしまう。

			ただ、それでも mul が同じ結果で成功するのもまた疑問。
		*/
		buffer1->findParameter("g_type")->setInt(101);
		auto c = renderAndCapture();
		ASSERT_EQ(true, c.r > 200);
		//ASSERT_EQ(true, c.g > 200);
		ASSERT_EQ(true, c.b > 200);
	}

	//* [ ] float3x4 (Vector4[3])
	{
		Matrix m(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0);
		m(0, 1) = 1;	// 転置とかされることなく、この配列アクセスで値が取り出せる
		buffer1->findParameter("g_type")->setInt(21);
		buffer2->findParameter("g_mat34")->setMatrix(m);
		ASSERT_EQ(true, renderAndCapture().r > 200);
	}
	//* [ ] float2x2 (Vector2[2])
	{
		Matrix m(
			0, 0, 0, 0,
			1, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0);
		buffer1->findParameter("g_type")->setInt(22);
		buffer2->findParameter("g_mat22")->setMatrix(m);
		ASSERT_EQ(true, renderAndCapture().g > 200);
	}
	//* [ ] float4x3 (Vector3[4])
	{
		Matrix m(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 1, 0);
		buffer1->findParameter("g_type")->setInt(23);
		buffer2->findParameter("g_mat43")->setMatrix(m);
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}
	//* [ ] float4x4 (Vector4[4])
	{
		Matrix m(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 1,
			0, 0, 0, 0);
		buffer1->findParameter("g_type")->setInt(24);
		buffer2->findParameter("g_mat44")->setMatrix(m);
		ASSERT_EQ(true, renderAndCapture().r > 200);
	}

	// ※ TODO: 以下、行列配列だけ、中身が転置されてしまうようなので要調査。
	// 現状はその想定で基準を組んでいる。

	//* [ ] float3x4[3] (Vector4[3][,,])
	{
		Matrix m[3] = {
			{},
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 0,
			},
			{} };
		m[1](0, 1) = 1;
		buffer1->findParameter("g_type")->setInt(31);
		buffer2->findParameter("g_mat34ary3")->setMatrixArray(m, 3);
		ASSERT_EQ(true, renderAndCapture().b > 200);
	}

	//* [ ] float4x4 (Vector4[4][,,])
	{
		Matrix m[3] = {
			{},
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 0, 0, 1,
				0, 0, 0, 0,
			},
		{} };
		buffer1->findParameter("g_type")->setInt(34);
		buffer2->findParameter("g_mat44ary3")->setMatrixArray(m, 3);
		ASSERT_EQ(true, renderAndCapture().g > 200);
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, Texture)
{
	auto shader1 = Shader::create(LN_ASSETFILE("TextureTest-1.vsh"), LN_ASSETFILE("TextureTest-1.psh"));
	
	auto vertexDecl1 = newObject<VertexDeclaration>();
	vertexDecl1->addVertexElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);
	vertexDecl1->addVertexElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

	struct Vertex
	{
		Vector2 uv;
		Vector3 pos;
	};
	Vertex v[] = {
		{ { 0, 0 }, { -1, 1, 0 }, },
		{ { 1, 0 }, { 0, 1, 0 }, },
		{ { 0, 1 }, { -1, 0, 0 }, },
		{ { 1, 1 }, { 0, 0, 0 }, },
	};
	auto vb1 = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto tex1 = newObject<Texture2D>(2, 2);
	auto bmp1 = tex1->map(MapMode::Write);
	bmp1->setPixel32(0, 0, Color32(255, 0, 0, 255));
	bmp1->setPixel32(1, 0, Color32(255, 0, 255, 255));
	bmp1->setPixel32(0, 1, Color32(0, 255, 0, 255));
	bmp1->setPixel32(1, 1, Color32(0, 0, 255, 255));

	shader1->findParameter("g_texture1")->setTexture(tex1);

	auto ctx = Engine::graphicsContext();
	ctx->setVertexDeclaration(vertexDecl1);
	ctx->setVertexBuffer(0, vb1);
	ctx->setIndexBuffer(nullptr);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

	// * [ ] default
	{
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);

		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-TextureTest-1.png"));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, SamplerState)
{
	auto shader1 = Shader::create(LN_ASSETFILE("TextureTest-1.vsh"), LN_ASSETFILE("TextureTest-1.psh"));

	auto vertexDecl1 = newObject<VertexDeclaration>();
	vertexDecl1->addVertexElement(0, VertexElementType::Float2, VertexElementUsage::TexCoord, 0);
	vertexDecl1->addVertexElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);

	struct Vertex
	{
		Vector2 uv;
		Vector3 pos;
	};
	Vertex v[] = {
		{ { 0, 0 },{ -1, 1, 0 }, },
		{ { 2, 0 },{ 0, 1, 0 }, },
		{ { 0, 2 },{ -1, 0, 0 }, },
		{ { 2, 2 },{ 0, 0, 0 }, },
	};
	auto vb1 = newObject<VertexBuffer>(sizeof(v), v, GraphicsResourceUsage::Static);

	auto tex1 = newObject<Texture2D>(2, 2);
	auto bmp1 = tex1->map(MapMode::Write);
	bmp1->setPixel32(0, 0, Color32(255, 0, 0, 255));
	bmp1->setPixel32(1, 0, Color32(255, 0, 255, 255));
	bmp1->setPixel32(0, 1, Color32(0, 255, 0, 255));
	bmp1->setPixel32(1, 1, Color32(0, 0, 255, 255));

	shader1->findParameter("g_texture1")->setTexture(tex1);

	auto ctx = Engine::graphicsContext();
	ctx->setVertexDeclaration(vertexDecl1);
	ctx->setVertexBuffer(0, vb1);
	ctx->setIndexBuffer(nullptr);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

	// * [ ] default
	{
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);
		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-SamplerState-1.png"));
	}

	// * [ ] Linear, Clamp
	{
		auto sampler = newObject<SamplerState>();
		sampler->setFilterMode(TextureFilterMode::Linear);
		sampler->setAddressMode(TextureAddressMode::Clamp);
		tex1->setSamplerState(sampler);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);
		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-SamplerState-2.png"));
	}
}

//------------------------------------------------------------------------------
//## RenderState 関係のテスト。設定が Native layer まで渡ることを確認する。
TEST_F(Test_Graphics_LowLevelRendering, RenderStateTest)
{
	auto shader1 = Shader::create(LN_ASSETFILE("SimplePosColor.vsh"), LN_ASSETFILE("SimplePosColor.psh"));

	auto vertexDecl1 = newObject<VertexDeclaration>();
	vertexDecl1->addVertexElement(0, VertexElementType::Float3, VertexElementUsage::Position, 0);
	vertexDecl1->addVertexElement(0, VertexElementType::Float4, VertexElementUsage::Color, 0);

	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};
	Vertex v1[] = {	// 深度テスト用に少し奥に出しておく
		{ { -1, 1, 0.5 }, Color::Red },
		{ { 0, 1, 0.5 }, Color::Red },
		{ { -1, 0, 0.5 }, Color::Red },
		{ { 0, 0, 0.5 }, Color::Red },
	};
	auto vb1 = newObject<VertexBuffer>(sizeof(v1), v1, GraphicsResourceUsage::Static);

	Vertex v2[] = {
		{ { -0.5, 0.5, 0 }, Color::Blue },
		{ { 0.5, 0.5, 0 }, Color::Blue },
		{ { -0.5, -0.5, 0 }, Color::Blue },
		{ { 0.5, -0.5, 0 }, Color::Blue },
	};
	auto vb2 = newObject<VertexBuffer>(sizeof(v2), v2, GraphicsResourceUsage::Static);

	Vertex v3[] = {	// 裏面テスト用
		{ { 0, 0, 0 }, Color::Green },
		{ { 0, -1, 0 }, Color::Green },
		{ { 1, 0, 0 }, Color::Green },
		{ { 1, -1, 0 }, Color::Green },
	};
	auto vb3 = newObject<VertexBuffer>(sizeof(v3), v3, GraphicsResourceUsage::Static);

	auto ctx = Engine::graphicsContext();
	ctx->setVertexDeclaration(vertexDecl1);
	ctx->setIndexBuffer(nullptr);
	ctx->setShaderPass(shader1->techniques()[0]->passes()[0]);

	// * [ ] check BlendState (RGB Add blend)
	{
		BlendStateDesc state1;
		state1.renderTargets[0].blendEnable = true;
		state1.renderTargets[0].sourceBlend = BlendFactor::One;
		state1.renderTargets[0].destinationBlend = BlendFactor::One;
		state1.renderTargets[0].blendOp = BlendOp::Add;

		ctx->clear(ClearFlags::All, Color::Gray, 1.0f, 0);

		ctx->setBlendState(BlendStateDesc());
		ctx->setVertexBuffer(0, vb1);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);
		
		ctx->setBlendState(state1);
		ctx->setVertexBuffer(0, vb2);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);

		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-RenderStateTest-1.png"));

		ctx->setBlendState(BlendStateDesc());	// 戻しておく
	}
	// * [ ] check RasterizerState
	{
		RasterizerStateDesc state1;
		state1.cullMode = CullingMode::Front;

		RasterizerStateDesc state2;
		state2.cullMode = CullingMode::Back;

		RasterizerStateDesc state3;
		state3.cullMode = CullingMode::None;

		RasterizerStateDesc state4;
		state4.fillMode = FillMode::Wireframe;

		ctx->setRasterizerState(state1);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setVertexBuffer(0, vb1);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 赤は描かれない
		ctx->setVertexBuffer(0, vb3);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 緑は描かれる
		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-RenderStateTest-2-1.png"));

		ctx->setRasterizerState(state2);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setVertexBuffer(0, vb1);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 赤は描かれる
		ctx->setVertexBuffer(0, vb3);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 緑は描かれない
		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-RenderStateTest-2-2.png"));

		ctx->setRasterizerState(state3);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setVertexBuffer(0, vb1);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 赤は描かれる
		ctx->setVertexBuffer(0, vb3);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 緑は描かれる
		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-RenderStateTest-2-3.png"));

		ctx->setRasterizerState(state4);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setVertexBuffer(0, vb2);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);
		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-RenderStateTest-2-4.png"));

		ctx->setRasterizerState(RasterizerStateDesc());	// 戻しておく
	}

	// * [ ] check DepthStencilState
	{
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setVertexBuffer(0, vb2);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 青 (z=0)
		ctx->setVertexBuffer(0, vb1);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 赤 (z=0.5)
		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-RenderStateTest-3-1.png"));


		DepthStencilStateDesc state1;
		state1.depthTestEnabled = false;

		ctx->setDepthStencilState(state1);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setVertexBuffer(0, vb2);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 青 (z=0)
		ctx->setVertexBuffer(0, vb1);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 赤 (z=0.5)
		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-RenderStateTest-3-2.png"));


		DepthStencilStateDesc state2;
		state2.depthWriteEnabled = false;

		ctx->setDepthStencilState(state2);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setVertexBuffer(0, vb2);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 青 (z=0)
		ctx->setVertexBuffer(0, vb1);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 赤 (z=0.5)
		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-RenderStateTest-3-3.png"));

		ctx->setDepthStencilState(DepthStencilStateDesc());	// 戻しておく
	}

	// * [ ] check DepthStencilState (stencil test)
	{
		// ステンシル書き込み (参照値0xFF が型抜きの hole と考える)
		DepthStencilStateDesc state1;
		state1.stencilEnabled = true;							// true にしないと書き込まれない
		state1.frontFace.stencilPassOp = StencilOp::Replace;	// 描画できたところに参照値を書き込む
		state1.frontFace.stencilFunc = ComparisonFunc::Always;	// 常に成功（常に上書き）
		ctx->setDepthStencilState(state1);
		ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
		ctx->setVertexBuffer(0, vb1);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 赤 (z=0.5)

		// ステンシルテスト
		DepthStencilStateDesc state2;
		state2.stencilEnabled = true;
		state2.frontFace.stencilFunc = ComparisonFunc::Equal;	// 0xFFと等しければ成功 → カラーバッファ書き込み
		ctx->setDepthStencilState(state2);
		ctx->setVertexBuffer(0, vb2);
		ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);	// 青 (z=0)

		ASSERT_SCREEN(LN_ASSETFILE("Result/Test_Graphics_LowLevelRendering-RenderStateTest-4-1.png"));

		ctx->setDepthStencilState(DepthStencilStateDesc());	// 戻しておく

		// TODO: ステンシル書き込み時に描画はしない
	}
}

