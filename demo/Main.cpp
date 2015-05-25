#include "Common.h"

int main()
{
	RefPtr<Application> app(Application::Create());

	RefPtr<GUI::Workbench> workbench1(app->GetGUIManager()->CreateWorkbench());

	//app->GetGUIManager()->CreateUIElement("Button");
	RefPtr<GUI::Button> button1(LN_NEW GUI::Button(app->GetGUIManager()));
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

		workbench1->Render();
		swap1->Present();

	} while (app->UpdateFrame());

	return 0;
}
