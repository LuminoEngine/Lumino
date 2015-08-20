#include "Common.h"

void LN_STDCALL Button1_MouseMove(CoreObject* sender, GUI::MouseEventArgs* e)
{
	printf("Button1_MouseMove\n");
}

template < typename A1, typename A2 >
Delegate02< A1, A2 > LN_CreateDelegate2(void (LN_STDCALL *method)(A1 a, A2 s))
{
	return Delegate02< A1, A2 >(method);
}

int TestA()
{
	return 10;
}

int g_test2 = TestA();

int main()
{

#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	try
	{
		ApplicationConfigData appData;
		//appData.GraphicsAPI = Graphics::GraphicsAPI::OpenGL;
		RefPtr<Application> app(Application::Create(appData));

		app->GetPlatformManager()->GetMainWindow()->SetCursorVisible(false);


		app->GetGraphicsManager()->GetFontManager()->RegisterFontFile(LOCALFILE("../tools/VLGothic/VL-Gothic-Regular.ttf"));
		app->GetGraphicsManager()->GetFontManager()->RegisterFontFile(LOCALFILE("../tools/VLGothic/VL-PGothic-Regular.ttf"));

		// TODO: RefObject のデストラクタで、参照カウント1以上ならエラー。
		RefPtr<Graphics::Font> font1(Graphics::Font::Create(app->GetGraphicsManager()->GetFontManager()));	// TODO: ダサすぎる。デフォルトの設定方法も考える。
		font1->SetName(_T("VL Gothic"));
		font1->SetAntiAlias(true);
		app->GetGraphicsManager()->GetFontManager()->SetDefaultFont(font1);

		//RefPtr<GUI::RootFrame> workbench1(app->GetGUIManager()->CreateRootFrame());
		GUI::RootFrame* workbench1 = app->GetGUIManager()->GetDefaultRootFrame();
		//workbench1->SetSize(SizeF(640, 480));
		workbench1->SetPropertyValue(GUI::RootFrame::SizeProperty , SizeF(640, 480));

		auto grid1 = GUI::Grid::Create();
		grid1->AddColumnDefinition(180);
		grid1->AddColumnDefinition(GUI::ColumnDefinition::Star);
		workbench1->SetContent(grid1);

		auto demoList = GUI::ListBox::Create();
		GUI::Grid::SetColumn(demoList, 0);
		grid1->GetChildren()->Add(demoList);


		//RefPtr<Graphics::Brush> b(Graphics::ColorBrush::Blue, true);

		// Normal
		RefPtr<GUI::FloatEasing> easing1(LN_NEW GUI::FloatEasing());
		easing1->SetTargetName(_T("MouseOnBackground"));
		easing1->SetTargetProperty(GUI::UIElement::OpacityProperty->GetName());
		easing1->SetTargetValue(0.0f);
		easing1->SetEasingMode(Animation::EasingMode::EaseOutExpo);
		easing1->SetDuration(1.0f);

		// MouseOver
		RefPtr<GUI::FloatEasing> easing2(LN_NEW GUI::FloatEasing());
		easing2->SetTargetName(_T("MouseOnBackground"));
		easing2->SetTargetProperty(GUI::UIElement::OpacityProperty->GetName());
		easing2->SetTargetValue(1.0f);
		easing2->SetEasingMode(Animation::EasingMode::EaseOutExpo);
		easing2->SetDuration(1.0f);

		GUI::VisualStateGroupPtr vgroup1(LN_NEW GUI::VisualStateGroup(_T("CommonStates")));
		GUI::VisualStatePtr vstate1(LN_NEW GUI::VisualState(app->GetGUIManager(), _T("Normal")));
		vstate1->GetStoryboard()->AddTimeline(easing1);
		GUI::VisualStatePtr vstate2(LN_NEW GUI::VisualState(app->GetGUIManager(), _T("MouseOver")));
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


		//RefPtr<GUI::StackPanel> panel1(LN_NEW GUI::StackPanel(app->GetGUIManager()));
		//workbench1->SetContent(panel1);

		////app->GetGUIManager()->CreateUIElement("Button");
		//RefPtr<GUI::Button> button1(LN_NEW GUI::Button(app->GetGUIManager()));
		////button1->SetSize(SizeF(200, 300));
		//button1->SetSize(SizeF(64, 20));
		//button1->MouseMove.AddHandler(LN_CreateDelegate< CoreObject*, GUI::MouseEventArgs* >(Button1_MouseMove));
		//panel1->GetChildren()->Add(button1);
		////workbench1->SetContent(button1.GetObjectPtr());

		//RefPtr<GUI::Button> button2(LN_NEW GUI::Button(app->GetGUIManager()));
		//button2->SetSize(SizeF(80, 32));
		//button2->MouseMove.AddHandler(LN_CreateDelegate< CoreObject*, GUI::MouseEventArgs* >(Button1_MouseMove));
		//panel1->GetChildren()->Add(button2);

#if 0
		RefPtr<GUI::ListBox> listbox1(LN_NEW GUI::ListBox(app->GetGUIManager()));
		listbox1->SetSize(SizeF(200, 300));
		panel1->GetChildren()->Add(listbox1);

		RefPtr<GUI::Button> button3(LN_NEW GUI::Button(app->GetGUIManager()));
		button3->MouseMove.AddHandler(LN_CreateDelegate< CoreObject*, GUI::MouseEventArgs* >(Button1_MouseMove));
		listbox1->InsertItem(0, button3);
#endif
#if 0
		RefPtr<GUI::Thumb> thumb1(LN_NEW GUI::Thumb(app->GetGUIManager()));
		//thumb1->SetSize(SizeF(16, 32));
		workbench1->SetContent(thumb1);
#endif
#if 0
		RefPtr<GUI::Track> track1(LN_NEW GUI::Track(app->GetGUIManager()));
		track1->SetMaximum(70);
		track1->SetValue(20);
		track1->SetViewportSize(25);
		workbench1->SetContent(track1);
#endif
#if 0	// Grid のテスト
		RefPtr<GUI::Grid> grid1(LN_NEW GUI::Grid(app->GetGUIManager()));
		RefPtr<GUI::ColumnDefinition> col1(LN_NEW GUI::ColumnDefinition(app->GetGUIManager()));
		col1->SetWidth(200);
		RefPtr<GUI::ColumnDefinition> col2(LN_NEW GUI::ColumnDefinition(app->GetGUIManager()));
		RefPtr<GUI::RowDefinition> row1(LN_NEW GUI::RowDefinition(app->GetGUIManager()));
		RefPtr<GUI::RowDefinition> row2(LN_NEW GUI::RowDefinition(app->GetGUIManager()));
		row2->SetHeight(100);
		grid1->GetColumnDefinitions()->Add(col1);
		grid1->GetColumnDefinitions()->Add(col2);
		grid1->GetRowDefinitions()->Add(row1);
		grid1->GetRowDefinitions()->Add(row2);
		workbench1->SetContent(grid1);

		RefPtr<GUI::Track> track1(LN_NEW GUI::Track(app->GetGUIManager()));
		track1->SetOrientation(GUI::Orientation::Vertical);
		track1->SetMaximum(70);
		track1->SetValue(20);
		track1->SetViewportSize(25);
		grid1->GetChildren()->Add(track1);

		RefPtr<GUI::Track> track2(LN_NEW GUI::Track(app->GetGUIManager()));
		track2->SetMaximum(70);
		track2->SetValue(20);
		track2->SetViewportSize(25);
		GUI::Grid::SetRow(track2, 1);
		GUI::Grid::SetColumnSpan(track2, 2);
		grid1->GetChildren()->Add(track2);
#endif
#if 0	// Image のテスト
		RefPtr<Graphics::Texture> tex1(Graphics::Texture::Create(_T("D:/Proj/Lumino/src/GUI/Resource/DefaultSkin.png")));
		RefPtr<GUI::Image> image1(LN_NEW GUI::Image(app->GetGUIManager()));
		image1->SetSourceTexture(tex1);
		image1->SetSourceRect(Rect(64, 64, 16, 16));
		workbench1->SetContent(image1);
#endif
#if 0	
		RefPtr<GUI::ScrollBar> scrollBar1 = GUI::ScrollBar::Create(app->GetGUIManager());//(LN_NEW GUI::ScrollBar(app->GetGUIManager()));
		
		workbench1->SetContent(scrollBar1);
		scrollBar1->SetOrientation(GUI::Orientation::Vertical);
#endif
#if 0	
		RefPtr<GUI::ScrollViewer> scrollViewer = GUI::ScrollViewer::Create(app->GetGUIManager());
		workbench1->SetContent(scrollViewer);

		RefPtr<Graphics::Texture> tex1(Graphics::Texture::Create(_T("C:/Users/admin/Pictures/参考/5011525.png")));
		RefPtr<GUI::Image> image1(LN_NEW GUI::Image(app->GetGUIManager()));
		image1->SetSourceTexture(tex1);
		scrollViewer->SetContent(image1);
#endif
#if 0	
		auto textBlock1 = RefPtr<GUI::TextBlock>::Create(app->GetGUIManager());
		textBlock1->SetText(_T("Lumino"));
		workbench1->SetContent(textBlock1);
#endif
#if 0	
		RefPtr<Documents::TextElement> te1 = RefPtr<Documents::TextElement>::Create(app->GetDocumentsManager());
		te1->SetPropertyValue(Documents::TextElement::FontSizeProperty, 100);
		//int a = te1->GetPropertyValue(Documents::TextElement::FontSizeProperty).Cast<int>();

		RefPtr<GUI::TextBlock> textBlock1 = GUI::TextBlock::Create(app->GetGUIManager());
		textBlock1->SetText(_T("Lumino"));
		RefPtr<GUI::TextBlock> textBlock2 = GUI::TextBlock::Create(app->GetGUIManager());
		textBlock2->SetText(_T("Test2"));

		RefPtr<GUI::ListBox> listBox1 = GUI::ListBox::Create(app->GetGUIManager());
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
				app->GetGUIManager()->Render(swap1->GetBackBuffer()->GetSize());
				app->Render();
			}
				swap1->Present();

			Profiler::Instance.Commit();
			//uint64_t time = Profiler::Instance.GetCommitedGroups()[Profiler::Group_MainThread].Sections[Profiler::Section_MainThread_GUILayput].ElapsedTime;
			//
			//uint32_t us = time / 1000;
			//printf("t:%u\n", us);

		} while (app->UpdateFrame());

	}
	catch (Exception& e)
	{
		printf(e.what());
	}

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
