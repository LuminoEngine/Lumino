#pragma once
#include <LuminoCore/Runtime/Object.hpp>
#include <LuminoCore/Runtime/Property.hpp>
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
// - Target がプロパティとして持っている Object は、基本的に別 Asset を示す GUID で保存される。
//   - 別 Asset ファイルに保存したくない場合は、各 Object の serialize を実装する。（RigidBody の Shapes とかはこれになる）
//   - というか、C++ でリフレクションは相当厳しいので、まずは基本方針 serialize 実装かな。ちなみに Urho3D は Save/Load で行っている。
/*
    [2019/8/18] 保存について
    ----------
    AssetModel では保存まで面倒見ないほうがいいかも。
    このあたりのやり方は Urho3D あたりが参考になるが、ひとまず泥臭くいくしかないかなというところ。

    なので、ランタイム実行時にシーンロードするときなど、load のためだけに一時的な AssetModel をたくさん作る必要はない。
    （ルートだけは必要。IDとかをシリアライズする必要があるので）

    現時点では loadInternal とか持っているが、後で消す。
*/
LN_CLASS()
class AssetModel
    : public Object
{
	LN_OBJECT;
public:
    static const String AssetFileExtension; // ".lnasset"
    static const String EngineAssetsDirectory;

    // TODO: テスト用の一時機能。Asset の作成は AssetDatabase を使わなけばならない
	//static Ref<AssetModel> create(Object* target);

	//static Ref<AssetModel> load(const ln::Path& filePath);
	//static void save(Object* target, const ln::Path& filePath);

    const ln::TypeInfo* assetType() const;

    /** target */
    LN_METHOD()
    Object* target() const;

    template<class T>
    T* targetAs() const { return dynamic_cast<T*>(target()); }  // TODO: できれば TypeInfo で型チェック

    Path assetFilePath() const { return  m_assetFilePath; }

	Ref<AssetProperty> findProperty(const String& path);

    void addChild(AssetModel* model);
    void removeChild(AssetModel* model);
    const List<Ref<AssetModel>> children() const { return m_children; }

    static ln::Uuid readAssetId(const ln::Path& filePath);

    // TODO: internal
    bool m_externalObjectDeserialization = false;

protected:
    void onSerialize_deprecated(Serializer2_deprecated* sr) override;

LN_CONSTRUCT_ACCESS:
    AssetModel();

    /** init */
    void init();

    // TODO: テスト用の一時機能。Asset の作成は AssetDatabase を使わなけばならない
    /** init */
    LN_METHOD()
	void init(Object* target);

private:
    //ln::Result<> loadInternal(const ln::Path& filePath);
    //ln::Result<> saveInternal(const ln::Path& filePath);

    //LN_SERIALIZE_CLASS_VERSION(1);
    //void serialize(Archive& ar);


    //String m_assetType;
	Ref<Object> m_target;
    Ref<AssetModel> m_parent;
    List<Ref<AssetModel>> m_children;
    Path m_assetFilePath;
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

} // namespace ln
