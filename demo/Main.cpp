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
		
		//RefPtr<GUI::RootPane> workbench1(app->GetGUIManager()->CreateRootPane());
		GUI::RootPane* workbench1 = app->GetGUIManager()->GetDefaultRootPane();
		//workbench1->SetSize(SizeF(640, 480));
		workbench1->SetPropertyValue(GUI::RootPane::SizeProperty, SizeF(640, 480));

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





		Graphics::Renderer* r = app->GetGraphicsManager()->GetRenderer();
		Graphics::SwapChain* swap1 = app->GetGraphicsManager()->GetMainSwapChain();
		do
		{
			r->SetRenderTarget(0, swap1->GetBackBuffer());
			r->SetDepthBuffer(swap1->GetBackBufferDepth());
			r->Clear(true, true, Graphics::ColorF::Gray, 1.0f);

			Graphics::RenderState state;
			state.Blend = Graphics::BlendMode_Alpha;
			r->SetRenderState(state);

			workbench1->UpdateLayout();
			workbench1->Render();
			swap1->Present();

		} while (app->UpdateFrame());

	}
	catch (Exception& e)
	{
		printf(e.what());
	}

	return 0;
}
