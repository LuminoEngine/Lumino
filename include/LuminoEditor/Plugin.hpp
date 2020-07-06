#pragma once

namespace lna {
class EditorContext;
class AssetEditorModel;
}

namespace ln {
class UIFrame;
class NavigationMenuItem;
class AssetImporter;
class EditorPane;
class IEditorExtension;

enum class EditorExtensionType
{
    Default,

    AssetNavigator,

    /** アセットをインポートするための拡張機能 */
    AssetImporter,

    /** ある種類のファイル 1 つを編集するための拡張機能 */
    AssetEditor,

    // 別の AssetEditor を拡張する
    AssetSubEditor,
};


// Plugin に限らず、トランスパイらによって生成されたモジュールなども含む、DynamicLoad のルートインターフェイス。
class IModuleInterface
{
public:
    virtual const ln::Char* moduleId() const = 0;
    virtual const ln::Char* moduleDisplayName() const = 0;
};

class IPluginModule : public IModuleInterface
{
public:
    //virtual int getEditorExtensionCount() { return 0; }
    //virtual IEditorExtension* getEditorExtension(int index) { return nullptr; }

    // メインプロジェクトが開かれ、関係するプラグインがロードされたときに呼び出される。
    // この中で、Navigator や AssetEditorModelFactory の登録を行う。
    virtual void onActivate(lna::EditorContext* context) = 0;

    virtual void onDeactivate(lna::EditorContext* context) = 0;
};

class IEditorExtension
{
public:
    virtual const Char* id() const = 0;
    virtual const Char* displayName() const = 0;
    virtual EditorExtensionType getExtensionType() const = 0;
};

//// depreceted
//class IAssetNavigatorExtension : public IEditorExtension
//{
//public:
//    virtual EditorExtensionType getExtensionType() const { return EditorExtensionType::AssetNavigator; }
//    virtual void onAttached() = 0;
//    virtual void onDetached() = 0;
//    virtual NavigationMenuItem* getNavigationMenuItem() = 0;
//	virtual UIElement* getNavigationPane() = 0;
//};
//
//
//class IAssetImporterEditorExtension : public IEditorExtension
//{
//public:
//    static const int BasePriority = 100;
//
//    virtual EditorExtensionType getExtensionType() const { return EditorExtensionType::AssetImporter; }
//
//    // 指定したファイルパスが、この PluginModule で扱える可能性があるかどうかを判断する。
//    // filePath : 相対パスの場合、プロジェクトの Assets フォルダの中のファイルである。Assets フォルダからの相対パス。
//    //             絶対パスの場合、プロジェクトの外側のファイルである。
//    // return: 扱えない場合は 0. 拡張子が全く違う場合など。
//    //          拡張子だけ一致している場合は BasePriority.
//    //          保存フォルダも一致している場合は BasePriority + 1
//    //          完全一致は BasePriority + 10、など。
//    virtual int matchFilePath(const Path& filePath) = 0;
//
//    virtual Ref<AssetImporter> createImporter(const Char* assetSourceFilePath) = 0;
//};
//

//
//class IAssetEditorExtension : public IEditorExtension
//{
//public:
//    virtual EditorExtensionType getExtensionType() const { return EditorExtensionType::AssetEditor; }
//
//    /** この DocumentEditor が編集対象とするオブジェクトの種別。asset ファイルに埋め込まれる種類。 */
//    virtual const Char* typeKeyword() const = 0;
//
//    virtual Ref<lna::AssetEditorModel> createEditor() = 0;
//};

// 型名と、それを編集するための AssetEditorModel を生成するファクトリ
class AssetEditorModelFactory
	: public ln::Object
{
public:
    virtual bool checkTargetType(const ln::TypeInfo* assetType) = 0;
    virtual Ref<lna::AssetEditorModel> createAssetEditorModel() = 0;
};




using GetModuleClassFunc = ::ln::IPluginModule*();

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

