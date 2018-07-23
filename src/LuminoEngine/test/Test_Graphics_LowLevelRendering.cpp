#include "Common.hpp"

class Test_Graphics_LowLevelRendering : public ::testing::Test
{
public:
	virtual void SetUp()
	{
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

		ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-BasicTriangle-1.png"));
	}
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, VertexBuffer)
{
	//m_shader1->setVector("g_color", Vector4(1, 0, 0, 1));

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

			ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-VertexBuffer-2.png"));
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

			ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-VertexBuffer-3.png"));
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

			ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-VertexBuffer-4.png"));
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

			ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-VertexBuffer-5.png"));
		}
	}


	// TODO: 部分 lock
}

//------------------------------------------------------------------------------
TEST_F(Test_Graphics_LowLevelRendering, IndexBuffer)
{
	//m_shader1->setVector("g_color", Vector4(0, 0, 1, 1));

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

			ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-IndexBuffer-1.png"));
		}

		// * [ ] 一度レンダリングに使用されたバッファを、再更新できること
		{
			uint16_t indices[] = { 1, 2, 4 };
			memcpy(ib1->map(MapMode::Write), indices, ib1->bytesSize());

			ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
			ctx->drawPrimitiveIndexed(PrimitiveType::TriangleList, 0, 1);

			ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-IndexBuffer-2.png"));
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

				ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-IndexBuffer-2.png"));	// ↑と同じ結果
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

	ctx->clear(ClearFlags::All, Color::White, 1.0f, 0);
	ctx->drawPrimitive(PrimitiveType::TriangleStrip, 0, 2);

	ASSERT_SCREEN(LN_ASSETFILE("Test_Graphics_LowLevelRendering-TextureTest-1.png"));
}
