#include "RegisterTypesGenerator.hpp"

//==============================================================================
// RegisterTypesGenerator

void RegisterTypesGenerator::generate(Module* module) {
    if (module->outputRegisterTypesFile.isEmpty()) return;

    auto types = db()->classes().filter([&](auto& t) {
        if (!t->isObjectGroup()) return false;
        if (t->isStatic()) return false;
        if (t->isRootObjectClass()) return false;
        if (t->isDelegateObject()) return false;
        if (t->isPromise()) return false;
        if (t->fullName() == U"ln::PromiseFailureDelegate") return false;
        if (t->fullName() == U"ln::Variant") return false;
        if (t->fullName() == U"ln::ComponentList") return false;    // TODO: これは LN_CLASS ではなく LN_LIST とかでマークしたい。
        return t->module() == module; });


    OutputBuffer text;
    for (auto& type : types) {
        text.AppendLine(U"context->registerType<{}>();", type->fullName());
    }

    {
        ln::FileSystem::writeAllText(module->outputRegisterTypesFile, text.toString(), ln::TextEncoding::getEncoding(ln::EncodingType::UTF8));
    }
}
