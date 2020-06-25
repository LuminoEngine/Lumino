
#include <LuminoEngine.hpp>
#include <LuminoEngine/Mesh/MeshProcessing.hpp>
#include <LuminoEngine/ImageEffect/TonemapImageEffect.hpp>
using namespace ln;

class App_Sandbox_PostEffect : public Application
{
    void onInit() override
    {
        Engine::renderView()->setGuideGridEnabled(true);
        Engine::camera()->addComponent(CameraOrbitControlComponent::create());
        Engine::renderView()->setBackgroundColor(Color::Gray);

        //auto mesh = StaticMesh::create(u"D:/Tech/Tools/cornellBox/BJS-2.79-Cycles-gltf/assets/cornellBox-2.79-Cycles-gltf.bin");

        auto builder = makeObject<MeshGeometryBuilder>();
        builder->beginSection();
        builder->addPlane(Vector2(10, 10), 1, 1);
        builder->addPlane(Vector2(10, 10), 1, 1);
        builder->endSection();
        auto mesh = builder->buildMesh();

        auto model = makeObject<StaticMeshModel>();
        model->addMeshContainerNode(mesh);
        model->addMaterial(Material::create());
        model->updateNodeTransforms();

        auto obj = makeObject<StaticMesh>(model);


        //auto effect1 = makeObject<TonemapImageEffect>();
        //Engine::renderView()->addImageEffect(effect1);

        /*
        
        
	// front
	boxMesh->addSquare(
		Vertex{ Vector3(-10, 20, 10), Vector2(0, 0), -Vector3::UnitZ, Color::White },
		Vertex{ Vector3(-10, 0, 10), Vector2(0, 0), -Vector3::UnitZ, Color::White },
		Vertex{ Vector3(10, 0, 10), Vector2(0, 0), -Vector3::UnitZ, Color::White },
		Vertex{ Vector3(10, 20, 10), Vector2(0, 0), -Vector3::UnitZ, Color::White });
	// up
	boxMesh->addSquare(
		Vertex{ Vector3(-10, 20, -10), Vector2(0, 0), -Vector3::UnitY, Color::White },
		Vertex{ Vector3(-10, 20, 10), Vector2(0, 0), -Vector3::UnitY, Color::White },
		Vertex{ Vector3(10, 20, 10), Vector2(0, 0), -Vector3::UnitY, Color::White },
		Vertex{ Vector3(10, 20, -10), Vector2(0, 0), -Vector3::UnitY, Color::White });
	// down
	boxMesh->addSquare(
		Vertex{ Vector3(-10, 0, 10), Vector2(0, 0), Vector3::UnitY, Color::White },
		Vertex{ Vector3(-10, 0, -10), Vector2(0, 0), Vector3::UnitY, Color::White },
		Vertex{ Vector3(10, 0, -10), Vector2(0, 0), Vector3::UnitY, Color::White },
		Vertex{ Vector3(10, 0, 10), Vector2(0, 0), Vector3::UnitY, Color::White });
	// left
	boxMesh->addSquare(
		Vertex{ Vector3(-10, 20, -10), Vector2(0, 0), Vector3::UnitX, Color::Red },
		Vertex{ Vector3(-10, 0, -10), Vector2(0, 0), Vector3::UnitX, Color::Red },
		Vertex{ Vector3(-10, 0, 10), Vector2(0, 0), Vector3::UnitX, Color::Red },
		Vertex{ Vector3(-10, 20, 10), Vector2(0, 0), Vector3::UnitX, Color::Red });
	// right
	boxMesh->addSquare(
		Vertex{ Vector3(10, 20, 10), Vector2(0, 0), -Vector3::UnitX, Color::Green },
		Vertex{ Vector3(10, 0, 10), Vector2(0, 0), -Vector3::UnitX, Color::Green },
		Vertex{ Vector3(10, 0, -10), Vector2(0, 0), -Vector3::UnitX, Color::Green },
		Vertex{ Vector3(10, 20, -10), Vector2(0, 0), -Vector3::UnitX, Color::Green });


	boxMesh->addBox(Vector3(4, 12, 4), Color::White, Matrix::makeTranslation(-3, 6, 2));

	boxMesh->addSphere(3, 16, 16, Matrix::makeTranslation(4, 3, 0));

	auto mesh = newObject<StaticMeshModel>(boxMesh);

	auto material = CommonMaterial::create();
	material->setMaterialTexture(Texture2D::getWhiteTexture());
	mesh->addMaterial(material);
	StaticMeshComponent::initialize(mesh);
	*/
    }

    void onUpdate() override
    {
    }
};

void Sandbox_PostEffect()
{
    App_Sandbox_PostEffect app;
	detail::ApplicationHelper::run(&app);
}




