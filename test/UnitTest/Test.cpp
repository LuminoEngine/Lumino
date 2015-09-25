// なんか試したくなった時用テスト
#include "TestConfig.h"
#include "../../src/Graphics/Device/DeviceInterface.h"

class Test_Temp : public ::testing::Test
{
protected:
	virtual void SetUp() {}
	virtual void TearDown() {}
};

//-----------------------------------------------------------------------------
TEST_F(Test_Temp, Basic)
{
#if 0
	// 床
	RefPtr<Physics::Plane> plane1(LN_NEW Physics::Plane(TestEnv::PhysicsManager));

	// 箱
	Matrix m = Matrix::Translation(Vector3(2, 10, 0));
	Physics::Box::ConfigData initialData;
	initialData.Mass = 1.0f;
	initialData.InitialTransform = &m;
	RefPtr<Physics::Box> box1(LN_NEW Physics::Box(TestEnv::PhysicsManager, Vector3(1, 2, 3), initialData));


	//Renderer* r = TestEnv::Renderer;
	SwapChain* swap = TestEnv::MainSwapChain;
	while (TestEnv::Application->DoEvents())
	{
		TestEnv::PhysicsManager->StepSimulation(0.016f);


		if (TestEnv::Application->GetMainWindow()->GetSize() != swap->GetBackBuffer()->GetSize())
		{
			swap->Resize(TestEnv::Application->GetMainWindow()->GetSize());
		}

		Renderer* r = TestEnv::BeginRendering();
		r->SetViewport(Rect(0, 0, swap->GetBackBuffer()->GetSize()));

		TestEnv::MMDSceneGraph->UpdateFrame(0.01f);
		TestEnv::MMDSceneGraph->GetMMDViewPane()->Render(swap->GetBackBuffer());

		TestEnv::EndRendering();
		::Sleep(10);
	}
#endif

#if 0
	while (TestEnv::Application->DoEvents())
	{
		::Sleep(32);
	}
#endif

#if 0
	//Platform::ApplicationSettings s;
	//Platform::Application pm(s);

	//Graphics::GraphicsManagerConfigData gmcd;
	//gmcd.MainWindow = pm.GetMainWindow();
	//Graphics::GraphicsManager gm(gmcd);

	//Device::IGraphicsDevice* device = gm.GetGraphicsDevice()->GetDeviceObject();

	RefPtr<Platform::Window> window2(Platform::Window::Create(_T("win2"), Size(200, 200), true));
	//RefPtr<Device::ISwapChain> swap2(device->CreateSwapChain(window1));

	//Graphics::SwapChainPtr swapChain = gm.GetMainSwapChain();

	//Device::ITexture* swap1RT = device->CreateRenderTarget(640, 480, 1, Device::SurfaceFormat_A8R8G8B8);
	//Device::ITexture* swap1DT = device->CreateDepthBuffer(640, 480, Device::SurfaceFormat_D24S8);



	// 頂点レイアウト・頂点バッファ
	VertexElement elements[] =
	{
		{ 0, VertexElementType_Float3, VertexElementUsage_Position, 0 },
		{ 0, VertexElementType_Color4, VertexElementUsage_Color, 0 },
	};
	struct Vertex
	{
		Vector3		Pos;
		uint32_t	Color;
	};
	Vertex vertices[] = {
			{ Vector3(-0.5, 0.5, 0.0), 0xFF0000FF },
			{ Vector3(0.5, -0.5, 0.0), 0xFFFF00FF },
			{ Vector3(-0.5, -0.5, 0.0), 0xFF00FFFF },
	};
	RefPtr<VertexBuffer> vb(VertexBuffer::Create(
		elements, LN_ARRAY_SIZE_OF(elements), LN_ARRAY_SIZE_OF(vertices), vertices, DeviceResourceUsage_Static));

	// シェーダ
	const char code[] =
		"#ifdef LN_GLSL_VERTEX\n"
		//"attribute vec4 gl_Vertex;\n"
		"attribute vec4 ln_Vertex;\n"
		"attribute vec4 ln_Color;\n"
		"varying vec4 vColor;\n"
		"void main() {\n"
		"	vColor = ln_Color;\n"
		//"	gl_Position = gl_Vertex;\n"
		"	gl_Position = ln_Vertex;\n"
		"}\n"
		"#endif\n"

		
		"#ifdef LN_GLSL_FRAGMENT\n"
		"varying vec4 vColor;\n"
		"void main()\n"
		"{\n"
		"	gl_FragColor = vColor;\n"
		//"	gl_FragColor = vec4(1, 0, 0, 1);\n"
		"}\n"
		"#endif\n"		// 必ず改行が必要
		
		;
	ShaderCompileResult result;
	RefPtr<Shader> shader(Shader::Create(code));

	

	//Device::IRenderer* r = device->GetRenderer();

	// MainWindow
	//device->GetDefaultSwapChain()->Begin();
	//r->SetRenderTarget(0, swap1RT);
	//r->SetDepthBuffer(swap1DT);
	//r->Clear(true, false, ColorF(0, 1, 1, 1), 1.0f);

	////device->GetRenderer()->SetVertexBuffer(vb);
	////shader->GetTechnique(0)->GetPass(0)->Begin();
	////device->GetRenderer()->DrawPrimitive(Device::PrimitiveType_TriangleList, 0, 1);
	////shader->GetTechnique(0)->GetPass(0)->End();

	////device->GetDefaultSwapChain()->End();
	//device->GetDefaultSwapChain()->Present(swap1RT);




	//device->GetRenderer()->Clear(true, true, ColorF(0, 0, 1, 0), 1.0f);

	//device->GetRenderer()->SetVertexBuffer(vb);
	//shader->GetTechnique(0)->GetPass(0)->Begin();
	//device->GetRenderer()->DrawPrimitive(Device::PrimitiveType_TriangleList, 0, 1);
	//shader->GetTechnique(0)->GetPass(0)->End();

	//swap2->Present();


	Renderer* r = TestEnv::Renderer;//gm.GetRenderer();
	SwapChain* swap1 = TestEnv::MainSwapChain;
	//RefPtr<SwapChain> swap2(LN_NEW SwapChain(window2));
	
	int count = 0;
	while (TestEnv::Application->DoEvents())
	{
		r->SetRenderTarget(0, swap1->GetBackBuffer());
		r->SetDepthBuffer(swap1->GetBackBufferDepth());
		r->Clear(true, true, ColorF(0, 0.01f * count, 1, 1), 1.0f);
		r->SetVertexBuffer(vb);
		shader->GetTechniques()[0]->GetPasses()[0]->Apply();
		r->DrawPrimitive(PrimitiveType_TriangleList, 0, 1);
		swap1->Present();

		//r->SetRenderTarget(0, swap2->GetBackBuffer());
		//r->SetDepthBuffer(swap2->GetBackBufferDepth());
		//r->Clear(true, true, ColorF(0.01f * count, 1, 0, 1), 1.0f);
		//swap2->Present();

		++count;
		Threading::Thread::Sleep(32);

		if (count == 1)
		{
			swap1->GetBackBuffer()->Lock()->Save(LOCALFILE("Test.png"));
			swap1->GetBackBuffer()->Unlock();
		}

		//gm.GetMainSwapChain()->Present();
		//::Sleep(1000);
	}
#endif
}

