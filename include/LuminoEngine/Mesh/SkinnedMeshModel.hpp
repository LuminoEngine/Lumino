
#pragma once
#include "Mesh.hpp"

namespace ln {

class SkinnedMeshBoneResource
	: public Object
{
};


class SkinnedMeshBone
	: public Object
{
};

class SkinnedMeshModel
	: public StaticMeshModel
{
public:
	//void addMeshContainer(MeshContainer* meshContainer);
	//void addMaterial(AbstractMaterial* material);


	//const List<Ref<MeshContainer>>& meshContainers() const { return m_meshContainers; }
	//const List<Ref<AbstractMaterial>>& materials() const { return m_materials; }


private:
	//List<Ref<MeshContainer>> m_meshContainers;
	//List<Ref<AbstractMaterial>> m_materials;
};

} // namespace ln

