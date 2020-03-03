Lumino の基本
==========

この章では、Lumino を使ってアプリケーションを作成するための最も基本的な流れについて学びます。

最小のプログラム
----------

「最初のプログラム」で見たように、Lumino でアプリを開発するには Application クラスの実装から始めます。

空のウィンドウを表示するだけの最小限のプログラムは、次のようになります。

# [C++](#tab/lang-cpp)

```cpp
#include <Lumino.hpp>

class App : public Application
{
};

LUMINO_APP(App);
```

# [Ruby](#tab/lang-ruby)

```ruby
require "lumino"

class App < Application
end

App.new.run
```

---

実行して、ウィンドウを表示してみましょう。

![](img/basic-1.png)

この時点でできることは、クローズボタンなどでウィンドウを閉じるだけです。

文字を表示したり、ユーザー入力を受けてインタラクションを実現するためにはこの App クラスにいくつかのメソッドを実装する必要があります。


初期化と更新
----------

小さな Lumino アプリケーションを作成するための基本的なタスクは次の2つです。

- プログラムの開始時に変数を初期化する
- プログラムが動き出したら、繰り返し変数を変更する

これらを行うために、次のように 2 つのメソッド定義を追加します。

# [C++](#tab/lang-cpp)

```cpp
#include <Lumino.hpp>

class App : public Application
{
    void onInit() override
    {
    }

    void onUpdate() override
    {
    }
};

LUMINO_APP(App);
```

# [Ruby](#tab/lang-ruby)

```ruby
require "lumino"

class App < Application
  def on_init
  end

  def on_update
  end
end

App.new.run
```

---

空のメソッドを追加しただけなので、実行するとウィンドウは表示できますが、動きは変わりません。

この後のチュートリアルやサンプルで紹介するプログラムでは、プログラムの開始時に1回呼び出される onInit() メソッドで変数を初期化し、onUpdate() メソッドでこれらの変数を変更して、アプリケーションを実装していくことになります。

> [!Note]
> onUpdate() は 1秒間に 60 回、繰り返し実行されます。
> この 1 回分の実行単位を `フレーム` と呼び、「1 秒間は 60 フレーム」といったように使います。


Hello, Lumino!
----------

ウィンドウに文字列を表示してみましょう。

テキストや数値を画面に表示するには、Lumino の デバッグようの機能である、Debug クラスの print() メソッドを使うと簡単にできます。

# [C++](#tab/lang-cpp)

```cpp
#include <Lumino.hpp>

class App : public Application
{
    void onInit() override
    {
        Debug::print(u"Hello, Lumino!");
    }

    void onUpdate() override
    {
    }
};

LUMINO_APP(App);
```

# [Ruby](#tab/lang-ruby)

```ruby
require "lumino"

class App < Application
  def on_init
    Debug.print("Hello, Lumino!")
  end

  def on_update
  end
end

App.new.run
```

---

文字列がウィンドウ上に表示されます。（その後、しばらくすると消えます）

![](img/basic-2.png)


文字列を表示し続ける
----------

次は onUpdate で文字列を表示してみます。

# [C++](#tab/lang-cpp)
```cpp
#include <Lumino.hpp>

class App : public Application
{
    void onInit() override
    {
    }

    void onUpdate() override
    {
        Debug::print(String::format(u"Time: {0}", Engine::time()));
    }
};

LUMINO_APP(App);
```
# [Ruby](#tab/lang-ruby)
```ruby
require "lumino"

class App < Application
  def on_init
  end

  def on_update
    Debug.print("Time: %f" % Engine.time)
  end
end

App.new.run
```
---

`Engine::time()` は アプリケーションの起動からの経過時間を返します。これを利用して、onUpdate() がどのくらいの頻度で実行されているのかを確認してみます。

![](img/basic-3.png)

実行してみると、画面からあふれるほどのテキストが表示されてしまいました。

繰り返し実行されているのはわかりましたが、今は過去の情報は不要です。

`Debug::print()` は第一引数に数値を指定することで、テキストの表示時間をコントロールできます。次のように 0 を指定することで、テキストは 1 フレームの間だけ表示されるようになります。

# [C++](#tab/lang-cpp)
```cpp
#include <Lumino.hpp>

class App : public Application
{
    void onInit() override
    {
    }

    void onUpdate() override
    {
        Debug::print(0, String::format(u"Time: {0}", Engine::time()));
    }
};

LUMINO_APP(App);
```
# [Ruby](#tab/lang-ruby)
```ruby
require "lumino"

class App < Application
  def on_init
  end

  def on_update
    Debug.print(0, "Time: %f" % Engine.time)
  end
end

App.new.run
```
---

修正したら、実行してみましょう。

![](img/basic-4.gif)

シンプルなタイマーができました！

テキストは 1 秒間に 60 回、表示と消去を繰り返すことで、リアルタイムに変化しているように見えます。
