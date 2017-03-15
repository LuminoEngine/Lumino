
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

	auto image1 = UIImage::Create(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image1->SetSize(Size(64, 64));
	mainWindow->AddChild(image1);

	auto thumb1 = UIThumb::Create();
	thumb1->SetSize(Size(80, 32));
	mainWindow->AddChild(thumb1);

	auto scrollBar1 = UIScrollBar::Create();
	//scrollBar1->SetSize(Size(Math::NaN, 32));
	scrollBar1->SetMaximum(10);
	scrollBar1->SetValue(3);
	scrollBar1->SetViewportSize(2);
	mainWindow->AddChild(scrollBar1);

	auto scrollViewer1 = UIScrollViewer::Create();
	scrollViewer1->SetSize(Size(100, 100));
	scrollViewer1->SetBackground(Brush::Red);
	mainWindow->AddChild(scrollViewer1);

	auto image2 = UIImage::Create(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image2->SetSize(Size(200, 200));
	scrollViewer1->AddChild(image2);



	


	while (Engine::Update())
	{
	}

	Engine::Terminate();

}



