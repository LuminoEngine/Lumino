#include "Common.h"

void LN_STDCALL Button1_MouseMove(CoreObject* sender, GUI::MouseEventArgs* e)
{
	//printf("Button1_MouseMove\n");
}

template < typename A1, typename A2 >
Delegate02< A1, A2 > LN_CreateDelegate2(void (LN_STDCALL *method)(A1 a, A2 s))
{
	return Delegate02< A1, A2 >(method);
}

int main()
{
	try
	{

		ApplicationConfigData appData;
		RefPtr<Application> app(Application::Create(appData));

		//RefPtr<GUI::RootPane> workbench1(app->GetGUIManager()->CreateRootPane());
		GUI::RootPane* workbench1 = app->GetGUIManager()->GetDefaultRootPane();
		//workbench1->SetSize(SizeF(640, 480));
		workbench1->SetValue(GUI::RootPane::SizeProperty, SizeF(640, 480));

		//app->GetGUIManager()->CreateUIElement("Button");
		RefPtr<GUI::Button> button1(LN_NEW GUI::Button(app->GetGUIManager()));
		//button1->SetSize(SizeF(200, 300));
		button1->AddMouseMoveHandler(LN_CreateDelegate< CoreObject*, GUI::MouseEventArgs* >(Button1_MouseMove));
		workbench1->SetContent(button1.GetObjectPtr());

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
