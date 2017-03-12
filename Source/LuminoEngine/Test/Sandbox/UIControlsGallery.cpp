
#include <LuminoEngine.h>
using namespace ln;


void UIControlsGallery()
{
	Engine::Initialize();

	auto* mainWindow = Engine::GetMainWindow();
	mainWindow->SetLayoutPanel(UIStackPanel::Create());

	auto button1 = UIButton::Create();
	button1->SetSize(Size(80, 32));
	mainWindow->AddChild(button1);

	auto button2 = UIButton::Create();
	button2->SetSize(Size(80, 32));
	mainWindow->AddChild(button2);
	button2->Focus();	// TODO: AddChild した後でないとフォーカス持てない。これでいいの？


	while (Engine::Update())
	{
	}

	Engine::Terminate();

}



