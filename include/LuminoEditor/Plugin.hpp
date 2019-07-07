#pragma once

namespace ln {

enum class PluginType
{
    /** ある種類のファイル 1 つを編集するためのプラグイン */
    DocumentEditor,
};

class IPlugin
{
public:
    virtual PluginType pluginType() const = 0;
};

class IDocumentEditorPlugin : public IPlugin
{
public:
    virtual PluginType pluginType() const { return PluginType::DocumentEditor; }

    /** この DocumentEditor が編集対象とするオブジェクトの種別。asset ファイルに埋め込まれる種類。 */
    virtual const Char* typeKeyword() const = 0;
};

} // namespace ln
