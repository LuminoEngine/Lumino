
#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public
#include <LuminoEngine.h>
#include <Lumino/Testing/TestHelper.h>
#include <Lumino/Scene/OffscreenWorldView.h>
#include <Lumino/Rendering/SSAOImageEffect.h>

#include "../Source/Shader/LuminoShader.h"

using namespace ln;




float tangent(Vector3 p, Vector3 s) {
	return (p.z - s.z) / (s.getXY() - p.getXY()).getLength();
}


void UIControlsGallery()
{
	//auto buf = FileSystem::readAllBytes(_T("D:/Proj/LN/HC1/External/Lumino/Source/LuminoEngine/Source/Rendering/Resource/ClusteredShadingDefault.fx"));
	//std::string code((const char*)buf.getConstData(), buf.getSize());
	//LinaShaderIRGenerater lsgen;
	//lsgen.initialize();
	//lsgen.loadRawHLSL(code);
	//lsgen.finalize();


	// へいたん
	float tl1 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 + 0.5, 0.1, 0.5)));
	float tr1 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 - 0.5, 0.1, 0.5)));
	float d1 = ((tl1 + tr1) / Math::PI);	// 0.0

	// おくへすぼむ
	float tl2 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 + 0.5, 0.1, 0.25)));
	float tr2 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 - 0.5, 0.1, 0.25)));
	float d2 = ((tl2 + tr2) / Math::PI);	// 0.2

	// おくへもっとすぼむ
	float tl4 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 + 0.5, 0.1, 0.05)));
	float tr4 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 - 0.5, 0.1, 0.05)));
	float d4 = ((tl4 + tr4) / Math::PI);	// 0.4

	// おくへひらく
	float tl3 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 + 0.5, 0.1, 0.75)));
	float tr3 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 - 0.5, 0.1, 0.75)));
	float d3 = ((tl3 + tr3) / Math::PI);	// -0.2


	float tl5 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 + 0.5, 0.1, 0.8)));
	float tr5 = atan(tangent(Vector3(0.1, 0.1, 0.5), Vector3(0.1 - 0.5, 0.1, 0.3)));
	float d5 = ((tl5 + tr5) / Math::PI);

	EngineSettings::addAssetsDirectory(LN_LOCALFILE("../../../../Samples/Assets"));
	//EngineSettings::setGraphicsAPI(GraphicsAPI::OpenGL);
	Engine::initialize();




	//Engine::getWorld3D()->setVisibleGridPlane(true);

	Engine::getCamera3D()->addComponent(newObject<CameraMouseMoveBehavior>());

	auto* uiRoot = Engine::getDefaultUILayer()->GetLayoutView();

	auto text1 = UITextField::create();
	text1->setPosition(Point(10, 20));
	text1->setSize(Size(200, 32));
	//uiRoot->addChild(text1);

#if 0
	//auto* uiRoot = Engine::getMainWindow();
	auto* uiRoot = Engine::getDefaultUILayer()->GetLayoutView();



	auto tree1 = UITreeView::create();
	tree1->setWidth(130);
	//tree1->setBackground(UIColors::getBrush(UIColorIndex::Blue, 9));
	auto item1 = tree1->addTextItem(_LT("item1"));
	auto item2 = tree1->addTextItem(_LT("item2"));
	auto item2_1 = item2->addTextItem(_LT("item2_1"));
	auto item2_2 = item2->addTextItem(_LT("item2_2"));
	auto item3 = tree1->addTextItem(_LT("item3"));


	auto menu = newObject<UIContextMenu>();
	menu->addMenuItem(_LT("追加"), [&item1]() { printf("item1\n"); item1->addTextItem(_LT("item a1")); });
	menu->addMenuItem(_LT("削除"), [&item1]() { printf("item2\n"); });
	item1->setContextMenu(menu);
#if 0
	uiRoot->addChild(tree1);
#else

	auto grid1 = UIGridLayout::create();
	grid1->addColumnDefinition(GridLengthType::Auto);
	grid1->addColumnDefinition(GridLengthType::Ratio);
	grid1->addColumnDefinition(GridLengthType::Auto);
	//grid1->setBackground(Brush::Blue);
	//grid1->set
	
	Application::getMainWindow()->removeChild(Engine::getMainViewport());
	//Application::getMainWindow()->addChild(grid1);
	Application::getMainWindow()->setLayoutPanel(grid1);
	//Application::getMainWindow()->setBackground(UIColo);
	//Engine::getMainViewport()->setBackground(Brush::Black);

	grid1->addChild(Engine::getMainViewport(), 0, 1);
	grid1->addChild(tree1, 0, 0);

	auto flow1 = UIFlowLayout::create();
	flow1->setWidth(100);
	//flow1->setBackground(Brush::Red);
	grid1->addChild(flow1, 0, 2);

	{
		//auto text1 = UITextField::create();
		////text1->setBackground(Brush::Red);
		//text1->setText(_LT("いんすぺくた"));
		//auto button1 = UIButton::create();
		//flow1->add(text1);
		////flow1->add(button1);

		auto text1 = UITextBlock::create();
		text1->setText(_LT("いんすぺくた"));
		flow1->add(text1);
		flow1->setBackground(Brush::White);
	}

	//auto popup = newObject<UIPopup>();
	//popup->setSize(Size(50, 50));
	//popup->setBackground(Brush::Red);
	//popup->open(tree1);
#endif

#endif

#if 0
	//auto meshModel = ln::Assets::loadMeshModel(_LT("D:/Proj/LN/HC1/Assets/Graphics/TestMap1.mqo"));
	auto meshModel = ln::Assets::loadMeshModel(_T("D:/Proj/LN/HC1/Assets/Graphics/Map1.mqo"));
	auto res = meshModel->getMeshResource(0);
	//int vc = res->getVertexCount();
	//for (int i = 0; i < vc; i++)
	//{
	//	res->getPosition(i).print();
	//}
	//int ic = res->getIndexCount();
	auto mesh = ln::StaticMeshComponent::create(meshModel);
	//auto mesh = StaticMeshComponent::createBox(2, 1, 1);
	auto obj3D = newObject<WorldObject3D>();
	obj3D->addComponent(mesh);
	obj3D->setScale(0.1);
#endif
	
	






	//auto grid1 = UIGridLayout::create();
	//grid1->addColumnDefinition(GridLengthType::Pixel, 400);
	//Engine::getMainWindow()->setLayoutPanel(grid1);

	//auto listBox1 = UIListBox::create();
	//listBox1->setWidth(200);
	//uiRoot->addChild(listBox1);

	//auto item = listBox1->addTextItem(_LT("スプライト"));
	//auto button1 = UIButton::create(_LT(">"), 20, 20);
	////button1->setPosition(Point(2, 0));
	//button1->margin = Thickness(2);
	//button1->setAnchor(AlignmentAnchor::RightOffsets | AlignmentAnchor::VCenter);
	//item->addChild(button1);

	//listBox1->addTextItem(_LT("UI"));


	//auto slider = UISlider::create(0.75, 0.5, 2.0);
	//slider->setPosition(Point(10, 10));
	//slider->setWidth(300);
	//slider->setHeight(16);
	////slider->setOrientation(Orientation::Vertical);
	////uiRoot->addChild(slider);


	//auto window2 = newObject<UIDiagnosticsWindow>();
	//window2->setBackground(SolidColorBrush::White);


	//RawFont::registerFontFile(_LT("C:/LocalProj/tmp/font-awesome-4.7.0/fonts/FontAwesome.otf"));
	//auto font = Font::create(_LT("FontAwesome"), 32);

	//auto text = TextBlock2D::create();
	//text->setFont(font);
	//text->setText(_LT("A"));

	//auto icon1 = GlyphIcon2D::create("fa-globe", 64);
	//auto icon2 = GlyphIcon2D::create("fa-play-circle-o", 64);
	//auto icon3 = GlyphIcon2D::create("fa-android", 64);
	//icon1->setPosition(32, 32);
	//icon2->setPosition(96, 32);
	//icon3->setPosition(160, 32);


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
//	//material->setMaterialTexture(ln::Assets::loadTexture(_LT("D:/GameProjects/Chronicles/sky/incskies_024_png8/incskies_024_8k_.png")));
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




	//auto shader = Shader::create(StringRef(_LT("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/Sandbox/SSBasic2D.fx")), true);
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
	//text1->SetText(_LT("text"));
	//stack1->addChild(text1);

#if 0	// 雨
Engine::getDefault3DLayer()->setBackgroundColor(Color::Gray);

	//Camera::GetMain3DCamera()->SetFarClip(10000);
	auto m1 = SpriteParticleModel::create();
	m1->m_maxParticles = 10000;
	m1->setSpawnRate(1000);
	m1->setLifeTime(1.0);
	m1->m_loop = true;

	m1->setSize(0.05, 0.05);

	m1->m_shapeType = ParticleEmitterShapeType::Box;
	m1->m_shapeParam.set(10, 0, 10);

	m1->m_particleDirection = ParticleDirectionType::MovementDirection;
	m1->m_forwardVelocity.minValue = -12;
	m1->m_forwardVelocity.maxValue = -12;
	m1->m_lengthScale = 10;

	auto material = DiffuseMaterial::create();
	material->setMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->setShader(Shader::getBuiltinShader(BuiltinShader::Sprite));
	m1->setMaterial(material);

	auto particle1 = newObject<ParticleEmitterComponent>(m1);
	particle1->setBlendMode(BlendMode::Add);
	//particle1->SetPosition(0, 12, 0);
	//particle1->setAngles(Math::PI, 0, 0);
	auto particle1obj = newObject<WorldObject3D>();
	particle1obj->addComponent(particle1);
	//particle1obj->setPosition(0, 12, 0);

	//auto m2 = SpriteParticleModel::create();
	//m2->m_maxParticles = 1000;
	//m2->SetSpawnRate(200);
	//m2->SetLifeTime(0.2);
	//m2->m_loop = true;
	//m2->SetSize(0.1, 0.1);
	//m2->m_minSizeVelocity = 3;
	//m2->m_maxSizeVelocity = 3;
	//m2->m_shapeType = ParticleEmitterShapeType::Box;
	//m2->m_shapeParam.Set(10, 0, 10);
	//m2->m_particleDirection = ParticleDirectionType::Horizontal;
	//m2->SetMaterial(material);

	//auto particle2 = ParticleEmitter3D::create(m2);
	//particle2->SetBlendMode(BlendMode::Add);
#endif


	//auto ssao = newObject<SSAOImageEffect>();
	//Engine::getDefault3DLayer()->addPostEffect(ssao);
	//Engine::getCamera3D()->getCameraComponent()->setFarClip(100);


	auto ps1 = TransitionPostEffect::create();
	Engine::getDefault3DLayer()->addPostEffect(ps1);
	ps1->transition(1, nullptr, 0);	// フェードイン


	Bitmap heightmap(LN_LOCALFILE("Assets/heightmap.png"));
	
	//auto box1 = StaticMeshComponent::create(LN_LOCALFILE("Assets/cube.mqo"));
	//auto box1 = StaticMeshComponent::create(LN_LOCALFILE("Assets/cylinder2.mqo"));
	//auto box1 = StaticMeshComponent::create(LN_LOCALFILE("Assets/Plant1.mqo"));
	SizeI size = heightmap.getSize();
	auto meshRes1 = MeshResource::create(size.width * size.height, (size.width - 1) * (size.height - 1) * 6);
	for (int y = 0; y < size.height; y++)
	{
		for (int x = 0; x < size.width; x++)
		{
			int base = x + y * size.width;
			meshRes1->setPosition(base, Vector3(x,  0.1 * heightmap.getPixel(x, y).r, -y));	//おくから
			meshRes1->setNormal(base, Vector3::UnitZ);
			meshRes1->setUV(base, Vector2(static_cast<float>(x) / size.width, static_cast<float>(y) / size.height) * 64.0);
			meshRes1->setColor(base, Color::White);
		}
	}
	for (int y = 0; y < (size.height - 1); y++)
	{
		for (int x = 0; x < (size.width - 1); x++)
		{
			int base = (x + y * (size.width - 1)) * 6;
			meshRes1->setIndex(base + 0, (x + 0) + (y + 0) * size.width);
			meshRes1->setIndex(base + 1, (x + 0) + (y + 1) * size.width);
			meshRes1->setIndex(base + 2, (x + 1) + (y + 0) * size.width);
			meshRes1->setIndex(base + 3, (x + 1) + (y + 0) * size.width);
			meshRes1->setIndex(base + 4, (x + 0) + (y + 1) * size.width);
			meshRes1->setIndex(base + 5, (x + 1) + (y + 1) * size.width);
		}
	}

	auto tex1 = Assets::loadTexture(LN_LOCALFILE("Assets/grid_uv.png"));
	auto material1 = CommonMaterial::create();
	material1->setMaterialTexture(tex1);
	auto meshModel1 = newObject<StaticMeshModel>(meshRes1);
	meshModel1->addMaterial(material1);
	auto mesh1 = StaticMeshComponent::create(meshModel1);
	//auto meshObj1 = newObject<WorldObject3D>();
	//meshObj1->addComponent(mesh1);
	//meshObj1->setPosition(-50, -20, 100);
		





	auto planeMesh = StaticMeshComponent::createPlane(Vector2(20, 20), 1, 1);

	auto cornellBox = CornellBox::create();
	auto cornellBoxObj = newObject<WorldObject3D>();
	cornellBoxObj->addComponent(cornellBox);

	Engine::getCamera3D()->setPosition(0, 10, -30);

	auto ambientLight1 = AmbientLight::create();
	Engine::getWorld3D()->addWorldObject(ambientLight1, true);


	//auto spotLight1 = SpotLight::create(Color::White, 30, Math::PI / 3);
	//spotLight1->transform.lookAt(-Vector3::UnitY);
	//spotLight1->setPosition(0, 20, 0);
	////spotLight1->setIntensity(2.0f);
	//Engine::getWorld3D()->addWorldObject(spotLight1, true);


	auto pointLight1 = PointLight::create(/*Color::White, 2*/);
	pointLight1->setPosition(-9, 10, 0);
	//pointLight1->setIntensity(2.0f);
	Engine::getWorld3D()->addWorldObject(pointLight1, true);


	auto directionalLight1 = DirectionalLight::create(Color::White);
	directionalLight1->getDirectionalLightComponent()->setShadowCast(true);
	//Quaternion rot;
	//rot.rotateX(Math::degreesToRadians(50));
	//rot.rotateY(Math::degreesToRadians(-30));
	//directionalLight1->setRotation(rot);
	directionalLight1->setPosition(30, 30, -30);
	directionalLight1->transform.lookAt(Vector3::Zero);
	Engine::getWorld3D()->addWorldObject(directionalLight1, true);


	Engine::getWorld3D()->setAmbientColor(Color(0.25, 0.25, 0.25, 1.0));
	Engine::getWorld3D()->setAmbientSkyColor(Color::Blue.withAlpha(0.5));
	Engine::getWorld3D()->setAmbientGroundColor(Color::Green.withAlpha(0.25));
	Engine::getWorld3D()->setFogColor(Color(1, 1, 1, 0.5));
	Engine::getWorld3D()->setFogDensity(0.03);
#if 0

	int ClusterDepth = 32;
	int ClusterWidth = 16;
	int ClusterHeight = 16;

	auto clustersTexture = tr::Texture3D::create(ClusterWidth, ClusterHeight, ClusterDepth);

	
	{
		auto camPos = Engine::getCamera3D()->getPosition();

		auto vm = Matrix::makeLookAtLH(camPos, Vector3(0, 0, 0), Vector3(0, 1, 0));
		auto pm = Matrix::makePerspectiveFovLH(Math::PI / 4, 640.f / 480.f, 1.0f, 1000.0f);
		//auto pm = Matrix::makeOrthoLH(640.f, 480.f, 1.0f, 1000.0f);




		Vector3 lightPos(0, 0, -7.5);
		float lightRadius = 1.5;


		Vector3 cp = Vector3::transformCoord(lightPos, vm);	// カメラから見た位置。奥が Z+。一番手前は 0.0
		Vector3 minAABB(cp - lightRadius);
		Vector3 maxAABB(cp + lightRadius);

		Vector3 vp_pos = Vector3::transformCoord(cp, pm);
		Vector3 vp_minAABB = Vector3::transformCoord(minAABB, pm);
		Vector3 vp_maxAABB = Vector3::transformCoord(maxAABB, pm);



		Vector3 test_n = Vector3::transformCoord(Vector3(0, 0, 1.f), pm);
		Vector3 test_f = Vector3::transformCoord(Vector3(0, 0, 1000.f), pm);

		test_n = Vector3::transformCoord(Vector3(0, 0, 1.f), vm);
		test_f = Vector3::transformCoord(Vector3(0, 0, 1000.f), vm);

		float near = 1.0f;
		float far = 100.0f;

		// 深度は 0.0 - 1.0 の線形でほしい
		vp_minAABB.z = ((cp.z - lightRadius) - near) / (far - near);
		vp_maxAABB.z = ((cp.z + lightRadius) - near) / (far - near);


		struct UF
		{
			static float bias(float b, float x)
			{
				return pow(x, log(b) / log(0.5));
			}
		};

		// 先に通常の VFカリングしておこう

		float xs = 2.0f / ClusterWidth;
		float ys = 2.0f / ClusterHeight;
		float zs = 1.0f / ClusterDepth;
		float biasBase = 0.1;

		printf("\n");
		for (int y = 0; y < ClusterHeight; y++)
		{
			float cb = (ys * y) - 1.0f;
			float ct = (ys * (y + 1)) - 1.0f;
			if ((vp_maxAABB.y > cb && vp_minAABB.y < ct))
			{
				for (int x = 0; x < ClusterWidth; x++)
				{
					float cl = (xs * x) - 1.0f;
					float cr = (xs * (x + 1)) - 1.0f;
					if ((vp_maxAABB.x > cl && vp_minAABB.x < cr))
					{
						for (int z = 0; z < ClusterDepth; z++)
						{
							float czn = UF::bias(biasBase, zs * z);
							float czf = UF::bias(biasBase, zs * (z + 1));
							if ((vp_maxAABB.z > czn && vp_minAABB.z < czf))
							{
								printf("%d,%d,%d\n", x, y, z);

								clustersTexture->setPixel32(x, y, z, Color32(255, 255, 255, 0));
							}
						}
					}
				}
			}
		}

		//for (int y = 0; y < ClusterHeight; y++)
		//{
		//	for (int x = 0; x < ClusterWidth; x++)
		//	{
		//		for (int z = 0; z < ClusterDepth; z++)
		//		{
		//			if (y == 2 || x == 8)
		//			clustersTexture->setPixel32(x, y, z, Color32(255, 255, 255, 0));
		//		}
		//	}
		//}
		material1->setTextureParameter(_T("clustersTexture"), clustersTexture);
		material1->setVectorParameter(_T("cam_pos"), Vector4(camPos, 1));
		//clusterdShader->findVariable()


		// printf("%f\n", UF::bias(0.1, 0.1 * i));

		//int line = ((ClusterDepth - 1) - i);	// 奥から
		//Vector3 pos(0, 0, ((far - near) / ClusterDepth) * line  + near);




		// 10分割程度じゃやくにたたない。
		/*
		1	0.99975
		2	0.999572
		3	0.999334
		4	0.999001
		5	0.998502
		6	0.997672
		7	0.996016
		8	0.99108
		9	0
		*/


		//int CLUSTER_Z = 32;
		//int CLUSTER_Y = 8;
		//int CLUSTER_X = 32;

		/*
			Viewport 座標系から直接 tex3D できるようにしたい。ちなみにサンプルはワールド上でやっている。
		*/

		printf("");
	}
#endif






	
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
		text1->SetText(_LT("Button"));
		stack1->AddChild(text1);

		auto button1 = UIButton::create();
		//button1->setWidth(80);
		//button1->setHeight(24);
		button1->SetText(_LT("Button2"));
		stack1->AddChild(button1);

		auto button2 = UIToggleButton::create();
		button2->SetText(_LT("Button2"));
		stack1->AddChild(button2);

		auto slider1 = UISlider::create();
		slider1->SetHeight(32);
		stack1->AddChild(slider1);

		auto field1 = UITextField::create();
		field1->SetWidth(200);
		field1->SetHeight(32);
		field1->SetText(_LT("field"));
		field1->SetBackground(Brush::Gray);
		stack1->AddChild(field1);

		grid1->AddChild(stack1);

		// TODO: テキストサイズに合わせる(サイズ省略)
	}
#endif

	//auto text = UITextBlock::create();//TextBlock2DComponent::create(_LT("Hello, world!"));
	//text->SetText(_LT("Hello"));
	//text->foreground = Brush::Blue;
	//mainWindow->addChild(text);

	//auto textbox1 = UITextBox::create();
	//textbox1->setWidth(300);
	//textbox1->setHeight(32);
	//textbox1->setBackground(Brush::Gray);
	//textbox1->SetText(_LT("Hello, world!"));
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
	//button1->SetText(_LT("Button1"));
	mainWindow->AddChild(button1);

	auto button2 = UIButton::create();
	button2->SetSize(Size(80, 32));
	mainWindow->AddChild(button2);
	button2->Focus();	// TODO: addChild した後でないとフォーカス持てない。これでいいの？

	// TODO: 描画時、Brush が変わってない？
	auto image3 = UIImage::create(_LT("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image3->SetSize(Size(20, 20));
	button2->AddChild(image3);


	auto image1 = UIImage::create(_LT("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
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

	auto image2 = UIImage::create(_LT("C:/Proj/LuminoStudio/external/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/img2.png"));
	image2->SetSize(Size(200, 200));
	scrollViewer1->AddChild(image2);

#endif


	float t = 0;
	while (!Engine::isEndRequested())
	{





		Camera* cam = Engine::getCamera3D();
		CameraComponent* camc = cam->getCameraComponent();
		Vector3 camPos = cam->getPosition();//Vector3(0, 0, -10);
		








		Engine::updateFrame();

		if (Input::isTriggered(InputButtons::Submit))
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
			//shader->findVariable(_LT("time"))->SetFloat(t);

		Engine::presentFrame();

		//printf("%f\n", slider->getValue());

		//printf("%p\n", Input::getAnyActiveTriggered());


		//printf("----------\n");
		//window2->GetRenderDiagnostic()->print();

	}


	//while (Engine::update())
	//{
	//	//printf("----\n");
	//}

	Engine::terminate();

}



