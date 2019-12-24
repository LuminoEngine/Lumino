#pragma once
#include <LuminoEditor/Plugin.hpp>
#include "../AssetEditor/AssetEditor.hpp"

/*
    タイルマップをシーンエディタといい感じに協調して編集したいけどどうする？
    ----------
    Sub-Scene の考え方で行ってみよう。
    Scene はプレハブを使わない場合、巨大なオブジェクトツリーのデータとなるとなる。
    
    プレハブを使う場合はその一部分を別の .lnasset ファイルに追い出せ、別の EditorView で編集できる。
    
    しかし、プレハブ化しない場合は View を編集中の SceneEditor の子 Editor として扱わなけれればならない。
    なぜなら、Scene を表す AssetObject と Tilemap を表す AssetObject を分けることはできないから。
    （プレハブ化すればいいんだけど…）
    プレハブ化すると、Scene 用と Tilemap 用で 2 つのタブを開く必要があり、ちょっと煩わしい。
    データの表現としては正しいんだろうけど、そうと理解しないと最初とっつきにくい。
    
    なので、SceneEditor というひとつの View の中に、別の View を Eclipse のパースペクティブ切り替えみたいに内包できるようにしたい。
    → WorldObject の編集は基本的に右側のインスペクタで行うが、Tilemap のようにインスペクタでは足りないものは別 View を開く、みたいな。

    [2019/8/12]
    そうすると、そういった Object 編集の Ext の位置づけは、任意の Editor に対する拡張、ということになる。
    SceneEditor を拡張する TilemapEditor とか、あるいは Scene でなくても、Database のデフォルトの Editor を拡張するものとかも考えられる。
    これ系の拡張は、何らかの Editor を開いたら Manager から検索して、その Editor にアタッチして、あとは必要なタイミングで動いてもらうみたいな動きになる。
*/
class SceneEditor : public lna::AssetEditor
{
public:
    virtual void onOpened(ln::AssetModel* asset, ln::UIContainerElement* frame) override;
    virtual void onClosed() override;
};

class SceneEditorPlugin : public ln::IAssetEditorExtension
{
public:
    SceneEditorPlugin();
    virtual const ln::Char* id() const { return u"7B7D6551-3105-4877-99C4-D4EE8EA06482"; }
    virtual const ln::Char* displayName() const { return u"SceneEditorPlugin"; }
    virtual const ln::Char* typeKeyword() const { return u"Scene"; }
    virtual ln::Ref<lna::AssetEditor> createEditor() override;

private:
};
