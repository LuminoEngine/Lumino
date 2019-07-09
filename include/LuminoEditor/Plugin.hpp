#pragma once

namespace ln {
class AssetImporter;
class IEditorExtension;

enum class EditorExtensionType
{
    /** アセットをインポートするための拡張機能 */
    AssetImporter,

    /** ある種類のファイル 1 つを編集するための拡張機能 */
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
    virtual const Char* id() const = 0;
    virtual const Char* displayName() const = 0;
    virtual EditorExtensionType getExtensionType() const = 0;
};

class IAssetImporterEditorExtension : public IEditorExtension
{
public:
    virtual EditorExtensionType getExtensionType() const { return EditorExtensionType::AssetImporter; }
    virtual Ref<AssetImporter> createImporter(const Char* assetSourceFilePath) = 0;
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

