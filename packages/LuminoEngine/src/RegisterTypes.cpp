#include <LuminoCore/Runtime/RuntimeContext.hpp>
#include <LuminoEngine/Engine/CoreApplication.hpp>
#include <LuminoEngine/Runtime/detail/BindingValidation.hpp>
#include <LuminoEngine/Base/Event.hpp>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Asset/AssetObject.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>


namespace ln {

void registerModuleTypes_Runtime(RuntimeContext* context) {
#include "RegisterTypes.inl"
}

} // namespace ln
