
#include "ViewModel.hpp"

ViewModel* ViewModel::instance()
{
	static ViewModel i;
	return &i;
}

ViewModel::ViewModel()
{}

void ViewModel::load()
{
	auto settings = ln::makeObject<ln::MeshImportSettings>();
	settings->setSkeletonImport(false);

	m_model = ln::MeshModel::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Mesh/SkinnedAxis1.glb", settings);
	m_meshComponent = ln::makeObject<VisualizedMeshComponent>();
	m_meshComponent->setModel(m_model);
	m_meshObject = ln::makeObject<ln::WorldObject>();
	m_meshObject->addComponent(m_meshComponent);

	auto* ac = m_meshComponent->model()->animationController();
	//ac->play();
}
