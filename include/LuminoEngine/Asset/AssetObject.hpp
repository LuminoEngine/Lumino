
#pragma once
#include "../Engine/Property.hpp"
#include "Common.hpp"

namespace ln {
class AssetProperty;

// - WorldObject と AssetModel は 1:1
// - View と AssetModel は n:1 (View はインスペクタやプレビューなど様々なところから参照できる)
// - 【AssetModel は MVVM でいう ViewModel と Model(save,loadできる)】に相当する。View への変更通知機能を持つ。（強引に分類するなら Model に相当する）
//   - Prefab の変更を自動的に伝播させたりするのに使う。
// - データは AssetModel -> WorldObject の方向にしか書き込まれない。WorldObject は AssetModel に対する readonly な View の位置づけ。
//   - エディタでは、WorldObject を編集するのではなく、AssetModel を編集することになる。
// - Scene と SceneAsset にぶら下がる WorldObject と WorldObjectAsset のツリーは2重管理になる。これは WPF でも View と ViewModel で発生する構図。
// - 編集操作はすべて AssetModel 側から行う。Scene のツリーに WorldObject を追加したければ、SceneAsset::createNewObject() とかを呼び出す。
// - すべての AssetModel がファイルに保存されるとは限らない。WorldObjectAsset は通常、SceneAsset のファイルに含まれる。プレハブの場合は独立する予定。
//   - このように WorldObject などの階層構造を表現するため、AssetModel も親子関係を持つ。
// - Unity の SerializedObject に相当する。https://anchan828.github.io/editor-manual/web/serializedobject.html
//   - ※ScriptableObject ではない。
//   - SerializedObject 同様、編集対象の Object (WorldObject や Component など) をラップし、プロパティを操作する。
//   - AssetModel を派生して使うことはない。
// - 基本的な使い方も SerializedObject と同じく、update -> プロパティset -> apply がワンセット。
//   - apply で Undo スタックに変更履歴が詰まれる。これにより複数のプロパティをまとめてひとつの undo に入れることも可能。
class AssetModel
    : public Object
{
	LN_OBJECT;
public:
	static Ref<AssetModel> create(Object* target);

	static Ref<AssetModel> load(const ln::Path& filePath);
	static void save(Object* target, const ln::Path& filePath);

    Object* target() const;

	Ref<AssetProperty> findProperty(const String& path);

LN_CONSTRUCT_ACCESS:
    AssetModel();
    void init();
	void init(Object* target);

private:
	Ref<Object> m_target;
    Ref<AssetModel> m_parent;
    List<Ref<AssetModel>> m_children;
};

class AssetProperty
	: public Object
{
public:
    template<class TValue>
    void setValue(const TValue& value)
    {
        auto pair = m_property.resolve();
        if (pair.second) {
            if (auto* accessor = dynamic_cast<TypedPropertyAccessor<TValue>*>(pair.second)) {
                accessor->setValueDirect(pair.first, value);
            }
            //static_cast<TypedPropertyAccessor<TValue>*>(pair.second)->setValueDirect(pair.first, value);
        }
    }

    template<class TValue>
    TValue getValue()
    {
        TValue value;
        auto pair = m_property.resolve();
        if (pair.second) {
            static_cast<TypedPropertyAccessor<TValue>*>(pair.second)->getValueDirect(pair.first, &value);
        }
        else {
            LN_FATAL();
        }
        return value;
    }

LN_CONSTRUCT_ACCESS:
	AssetProperty();
	void init(const PropertyRef& prop);

private:
	PropertyRef m_property;
};

//class AssetSerializer
//{
//public:
//    static ln::String serialize(AssetModel* asset);
//    static Ref<AssetModel> deserialize(const String& str);
//
//private:
//    AssetSerializer();
//    void processAsset(AssetModel* asset);
//
//    //struct Ploxy
//    //{
//    //    AssetModel* asset;
//
//    //    LN_SERIALIZE_CLASS_VERSION(1);
//    //    void serialize(Archive& ar);
//    //};
//};

} // namespace ln
