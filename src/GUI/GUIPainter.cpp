
#include "../Internal.h"
#include "GUIPainter.h"

namespace Lumino
{
LN_NAMESPACE_GUI_BEGIN
namespace Internal
{
	
//=============================================================================
// GUIPainter
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIPainter::GUIPainter(GraphicsManager* manager)
	: RenderingContext(manager)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIPainter::~GUIPainter()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
//void GUIPainter::SetRenderTarget(Graphics::Texture* renderTarget)
//{
//	SetProjection(renderTarget->GetSize());
//}

} // namespace Internal
LN_NAMESPACE_GUI_END
} // namespace Lumino
