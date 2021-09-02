#include <Lumino.hpp>

class ObjectClassBasicExample : public Application
{
    void onInit() override
    {
        // Object クラスは、Lumino が公開しているほとんどすべてのクラスの基本クラスです。
        // 全ての Object クラスの派生クラスは、makeObject 関数によってインスタンスを作成できます。
        // ちょうど C++ の std::make_shared と同じように動作し、作成されたインスタンスを指すスマートポインタを返します。
        Ref<Material> material1 = makeObject<Material>();
        material1->setColor(Color::Red);
        

        // 一部のよく使われるクラスは、Builder パターンによるインスタンスの作成をサポートしています。
        Ref<Material> material2 = Material::With()
            .color(Color::Red)      // Material の color プロパティを指定する
            .build();               // Material のインスタンスを作成して返す


        // "0-Hello" のプログラムは、Builder を使わずに書くと次のようになります。
        auto text1 = makeObject<UIText>();
        text1->setText(u"Hello, Lumino!");
        text1->addInto();           // デフォルトのビューへ追加する


        // 3D や GUI のオブジェクトは addInto() を使いシーン (World や Window) に追加することで画面上に表示されます。
        // Builder パターンを使う場合、インスタンスの作成とシーンへの追加を同時に行うことができます。
        // World や Window については Graphcis や GUI のサンプルを参照してください。
        auto text2 = UIText::With()
            .text(u"Hello, Lumino!")
            .buildInto();
    }
};

LUMINO_APP(ObjectClassBasicExample);
