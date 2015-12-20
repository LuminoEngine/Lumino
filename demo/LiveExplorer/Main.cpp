#include "Common.h"
#include "../../src/ApplicationImpl.h"
#include "../../src/Modeling/ModelManager.h"
#include <Lumino/Tilemap/TileMap.h>
#include <Lumino/Scene/TileMapNode.h>
#include <Lumino/Testing/TestUtils.h>

void LN_STDCALL Button1_MouseMove(CoreObject* sender, MouseEventArgs* e)
{
	printf("Button1_MouseMove\n");
}

int TestA()
{
	return 10;
}

void Button_Click(RoutedEventArgs* e)
{
	printf("click\n");
}

int g_test2 = TestA();

class TestRef1 : public RefObject
{

};

int main()
{

#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//ByteBuffer buf1;
	//buf1.Resize(sizeof(Matrix) * 1000);
	//ByteBuffer buf2;
	//buf2.Resize(sizeof(Matrix) * 1000);

	//for (int i = 0; i < 10; ++i)
	//{
	//	ElapsedTimer t;
	//	t.Start();
	//	memcpy(buf1.GetData(), buf2.GetData(), buf1.GetSize());
	//	printf("%lld\n", t.GetElapsedTimeNS());
	//}


	try
	{
		auto fff = ColorF::Green;
		ColorBrush ff(ColorF(1, 0, 0, 1));
		auto aa = ColorBrush::Green;
		ApplicationSettings appData;
		appData.GraphicsAPI = GraphicsAPI::DirectX9;
		appData.RenderingType = RenderingType::Deferred;
		//appData.DirectMusicMode = DirectMusicMode::Normal;
		Engine::Initialize(appData);

		//RefPtr<Texture2D> tex1(Texture2D::Create(_T("D:/tmp/9.png")), false);
		//RefPtr<Sprite> sp1(Sprite::Create3D(), false);
		//sp1->SetTexture(tex1);
		//sp1->SetPosition(Vector3(100, 0, 0));
		//sp1->SetScale(Vector3(0.1, 0.1, 0.1));

		auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::Create();
		Camera::GetDefault3DCamera()->SetCameraBehavior(cb);
		//Camera::GetDefault3DCamera()->SetPosition(Vector3(200,0,-20));

		//auto effect1 = VisualEffect::Create(_T("D:/Programing/Effekseer/Effekseer-master/Release/Sample/00_Basic/Laser01.efk"));
		auto effect1 = VisualEffect::Create(LN_LOCALFILE("../Media/Sun1.efk"));
		effect1->Play();
#if 0
		RefPtr<Texture2D> tex2(Texture2D::Create(_T("D:/tmp/tiles.png")), false);
		RefPtr<TileMap> tilemap(LN_NEW TileMap(), false);
		RefPtr<TileSet> tileset(LN_NEW TileSet(), false);
		tileset->SetImageSource(tex2);
		tileset->SetTileSize(Size(32, 32));
		tilemap->SetTileSet(tileset);
		RefPtr<TileLayer> tileLayer(LN_NEW TileLayer(), false);
		tileLayer->Resize(20,15);
		tileLayer->SetTileId(1, 1, 1);
		tilemap->GetLayers()->Add(tileLayer);

#endif
		RefPtr<TileMapNode> tilemapNode(TileMapNode::Create3D(), false);
		//tilemapNode->SetTileMap(tilemap);

		RefPtr<Physics::Collider> collider2(Physics::PlaneCollider::Create(), false);
		RefPtr<Physics::RigidBody> rigidBody2(Physics::RigidBody::Create(collider2), false);

		//RefPtr<Physics::RigidBody> b1(LN_NEW Physics::Capsule(Physics::PhysicsManager::Instance, 2, 4, 1.0f));
		RefPtr<Physics::Collider> collider1(Physics::SphereCollider::Create(2), false);
		RefPtr<Physics::RigidBody> rigidBody1(Physics::RigidBody::Create(collider1), false);
		rigidBody1->SetPosition(0, 10, 0);
		rigidBody1->SetMass(1.0f);

		RefPtr<Physics::Collider> collider3(Physics::CapsuleCollider::Create(0.5, 2), false);
		RefPtr<Physics::RigidBody> rigidBody3(Physics::RigidBody::Create(collider3), false);
		rigidBody3->SetPosition(0.1, 10, 0.5);
		rigidBody3->SetMass(1.0f);
		rigidBody3->SetConstraintFlags(Physics::RigidBodyConstraintFlags::FreezePositionZ);

		while (Engine::UpdateFrame())
		{
			if (Input::IsPress(InputButtons::Left))
			{
				rigidBody3->ApplyForce(Vector3(-10.1, 0, 0));
			}

			//rigidBody1->GetWorldTransform().GetPosition().Print();
			Physics::PhysicsManager::Instance->SyncBeforeStepSimulation();
			Physics::PhysicsManager::Instance->StepSimulation(0.016);
		}




		//GCPtr<Sound> s = Sound::Create(_T("D:/Proj/Lumino/bindings/Common/Media/ln21.mid"));
		//GCPtr<Sound> s = Sound::Create(_T("D:/tmp/ZIGG-ZAGG.mp3"));
		//s->SetLoopEnabled(true);
		////s->SetPitch(110);
		//s->Play();

		//GameAudio::PlayBGM(_T("D:/tmp/ZIGG-ZAGG.mp3"));

		//::Sleep(1000);

		//s->Stop();

		


#if 0
		auto* captionStyle = Style::Create();
		captionStyle->AddSetter(UITextElement::FontSizeProperty, 24);
		captionStyle->AddSetter(UITextElement::ForegroundProperty, ColorBrush::Blue);


		RefPtr<GUIContext> context1 = GUIContext::Create();
		context1->InjectViewportSizeChanged(640, 480);


		Grid* rootGrid = Grid::Create();
		context1->SetRootElement(rootGrid);

		{
			auto* panel = StackPanel::Create(rootGrid);
			panel->SetHorizontalAlignment(HorizontalAlignment::Left);

			auto* text = TextBlock::Create(panel, _T("test"));
			text->SetStyle(captionStyle);
			text->SetHorizontalAlignment(HorizontalAlignment::Center);

			auto* button1 = UIButton::Create(panel, _T("Normal"));
			button1->SetSize(SizeF(120, 40));
			button1->Click += [](RoutedEventArgs* e) { printf("ttt\n"); };

			auto* button2 = UIButton::Create(panel, _T("Disabled"));
			button2->SetSize(SizeF(120, 40));
			button2->SetEnabled(false);

		}




		while (Engine::UpdateFrame())
		{
			AutoReleasePool::GetCurrent()->Clear();
		}

		AutoReleasePool::GetCurrent()->Clear();

		//s->Stop();
		//s.SafeRelease();

		Engine::Finalize();
#endif

#if 0
		//RefPtr<TestRef1> ttt(LN_NEW TestRef1());
		//GenericCoreList<TestRef1*> testlist22;
		//testlist22.Add(ttt);

		//return 0;

		ApplicationSettings appData;
		//appData.GraphicsAPI = Graphics::GraphicsAPI::OpenGL;
		//appData.RenderingType = Graphics::RenderingType::Immediate;
		RefPtr<ApplicationImpl> app(ApplicationImpl::Create(appData));
		app->Initialize();


		app->GetPlatformManager()->GetMainWindow()->SetCursorVisible(false);


		app->GetGraphicsManager()->GetFontManager()->RegisterFontFile(LOCALFILE("../../tools/VLGothic/VL-Gothic-Regular.ttf"));
		app->GetGraphicsManager()->GetFontManager()->RegisterFontFile(LOCALFILE("../../tools/VLGothic/VL-PGothic-Regular.ttf"));

		// TODO: RefObject のデストラクタで、参照カウント1以上ならエラー。
		RefPtr<Graphics::Font> font1(Graphics::Font::Create(app->GetGraphicsManager()->GetFontManager()));	// TODO: ダサすぎる。デフォルトの設定方法も考える。
		


		//GenericCoreList<int> testlist1;
		//testlist1.Add(10);
		//printf("%d\n", testlist1.GetAt(0));

		////const Graphics::Font* f;
		////Graphics::Font* f2;
		////f2 = f;

		//GenericCoreList<Graphics::Font*> testlist2;
		//testlist2.Add(font1.GetObjectPtr());
		//printf("%p\n", testlist2.GetAt(0));
		//printf("%p\n", font1.GetObjectPtr());

		//return 0;
		
		font1->SetName(_T("VL Gothic"));
		font1->SetAntiAlias(true);
		app->GetGraphicsManager()->GetFontManager()->SetDefaultFont(font1);

		//RefPtr<RootFrame> workbench1(app->GetGUIManager()->CreateRootFrame());
		RootFrame* workbench1 = app->GetGUIManager()->GetDefaultRootFrame();
		//workbench1->SetSize(SizeF(640, 480));
		workbench1->SetPropertyValue(RootFrame::SizeProperty , SizeF(640, 480));

		auto grid1 = Grid::Create();
		grid1->AddColumnDefinition(180);
		grid1->AddColumnDefinition(ColumnDefinition::Star);
		workbench1->SetContent(grid1);

		auto demoList = ListBox::Create();
		Grid::SetColumn(demoList, 1);
		grid1->GetChildren()->Add(demoList);



		//RefPtr<Graphics::Brush> b(Graphics::ColorBrush::Blue, true);

		// Normal
		RefPtr<FloatEasing> easing1(LN_NEW FloatEasing());
		easing1->SetTargetName(_T("MouseOnBackground"));
		easing1->SetTargetProperty(UIElement::OpacityProperty->GetName());
		easing1->SetTargetValue(0.0f);
		easing1->SetEasingMode(Animation::EasingMode::EaseOutExpo);
		easing1->SetDuration(1.0f);

		// MouseOver
		RefPtr<FloatEasing> easing2(LN_NEW FloatEasing());
		easing2->SetTargetName(_T("MouseOnBackground"));
		easing2->SetTargetProperty(UIElement::OpacityProperty->GetName());
		easing2->SetTargetValue(1.0f);
		easing2->SetEasingMode(Animation::EasingMode::EaseOutExpo);
		easing2->SetDuration(1.0f);

		VisualStateGroupPtr vgroup1(LN_NEW VisualStateGroup(_T("CommonStates")));
		VisualStatePtr vstate1(LN_NEW VisualState(app->GetGUIManager(), _T("Normal")));
		vstate1->GetStoryboard()->AddTimeline(easing1);
		VisualStatePtr vstate2(LN_NEW VisualState(app->GetGUIManager(), _T("MouseOver")));
		vstate2->GetStoryboard()->AddTimeline(easing2);
		vgroup1->AddState(vstate1);
		vgroup1->AddState(vstate2);


		for (auto& pair1 : DemoManager::m_demosTable)
		{
			auto group1 = demoList->AddGroup(pair1.first);

			for (auto& pair2 : pair1.second)
			{
				auto group2 = group1->AddGroup(pair2.first);
				//group2->SetBackground(b);
				for (auto& info : pair2.second)
				{
					auto item = demoList->AddTextItem(info.Caption);
					//item->SetBackground(b);
					group2->AddItem(item);


					
					item->GetVisualStateGroups()->Add(vgroup1);
				}
			}
		}




		RefPtr<TextBox> textBox1(TextBox::Create(app->GetGUIManager()));
		Grid::SetColumn(textBox1, 0);
		textBox1->Focus();
		textBox1->SetText(_T("lumino text."));
		textBox1->Select(3, 2);
		grid1->GetChildren()->Add(textBox1);
		grid1->GetChildren()->Remove(textBox1);
		grid1->GetChildren()->Add(textBox1);

		//textBox1->SetFontFamily(_T("MS 明朝"));
		app->GetGUIManager()->GetDefaultRootFrame()->SetFontFamily(_T("MS Gpthic"));
		_tprintf(textBox1->GetFontFamily());
		app->GetGUIManager()->GetDefaultRootFrame()->SetFontFamily(_T("MS PGpthic"));
		_tprintf(textBox1->GetFontFamily());	// TODO: "MS 明朝" が表示できない・・・？


		RefPtr<Sprite> sprite(Sprite::Create());

		//GenericCoreList<Sprite*> splist;
		//splist.Add(sprite);

		//sprite->SetSize(SizeF(1, 1));
		//sprite->SetTexture(tex);
		//sprite->SetShader(mmeShader);

		//app->GetSceneGraphManager()->Get3DRootNode()->AddChild(sprite);

		//RefPtr<Audio::Sound> sound1(Audio::Sound::Create(_T("D:/Proj/Lumino.Audio/test/UnitTest/Audio/TestData/ln21.mid")));
		//sound1->SetLoopEnabled(true);
		//sound1->SetPitch(110);
		//sound1->SetVolume(0);
		//sound1->FadeVolume(100, 10, Audio::SoundFadeBehavior::Continue);
		//sound1->Play();

		//RefPtr<StackPanel> panel1(LN_NEW StackPanel(app->GetGUIManager()));
		//workbench1->SetContent(panel1);

		////app->GetGUIManager()->CreateUIElement("Button");
		//RefPtr<Button> button1(LN_NEW Button(app->GetGUIManager()));
		////button1->SetSize(SizeF(200, 300));
		//button1->SetSize(SizeF(64, 20));
		//button1->MouseMove.AddHandler(LN_CreateDelegate< CoreObject*, MouseEventArgs* >(Button1_MouseMove));
		//panel1->GetChildren()->Add(button1);
		////workbench1->SetContent(button1.GetObjectPtr());

		//RefPtr<Button> button2(LN_NEW Button(app->GetGUIManager()));
		//button2->SetSize(SizeF(80, 32));
		//button2->MouseMove.AddHandler(LN_CreateDelegate< CoreObject*, MouseEventArgs* >(Button1_MouseMove));
		//panel1->GetChildren()->Add(button2);

#if 0
		RefPtr<ListBox> listbox1(LN_NEW ListBox(app->GetGUIManager()));
		listbox1->SetSize(SizeF(200, 300));
		panel1->GetChildren()->Add(listbox1);

		RefPtr<Button> button3(LN_NEW Button(app->GetGUIManager()));
		button3->MouseMove.AddHandler(LN_CreateDelegate< CoreObject*, MouseEventArgs* >(Button1_MouseMove));
		listbox1->InsertItem(0, button3);
#endif
#if 0
		RefPtr<Thumb> thumb1(LN_NEW Thumb(app->GetGUIManager()));
		//thumb1->SetSize(SizeF(16, 32));
		workbench1->SetContent(thumb1);
#endif
#if 0
		RefPtr<Track> track1(LN_NEW Track(app->GetGUIManager()));
		track1->SetMaximum(70);
		track1->SetValue(20);
		track1->SetViewportSize(25);
		workbench1->SetContent(track1);
#endif
#if 0	// Grid のテスト
		RefPtr<Grid> grid1(LN_NEW Grid(app->GetGUIManager()));
		RefPtr<ColumnDefinition> col1(LN_NEW ColumnDefinition(app->GetGUIManager()));
		col1->SetWidth(200);
		RefPtr<ColumnDefinition> col2(LN_NEW ColumnDefinition(app->GetGUIManager()));
		RefPtr<RowDefinition> row1(LN_NEW RowDefinition(app->GetGUIManager()));
		RefPtr<RowDefinition> row2(LN_NEW RowDefinition(app->GetGUIManager()));
		row2->SetHeight(100);
		grid1->GetColumnDefinitions()->Add(col1);
		grid1->GetColumnDefinitions()->Add(col2);
		grid1->GetRowDefinitions()->Add(row1);
		grid1->GetRowDefinitions()->Add(row2);
		workbench1->SetContent(grid1);

		RefPtr<Track> track1(LN_NEW Track(app->GetGUIManager()));
		track1->SetOrientation(Orientation::Vertical);
		track1->SetMaximum(70);
		track1->SetValue(20);
		track1->SetViewportSize(25);
		grid1->GetChildren()->Add(track1);

		RefPtr<Track> track2(LN_NEW Track(app->GetGUIManager()));
		track2->SetMaximum(70);
		track2->SetValue(20);
		track2->SetViewportSize(25);
		Grid::SetRow(track2, 1);
		Grid::SetColumnSpan(track2, 2);
		grid1->GetChildren()->Add(track2);
#endif
#if 0	// Image のテスト
		RefPtr<Graphics::Texture> tex1(Graphics::Texture::Create(_T("D:/Proj/Lumino/src/GUI/Resource/DefaultSkin.png")));
		RefPtr<Image> image1(LN_NEW Image(app->GetGUIManager()));
		image1->SetSourceTexture(tex1);
		image1->SetSourceRect(Rect(64, 64, 16, 16));
		workbench1->SetContent(image1);
#endif
#if 0	
		RefPtr<ScrollBar> scrollBar1 = ScrollBar::Create(app->GetGUIManager());//(LN_NEW ScrollBar(app->GetGUIManager()));
		
		workbench1->SetContent(scrollBar1);
		scrollBar1->SetOrientation(Orientation::Vertical);
#endif
#if 0	
		RefPtr<ScrollViewer> scrollViewer = ScrollViewer::Create(app->GetGUIManager());
		workbench1->SetContent(scrollViewer);

		RefPtr<Graphics::Texture> tex1(Graphics::Texture::Create(_T("C:/Users/admin/Pictures/参考/5011525.png")));
		RefPtr<Image> image1(LN_NEW Image(app->GetGUIManager()));
		image1->SetSourceTexture(tex1);
		scrollViewer->SetContent(image1);
#endif
#if 0	
		auto textBlock1 = RefPtr<TextBlock>::Create(app->GetGUIManager());
		textBlock1->SetText(_T("Lumino"));
		workbench1->SetContent(textBlock1);
#endif
#if 0	
		RefPtr<Documents::TextElement> te1 = RefPtr<Documents::TextElement>::Create(app->GetDocumentsManager());
		te1->SetPropertyValue(Documents::TextElement::FontSizeProperty, 100);
		//int a = te1->GetPropertyValue(Documents::TextElement::FontSizeProperty).Cast<int>();

		RefPtr<TextBlock> textBlock1 = TextBlock::Create(app->GetGUIManager());
		textBlock1->SetText(_T("Lumino"));
		RefPtr<TextBlock> textBlock2 = TextBlock::Create(app->GetGUIManager());
		textBlock2->SetText(_T("Test2"));

		RefPtr<ListBox> listBox1 = ListBox::Create(app->GetGUIManager());
		workbench1->SetContent(listBox1);
		workbench1->ApplyTemplate();

		listBox1->InsertListBoxItem(0, textBlock1);
		listBox1->InsertListBoxItem(1, textBlock2);
#endif

		//Graphics::Bitmap bmp(_T());
		//RefPtr<Graphics::Font> font = Imaging::Font::CreateBitmapFont(app->GetGraphicsManager()->GetFontManager());

		Profiler::Instance.SetEnabled(true);

		Graphics::Renderer* r = app->GetGraphicsManager()->GetRenderer();
		Graphics::SwapChain* swap1 = app->GetGraphicsManager()->GetMainSwapChain();
		do
		{
			r->SetRenderTarget(0, swap1->GetBackBuffer());
			r->SetDepthBuffer(swap1->GetBackBufferDepth());
			r->Clear(Graphics::ClearFlags::All, Graphics::ColorF::White);

			Graphics::RenderState state;
			state.Blend = Graphics::BlendMode_Alpha;
			r->SetRenderState(state);

			{

				ScopedProfilerSection prof(Profiler::Group_MainThread, Profiler::Section_MainThread_GUILayput);

				workbench1->ApplyTemplate();
				app->GetGUIManager()->InjectElapsedTime(0.016f);
				workbench1->UpdateLayout();
				workbench1->UpdateTransformHierarchy();
				app->GetGUIManager()->SetViewPixelSize(swap1->GetBackBuffer()->GetSize());
				//app->GetGUIManager()->Render(swap1->GetBackBuffer()->GetSize());
				app->Render();
			}
				swap1->Present();

			Profiler::Instance.Commit();
			//uint64_t time = Profiler::Instance.GetCommitedGroups()[Profiler::Group_MainThread].Sections[Profiler::Section_MainThread_GUILayput].ElapsedTime;
			//
			//uint32_t us = time / 1000;
			//printf("t:%u\n", us);

		} while (app->UpdateFrame());
#endif
	}
	catch (Exception& e)
	{
		printf(e.what());
	}

	Engine::Finalize();
	return 0;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
DemoManager::DemoMainFunc DemoManager::RegisterDemo(const char* name, DemoMainFunc func)
{
	String t = name;
	Array<String> tokens = t.Split(_T("."));
	DemoInfo info;
	info.Group1 = tokens[0];
	info.Group2 = tokens[1];
	info.Caption = tokens[2];
	info.Main = func;
	m_demos.Add(info);

	m_demosTable[info.Group1][info.Group2].Add(info);
	return func;
}

Array<DemoManager::DemoInfo>			DemoManager::m_demos;
std::map<String, std::map<String, Array<DemoManager::DemoInfo> > >	DemoManager::m_demosTable;
