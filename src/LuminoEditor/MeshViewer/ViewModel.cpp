﻿
#include "ViewModel.hpp"
#include "../src/Mesh/MeshModelInstance.hpp"

ViewModel* ViewModel::instance()
{
	static ViewModel i;
	return &i;
}

ViewModel::ViewModel()
{}

void ViewModel::load()
{
	ln::Scene::setAntialiasEnabled(true);

	auto settings = ln::makeObject<ln::MeshImportSettings>();
	//settings->setSkeletonImport(false);
	//settings->setCharacterModelFormat(true);

	//m_model = ln::MeshModel::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Mesh/SkinnedAxis1.glb", settings);
	//m_model = ln::MeshModel::load(u"D:/Documents/Modeling/HC5-6-export.glb", settings);
	//m_model = ln::MeshModel::load(LN_GLTF_SAMPLE_MODELS_DIR "2.0/Box/glTF-Binary/Box.glb", settings);
	//m_model = ln::MeshModel::load(LN_GLTF_SAMPLE_MODELS_DIR "2.0/AnimatedMorphCube/glTF-Binary/AnimatedMorphCube.glb", settings);
	//m_model = ln::MeshModel::load("C:/Proj/LN/Lumino/build/ExternalSource/glTF-Sample-Models//2.0//Buggy//glTF-Binary//Buggy.glb", settings);
	//m_model = ln::MeshModel::load("C:/Proj/LN/Lumino/build/ExternalSource/glTF-Sample-Models/2.0/2CylinderEngine/glTF-Binary/2CylinderEngine.glb", settings);
	//m_model = ln::MeshModel::load("C:/Proj/LN/Lumino/build/ExternalSource/glTF-Sample-Models/2.0/AlphaBlendModeTest/glTF-Binary/AlphaBlendModeTest.glb", settings);
	//m_model = ln::MeshModel::load(u"C:/Proj/LN/Lumino/build/ExternalSource/glTF-Sample-Models/2.0/AlphaBlendModeTest/glTF/AlphaBlendModeTest.gltf", settings);
	m_model = ln::MeshModel::load(u"D:/Materials/VRM/Alicia_VRM/Alicia/VRM/AliciaSolid.vrm");
	//m_model = ln::MeshModel::load(u"D:/Materials/VRM/Vita.vrm");
	
	
	m_meshComponent = ln::makeObject<VisualizedMeshComponent>();
	m_meshComponent->setModel(m_model);
	//m_meshComponent->setShadingModel(ln::ShadingModel::Unlit);
	m_meshComponent->setBlendMode(ln::BlendMode::Alpha);
	m_meshObject = ln::makeObject<ln::WorldObject>();
	m_meshObject->addComponent(m_meshComponent);
	m_meshObject->addInto();

	auto* ac = m_meshComponent->model()->animationController();
	//ac->play();
	//ac->addClip(ln::AnimationClip::load(u"D:/Materials/Mixamo/Idle.bvh"));

	//ln::AnimationClip::loadAsync(u"D:/Materials/Mixamo/Idle.bvh")->thenWith([=](auto clip) {
	//	ac->addClip(clip);
	//});

	//ln::AnimationClip::loadAsync(u"D:/Materials/Mixamo/Walk.bvh")->thenWith([=](auto clip) {
	//	ac->addClip(clip);
	//});

	//m_meshComponent->modelInstance()->morphs()[7]->setWeight(1, 0.9);
}
