
#include "Common.h"
#include "../../src/EngineManager.h"
#include "../../src/Modeling/ModelManager.h"
#include "../../src/Graphics/GraphicsManager.h"
#include "../../src/Graphics/Device/GraphicsDriverInterface.h"
#include <Lumino/Tilemap/TileMapModel.h>
#include <Lumino/Scene/TileMap.h>
#include <Lumino/Scene/MeshModelObject.h>
#include <Lumino/Scene/SpriteModelObject.h>
#include <Lumino/Testing/TestUtils.h>
//#include <Lumino/Platform/Win32/Win32PlatformWindow.h>
#include "../../../src/Scene/MME/MMEShader.h"


#include <Lumino/Graphics/PerlinNoise.h>
#include <Lumino/Graphics/BitmapPainter.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/SceneTR.h>
#include <thread>

#include <Lumino/UI/UIListBox.h>
#include <Lumino/UI/UIPanel.h>


//void LN_STDCALL Button1_MouseMove(CoreObject* sender, MouseEventArgs* e)
//{
//	printf("Button1_MouseMove\n");
//}

int TestA()
{
	return 10;
}


int g_test2 = TestA();

class TestRef1 : public RefObject
{
	
};

/*
(x1,y1)始点
(x2,y2)制御点
(x3,y3)終点 
*/
float BezierCurve2D(float x1, float x2, float x3, float t)
{
	float tp = 1.f - t;
	return t*t*x3 + 2.f*t*tp*x2 + tp*tp*x1;
}

/*
(x1,y1)始点
(x2,y2)制御点
(x3,y3)制御点
(x4,y4)終点 
*/
float BezierCurve1(float x1, float x2, float x3, float x4, float t)
{
	float tp = 1.f - t;
	return t*t*t*x4 + 3.f*t*t*tp*x3 + 3.f*t*tp*tp*x2 + tp*tp*tp*x1;
}


//LRESULT TestWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam, bool* handled)
//{
//	printf("%x\n", msg);
//	return 0;
//}

#if 0
class TestClass1
{
public:
	int value = 0;
};
class TestClass2
{
public:
	TestClass2()
		: t1(new TestClass1())
	{	
	}
	TestClass1* t1;
};

volatile TestClass2* g_ptr = nullptr;
volatile bool bbb = false;

void Thread1()
{
	//printf("Thread1\n");
	volatile int i = 0;
	for (; i < 100000000; ++i) {
	}
	g_ptr = new TestClass2();
	i = 0;
	for (; i < 100000000; ++i) {
	}
	i = 0;
	for (; i < 100000000; ++i) {
	}
	volatile TestClass2* t = g_ptr;
	g_ptr = nullptr;
	delete t;
	//printf("delete\n");
}

#pragma optimize("", off)
void Thread2()
{
	//printf("Thread2\n");
	while (g_ptr == nullptr) {}
	bbb = true;
	do 
	{
		g_ptr->t1->value++;
	} while (g_ptr != nullptr);
	//printf("Thread2 e\n");
}

#pragma optimize("", on)
#endif


//template<typename T>
//static void MarkResource(T& value)
//{
//	printf("MarkResource<T>\n");
//}
//template<typename T>
//static void MarkResource(const T& value)
//{
//	printf("MarkResource<const T>\n");
//}
//
//template<>
//static void MarkResource<RefObject*>(RefObject*& value)
//{
//	printf("MarkResource<RefObject*>\n");
//}
//
//template<>
//static void MarkResource<Point>(Point& value)
//{
//	printf("MarkResource<Point>\n");
//}




int main()
{

#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//Vector3 v;
	//Texture* tex = nullptr;
	//Point pt;
	//MarkResource(10);
	//MarkResource(v);
	//MarkResource(tex);
	//MarkResource(pt);

	//std::weak_ptr
	try
	{
		//ElapsedTimer timer;

		//ConditionFlag flag;
		//flag.SetFalse();

		//timer.Start();
		//Delegate<void()> func([&flag, &timer]()
		//{
		//	printf("%llu\n", timer.GetElapsedTimeNS());
		//	return;
		//	while (true)
		//	{
		//		flag.Wait();
		//		printf("%llu\n", timer.GetElapsedTimeNS());
		//		flag.SetFalse();
		//	}
		//});
		//DelegateThread thr;
		//thr.Start(func);
		//::Sleep(1000);

		////for (int i = 0; i < 10; ++i)
		////{
		////	timer.Start();
		////	flag.SetTrue();
		////	Thread::Sleep(10);
		////}

		//return 0;

#if 0
		for (int i = 0; i < 100; ++i)
		{
			DelegateThread thr1;
			DelegateThread thr2;
			thr1.Start(Thread1);
			thr2.Start(Thread2);
			thr1.Wait();
			thr2.Wait();
			printf("con %d\n", i);
		}
		printf("end\n");
		return 0;
		//printf("%d\n", g_ptr->t1->value);
#endif
#if 0
		StreamReader reader(_T("D:/Proj/Volkoff/External/Lumino/src/Graphics/Resource/DotNetColors.txt"));
		StreamWriter file1(_T("decls.txt"));
		StreamWriter file2(_T("defs.txt"));
		String line;
		while (reader.ReadLine(&line))
		{
			auto tokens = line.Split(_T(" "), StringSplitOptions::RemoveEmptyEntries);
			//Color32 c = Color32::FromString(tokens[1]);
			Color c = Color::FromString(tokens[1]);
			file1.WriteLine(_T("static const Color {0,-22} /** ({1}, {2}, {3}) */"), tokens[0] + _T(";"), c.r, c.g, c.b);
			file2.WriteLine(_T("Color Color::{0}({1}, {2}, {3});"), tokens[0], c.r, c.g, c.b);
		}
		return 0;
#endif
#if 0
		StreamWriter file1(_T("ColorTable.txt"));
		String line;
		for (int i = 0; i < 256; ++i)
		{
			file1.Write(String::Format(_T("{0}, "), ((double)i) / 255.0));
			if (i % 16 == 15) file1.WriteLine();
		}
		return 0;
#endif
#if 0
		Bitmap bmp(LN_LOCALFILE("../../test/UnitTest/Graphics/TestData/Test_Graphics_RenderingContext1.png"));
		for (int y = 0; y < bmp.GetSize().height; ++y)
		{
			for (int x = 0; x < bmp.GetSize().width; ++x)
			{
				Color c = bmp.GetPixel(x, y);
				c.r = (c.r / 31) * 31;
				c.b = (c.b / 63) * 63;
				c.g = (c.g / 31) * 31;
				bmp.SetPixel(x, y, c);
			}
		}
		bmp.Save(_T("Test_Graphics_RenderingContext1.png"));
		return 0;
#endif
#if 0
		StreamWriter file1(_T("GifPalette1.txt"));
		int rc = 8;
		int gc = 4;
		int bc = 8;
		int idx = 0;
		for (int b = 0; b < bc; ++b)
		{
			for (int g = 0; g < gc; ++g)
			{
				for (int r = 0; r < rc; ++r)
				{
					file1.Write(_T("{{ 0x{0:X2}, 0x{1:X2}, 0x{2:X2} }}, "),
						(r == rc - 1) ? 255 : r * (255 / rc - 1),
						(g == gc - 1) ? 255 : g * (255 / gc - 1),
						(b == bc - 1) ? 255 : b * (255 / bc - 1));
					if (idx % 8 == 7) file1.WriteLine();
					++idx;

				}
			}
		}
		return 0;
#endif







		EngineSettings appData;
		EngineSettings::SetGraphicsAPI(GraphicsAPI::DirectX9);
		EngineSettings::SetGraphicsRenderingType(GraphicsRenderingType::Immediate);
		EngineSettings::SetDirectMusicMode(DirectMusicMode::Normal);
		//EngineSettings::SetMainWindowSize(160, 120);
		//EngineSettings::SetMainBackBufferSize(160, 120);
		Engine::Initialize();


		auto uiRoot = UIContext::GetMainContext()->GetMainWindowView()->GetLayoutRoot();
		auto canvas1 = UICanvas::Create();
		canvas1->SetHorizontalAlignment(HorizontalAlignment::Stretch);
		canvas1->SetVerticalAlignment(VerticalAlignment::Stretch);
		//canvas1->SetBackground(ColorBrush::DimGray);
		uiRoot->SetContent(canvas1);

		auto listbox1 = tr::UIListBox::Create();
		//listbox1->SetHorizontalAlignment(HorizontalAlignment::Stretch);
		//listbox1->SetVerticalAlignment(VerticalAlignment::Stretch);
		listbox1->SetPosition(PointF(5, 10));
		listbox1->SetSize(SizeF(100, 200));
		listbox1->SetBackground(ColorBrush::Red);
		auto item1 = listbox1->AddTextItem(_T("item1"));
		auto item2 = listbox1->AddTextItem(_T("item2"));
		item1->SetBackground(ColorBrush::Green);
		item2->SetBackground(ColorBrush::Blue);
		canvas1->AddChild(listbox1);

		Viewport::GetMainWindowViewport()->SetBackgroundColor(Color::Gray);

		while (Engine::UpdateFrame())
		{
		}

		return 0;










		//auto tex = Assets::LoadTexture(LN_LOCALFILE("../../test/UnitTest/Graphics/TestData/0129.png"));

		auto noiseTex1 = Texture2D::Create(SizeI(256, 256));

		auto tex = Texture2D::Create(SizeI(256, 256));
		auto sp1 = Sprite2D::Create(noiseTex1);
		//sp1->SetVisible(false);

		PerlinNoise noise;
		noise.SetTiledRepeatFrequency(4);
		double frequency = 4;
		const double fx = tex->GetWidth() / frequency;
		const double fy = tex->GetHeight() / frequency;
		int octaves = 8;	// 粗さ

		Bitmap bmp1(SizeI(256, 256), PixelFormat::R8G8B8A8);
		for (int y = 0; y < bmp1.GetSize().height; ++y)
		{
			for (int x = 0; x < bmp1.GetSize().width; ++x)
			{
				double n = noise.Noise2D(x / fx, y / fy, octaves);
				//double n = noise.OctavePerlin(x / fx, y / fy, 0, octaves, 0.5f);
				//double n = noise.Noise(x/fx, y / fy);

				//if (n < 0)
				//{
				//	double n1 = noise.Noise(1, 98);
				//	double n2 = noise.Noise(200, 400);
				//	double n3 = noise.Noise(0.1f, 0.98f);
				//	double n4 = noise.Noise(3.1f, 5.98f);
				//	double n5 = noise.Noise(0.09999990f, 0.98000002f);
				//	double n6 = noise.Noise(0.0999999046f, 0.980000019f);
				//}

				

				//n = Math::Clamp(n*0.5 + 0.5, 0.0, 1.0);
				//n *= n;

				float weight = (2.0f * y / bmp1.GetSize().height);
				//weight -= 1.0f;
				n = Math::Clamp01(n * weight);
				n *= n;

				unsigned char gray = static_cast<unsigned char>(n*255.99);
				//gray = 255 - gray;

				bmp1.SetPixel(x, y, Color32(gray, gray, gray, 255));
				//bmp1.SetPixel(x, y, Color32(255, 255, 255, gray));
			}
		}
		Bitmap* nbmp = noiseTex1->Lock();
		nbmp->BitBlt(0, 0, &bmp1, Rect(0, 0, 256, 256), Color32::White, false);
		noiseTex1->Unlock();

		int src_w = bmp1.GetSize().width;
		int src_h = bmp1.GetSize().height;
		Bitmap dst1(SizeI(512, 256), PixelFormat::R8G8B8A8);
		{
			// http://sssiii.seesaa.net/article/411311322.html?seesaa_related=related_article
			// rの最大値を計算（定数でも可）
			float halfW = bmp1.GetSize().width / 2.0f;
			float halfH = bmp1.GetSize().height / 2.0f;
			float r_max = sqrt(halfW*halfW + halfH*halfH);
			// x,y座標からr,Θ座標を計算
			float r, theta;
			int xa, ya;
			int dst_w = dst1.GetSize().width;
			int dst_h = dst1.GetSize().height;
			for (int yb = 0; yb < dst_h; yb++)
			{
				int half_dst_h = (dst_h / 2);
				int ny = yb - half_dst_h;
				for (int xb = 0; xb < dst_w; xb++)
				{

					//float lambda = (xb / (float)(dst_w - 1))*2.0*Math::PI;
					float theta = (ny / (float)(half_dst_h - 1))*(Math::PI/2);	// -90度～90度
					float ry = (1.0f / Math::PI) * tan(Math::Clamp(theta*0.75f, -Math::PI / 2, Math::PI / 2));//tan(log(Math::PI / 4 + theta));//cos(log(theta));//
					int ya = (ry * half_dst_h) + half_dst_h;
					int xa = xb;

					if (0 <= xa && xa <= src_w - 1 && 0 <= ya && ya <= src_h - 1)
					{
						Color32 c = bmp1.GetPixel(xa, ya);
						dst1.SetPixel(xb, yb, c);
					}
					else
					{
						dst1.SetPixel(xb, yb, Color32::Red);
					}
#if 0
					// x, y座標をr, Θ座標に変換
					r = (1.0 - yb / (float)(dst_h - 1))*r_max;
					theta = (xb / (float)(dst_w - 1))*2.0*Math::PI - Math::PI;
					// r,Θから原画像のx,y座標を計算
					xa = (int)(r*cos(theta) + halfW);
					ya = (int)(r*sin(theta) + halfH);
					// 色のコピー
					if (0 <= xa && xa <= src_w - 1 && 0 <= ya && ya <= src_h - 1)
					{
						Color32 c = bmp1.GetPixel(xa, ya);
						dst1.SetPixel(xb, yb, c);
					}
					else
					{
						dst1.SetPixel(xb, yb, Color32::Red);
					}
#endif
				}
			}
		}


		//Bitmap dst1flip(SizeI(256, 256), PixelFormat::R8G8B8A8);
		//BitmapFilter filter;
		//filter.FlipVertical(&dst1flip, &dst1);

		Bitmap* bmp = tex->Lock();
		//bmp->BitBlt(0, 0, &dst1flip, Rect(0, 0, 256, 128), Color32::White, false);
		bmp->BitBlt(0, 0, &dst1, Rect(0, 0, 256, 256), Color32::White, false);
		//bmp->BitBlt(0, 0, &bmp1, Rect(0, 0, 256, 256), Color32::White, false);
		tex->Unlock();

		auto shader = MMEShader::Create(_T("D:/Proj/Volkoff/External/Lumino/src/Scene/Resource/BasicForwardRendering.fx"));
		auto mesh = StaticMesh::CreateSphere(20, 32, 16, MeshCreationFlags::ReverseFaces);
		mesh->SetShader(shader);
		mesh->GetMaterials()->GetAt(0)->SetTextureParameter(Material::MaterialTextureParameter, noiseTex1);
		mesh->GetMaterials()->GetAt(0)->SetColorParameter(Material::EmissiveParameter, Color::White);
		


		//auto shader2 = MMEShader::Create(_T("C:/Proj/Lumino/src/Scene/Resource/Cloud.fx"));
		//auto plane = StaticMesh::CreateSquarePlane(Vector2(100, 100), Vector3::UnitY);
		//plane->SetShader(shader2);
		//plane->GetMaterials()->GetAt(0)->SetTextureParameter(Material::MaterialTextureParameter, noiseTex1);
		//plane->GetMaterials()->GetAt(0)->SetColorParameter(Material::EmissiveParameter, Color::White);
		//plane->SetVisible(false);



		auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
		Camera::GetMain3DCamera()->SetCameraBehavior(cb);

		//auto grid1 = Grid::Create3D();

		auto m = SpriteParticleModel::Create();
		m->SetSpawnRate(100);
		m->SetLifeTime(3.0f);
		m->m_maxParticles = 300;

		//m->m_shapeType = ParticleEmitterShapeType::Sphere;
		//m->m_shapeParam.x = 10;


		m->m_shapeType = ParticleEmitterShapeType::Cone;
		m->m_shapeParam.x = Math::PI * 0.1;
		m->m_shapeParam.y = 2;

		//m->SetPositionRange(Vector3(-10, 10, -10), Vector3(10, 10, 10));
		m->SetVelocity(Vector3(0, -3, 0));
		//m->SetAccel(Vector3(0, -8, 0));
		m->SetTexture(Texture2D::Create(LN_LOCALFILE("../Media/Spark1.png"), TextureFormat::R8G8B8A8, false));
		auto particle1 = SpriteParticle::Create3D(m);
		particle1->SetBlendMode(BlendMode::Subtract);
		particle1->SetPosition(2, 0, 0);

		auto particle2 = SpriteParticle::Create3D(m);
		particle2->SetBlendMode(BlendMode::Subtract);
		particle2->SetPosition(3, 0, 0);

		particle1->AddChild(particle2);

		//GameAudio::PlayBGM("D:/Proj/Lumino/bindings/Ruby/sample/Media/ln21.mid");
		//GameAudio::PlayBGM("D:/tmp/GrandSky.mp3");

#if 0
		//auto* window = dynamic_cast<Win32NativeWindow*>(Engine::GetApplication()->GetNativeMainWindow());
		//window->UserWndProc = TestWndProc;

		//auto tex = Texture2D::Create(LN_LOCALFILE("../../test/UnitTest/Graphics/TestData/Test_Graphics_RenderingContext1.png"));
		auto tex = Assets::LoadTexture(LN_LOCALFILE("../../test/UnitTest/Graphics/TestData/Test_Graphics_RenderingContext1.png"));
		tex = Assets::LoadTexture(LN_LOCALFILE("../../test/UnitTest/Graphics/TestData/Test_Graphics_RenderingContext1.png"));
		
		//tex->DrawText(_T("lumino"), Rect(0, 0, 100, 100), nullptr, Color::Red, Color::Blue, 0, TextAlignment::Left);
		auto size = Font::GetDefaultFont()->GetTextSize(_T("lumino"));
		auto sp1 = Sprite2D::Create(tex);
		//sp1.SafeRelease();
		auto sp2 = Sprite2D::Create(LN_LOCALFILE("../../test/UnitTest/Graphics/TestData/Test_Graphics_IndexBuffer.BasicTriangle.png"));
		sp2->SetPosition(100, 240, 0);
		

		//auto if1 = ScreenMotionBlurImageEffect::Create();
		auto if2 = ToneImageEffect::Create();
		//if2->SetTone(ToneF(-0.1, -0.4, -0.7, 1.0));
		//if2->ChangeTone(ToneF(-0.1, -0.4, -0.7, 1.0), 5);
		//CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(if1);
		CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(if2);
			//Viewport::GetMainWindowViewport()->AddViewportLayer();

		//auto t1 = Texture2D::Create(LN_LOCALFILE("TestData/Test_Graphics_RenderingContext1.png"));
		//auto t2 = RenderTarget::Create(t1->GetSize());


		//auto s = Shader::Create(_T("C:/Proj/Lumino/src/Scene/Resource/SSBasic2D.fx"));
		//auto s = Shader::Create(LN_LOCALFILE("../Media/TestShader1.fx"));


		//Color cc;
		//cc.r = 1000;
		//cc.Set(100, 0, if2->GetReferenceCount(), 1);
		//cc.r = (if2->GetRefCount(), 0, 0, 1);


		//for (SoundPtr s : sounds) { s->Play(); }


		//ElapsedTimer t;
		//t.Start();
#endif
		//Viewport::GetMainWindowViewport()->SetBackgroundColor(Color::BlueViolet);

		//Viewport::GetMainWindowViewport()

		//auto frameCapturer = FrameCapturer::Create();
		//frameCapturer->SetCapturerTarget(GraphicsManager::GetInstance()->GetMainSwapChain()->GetBackBuffer());
		//frameCapturer->StartRecording();
		

		float fc = 0;
		while (true)
		{
			//if (Input::IsTriggered(InputButtons::Left))
			//{
			//	GameAudio::PlaySE("D:/Proj/Volkoff/Volkoff/Data/Sound/SE/sword1.wav");
			//}

			//printf("%d\n", t.GetElapsedTime());

			//sp1->SetAngles(0, 0, fc);

			fc += 0.01;

			//frameCapturer->Record();

			if (!Engine::UpdateFrame()) {
				break;
			}

			//if (Engine::BeginRendering())
			//{
			//	Engine::Render();

			//	RenderingContext* r = RenderingContext::GetContext();

			//	//if (sp->GetTexture() == nullptr)
			//	//{
			//	//	auto t1 = Texture2D::Create(_T("C:/Proj/Lumino/test/UnitTest/Graphics/TestData/Test_Graphics_RenderingContext1.png"));
			//	//	auto t2 = RenderTarget::Create(t1->GetSize());
			//	//	r->Blt(t1, t2);
			//	//	sp->SetTexture(t2);
			//	//}


			//	Engine::EndRendering();
			//}

		}

		//frameCapturer->StopRecording();

		Engine::Terminate();
	}
	catch (Exception& e)
	{
		printf(e.what());
	}

	return 0;
}


//------------------------------------------------------------------------------
DemoManager::DemoMainFunc DemoManager::RegisterDemo(const char* name, DemoMainFunc func)
{
    String t = String::FromNativeCharString(name);
	Array<String> tokens = t.Split(_T("."));
	DemoInfo info;
	info.Group1 = tokens[0];
	info.Group2 = tokens[1];
	info.Caption = tokens[2];
	info.Main = func;
	//m_demos.Add(info);

	//m_demosTable[info.Group1][info.Group2].Add(info);
	return func;
}

Array<DemoManager::DemoInfo>			DemoManager::m_demos;
std::map<String, std::map<String, Array<DemoManager::DemoInfo> > >	DemoManager::m_demosTable;
