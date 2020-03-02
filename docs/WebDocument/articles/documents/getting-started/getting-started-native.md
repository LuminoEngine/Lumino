C++ で Lumino をはじめる
==========

> [!Note]
> このページでは Windows で開発を始める手順を説明しています。macOS についてはまだ書かれていません。

開発環境の準備を行う
--------

### Visual Studio をインストールする

1. [Visual Studio の Web ページ](https://visualstudio.microsoft.com/ja/downloads/) から Visual Studio 2019 Community をダウンロードし、インストールしてください。
2. Visual Studio インストーラーを実行して C++ 開発ツール（C++ によるデスクトップ開発）をインストールします。
![](img/IntroductionAndGettingStarted-1.png)


### Lumino をインストールする

1. [Lumino のインストーラ](http://nnmy.sakura.ne.jp/archive/lumino/v0.9.0/Lumino-v0.9.0-Windows.msi) をダウンロードし、実行してください。インストーラの指示に従ってインストールを行います。
2. 実行時に Windows SmartScreen の注意メッセージが表示された場合は「詳細情報」→「実行」を押してください。


プロジェクトを作成する
--------
ターミナル（コマンドプロンプト）を開いて次のコマンドを入力します。

```sh
lumino init hello
```

カレントディレクトリの下に `hello` というフォルダが作成され、その中に必要なファイルが作成されます。

プロジェクトを実行する
--------

`hello` フォルダの中には Visual Studio のソリューションファイル(.sln) が作成されています。

このファイルを Visual Studio で開くことで、Visual Studio 上で開発することができます。

プロジェクトの中に作成されている `src/App.cpp` が Lumino のエントリーポイントです。ここには次のようなプログラムが書かれています。

```cpp
#include <Lumino.hpp>

class HelloApp : public Application
{
    void onStart() override
    {
        auto text = UITextBlock::create(u"Hello, Lumino!");
    }

    void onUpdate() override
    {
    }
};

LUMINO_APP(HelloApp);
```

プログラムの実行は、[デバッグ] メニューの [デバッグの開始] から行います。

![](img/first-program.png)

中央に "Hello, Lumino!" と書かれたウィンドウが表示されましたか？

これで Lumino を使うための準備が整いました。次は [チュートリアル](../first-tutorial/1-basic.md) に進みましょう！


