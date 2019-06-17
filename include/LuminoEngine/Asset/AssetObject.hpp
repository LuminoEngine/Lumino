
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
class AssetObject
    : public Object
{
	LN_OBJECT;
public:


LN_CONSTRUCT_ACCESS:
    AssetObject();
    void init();
};

} // namespace ln
