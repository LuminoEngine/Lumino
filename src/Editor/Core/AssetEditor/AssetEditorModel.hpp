#pragma once
#include "../App/ToolPanesArea.hpp"

namespace lna {
class EditorContext;

enum class AssetEditorViewType
{
    Scene,
};

enum class EditorPaneKind
{
    // Navigation と DocumentView の間に配置される、現在開いている Document の動作モードおよび、それに関連する主データを表示するための場所。
    // 例えば Scene Editor では、以下のようなモードがある
    // - A. プレハブ (WorldObject) を配置するモード（Blender のオブジェクトモード）
    // - B. タイルマップやテラインの編集を行うモード（Blender の編集モード。タイルマップは別窓ではなくシーン内で編集したい）
    // - C. グレーボクシングのための定義済みシェイプの配置モード
    // ここで選ぶモードに応じて、右側のドキュメント、Inspector, Tool 部分は大きく様子を変える。
    // 
    Mode,

    Inspector,

    Tool,
};

/*
    あるひとつの Asset のメインエディタのモデルクラス。
    イメージとしては丁度ドキュメントビューのタブひとつに当たるが、このクラスから DocumentView の他、各種 ToolPane も生成する。

    Note: 理想的には別途 Model の型から対応する View を生成する Factory を作るべきだが、かえって複雑になる（なった）ので、必要最小限で行ってみる。

    なお、AssetEditor の生成には、必ずしも Navigation が必要というわけではない。
*/
class AssetEditorModel
    : public ln::UIViewModel
{
public:
    AssetEditorViewType viewType() const { return AssetEditorViewType::Scene; }

    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) {}
    virtual void onClosed() {}

    // ドキュメントがアクティブになったりしたときに呼ばれる。
    // AssetEditor は派生クラスのメンバ変数など内部 AssetEditorViewModel を持つことができるが、
    // それをアクティブにしたい場合はこの実装で処理を行う。
    virtual void onActivate() {}

    virtual Ref<ln::List<Ref<ln::EditorPane>>> getEditorPanes(EditorPaneKind kind) { return nullptr; }

    EditorContext* editorContext() const { return m_editorContext; }

protected:
    AssetEditorModel();

public: // TODO: internal
    EditorContext* m_editorContext;
};

} // namespace lna
