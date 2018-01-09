
#include <LuminoEngine.h>
#include <Lumino/Mesh/GizmoModel.h>
#include <Lumino/Scene/MeshModelObject.h>
#include <Lumino/Scene/StaticMesh.h>
#include <Lumino/Scene/Light.h>
#include <Lumino/Framework/Application.h>
#include <Lumino/Framework/GameScene.h>
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/UI/UIListBox.h>
#include <Lumino/Testing/TestHelper.h>
#include <Lumino/IO/Compression.h>
//#include "../../src/Graphics/Mesh/GizmoModel.h"
using namespace ln;


void UIControlsGallery();
int DxLibMain();




#if 0



template<class T, typename... TArgs>
Ref<T> NewObject(TArgs... args)
{
	auto ptr = Ref<T>::makeRef();
	//ptr->initialize(args);
	return ptr;
}


class Simple2DCharacterController
	: public Component
{
public:

	virtual void OnUpdate() override
	{
		auto player = GetOwnerObject();
		
		if (Input::IsPressed(InputButtons::Right))
			player->transform.Translate(1, 0);
	}
};


class TestScene1
	: public GameScene
{
public:

	virtual void OnStart() override
	{
		auto player = WorldObject::create();
		player->AddComponent(Sprite2DComponent::create(_LT("C:/LocalProj/設計ツール/dll_48x48.png")));
		player->AddComponent(NewObject<Simple2DCharacterController>());
	}

	virtual void OnUpdate() override
	{
		GameScene::OnUpdate();
	}

	virtual void OnTerminate() override
	{
	}

};

void Main()
{
	GameApplication app;
	app.Run(NewObject<TestScene1>());
}

#else



void Main()
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	//ZipFile::extractToDirectory(_T("D:\\MMD\\Materials\\Models\\人物\\にがもん式霊夢V01_s05a.zip"), _T("D:\\tmp\\V01_s05a"));

	Engine::initialize();

	while (Engine::update())
	{
	}

	Engine::terminate();

	return;

	//UIControlsGallery();
	//DxLibMain();
	return;


	//class Foo{
	//public:
	//	void FooFunc(){}
	//};
	//Foo foo;
	//std::function<void()> Func = std::bind(&Foo::FooFunc, &foo);
	//std::function<void()> f2;
	//f2 = Func;
	//// メンバ関数を通常の関数として呼べる
	//Func();




	Ray ray(Vector3(1, 0, 0), Vector3(0, 0, 1));
	Matrix mat = Matrix::makeScaling(2) * Matrix::makeRotationY(Math::PI / 4)/* * Matrix::MakeTranslation(2, 0, 0)*/;
	mat.inverse();

	Ray ray2 = ray;
	ray2.direction += ray2.origin;
	ray2.origin.transformCoord(mat);
	ray2.direction.transformCoord(mat);
	ray2.direction -= ray2.origin;

	std::function<void(int)> f = [ray](int a) {};


	//EngineSettings::SetGraphicsAPI(GraphicsAPI::OpenGL);
	EngineSettings::setGraphicsRenderingType(GraphicsRenderingType::Threaded);//GraphicsRenderingType::Immediate);//
	Engine::initialize();
	//Engine::getMainViewport()->SetBackgroundColor(Color32::Gray);
	Engine::getMainViewport()->setPlacement(ViewportPlacement::AutoResize);

	//Engine::getDefaultSceneGraph3D()->visibleGridPlane = true;

	Engine::getCamera3D()->addComponent(newObject<CameraMouseMoveBehavior>());

	//auto mesh = SkinnedMeshComponent::create(_LT("D:/MMD/Materials/モデル/Appearance Miku/Appearance Miku_BDEF.pmx"));

	//auto tex = Texture2D::create("D:/Proj/Volkoff/External/Lumino/test/UnitTest/Scene/TestData/Dice1.png");
	//auto sprite1 = Sprite3DComponent::create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite2 = Sprite3DComponent::create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite3 = Sprite3DComponent::create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite4 = Sprite3DComponent::create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite5 = Sprite3DComponent::create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);
	//auto sprite6 = Sprite3DComponent::create(1, 1, tex); sprite1->SetSrcRect(0, 0, 50, 50);

	//auto mesh = StaticMeshComponent::createSphere(2, 4, 4);

#if 0
	auto blur = ScreenMotionBlurImageEffect::create();
	//blur->setAmount(0.5f);
	//blur->setRadialScale(1.05f);
	CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(blur);

	//auto tonePE23 = ToneImageEffect::create();
	//tonePE23->setTone(ToneF(1, 0,0, 1.0));
	//CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(tonePE23);

	//auto tonePE2 = ToneImageEffect::create();
	//tonePE2->setTone(ToneF(0,0,1, 1.0));
	//CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(tonePE2);

	auto tonePE = ToneImageEffect::create();
	//tonePE->setTone(ToneF(0,0,1, 1.0));
	tonePE->ChangeTone(ToneF(1, 0, 1, 0), 5);
	CameraViewportLayer::GetDefault2D()->GetImageEffects()->Add(tonePE);
#endif

#if 0
	//Engine::GetMainLight3D()->setPosition();
	auto mLogoSprite = Sprite2DComponent::create(_LT("D:/Proj/Volkoff/Volkoff/Data/Graphics/Frontend/Logo_1.png"));

	int map[5*5] =
	{
		1, 0, 0, 0, 1,
		1, 0, 0, 0, 1,
		1, 0, 0, 0, 1,
		1, 0, 0, 0, 1,
		1, 0, 0, 0, 1,
	};

	auto t = Assets::LoadTexture(_LT("D:/Proj/Volkoff/Volkoff/Data/Graphics/MapChip_1.png"));

	auto m_tileset = TileSet::create();
	m_tileset->SetImageSource(t);
	m_tileset->SetTileSize(SizeI(20, 20));


	auto tilemapModel = TileMapModel::create();
	tilemapModel->SetTileSet(m_tileset);

	auto m_tileLayer = TileLayer::create();
	m_tileLayer->Resize(5, 5);
	tilemapModel->GetLayers()->Add(m_tileLayer);

	auto mTilemap = TileMap::Create3D();
	mTilemap->SetTileMap(tilemapModel);
	mTilemap->SetPriority(1000);
	mTilemap->SetDepthWriteEnabled(false);

	for (int i = 0; i < 5 * 5; ++i)
	{
		m_tileLayer->SetTileId(i % 100, i / 100, map[i]);
	}
#endif

	EngineDiag::setDisplayMode(EngineDiagDisplayMode::FpsSummary);

	Input::addBinding(_LT("AA"), KeyGesture::create(Keys::C));
	Input::addBinding(_LT("GG"), KeyGesture::create(Keys::A));

	//auto spr1 = Sprite2DComponent::create(_LT("D:/Proj/LuminoStudio/Engine/External/Lumino/test/UnitTest/Graphics/TestData/Sprite2.png"));

	//auto text = TextBlock2DComponent::create(_LT("Hello, world!"));

#if 0

	auto m = SpriteParticleModel::create();
	m->SetSpawnRate(100);
	m->SetLifeTime(3.0f);
	m->m_maxParticles = 300;

	//m->m_shapeType = ParticleEmitterShapeType::Sphere;
	//m->m_shapeParam.x = 10;


	m->m_shapeType = ParticleEmitterShapeType::Cone;
	m->m_shapeParam.x = Math::PI * 0.1;
	m->m_shapeParam.y = 2;

	//m->SetPositionRange(Vector3(-10, 10, -10), Vector3(10, 10, 10));
	m->SetVelocity(Vector3(0, -3, 0));
	//m->SetAccel(Vector3(0, -8, 0));

	auto material = DiffuseMaterial::create();
	material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->SetEmissive(Color::White);
	m->SetMaterial(material);
	//m->setTexture(Texture2D::create(LN_LOCALFILE("../Media/Spark1.png"), TextureFormat::R8G8B8A8, false));

	auto particle1 = SpriteParticle::Create3D(m);
	particle1->SetBlendMode(BlendMode::Add);
	particle1->SetPosition(2, 0, 0);

	//auto particle2 = SpriteParticle::Create3D(m);
	//particle2->setBlendMode(BlendMode::Subtract);
	//particle2->setPosition(3, 0, 0);

	//particle1->addChild(particle2);
#endif

#if 0
	auto m1 = SpriteParticleModel::create();
	m1->SetSpawnRate(2);
	m1->SetLifeTime(2.0f);
	m1->m_maxParticles = 10;
	m1->m_shapeType = ParticleEmitterShapeType::Cone;
	m1->m_shapeParam.x = Math::PI * 0.1;
	m1->m_shapeParam.y = 2;

	auto m2 = SpriteParticleModel::create();
	m2->SetSpawnRate(2);
	m2->SetLifeTime(2.0f);
	m2->m_maxParticles = 10;
	m2->m_shapeType = ParticleEmitterShapeType::Cone;
	m2->m_shapeParam.x = Math::PI * 0.1;
	m2->m_shapeParam.y = 2;
	m1->SetSubParticle(m2);

	auto material = DiffuseMaterial::create();
	material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	m1->SetMaterial(material);
	m2->SetMaterial(material);
	m2->Commit();

	auto particle1 = SpriteParticle::Create3D(m1);
	particle1->SetBlendMode(BlendMode::Add);
	particle1->SetPosition(2, 0, 0);

#endif
#if 0
	auto m1 = SpriteParticleModel::create();
	m1->SetSpawnRate(40);
	m1->SetLifeTime(5.0f);
	m1->m_maxParticles = 100;
	m1->m_shapeType = ParticleEmitterShapeType::Box;
	m1->m_minPosition.Set(-10, 10, -10);
	m1->m_maxPosition.Set(10, 10, 10);
	m1->SetVelocity(Vector3(0, -5,0));
	m1->SetSize(0.5);
	//m1->m_loop = false;

	auto material = DiffuseMaterial::create();
	material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	m1->SetMaterial(material);

	auto particle1 = SpriteParticle::Create3D(m1);
	particle1->SetBlendMode(BlendMode::Add);
	particle1->SetPosition(2, 0, 0);

#endif
#if 0	// 半円ほわほわ
	auto m1 = SpriteParticleModel::create();
	m1->SetSpawnRate(40);
	m1->SetLifeTime(5.0f);
	m1->m_maxParticles = 100;
	m1->m_movementType = ParticleMovementType::Radial;
	m1->m_shapeParam.x = 10;
	m1->m_axis.minValue.Set(0, 1, 0);
	m1->m_axis.maxValue.Set(0, 1, 0);
	m1->m_angle.minValue = 0;
	m1->m_angle.maxValue = Math::PI;
	m1->m_forwardPosition.minValue = 1;
	m1->m_forwardPosition.maxValue = 2;
	m1->SetSize(0.5);

	auto material = DiffuseMaterial::create();
	material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	m1->SetMaterial(material);

	auto particle1 = SpriteParticle::Create3D(m1);
	particle1->SetBlendMode(BlendMode::Add);
	particle1->SetPosition(2, 0, 0);

#endif
#if 0	// くるくる
	auto m1 = SpriteParticleModel::create();
	m1->SetSpawnRate(40);
	m1->SetLifeTime(5.0f);
	m1->m_maxParticles = 100;
	m1->m_movementType = ParticleMovementType::Radial;
	m1->m_shapeParam.x = 10;
	m1->m_axis.minValue.Set(0, 1, 0);
	m1->m_axis.maxValue.Set(0, 1, 0);
	m1->m_angle.minValue = 0;
	m1->m_angle.maxValue = Math::PI * 2;
	m1->m_angleVelocity.minValue = 0.25;
	m1->m_angleVelocity.maxValue = 1.0;
	m1->m_forwardPosition.minValue = 5;
	m1->m_forwardPosition.maxValue = 10;
	m1->SetSize(0.5);

	auto material = DiffuseMaterial::create();
	material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	m1->SetMaterial(material);

	auto particle1 = SpriteParticle::Create3D(m1);
	particle1->SetBlendMode(BlendMode::Add);
	particle1->SetPosition(2, 0, 0);

#endif
#if 0	// 引力斥力
	auto m1 = SpriteParticleModel::create();
	m1->SetSpawnRate(100);
	m1->SetLifeTime(5.0f);
	m1->m_maxParticles = 1000;
	m1->m_movementType = ParticleMovementType::Radial;
	m1->m_shapeParam.x = 10;
	m1->m_axis.minValue.Set(0, 1, 0);
	m1->m_axis.maxValue.Set(0, 1, 0);
	m1->m_angle.minValue = 0;
	m1->m_angle.maxValue = Math::PI * 2;
	m1->m_angleVelocity.minValue = 0.25;
	m1->m_angleVelocity.maxValue = 1.0;
	m1->m_forwardPosition.minValue = 5;
	m1->m_forwardPosition.maxValue = 10;
	m1->m_forwardVelocity.minValue = -5;
	m1->m_forwardVelocity.maxValue = 0;
	m1->SetSize(0.5);

	auto material = DiffuseMaterial::create();
	material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	m1->SetMaterial(material);

	auto particle1 = SpriteParticle::Create3D(m1);
	particle1->SetBlendMode(BlendMode::Add);
	particle1->SetPosition(2, 0, 0);

#endif
#if 0	// point trail
	auto m1 = SpriteParticleModel::create();
	m1->SetSpawnRate(4);
	m1->SetLifeTime(5.0f);
	m1->m_maxParticles = 1000;
	m1->m_movementType = ParticleMovementType::Radial;
	m1->m_shapeParam.x = 10;
	m1->m_axis.minValue.Set(0, 1, 0);
	m1->m_axis.maxValue.Set(0, 1, 0);
	m1->m_angle.minValue = 0;
	m1->m_angle.maxValue = Math::PI * 2;
	m1->m_angleVelocity.minValue = 0.25;
	m1->m_angleVelocity.maxValue = 1.0;
	m1->m_forwardPosition.minValue = 5;
	m1->m_forwardPosition.maxValue = 10;
	m1->SetSize(0.5);
	m1->m_trailType = ParticlTrailType::Point;
	m1->m_trailTime = 0.5f;
	m1->m_loop = false;

	auto material = DiffuseMaterial::create();
	material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	m1->SetMaterial(material);

	auto particle1 = ParticleEmitter::Create3D(m1);
	particle1->SetBlendMode(BlendMode::Subtract);
	particle1->SetPosition(2, 0, 0);

#endif
#if 0	// 放射
	Camera::GetMain3DCamera()->SetFarClip(10000);
	auto m1 = SpriteParticleModel::create();
	m1->m_maxParticles = 128;
	m1->SetSpawnRate(12);
	m1->SetLifeTime(3.0);
	m1->SetAutoFadeTime(0.3, 0.1);
	m1->m_loop = true;

	m1->SetSize(15, 35);

	m1->m_trailType = ParticlTrailType::Point;
	m1->m_trailTime = 0.1;

	m1->m_movementType = ParticleMovementType::Radial;
	m1->m_axis.minValue.Set(0, 0, 1);
	m1->m_axis.maxValue.Set(0, 0, 1);
	m1->m_angle.minValue = 0;
	m1->m_angle.maxValue = Math::PI * 2;
	m1->m_forwardVelocity.minValue = 5*60;
	m1->m_forwardVelocity.maxValue = 0.5 * 60;
	//m1->m_forwardPosition.minValue = 5;
	//m1->m_forwardPosition.maxValue = 10;

	m1->m_sizeRandomSource = ParticleRandomSource::ByBaseValue;	// サイズが小さいものほど、
	m1->m_forwardVelocity.randomSource = ParticleRandomSource::ByBaseValueInverse;	// 速度が大きい


	auto material = DiffuseMaterial::create();
	material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	m1->SetMaterial(material);

	auto particle1 = ParticleEmitter3D::create(m1);
	particle1->SetBlendMode(BlendMode::Subtract);
	particle1->SetPosition(2, 0, 200);

#endif
#if 0	// 雨
	Camera::GetMain3DCamera()->SetFarClip(10000);
	auto m1 = SpriteParticleModel::create();
	m1->m_maxParticles = 10000;
	m1->SetSpawnRate(1000);
	m1->SetLifeTime(1.0);
	m1->m_loop = true;

	m1->SetSize(0.05, 0.05);

	m1->m_shapeType = ParticleEmitterShapeType::Box;
	m1->m_shapeParam.Set(10, 0, 10);

	m1->m_particleDirection = ParticleDirectionType::MovementDirection;
	m1->m_forwardVelocity.minValue = -12;
	m1->m_forwardVelocity.maxValue = -12;
	m1->m_lengthScale = 10;

	auto material = DiffuseMaterial::create();
	material->SetMaterialTexture(Texture2D::create(LN_LOCALFILE("../UnitTest/Scene/TestData/Particle1.png")));
	material->SetShader(Shader::GetBuiltinShader(BuiltinShader::Sprite));
	m1->SetMaterial(material);

	auto particle1 = ParticleEmitter3D::create(m1);
	particle1->SetBlendMode(BlendMode::Add);
	particle1->SetPosition(0, 12, 0);
	//particle1->setAngles(Math::PI, 0, 0);



	auto m2 = SpriteParticleModel::create();
	m2->m_maxParticles = 1000;
	m2->SetSpawnRate(200);
	m2->SetLifeTime(0.2);
	m2->m_loop = true;
	m2->SetSize(0.1, 0.1);
	m2->m_minSizeVelocity = 3;
	m2->m_maxSizeVelocity = 3;
	m2->m_shapeType = ParticleEmitterShapeType::Box;
	m2->m_shapeParam.Set(10, 0, 10);
	m2->m_particleDirection = ParticleDirectionType::Horizontal;
	m2->SetMaterial(material);

	auto particle2 = ParticleEmitter3D::create(m2);
	particle2->SetBlendMode(BlendMode::Add);
#endif

	//auto uiRoot = UIContext::getMainContext()->GetMainWindowView()->getLayoutRoot();
	//auto listBox = tr::UIListBox::create();
	//listBox->addTextItem(_LT("test"));
	//listBox->addTextItem(_LT("fff"));
	//uiRoot->setContent(listBox);

	//GameAudio::playBGM("D:/GameProjects/Materials/BGM/Windsphere/call.mp3");
	


	//auto gizmo = static_cast<CameraViewportLayer*>(Engine::getDefault3DLayer())->CreateGizmo();

	//auto sp = Sprite3DComponent::create(2, 2, Texture2D::create(_LT("D:/GameProjects/Chronicles/110220c_as019.jpg")));
	//sp->setTone(ToneF(0, 0, 1, 1.0));
	//gizmo->setup(Matrix::Identity, sp->getTransform());//Matrix::MakeTranslation(1, 0, 0));
	//
	//
	//gizmo->addOnTargetTransformChanged([sp](tr::GizmoModel* g)
	//{
	//	sp->setTransform(g->GetTargetTransform());
	//});
	//gizmo->addOnSubmitEditing([sp](tr::GizmoModel* g)
	//{
	//	//sp->setTransform(g->GetTargetTransform());
	//});


	//auto tex1 = Texture2D::create(32, 32);
	//tex1->Clear(Color32::Red);
	//auto box1 = StaticMeshComponent::createBox(Vector3(5, 5, 5));
	//auto box1 = StaticMeshComponent::createTeapot();
	//auto box1 = StaticMeshComponent::create(_LT("D:/Proj/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/MqoTest1.mqo"));
	//auto box1 = StaticMeshComponent::create(_LT("C:/Proj/FluoriteSolution/External/Lumino/Source/LuminoEngine/Test/UnitTest/Graphics/TestData/Plant1.mqo"));

	//box1->GetMaterials()->GetAt(0)->setMaterialTexture(tex1);
	////box1->setTone(ToneF(0, 0, 1, 1.0));
	//gizmo->setup(Matrix::Identity, box1->getTransform());//Matrix::MakeTranslation(1, 0, 0));

	//auto mesh3 = StaticMeshComponent::createPlane(Vector2(3, 3), 1, 1);
	//mesh3->setPosition(-2, 0, 0);

#if 0
	List<Ref<StaticMeshComponent>> boxList;
	for (int z = 0; z < 10; z++)
	{
		for (int y = 0; y < 10; y++)
		{
			for (int x = 0; x < 10; x++)
			{
				auto mesh2 = StaticMeshComponent::CreateBox();
				mesh2->SetPosition(x * 2, y * 2, z * 2);
				boxList.Add(mesh2);
			}
		}
	}
#endif

#if 0
	auto col1 = BoxCollisionShape::create(1, 2, 3);
	auto body1 = RigidBodyComponent::create(col1);
	body1->SetPosition(10, 0, 0);
#endif
	//static_cast<CameraViewportLayer*>(Engine::getDefault3DLayer())->setDebugDrawFlags(WorldDebugDrawFlags::PhysicsInfo);

	auto uiRoot = Application::getMainWindow();
	//auto thumb = UIThumb::create();
	////thumb->setPosition(Point(100, 200));
	//thumb->setSize(Size(30,60));
	////thumb->setBackground(ColorBrush::Red);
	////textBlock1->SetText(_LT("TextBlock"));
	//uiRoot->setContent(thumb);
	auto track = UIScrollBar::create();
	//track->setSize(Size(200, NAN));
	track->setMaximum(10);
	track->setValue(3);
	track->setViewportSize(2);
	uiRoot->addChild(track);


	while (!Engine::isEndRequested())
	{
		Engine::updateFrame();
		//if (Engine::BeginRendering())
		{
			Engine::renderFrame();


			//Engine::getDefaultSceneGraph3D()->getRenderer()->drawLinePrimitive(
			//	Vector3(0, 0, 0), Color::Red,
			//	Vector3(5, 5, 5), Color::White);


			//gizmo->render(Engine::getDefault3DLayer()->getRenderer());

			Engine::presentFrame();
		}

		//body1->getWorldTransform().GetPosition().print();


		//if (Input::isTriggered(InputButtons::Ok))
		//{
		//	gizmo->setGizmoType(tr::GizmoType::Scaling);
		//}
		//if (Input::isTriggered(InputButtons::Cancel))
		//{
		//	gizmo->setGizmoType(tr::GizmoType::Rotation);
		//}
	}

	//while (Engine::update())
	//{
	//	//printf("----\n");
	//	if (Input::isTriggered(InputButtons::Cancel))
	//	{
	//		//tonePE->setTone(ToneF(-1, -1, -1, 0));
	//		//tonePE->changeTone(ToneF(1, 1, 1, 0), 0.5);
	//		//blur->setBlurStatus(0.5, Vector2::Zero, 1.05, 0.5);

	//		//Engine::getMainWindow()->setSize(SizeI(200, 100));
	//	}
	//	//blur->setBlurStatus(0.9f, Vector2::Zero, 1.02);
	//}
}

#endif
