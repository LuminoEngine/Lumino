
#if 0
#include "Internal.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Scene/SceneGraph.h>
#include <Lumino/Scene/Grid.h>
#include "SceneGraphManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_SCENE_BEGIN

//==============================================================================
// Grid
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Grid, VisualComponent);

//------------------------------------------------------------------------------
GridPtr Grid::Create3D()
{
	auto ptr = GridPtr::MakeRef();
	ptr->initialize(SceneGraphManager::Instance->GetDefault3DSceneGraph());
	SceneGraphManager::Instance->GetDefault3DSceneGraph()->GetRootNode()->AddChild(ptr);
	return ptr;
}

//------------------------------------------------------------------------------
Grid::Grid()
{
}

//------------------------------------------------------------------------------
Grid::~Grid()
{
}

//------------------------------------------------------------------------------
void Grid::initialize(SceneGraph* owner)
{
	VisualComponent::initialize(owner, 1);
}

//------------------------------------------------------------------------------
void Grid::DrawSubset(SceneGraphRenderingContext* dc, int subsetIndex)
{
	int divCount = 20;
	Vector3 planeMin(-10.0f, 0, -10.0f);
	Vector3 planeMax(10.0f, 0, 10.0f);
	Vector3 step = (planeMax - planeMin) / divCount;

	for (int x = 0; x <= divCount; ++x)
	{
		dc->DrawLinePrimitive(
			Vector3(planeMin.x + step.x * x, 0, planeMin.z), Color::DimGray,
			Vector3(planeMin.x + step.x * x, 0, planeMax.z), Color::DimGray);
	}

	for (int z = 0; z <= divCount; ++z)
	{
		dc->DrawLinePrimitive(
			Vector3(planeMin.x, 0, planeMin.z + step.z * z), Color::DimGray,
			Vector3(planeMax.x, 0, planeMin.z + step.z * z), Color::DimGray);
	}
}

LN_NAMESPACE_SCENE_END
LN_NAMESPACE_END

#endif
