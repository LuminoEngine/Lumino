#pragma once
#include "Common.h"

class EditorExtension : public ln::IPluginModule
{
public:
    EditorExtension();
    virtual const ln::Char* moduleId() const { return u"E3E852F4-FA79-4337-80E8-DBDB3F23ED51"; }
    virtual const ln::Char* moduleDisplayName() const { return u"EditorExtension"; }
    virtual int getEditorExtensionCount() { return 0; }
    virtual ln::IEditorExtension* getEditorExtension(int index) { return nullptr; }
    virtual void onActivate(lna::EditorContext* context) {}
    virtual void onDeactivate(lna::EditorContext* context) {}

private:
};
