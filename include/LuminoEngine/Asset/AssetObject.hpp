
#pragma once
#include "Common.hpp"

namespace ln {

// - WorldObject と AssetObject は 1:1
// - View と AssetObject は n:1 (View はインスペクタやプレビューなど様々なところから参照できる)
// - 【AssetObject は MVVM でいう ViewModel と Model(save,loadできる)】に相当する。View への変更通知機能を持つ。（強引に分類するなら Model に相当する）
//   - Prefab の変更を自動的に伝播させたりするのに使う。
// - データは AssetObject -> WorldObject の方向にしか書き込まれない。WorldObject は AssetObject に対する readonly な View の位置づけ。
//   - エディタでは、WorldObject を編集するのではなく、AssetObject を編集することになる。
// - Scene と SceneAsset にぶら下がる WorldObject と WorldObjectAsset のツリーは2重管理になる。これは WPF でも View と ViewModel で発生する構図。
// - 編集操作はすべて AssetObject 側から行う。Scene のツリーに WorldObject を追加したければ、SceneAsset::createNewObject() とかを呼び出す。
// - すべての AssetObject がファイルに保存されるとは限らない。WorldObjectAsset は通常、SceneAsset のファイルに含まれる。プレハブの場合は独立する予定。
//   - このように WorldObject などの階層構造を表現するため、AssetObject も親子関係を持つ。
// - Unity の SerializedObject に相当する。https://anchan828.github.io/editor-manual/web/serializedobject.html
//   - ※ScriptableObject ではない。
//   - SerializedObject 同様、編集対象の Object (WorldObject や Component など) をラップし、プロパティを操作する。
//   - AssetObject を派生して使うことはない。
// - 基本的な使い方も SerializedObject と同じく、update -> プロパティset -> apply がワンセット。
//   - apply で Undo スタックに変更履歴が詰まれる。これにより複数のプロパティをまとめてひとつの undo に入れることも可能。
class AssetObject
    : public Object
{
	LN_OBJECT;
public:


LN_CONSTRUCT_ACCESS:
    AssetObject();
    void init();

private:
    Ref<AssetObject> m_parent;
    List<Ref<AssetObject>> m_children;
};

} // namespace ln
