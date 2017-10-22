
#define LN_INTERNAL_ACCESS				public
#define LN_PROTECTED_INTERNAL_ACCESS	public
#include <LuminoEngine.h>
#include <Lumino/Testing/TestHelper.h>
#include <Lumino/Scene/OffscreenWorldView.h>

using namespace ln;






void UIControlsGallery()
{
	EngineSettings::addAssetsDirectory(LN_LOCALFILE("../../../../Samples/Assets"));

	Engine::initialize();




	Engine::getWorld3D()->setVisibleGridPlane(true);

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
	auto clusterdShader = Shader::create(LN_LOCALFILE("Assets/NoLightingRendering.fx"));
	auto tex1 = Assets::loadTexture(LN_LOCALFILE("Assets/grid_uv.png"));
	auto material1 = Material::create();
	material1->setMaterialTexture(tex1);
	material1->setShader(clusterdShader);
	auto meshModel1 = newObject<StaticMeshModel>(meshRes1);
	meshModel1->addMaterial(material1);
	auto mesh1 = StaticMeshComponent::create(meshModel1);
	mesh1->setShader(clusterdShader);
	//auto meshObj1 = newObject<WorldObject3D>();
	//meshObj1->addComponent(mesh1);
	//meshObj1->setPosition(-50, -20, 100);
		








	class LightClusters
	{
	public:
		struct AABB
		{
			Vector3 min;
			Vector3 max;
		};

		struct PointLightInfo
		{
			Vector4 pos;
			//float	range;
			Color	color;
		};

		int m_clusterWidth = 16;
		int m_clusterHeight = 16;
		int m_clusterDepth = 32;

		Ref<tr::Texture3D>		m_clustersTexture;
		std::vector<int>		m_clustersAddCount;
		std::vector<Color32>	m_clustersData;		// TODO: Texture3D がまだ setData も getMappedData もサポートしていないので。できれば getMappedData にそのまま書き込みたい

		Matrix	m_view;
		Matrix	m_proj;
		float	m_clipRange;

		List<PointLightInfo>	m_pointLights;

		Ref<Texture2D>	m_pointLightInfoTexture;
		const int MaxLights = 64;

		void init()
		{
			m_clustersTexture = tr::Texture3D::create(m_clusterWidth, m_clusterHeight, m_clusterDepth);
			m_pointLightInfoTexture = Texture2D::create(2, MaxLights, TextureFormat::R32G32B32A32_Float, false);
			m_pointLights.reserve(MaxLights);
		}

		void beginMakeClusters(
			const Vector3& cameraPos, const Vector3& cameraLookAt, const Vector3& cameraUp,
			float fov, float aspect, float nearClip, float farClip)
		{
			Vector3 cameraDir = Vector3::normalize(cameraLookAt - cameraPos);
			m_view = Matrix::makeLookAtLH(cameraPos, cameraLookAt, cameraUp);
			m_proj = Matrix::makePerspectiveFovLH(fov, aspect, nearClip, farClip);


			//Vector3 cp3 = Vector3::transformCoord(Vector3(cameraPos + cameraDir), m_view);
			//Vector4 cp4 = Vector4::transform(Vector4(cameraPos + cameraDir, 1.0f), m_view);

			//Vector3 np3 = Vector3::transformCoord(Vector3(cameraPos + cameraDir), m_view * m_proj);
			//Vector4 np4 = Vector4::transform(Vector4(cameraPos + cameraDir, 1.0f), m_view * m_proj);

			//Vector4 np = Vector4::transform(Vector4(0, 0, nearClip, 1.0f), m_proj);
			//Vector4 fp = Vector4::transform(Vector4(0, 0, farClip, 1.0f), m_proj);


			//Vector4 np41 = Vector4::transform(Vector4(cameraPos + cameraDir * nearClip, 1.0f), m_view * m_proj);
			//Vector4 fp41 = Vector4::transform(Vector4(cameraPos + cameraDir * farClip, 1.0f), m_view * m_proj);


			////Vector3 ff = Vector3::transformCoord(Vector3(0, 0, 10), m_view);
			//{
			//	Vector3 cpos = Vector3(-10, -1, 0);//cameraPos;
			//	auto vm = Matrix::makeLookAtLH(cpos, Vector3(0, 0, 0), cameraUp);
			//	//vm.inverse();
			//	Vector4 cp4 = Vector4::transform(Vector4(5, 0, 0, 1), vm);

			//	//for (int i = 0; i < 100; i++)
			//	//{
			//	//	float z = 10.0f * i;
			//	//	float cl = bias(0.9, z / 1000.0);
			//	//	printf("%d\n", static_cast<int>(cl * m_clusterDepth));
			//	//}

			//	printf("");
			//}

			//float zn = np41.z / np41.w;
			//float zf = fp41.z / fp41.w;

			m_clipRange = farClip/* - nearClip*/;

			// clear
			for (int y = 0; y < m_clusterHeight; y++)
			{
				for (int x = 0; x < m_clusterWidth; x++)
				{
					for (int z = 0; z < m_clusterDepth; z++)
					{
						m_clustersTexture->setPixel32(x, y, z, Color32(0, 0, 0, 0));
					}
				}
			}

			m_clustersAddCount.clear();
			m_clustersData.clear();
			m_clustersAddCount.resize(m_clusterWidth * m_clusterHeight * m_clusterDepth);
			m_clustersData.resize(m_clusterWidth * m_clusterHeight * m_clusterDepth);

			m_pointLights.clear();
			m_pointLights.add(PointLightInfo{/* Vector3(1, 255, 0), 255, Color::Red*/});	// dummy
		}

		void addPointLight(const Vector3& lightPos, float lightSize, const Color& color)
		{
			float lightRadius = lightSize / 2;
			float lightId = m_pointLights.getCount() + 1;

			// カメラから見た位置。奥が Z+。一番手前は 0.0
			Vector4 cp = Vector4::transform(Vector4(lightPos, 1.0f), m_view);

			// ビュー空間での、AABB の前面と後面
			float zn = cp.z - lightRadius;
			float zf = cp.z + lightRadius;

			Vector3 viewPoints[4] =
			{
				Vector3::transformCoord(Vector3(cp.x - lightRadius, cp.y - lightRadius, zf), m_proj),	// 左上 奥
				Vector3::transformCoord(Vector3(cp.x + lightRadius, cp.y + lightRadius, zf), m_proj),	// 右下 奥
				Vector3::transformCoord(Vector3(cp.x - lightRadius, cp.y - lightRadius, zn), m_proj),	// 左上 手前
				Vector3::transformCoord(Vector3(cp.x + lightRadius, cp.y + lightRadius, zn), m_proj),	// 右下 手前
			};

			Vector3 vpMin, vpMax;
			vpMin.x = std::min(viewPoints[0].x, viewPoints[2].x);
			vpMin.y = std::min(viewPoints[0].y, viewPoints[2].y);
			vpMax.x = std::max(viewPoints[1].x, viewPoints[3].x);
			vpMax.y = std::max(viewPoints[1].y, viewPoints[3].y);

			//Vector3 vpMin = Vector3::transformCoord(Vector3(cp.x - lightRadius, cp.y - lightRadius, zf), m_proj);
			//Vector3 vpMax = Vector3::transformCoord(Vector3(cp.x + lightRadius, cp.y + lightRadius, zf), m_proj);
			//Vector3 vpMin = Vector3::transformCoord(Vector3(lightPos - lightRadius), m_view*m_proj);
			//Vector3 vpMax = Vector3::transformCoord(Vector3(lightPos + lightRadius), m_view*m_proj);
			//Vector3 vpMin = Vector3::transformCoord(Vector3(lightPos.x - lightRadius, lightPos.y - lightRadius, zn), m_view*m_proj);
			//Vector3 vpMax = Vector3::transformCoord(Vector3(lightPos.x + lightRadius, lightPos.y + lightRadius, zn), m_view*m_proj);


			//Vector3 vpMax2 = Vector3::transformCoord(Vector3(cp.x - lightRadius, cp.y, zn), m_proj);
			//Vector3 vpMax3 = Vector3::transformCoord(Vector3(cp.x + lightRadius, cp.y, zn), m_proj);
			
			float vpZn = zn / m_clipRange;
			float vpZf = zf / m_clipRange;


			// 効率悪そうだけど、クラスタ情報の書き込むみ先を探すには全クラスタ数のインデックスベースで for しなければならない。
			// bios で指数関数的なクラスタ分割を行うので、座標からクラスタの範囲を求めてループするのはちょっと複雑。



#if 0
			for (float dy = vpMin.y; dy < vpMin.y; dy += 1.0f)
			{
				for (int y = 0; y < m_clusterHeight; y++)
				{
					for (int y = 0; y < m_clusterHeight; y++)
					{
					}
				}
			}
#else

			//// カメラ座標系内での AABB
			//AABB cpAABB = { cp.GetXYZ() - lightRadius,  cp.GetXYZ() + lightRadius };

			//// ビューポート座標系内での AABB (厳密には AA でなくなる。奥方向へすぼむ台形となる)
			//Vector4 vpAABBmin = Vector4::transform(Vector4(cpAABB.min, 1.0f), m_proj);
			//Vector4 vpAABBmax = Vector4::transform(Vector4(cpAABB.max, 1.0f), m_proj);
			//AABB vpAABB = { vpAABBmin.GetXYZ() / vpAABBmin.w,  vpAABBmax.GetXYZ() / vpAABBmax.w };


			// index -> viewport
			float xs = 2.0f / m_clusterWidth;
			float ys = 2.0f / m_clusterHeight;
			float zs = 1.0f / m_clusterDepth;

			float biasBase = 0.9;

			for (int y = 0; y < m_clusterHeight; y++)
			{
				float cb = (ys * y) - 1.0f;
				float ct = (ys * (y + 1)) - 1.0f;
				if ((vpMax.y > cb && vpMin.y < ct))
				{
					for (int x = 0; x < m_clusterWidth; x++)
					{
						float cl = (xs * x) - 1.0f;
						float cr = (xs * (x + 1)) - 1.0f;
						if ((vpMax.x > cl && vpMin.x < cr))
						{
//							if (x == 1)
//							{
//								/*
//								
//	[0]							[1]						[2]
//
//
//
//				-0.875					-0.75
//vpMin=-14.5					vpMax=-0.8
//
//*/
//								printf("\n");
//							}

							for (int z = 0; z < m_clusterDepth; z++)
							{
								// 0.0..1.0
								float cn = zs * z;
								float cf = zs * (z + 1);

								float czn = bias(biasBase, cn);
								float czf = bias(biasBase, cf);
								//printf("d:%f\n", czf - czn);
								if (vpZf > czn && vpZn < czf)
								{
									putClusterData(x, y, z, lightId);
								}
							}
						}
					}
				}
			}

#endif
			//for (int z = 0; z < m_clusterDepth; z++)
			//{
			//	float cn = zs * z;
			//	float czn = bias(biasBase, cn);
			//	printf("%f\n", czn);
			//}

			PointLightInfo info;
			info.pos = Vector4(lightPos, lightRadius);
			info.color = color;
			m_pointLights.add(info);
		}

		void endMakeClusters()
		{
			// m_clustersData -> m_clustersTexture
			for (int y = 0; y < m_clusterHeight; y++)
			{
				for (int x = 0; x < m_clusterWidth; x++)
				{
					for (int z = 0; z < m_clusterDepth; z++)
					{
						m_clustersTexture->setPixel32(x, y, z, m_clustersData[((m_clusterWidth * m_clusterHeight * z) + (m_clusterWidth * y) + x)]);
					}
				}
			}

			//Bitmap bmp(&m_pointLights[0], SizeI(2, MaxLights), PixelFormat::FloatR32G32B32A32);
			//m_pointLightInfoTexture->setSubData(PointI(0, 0), &bmp);
			m_pointLightInfoTexture->setMappedData(&m_pointLights[0]);
		}

	private:

		static float bias(float b, float x)
		{
			return x;
			//return pow(x, log(b) / log(0.5));
		}

		void putClusterData(int x, int y, int z, int lightId)
		{
			int clustersAddCountIndex =
				((m_clusterWidth * m_clusterHeight * z) + (m_clusterWidth * y) + x);
			int clustersAddCount = m_clustersAddCount[clustersAddCountIndex];
			if (clustersAddCount < 4)
			{
				Color32& color = m_clustersData[clustersAddCountIndex];
				switch (clustersAddCount)
				{
				case 0:
					color.r = lightId;
					break;
				case 1:
					color.g = lightId;
					break;
				case 2:
					color.b = lightId;
					break;
				case 3:
					color.a = lightId;
					break;
				default:
					LN_UNREACHABLE();
					break;
				}
				//printf("%d,%d,%d(z:%f)\n", x, y, z, cn);
				//m_clustersTexture->setPixel32(x, y, z/*+8*/, Color32(lightId, 0, 0, 0));



				m_clustersAddCount[clustersAddCountIndex]++;
			}
		}

		//void setClustersAddCount(int x, int y, int z, int count)
		//{

		//}
		//int getClustersAddCount(int x, int y, int z, int count);
		
	};



	LightClusters lc;
	lc.init();


	auto planeMesh = StaticMeshComponent::createPlane(Vector2(20, 20), 1, 1);
	Material* mat2 = planeMesh->getStaticMeshModel()->m_materials->getAt(0);
	mat2->setMaterialTexture(tex1);
	//mat2->setShader(clusterdShader);
	//auto planeObj1 = newObject<WorldObject3D>();
	//planeObj1->addComponent(planeMesh);

	auto cornellBox = CornellBox::create();
	auto cornellBoxObj = newObject<WorldObject3D>();
	cornellBoxObj->addComponent(cornellBox);

	Engine::getCamera3D()->setPosition(0, 10, -30);

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
		lc.beginMakeClusters(
			camPos, Vector3(0, 0, 0), camc->getUpDirection(),
			camc->getFovY(), 640.0f / 480.0f, camc->getNearClip(), camc->getFarClip());

		lc.addPointLight(Vector3(0, 0, 0), 10, Color::White);
		lc.addPointLight(Vector3(5, 0, 5), 2, Color::Red);
		lc.addPointLight(Vector3(-5, 0, 5), 3, Color::Blue);
		lc.addPointLight(Vector3(5, 0, -5), 4, Color::Green);
		lc.addPointLight(Vector3(-5, 0, -5), 5, Color::Yellow);

		lc.addPointLight(Vector3(7, 0, 0), 10, Color::Magenta);
		lc.addPointLight(Vector3(-7, 0, 0), 4, Color::Cyan);
		lc.addPointLight(Vector3(0, 0, 7), 10, Color::AliceBlue);
		lc.addPointLight(Vector3(0, 0, -7), 2, Color::BlueViolet);

		lc.endMakeClusters();


		mat2->setTextureParameter(_T("clustersTexture"), lc.m_clustersTexture);
		mat2->setVectorParameter(_T("cam_pos"), Vector4(camPos, 1));

		mat2->setFloatParameter(_T("near"), camc->getNearClip());
		mat2->setFloatParameter(_T("far"), camc->getFarClip());


		//auto vm = Matrix::makeLookAtLH(Vector3(0, -1, -10), Vector3(1, -1, -10), Vector3::UnitY);
		//mat2->setMatrixParameter(_T("view"), vm);
		mat2->setMatrixParameter(_T("view"), camc->getViewMatrix());

		mat2->setTextureParameter(_T("m_pointLightInfoTexture"), lc.m_pointLightInfoTexture);








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



