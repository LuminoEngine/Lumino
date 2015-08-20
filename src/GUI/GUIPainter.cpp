
#include "../Internal.h"
#include "GUIPainter.h"

namespace Lumino
{
namespace GUI
{
namespace Internal
{
	
//=============================================================================
// GUIPainter
//=============================================================================
//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
GUIPainter::GUIPainter(Graphics::GraphicsManager* manager)
	: Graphics::Painter(manager)
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
} // namespace GUI
} // namespace Lumino
