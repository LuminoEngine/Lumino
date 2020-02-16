入力を受け取る
==========

この章では、キーボード、マウス、ゲームパッドといた様々なデバイスからの入力を、透過的に扱う方法を学びます。

方向キーでオブジェクトを移動する
--------------------

ボタンが押されているかどうか、といった入力を確認するには、 `Input` を使います。

次のプログラムは、キーボードの上下左右キーで Box を移動します。また、Z キーで位置をリセットします。

# [C++](#tab/lang-cpp)
```cpp
#include <Lumino.hpp>

class App : public Application
{
    Ref<BoxMesh> box;
    float posX = 0.0f;
    float posY = 0.0f;

    virtual void onInit() override
    {
        box = BoxMesh::create();
    }

    virtual void onUpdate() override
    {
        if (Input::pressed(u"left")) {
            posX -= 0.1;
        }

        if (Input::pressed(u"right")) {
            posX += 0.1;
        }

        if (Input::pressed(u"up")) {
            posY += 0.1;
        }

        if (Input::pressed(u"down")) {
            posY -= 0.1;
        }

        // Z key, reset position
        if (Input::triggered(u"submit")) {
            posX = 0;
            posY = 0;
        }

        box->setPosition(posX, posY, 0);
    }
};

LUMINO_APP(App);
```
# [Ruby](#tab/lang-ruby)
```ruby
```
---


![](img/input-1.gif)

`pressed` はキーが押されている間は true を返し、 `triggered` はキーが押された瞬間だけ true を返します。（これ以外にも離された瞬間を判定する `triggeredOff`、押している間は定期的に true を返す `repeated` が定義されています）

さて、それぞれ引数には `left` や `up` のように判定したいキーの名前を指定していますが、`submit` とはなんでしょうか。


仮想ボタンという考え方
----------

多くのコンシューマ機ではコントローラが1種類で、タイトルごとに「○ボタンは決定や攻撃」、「×ボタンはキャンセルやジャンプ」、のように役割が決まっています。

しかし様々なプラットフォームでの動作を想定する場合、キーボード、マウス、ゲームパッドやそれ以外のデバイスを接続されることを考えなければなりません。

またユーザーによってはゲームパッドで操作するよりもマウスとキーボードを組み合わせたプレイスタイルを好むこともあり、使いやすさのためにキーコンフィグを実装する必要も出てきます。

これらを全てカバーするようなプログラムを書くことは容易ではありませんが、Lumino ではこのような様々なケースへの対応負担を軽減するため、入力を抽象化して扱う機能を提供しています。


Input の動きを確認する
----------

`Input` では、 "キーボードの Z キー" 、や "ゲームパッドの1ボタン" といった具体的な物理ボタンを指定するのではなく、`ボタンの役割` を指定することで入力を判定します。

別途、この役割に物理ボタンを割り当てることで、入力を透過的に扱います。

例えば、先ほどの `submit` は「決定ボタン」を意味し、デフォルトでは `Zキー` と `Enterキー`、そして `ゲームパッドの1番ボタン` が割り当てられています。

次の表は、初期状態の割り当て一覧です。

| 名前 | キーボード | マウス | ゲームパッド  |
|------------------|------------|--------|---------------|
| left             | ← | -      | POV左, 第1軸- |
| right            | → | -      | POV右, 第1軸+ |
| up               | ↑ | -      | POV上, 第2軸- |
| down             | ↓ | -      | POV下, 第2軸+ |
| submit           | Z, Enter     | -      | 1 番ボタン    |
| cancel           | X, Esc | -      | 2 番ボタン    |
| menu             | X, Esc | -      | 3 番ボタン    |
| shift            | Shift | -      | 4 番ボタン    |
| pageup           | Q | -      | 5 番ボタン    |
| pagedown         | W | -      | 6 番ボタン    |
| any              | ※1 | ※1     | ※1            |

※1: 割り当てられている全てのボタンに対応します。

次のプログラムで、押されたボタンに対応する文字列が表示されることを確認してみましょう。

# [C++](#tab/lang-cpp)
```cpp
#include <Lumino.hpp>

class App : public Application
{
	virtual void onUpdate() override
	{
		if (Input::pressed(u"left")) Debug::print(0, u"left");
		if (Input::pressed(u"right")) Debug::print(0, u"right");
		if (Input::pressed(u"up")) Debug::print(0, u"up");
		if (Input::pressed(u"down")) Debug::print(0, u"down");
		if (Input::pressed(u"submit")) Debug::print(0, u"submit");
		if (Input::pressed(u"cancel")) Debug::print(0, u"cancel");
		if (Input::pressed(u"menu")) Debug::print(0, u"menu");
		if (Input::pressed(u"shift")) Debug::print(0, u"shift");
		if (Input::pressed(u"pageup")) Debug::print(0, u"pageup");
		if (Input::pressed(u"pagedown")) Debug::print(0, u"pagedown");
		if (Input::pressed(u"any")) Debug::print(0, u"any");
	}
};

LUMINO_APP(App);
```
# [Ruby](#tab/lang-ruby)
```ruby
```
---

![](img/input-2.gif)


ボタンの割り当てを変更する
----------

TODO:

