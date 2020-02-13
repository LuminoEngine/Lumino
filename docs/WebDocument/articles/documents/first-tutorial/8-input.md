入力を受け取る
==========

この章では、キーボード、マウス、ゲームパッドといた様々なデバイスからの入力を、統一された機能でアクセスする方法を学びます。

方向キーでオブジェクトを移動する
--------------------

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
        if (Input::isPressed(u"left")) {
            posX -= 0.1;
        }

        if (Input::isPressed(u"right")) {
            posX += 0.1;
        }

        if (Input::isPressed(u"up")) {
            posY += 0.1;
        }

        if (Input::isPressed(u"down")) {
            posY -= 0.1;
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









### 一般的なゲーム入力について

多くのコンシューマ機ではコントローラが1種類で、タイトルごとに「○ボタン」は攻撃、「□ボタン」防御、のように役割が決まっています。

しかしPCゲームのように様々なプラットフォームでの動作を想定する場合、キーボード、マウス、ゲームパッドやそれ以外のデバイスを接続されることを考えなければなりません。
またユーザーによってはゲームパッドで操作するよりもマウスとキーボードを組み合わせたプレイスタイルを好むこともあり、これらを全てカバーするようなプログラムを書くのは容易ではありません。

Lumino ではこのような様々なケースへの対応負担を軽減するため、入力を抽象化して扱う `Input` クラスを提供しています。





仮想ボタン
--------------------
入力を受け取るには Input クラスの機能を使用します。全ての入力デバイスへあらかじめ決められた名前でアクセスできます。

例えば次のコードは、"left" という名前で、「キーボードの ← キー」「ゲームパッドのスティックの ←」「ゲームパッドの ← ボタン」のいずれかが押されているかを判定します。

```cpp
if (Input::isPressed("left"))
{
    // 左ボタンが押されている
}
```

プログラム上では「左」に相当する入力がされているかだけを気をつければ良くなり、シンプルにコーディングできるようになります。

次の表は、初期状態の割り当て一覧です。

| 仮想ボタンの名前 | 定数                   | キーボード | マウス | ゲームパッド  |
|------------------|------------------------|------------|--------|---------------|
| left             | InputButtons::Left     | ← キー     | -      | POV左, 第1軸- |
| right            | InputButtons::Right    | → キー     | -      | POV右, 第1軸+ |
| up               | InputButtons::Up       | ↑ キー     | -      | POV上, 第2軸- |
| down             | InputButtons::Down     | ↓ キー     | -      | POV下, 第2軸+ |
| submit           | InputButtons::Submit   | Z キー     | -      | 1 番ボタン    |
| cancel           | InputButtons::Cancel   | X キー     | -      | 2 番ボタン    |
| menu             | InputButtons::Menu     | X キー     | -      | 3 番ボタン    |
| shift            | InputButtons::Shift    | Shift キー | -      | 4 番ボタン    |
| pageup           | InputButtons::PageUp   | Q キー     | -      | 5 番ボタン    |
| pagedown         | InputButtons::PageDown | W キー     | -      | 6 番ボタン    |
| any              | InputButtons::Any      | ※1         | ※1     | ※1            |

※1: 割り当てられている全てのボタンに対応します。

