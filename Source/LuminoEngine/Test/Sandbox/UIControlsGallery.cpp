
#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public
#include <LuminoEngine.h>
#include <Lumino/Testing/TestHelper.h>
#include <Lumino/Scene/OffscreenWorldView.h>

using namespace ln;


class UIDiagnosticsWindow
	: public UIFrameWindow
{
public:

protected:
	virtual void onRender(DrawingContext* context) override;

LN_CONSTRUCT_ACCESS:
	UIDiagnosticsWindow();
	virtual ~UIDiagnosticsWindow() = default;
	void initialize();

private:
	static const int ThreadNameColumnWidth = 80;
	static const int SafeAreaWidth = 160;
	static const int rowHeight = 32;

	void drawStatistics(DrawingContext* context,const Rect& windowRect);
	void drawGroupList(DrawingContext* context, const Rect& listRect);
	void drawSectionGraphBar(DrawingContext* context, ProfilingKey* key, float x, float y, const Rect& listRect, float* totalElapsed);

	Ref<Font>	m_font;
	float			m_limitElapsedTime = 1.0f / 60.0f;	// TODO: FPS

	Ref<UIButton>	m_recordButton;
};


//------------------------------------------------------------------------------
UIDiagnosticsWindow::UIDiagnosticsWindow()
	: UIFrameWindow()
	, m_font(nullptr)
{
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::initialize()
{
	UIFrameWindow::initialize();
	m_font = Font::getBuiltin(BuiltinFontSize::XXSmall);

	m_recordButton = UIButton::create(_T("Record"), 80, 24);
	addChild(m_recordButton);
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::onRender(DrawingContext* context)
{
	UIFrameWindow::onRender(context);
	//context->clear(ClearFlags::All, Color::AliceBlue);
	drawStatistics(context, Rect(640 - 8 - 300, 8, 300, 256));
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::drawStatistics(DrawingContext* context, const Rect& windowRect)
{
	Point loc(windowRect.x, windowRect.y);


	// キャプションバー
	//context->setOpacity(0.5f);
	//context->setBrush(SolidColorBrush::DimGray);
	//context->drawRectangle(Rect(windowRect.getTopLeft(), windowRect.width, 20));

	//context->setOpacity(1.0f);
	context->setFont(m_font);
	context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 8));
	context->drawText_(_T("Statistics"), Point(10, 20));

	//return;
	loc.y += 24;

	//auto time = Profiler2::getSection(ProfilingKeys::Engine_UpdateFrame)->getElapsedSeconds();
	//printf("%f\n", time);

	//-----------------------------------------------------
	// Main info
	context->drawText_(_T("Main information:"), Point(loc.x + 8, loc.y));
	loc.y += 16;
	loc.x += 16;

	TCHAR text[256] = { 0 };

	//StringTraits::sprintf(text, 256, _T("Graphics API    : %s"), m_manager->GetGraphicsAPI().ToString().c_str());
	//context->drawText_(text, loc);
	//loc.y += 16;

	//StringTraits::sprintf(text, 256, _T("Rendering type  : %s"), m_manager->GetRenderingType().ToString().c_str());
	//context->drawText_(text, loc);
	//loc.y += 16;

	StringTraits::sprintf(text, 256, _T("Average FPS     : %.1f"), Profiler2::GetMainFps());
	context->drawText_(text, loc);

	StringTraits::sprintf(text, 256, _T(" / Capacity : %.1f"), Profiler2::GetMainFpsCapacity());
	context->drawText_(text, Point(loc.x + 150, loc.y));
	loc.y += 16;

	//StringTraits::sprintf(text, 256, _T("Window Size     : %d x %d"), m_profiler->GetCommitedMainWindowSize().Width, m_profiler->GetCommitedMainWindowSize().Height);
	//context->drawText_(text, loc);
	//loc.y += 16;

	//StringTraits::sprintf(text, 256, _T("Backbuffer Size : %d x %d"), m_profiler->GetCommitedMainBackbufferSize().Width, m_profiler->GetCommitedMainBackbufferSize().Height);
	//context->drawText_(text, loc);

	loc.y += 16;
	loc.x -= 16;

	//-----------------------------------------------------
	// Threads performance

	context->drawText_(_T("Threads performance:"), Point(loc.x + 8, loc.y));
	loc.y += 16;

	// グループリストの描画
	Rect listRect(loc.x + 16, loc.y, windowRect.width - 32, windowRect.height - loc.y);
	drawGroupList(context, listRect);
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::drawGroupList(DrawingContext* context, const Rect& listRect)
{

	ProfilingKey* groups[] =
	{
		ProfilingKeys::Engine,
		ProfilingKeys::Rendering,
	};
	int groupCount = 2;

	// 名前カラム背景色
	context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 1));
	//context->setOpacity(0.2f);
	context->drawRectangle(Rect(listRect.getTopLeft(), ThreadNameColumnWidth, (float)(groupCount * rowHeight)));

	// バーのNG領域背景色
	Rect ngArea(
		listRect.x + (ThreadNameColumnWidth + SafeAreaWidth),
		listRect.y,
		listRect.width - (ThreadNameColumnWidth + SafeAreaWidth),
		groupCount * rowHeight);
	context->setBrush(UIColors::getBrush(UIColorIndex::Red, 2));
	context->drawRectangle(ngArea);

	// リスト縦罫線
	context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 5));
	//context->SetOpacity(0.7f);
	context->drawRectangle(Rect(listRect.x + ThreadNameColumnWidth - 1, listRect.y, 1, groupCount * rowHeight));

	int iGrout;
	for (iGrout = 0; iGrout < groupCount; iGrout++)
	{
		// row 上部罫線
		context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 5));
		//context->SetOpacity(0.7f);
		context->drawRectangle(Rect(listRect.x, listRect.y + (iGrout * rowHeight), listRect.width, 1.0f));

		// グループ名
		context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 8));
		//context->SetOpacity(1.0f);
		Point pt(listRect.x + 4, listRect.y + (iGrout * rowHeight) + 1);
		context->drawText_(groups[iGrout]->getName(), pt);

		// セクションを積み上げ棒グラフで表示
		float totalElapsed = 0;
		drawSectionGraphBar(context, groups[iGrout], listRect.x + ThreadNameColumnWidth, listRect.y + (iGrout * rowHeight), listRect, &totalElapsed);

		// ms
		TCHAR fps[256] = { 0 };
		StringTraits::sprintf(fps, 256, _T("%.1f ms"), totalElapsed * 1000.0f);
		pt.y += 16;
		context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 5));
		context->drawText_(fps, pt);
	}

	// row 下部罫線
	context->setBrush(UIColors::getBrush(UIColorIndex::Grey, 5));
	//context->SetOpacity(0.7f);
	context->drawRectangle(Rect(listRect.x, listRect.y + (groupCount * rowHeight), listRect.width, 1.0f));
}

//------------------------------------------------------------------------------
void UIDiagnosticsWindow::drawSectionGraphBar(DrawingContext* context, ProfilingKey* key, float x, float y, const Rect& listRect, float* totalElapsed)
{
	float time = 0;
	float nextX = 0;
	int colorIndex = (int)UIColorIndex::LightBlue;
	for (const auto childKey : key->getChildren())
	{
		auto* section = Profiler2::getSection(childKey);

		//if (section->getElapsedNanoseconds() > 0.0)
		{
			float ratio = section->getElapsedSeconds() / m_limitElapsedTime;
			time += section->getElapsedSeconds();

			Rect srcRect(x + nextX, y + 5, ratio * SafeAreaWidth, rowHeight - 10);
			if (listRect.x + listRect.width < srcRect.x + srcRect.width)
			{
				srcRect.width = listRect.x + listRect.width - srcRect.x;
			}

			context->setBrush(UIColors::getBrush((UIColorIndex)colorIndex, 2));
			context->drawRectangle(srcRect);

			nextX += ratio * SafeAreaWidth;
			if (listRect.x + listRect.width <= nextX) { break; }
		}
		
		colorIndex += 4;
		if (colorIndex > (UIColors::MaxIndex - 2))
		{
			colorIndex = 0;
		}
	}

	*totalElapsed = time;
}



void UIControlsGallery()
{
	Engine::initialize();


	Engine::getWorld3D()->setVisibleGridPlane(true);

	Engine::getCamera3D()->addComponent(newObject<CameraMouseMoveBehavior>());


	//auto* uiRoot = Engine::getMainWindow();
	auto* uiRoot = Engine::getDefaultUILayer()->GetLayoutView();


	//auto grid1 = UIGridLayout::create();
	//grid1->addColumnDefinition(GridLengthType::Pixel, 400);
	//Engine::getMainWindow()->setLayoutPanel(grid1);

	//auto listBox1 = UIListBox::create();
	//listBox1->setWidth(200);
	//uiRoot->addChild(listBox1);

	//auto item = listBox1->addTextItem(_T("スプライト"));
	//auto button1 = UIButton::create(_T(">"), 20, 20);
	////button1->setPosition(Point(2, 0));
	//button1->margin = Thickness(2);
	//button1->setAnchor(AlignmentAnchor::RightOffsets | AlignmentAnchor::VCenter);
	//item->addChild(button1);

	//listBox1->addTextItem(_T("UI"));


	auto slider = UISlider::create(0.75, 0.5, 2.0);
	slider->setPosition(Point(10, 10));
	slider->setWidth(300);
	slider->setHeight(16);
	//slider->setOrientation(Orientation::Vertical);
	//uiRoot->addChild(slider);


	auto window2 = newObject<UIDiagnosticsWindow>();
	window2->setBackground(SolidColorBrush::White);





//	auto sky = newObject<SkyComponent>();
//	Engine::getCamera3D()->addComponent(sky);
//	//sky->setDepthTestEnabled(false);
//	//sky->setDepthWriteEnabled(false);
//	//auto skyObj = ln::newObject<ln::WorldObject3D>();
//	//skyObj->addComponent(sky);
//	//skyObj->setPosition(10, 0, 0);
//
//	auto mirror = newObject<MirrorComponent>();
//	mirror->setDepthTestEnabled(false);
//	mirror->setDepthWriteEnabled(false);
//	auto mirrorObj = ln::newObject<ln::WorldObject3D>();
//	mirrorObj->addComponent(mirror);
//	mirrorObj->setPosition(10, 0, 0);
//
//
//
//	//auto meshModel = ln::StaticMeshModel::
//	auto mesh = ln::StaticMeshComponent::createSphere(1);
//	//mesh->getStaticMeshModel()->
//	auto material = mesh->getStaticMeshModel()->getMaterial(0);
//	//material->setMaterialTexture(ln::Assets::loadTexture(_T("D:/GameProjects/Chronicles/sky/incskies_024_png8/incskies_024_8k_.png")));
//	material->setMaterialTexture(Texture2D::getBlackTexture());
//	auto m_map = ln::newObject<ln::WorldObject3D>();
//	m_map->addComponent(mesh);
//#if 0
//	mesh->setBlendMode(BlendMode::Add);
//	mesh->setOpacity(0.75);
//	mesh->setShader(Shader::getBuiltinShader(BuiltinShader::Sprite));
//	m_map->setScale(-10.5, 10.5, 10.5);
//	m_map->setPosition(0, -3, 0);
//#else
//	mesh->setBlendMode(BlendMode::Normal);
//	mesh->setOpacity(0.75);
//	mesh->setShader(Shader::getBuiltinShader(BuiltinShader::Sprite));
//	m_map->setScale(-1.5, 1.5, 1.5);
//	m_map->setPosition(0, 2, 0);
//#endif


	//CameraComponent::getMain3DCamera()->setReflectionPlane(Plane(Vector3::UnitY));




	//auto shader = Shader::create(StringRef(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/Sandbox/SSBasic2D.fx")), true);
	//auto rect = Rectangle::create(RectF(0, 0, 3, 1));
	//rect->setShader(shader);
	//rect->setAngles(Math::PI/2, 0, 0);

	//auto grid1 = UIGridLayout::create(1, 1);
	//win1->addChild(grid1);


	//auto win1 = UIWindow::create();
	//win1->setWidth(200);
	//win1->setHeight(100);
	////win1->setBackground(Brush::DimGray);
	//mainWindow->addChild(win1);


	//auto grid1 = UIGridLayout::create(1, 1);
	//win1->addChild(grid1);
	//auto stack1 = UIStackPanel::create();
	//grid1->addChild(stack1);

	//auto text1 = UITextBlock::create();
	//text1->SetText(_T("text"));
	//stack1->addChild(text1);

	auto ps1 = TransitionPostEffect::create();
	Engine::getDefault3DLayer()->addPostEffect(ps1);
	
	//auto box1 = StaticMeshComponent::create(LN_LOCALFILE("Assets/cube.mqo"));
	//auto box1 = StaticMeshComponent::create(LN_LOCALFILE("Assets/cylinder2.mqo"));
	//auto box1 = StaticMeshComponent::create(LN_LOCALFILE("Assets/Plant1.mqo"));
	//auto box1 = StaticMeshComponent::create(_T("D:/Documents/Modeling/test4.mqo"));
	//auto mesh1 = newObject<WorldObject3D>();
	//mesh1->addComponent(box1);
	
#if 0
	auto font = Font::getDefault();
	font->SetSize(50);

	detail::Filled filled;
	filled.initialize();
	filled.setTessellationSteps(3);
	filled.DecomposeOutlineVertices(static_cast<detail::FreeTypeFont*>(font->ResolveRawFont()), 'A');
	filled.Tessellate();
	filled.MakeEdgeStroke();

	auto meshRes = MeshResource::create();
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
		//filled.m_vertexList[i].print();
	}
	meshRes->ResizeIndexBuffer(filled.m_triangleIndexList.GetCount(), IndexBufferFormat_UInt16);
	for (int i = 0; i < filled.m_triangleIndexList.GetCount(); i++)
	{
		meshRes->SetIndex(i, filled.m_triangleIndexList[i]);
	}

	auto mesh1 = NewObject<StaticMeshModel>(meshRes->m_manager, meshRes);
	auto mat1 = DiffuseMaterial::create();
	mat1->SetBlendMode(BlendMode::Alpha);
	mesh1->AddMaterial(mat1);

	auto mesh2 = StaticMeshComponent::create(mesh1);
#endif



#if 0
	auto* mainWindow = Engine::GetMainWindow();
	mainWindow->SetLayoutPanel(UIStackPanel::create());

	auto grid1 = UIGridLayout::create(1, 1);
	mainWindow->AddChild(grid1);

	// Button
	{
		auto stack1 = UIStackPanel::create();

		auto text1 = UITextBlock::create();
		text1->SetText(_T("Button"));
		stack1->AddChild(text1);

		auto button1 = UIButton::create();
		//button1->setWidth(80);
		//button1->setHeight(24);
		button1->SetText(_T("Button2"));
		stack1->AddChild(button1);

		auto button2 = UIToggleButton::create();
		button2->SetText(_T("Button2"));
		stack1->AddChild(button2);

		auto slider1 = UISlider::create();
		slider1->SetHeight(32);
		stack1->AddChild(slider1);

		auto field1 = UITextField::create();
		field1->SetWidth(200);
		field1->SetHeight(32);
		field1->SetText(_T("field"));
		field1->SetBackground(Brush::Gray);
		stack1->AddChild(field1);

		grid1->AddChild(stack1);

		// TODO: テキストサイズに合わせる(サイズ省略)
	}
#endif

	//auto text = UITextBlock::create();//TextBlock2DComponent::create(_T("Hello, world!"));
	//text->SetText(_T("Hello"));
	//text->foreground = Brush::Blue;
	//mainWindow->addChild(text);

	//auto textbox1 = UITextBox::create();
	//textbox1->setWidth(300);
	//textbox1->setHeight(32);
	//textbox1->setBackground(Brush::Gray);
	//textbox1->SetText(_T("Hello, world!"));
	//textbox1->foreground = Brush::Blue;
	//mainWindow->addChild(textbox1);


	//auto comboBox = tr::UIComboBox::create();
	//comboBox->setWidth(300);
	//comboBox->setHeight(32);
	//comboBox->setBackground(Brush::Gray);
	//mainWindow->addChild(comboBox);


	

#if 0
	auto* mainWindow = Engine::GetMainWindow();
	mainWindow->SetLayoutPanel(UIStackPanel::create());

	auto button1 = UIButton::create();
	button1->SetSize(Size(80, 32));
	//button1->SetText(_T("Button1"));
	mainWindow->AddChild(button1);

	auto button2 = UIButton::create();
	button2->SetSize(Size(80, 32));
	mainWindow->AddChild(button2);
	button2->Focus();	// TODO: addChild した後でないとフォーカス持てない。これでいいの？

	// TODO: 描画時、Brush が変わってない？
	auto image3 = UIImage::create(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image3->SetSize(Size(20, 20));
	button2->AddChild(image3);


	auto image1 = UIImage::create(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image1->SetSize(Size(64, 64));
	mainWindow->AddChild(image1);

	auto thumb1 = UIThumb::create();
	thumb1->SetSize(Size(80, 32));
	mainWindow->AddChild(thumb1);

	auto scrollBar1 = UIScrollBar::create();
	//scrollBar1->setSize(Size(Math::NaN, 32));
	scrollBar1->SetMaximum(10);
	scrollBar1->SetValue(3);
	scrollBar1->SetViewportSize(2);
	mainWindow->AddChild(scrollBar1);

	auto scrollViewer1 = UIScrollViewer::create();
	scrollViewer1->SetSize(Size(100, 100));
	scrollViewer1->SetBackground(Brush::Red);
	mainWindow->AddChild(scrollViewer1);

	auto image2 = UIImage::create(_T("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image2->SetSize(Size(200, 200));
	scrollViewer1->AddChild(image2);

#endif

	float t = 0;
	while (!Engine::isEndRequested())
	{
		Engine::updateFrame();

		if (Input::isTriggered(InputButtons::OK))
		{
			ps1->transition(1, nullptr, 0);
		}


			Engine::renderFrame();

			//Engine::getDefaultSceneGraph3D()->getRenderer()->setShader(shader);
			//Engine::getDefaultSceneGraph3D()->getRenderer()->drawScreenRectangle();
			
		//Engine::getDefaultSceneGraph3D()->GetDebugRenderer()->drawLinePrimitive(
		//	Vector3(0, 0, 0), Color::Red,
		//	Vector3(5, 5, 5), Color::White);

			t += 0.016;
			//shader->findVariable(_T("time"))->SetFloat(t);

		Engine::presentFrame();

		//printf("%f\n", slider->getValue());


		//printf("----------\n");
		//window2->GetRenderDiagnostic()->print();
	}

	//while (Engine::update())
	//{
	//	//printf("----\n");
	//}

	Engine::terminate();

}



