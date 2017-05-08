
#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public
#include <LuminoEngine.h>

using namespace ln;

void UIControlsGallery()
{
	Engine::Initialize();


	//Engine::GetDefaultSceneGraph3D()->visibleGridPlane = true;

	Engine::GetWorld3D()->SetVisibleGridPlane(true);

	auto cb = RefPtr<CylinderMouseMoveCameraBehavior>::MakeRef();
	CameraComponent::GetMain3DCamera()->SetCameraBehavior(cb);


	auto* mainWindow = Engine::GetMainWindow();


	auto listBox1 = UIListBox::Create();
	listBox1->SetWidth(200);
	mainWindow->AddChild(listBox1);

	auto item = listBox1->AddTextItem(_T("スプライト"));
	auto button1 = UIButton::Create(_T(">"), 20, 20);
	//button1->SetPosition(PointF(2, 0));
	button1->margin = ThicknessF(2);
	button1->SetAnchor(AlignmentAnchor::RightOffsets | AlignmentAnchor::VCenter);
	item->AddChild(button1);

	listBox1->AddTextItem(_T("UI"));

	
	//auto shader = Shader::Create(StringRef(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/Sandbox/SSBasic2D.fx")), true);
	//auto rect = Rectangle::Create(RectF(0, 0, 3, 1));
	//rect->SetShader(shader);
	//rect->SetAngles(Math::PI/2, 0, 0);

	//auto grid1 = UIGridLayout::Create(1, 1);
	//win1->AddChild(grid1);


	//auto win1 = UIWindow::Create();
	//win1->SetWidth(200);
	//win1->SetHeight(100);
	////win1->SetBackground(Brush::DimGray);
	//mainWindow->AddChild(win1);


	//auto grid1 = UIGridLayout::Create(1, 1);
	//win1->AddChild(grid1);
	//auto stack1 = UIStackPanel::Create();
	//grid1->AddChild(stack1);

	//auto text1 = UITextBlock::Create();
	//text1->SetText(_T("text"));
	//stack1->AddChild(text1);

	auto ps1 = TransitionPostEffect::Create();
	Engine::GetDefault3DLayer()->AddPostEffect(ps1);
	

#if 0
	auto font = Font::GetDefault();
	font->SetSize(50);

	detail::Filled filled;
	filled.Initialize();
	filled.setTessellationSteps(3);
	filled.DecomposeOutlineVertices(static_cast<detail::FreeTypeFont*>(font->ResolveRawFont()), 'A');
	filled.Tessellate();
	filled.MakeEdgeStroke();

	auto meshRes = MeshResource::Create();
	meshRes->ResizeVertexBuffer(filled.m_vertexList.GetCount());
	meshRes->AddSections(1);
	meshRes->GetSection(0)->MaterialIndex = 0;
	meshRes->GetSection(0)->StartIndex = 0;
	meshRes->GetSection(0)->PrimitiveNum = filled.m_triangleIndexList.GetCount() / 3;
	meshRes->GetSection(0)->primitiveType = PrimitiveType_TriangleList;
	for (int i = 0; i < filled.m_vertexList.GetCount(); i++)
	{
		meshRes->SetPosition(i, Vector3(filled.m_vertexList[i].pos, 0.0f));
		meshRes->SetColor(i, Color(0, 0, 0, filled.m_vertexList[i].alpha));
		//filled.m_vertexList[i].Print();
	}
	meshRes->ResizeIndexBuffer(filled.m_triangleIndexList.GetCount(), IndexBufferFormat_UInt16);
	for (int i = 0; i < filled.m_triangleIndexList.GetCount(); i++)
	{
		meshRes->SetIndex(i, filled.m_triangleIndexList[i]);
	}

	auto mesh1 = NewObject<StaticMeshModel>(meshRes->m_manager, meshRes);
	auto mat1 = DiffuseMaterial::Create();
	mat1->SetBlendMode(BlendMode::Alpha);
	mesh1->AddMaterial(mat1);

	auto mesh2 = StaticMeshComponent::Create(mesh1);
#endif



#if 0
	auto* mainWindow = Engine::GetMainWindow();
	mainWindow->SetLayoutPanel(UIStackPanel::Create());

	auto grid1 = UIGridLayout::Create(1, 1);
	mainWindow->AddChild(grid1);

	// Button
	{
		auto stack1 = UIStackPanel::Create();

		auto text1 = UITextBlock::Create();
		text1->SetText(_T("Button"));
		stack1->AddChild(text1);

		auto button1 = UIButton::Create();
		//button1->SetWidth(80);
		//button1->SetHeight(24);
		button1->SetText(_T("Button2"));
		stack1->AddChild(button1);

		auto button2 = UIToggleButton::Create();
		button2->SetText(_T("Button2"));
		stack1->AddChild(button2);

		auto slider1 = UISlider::Create();
		slider1->SetHeight(32);
		stack1->AddChild(slider1);

		auto field1 = UITextField::Create();
		field1->SetWidth(200);
		field1->SetHeight(32);
		field1->SetText(_T("field"));
		field1->SetBackground(Brush::Gray);
		stack1->AddChild(field1);

		grid1->AddChild(stack1);

		// TODO: テキストサイズに合わせる(サイズ省略)
	}
#endif

	//auto text = UITextBlock::Create();//TextBlock2DComponent::Create(_T("Hello, world!"));
	//text->SetText(_T("Hello"));
	//text->foreground = Brush::Blue;
	//mainWindow->AddChild(text);

	//auto textbox1 = UITextBox::Create();
	//textbox1->SetWidth(300);
	//textbox1->SetHeight(32);
	//textbox1->SetBackground(Brush::Gray);
	//textbox1->SetText(_T("Hello, world!"));
	//textbox1->foreground = Brush::Blue;
	//mainWindow->AddChild(textbox1);


	//auto comboBox = tr::UIComboBox::Create();
	//comboBox->SetWidth(300);
	//comboBox->SetHeight(32);
	//comboBox->SetBackground(Brush::Gray);
	//mainWindow->AddChild(comboBox);


	

#if 0
	auto* mainWindow = Engine::GetMainWindow();
	mainWindow->SetLayoutPanel(UIStackPanel::Create());

	auto button1 = UIButton::Create();
	button1->SetSize(Size(80, 32));
	//button1->SetText(_T("Button1"));
	mainWindow->AddChild(button1);

	auto button2 = UIButton::Create();
	button2->SetSize(Size(80, 32));
	mainWindow->AddChild(button2);
	button2->Focus();	// TODO: AddChild した後でないとフォーカス持てない。これでいいの？

	// TODO: 描画時、Brush が変わってない？
	auto image3 = UIImage::Create(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image3->SetSize(Size(20, 20));
	button2->AddChild(image3);


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

#endif

	float t = 0;
	while (!Engine::IsEndRequested())
	{
		Engine::UpdateFrame();

		if (Input::IsTriggered(InputButtons::OK))
		{
			ps1->Transition(1, nullptr, 0);
		}


			Engine::RenderFrame();

			//Engine::GetDefaultSceneGraph3D()->GetRenderer()->SetShader(shader);
			//Engine::GetDefaultSceneGraph3D()->GetRenderer()->DrawScreenRectangle();
			
		//Engine::GetDefaultSceneGraph3D()->GetDebugRenderer()->DrawLinePrimitive(
		//	Vector3(0, 0, 0), Color::Red,
		//	Vector3(5, 5, 5), Color::White);

			t += 0.016;
			//shader->FindVariable(_T("time"))->SetFloat(t);

		Engine::PresentFrame();
	}

	//while (Engine::Update())
	//{
	//	//printf("----\n");
	//}

	Engine::Terminate();

}



