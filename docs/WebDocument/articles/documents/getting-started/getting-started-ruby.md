Ruby で Lumino をはじめる
==========

必要な環境
----------

- Windows 10 (64bit)
- Ruby 2.6+ (64bit)

Ruby環境をセットアップする
----------

### Windows

1. [RubyInstaller ダウンロードページ] (https://rubyinstaller.org/) にアクセスして、最新のRuby + Devkit 64ビットリリース（x64名前に含まれているもの）をダウンロードします。

2. インストーラーを実行します。インストールが完了したら、「Run 'ridk install'」オプションを選択したままにして、MSYS2と開発ツールチェーンをインストールします。

3. コマンドラインで `ruby --version` を実行して、Ruby のバージョンが表示されることを確認します。


gem をインストールする
----------

コマンドラインで次のコマンドを実行します。

```sh
gem install lumino
```

プロジェクトを作成する
----------

任意のディレクトリで、次のコマンドを実行します。

```sh
lumino-rb new hello
```

`hello` というディレクトリが作成され、その中に `main.rb` というファイルが作成されます。

最初のプログラム
----------

`main.rb` には次のようなプログラムが書かれています。

```ruby
require "lumino"

class App < Application
  def on_init
    text = UITextBlock.new("Hello, Lumino!")
  end
end

App.new.run
```

このまま、次のコマンドで実行してみましょう。

```sh
ruby main.rb
```

![](img/first-program.png)

中央に "Hello, Lumino!" と書かれたウィンドウが表示されましたか？

これで Lumino を使うための準備が整いました。次は [チュートリアル](../first-tutorial/1-basic.md) に進みましょう！

> [!Note]
> Lumino はソースファイルだけではなくアセットの管理やデプロイのサポートなど、ゲームを開発するための一連の作業を `プロジェクト` という単位で管理します。
>
> Lumino を使うだけであれば `lumino-rb` コマンドのステップを省略して手動でソースファイルを作成することも可能ですが、作品の配布まで考える場合はプロジェクトの作成を推奨します。

