#include <LuminoEngine/Engine/EngineContext2.hpp>
#include <LuminoEngine/Runtime/detail/BindingValidation.hpp>
#include <LuminoEngine/Base/Event.hpp>
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoEngine/Asset/AssetObject.hpp>
#include <LuminoEngine/Asset/AssetModel.hpp>

namespace ln {

void registerModuleTypes_Runtime(EngineContext2* context) {
    context->registerType<ZVTestClass1>();
    context->registerType<ZVTestEventArgs1>();
    context->registerType<EventConnection>();
    context->registerType<Serializer2>();
    context->registerType<AssetObject>();
    context->registerType<AssetImportSettings>();
    context->registerType<AssetModel>();
}

} // namespace ln
