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
		RefPtr<Application> app(Application::Create(appData));

		//RefPtr<GUI::RootPane> workbench1(app->GetGUIManager()->CreateRootPane());
		GUI::RootPane* workbench1 = app->GetGUIManager()->GetDefaultRootPane();
		//workbench1->SetSize(SizeF(640, 480));
		workbench1->SetValue(GUI::RootPane::SizeProperty, SizeF(640, 480));

		RefPtr<GUI::StackPanel> panel1(LN_NEW GUI::StackPanel(app->GetGUIManager()));
		workbench1->SetContent(panel1);

		//app->GetGUIManager()->CreateUIElement("Button");
		RefPtr<GUI::Button> button1(LN_NEW GUI::Button(app->GetGUIManager()));
		//button1->SetSize(SizeF(200, 300));
		button1->SetSize(SizeF(64, 20));
		button1->MouseMove.AddHandler(LN_CreateDelegate< CoreObject*, GUI::MouseEventArgs* >(Button1_MouseMove));
		panel1->GetChildren()->Add(button1);
		//workbench1->SetContent(button1.GetObjectPtr());

		RefPtr<GUI::Button> button2(LN_NEW GUI::Button(app->GetGUIManager()));
		button2->SetSize(SizeF(80, 32));
		button2->MouseMove.AddHandler(LN_CreateDelegate< CoreObject*, GUI::MouseEventArgs* >(Button1_MouseMove));
		panel1->GetChildren()->Add(button2);

		RefPtr<GUI::ListBox> listbox1(LN_NEW GUI::ListBox(app->GetGUIManager()));
		listbox1->SetSize(SizeF(200, 300));
		panel1->GetChildren()->Add(listbox1);

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
