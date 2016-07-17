
#include "Internal.h"
#include <Lumino/Graphics/Mesh.h>

LN_NAMESPACE_BEGIN
	
//==============================================================================
// StaticMeshModel
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(StaticMeshModel, Object);

//------------------------------------------------------------------------------
StaticMeshModel::StaticMeshModel()
{
}

//------------------------------------------------------------------------------
StaticMeshModel::~StaticMeshModel()
{
}

//------------------------------------------------------------------------------
void StaticMeshModel::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
}

//------------------------------------------------------------------------------
void StaticMeshModel::Draw(GraphicsContext* g)
{

}

LN_NAMESPACE_END

