#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/SurfaceContext.hpp>
#include <LuminoEngine/Rendering/Kanata/KDrawEvent.hpp>

namespace ln {

//==============================================================================
// SurfaceContext

SurfaceContext::SurfaceContext()
    : context(nullptr) 
    , commandList(nullptr)
    , renderingContext()
    , drawEventList() {
}

SurfaceContext::~SurfaceContext() {

}

} // namespace ln
