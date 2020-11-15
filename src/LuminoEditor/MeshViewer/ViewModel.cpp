
#include "ViewModel.hpp"

ViewModel* ViewModel::instance()
{
	static ViewModel i;
	return &i;
}

ViewModel::ViewModel()
	: m_mesh()
{}

void ViewModel::load()
{
	m_mesh = ln::SkinnedMesh::load(u"C:/Proj/LN/Lumino/src/LuminoEngine/test/Assets/Mesh/SkinnedAxis1.glb");
}
