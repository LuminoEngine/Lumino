#pragma once

namespace ln {
class IEditorExtension;

enum class EditorExtensionType
{
    /** ある種類のファイル 1 つを編集するためのプラグイン */
    DocumentEditor,
};

// Plugin に限らず、トランスパイらによって生成されたモジュールなども含む、DynamicLoad のルートインターフェイス。
class IModuleInterface
{

};

class IPluginModule : public IModuleInterface
{
public:
    virtual int getEditorExtensionCount() = 0;
    virtual IEditorExtension* getEditorExtension(int index) = 0;
};

class IEditorExtension
{
public:
    virtual EditorExtensionType getExtensionType() const = 0;
};

class IDocumentEditorExtension : public IEditorExtension
{
public:
    virtual EditorExtensionType getExtensionType() const { return EditorExtensionType::DocumentEditor; }

    /** この DocumentEditor が編集対象とするオブジェクトの種別。asset ファイルに埋め込まれる種類。 */
    virtual const Char* typeKeyword() const = 0;
};


} // namespace ln

#if defined(_WIN32)
    #define LN_MODULE_EXPORT  __declspec(dllexport)
#elif defined(__GNUC__)
    #define LN_MODULE_EXPORT __attribute__((visibility("default")))
#endif

#define LUMINO_MODULE(moduleClass) \
     extern "C" LN_MODULE_EXPORT \
    ::ln::IPluginModule* ModuleClass() \
    { \
        static moduleClass plugin; \
        return &plugin; \
    }

