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

```
gem install lumino
```

プロジェクトを作成する
----------

任意のディレクトリで、次のコマンドを実行します。

```
lumino-rb new hello
```

`hello` というディレクトリが作成され、その中に `main.rb` というファイルが作成されます。

最初のプログラム
----------

```ruby
require "lumino"

class App < Application
  def on_init
    text = UITextBlock.new("Hello, Lumino!")
  end
end

App.new.run
```

![](img/first-program.png)

> [!Note]
> Lumino はソースファイルだけではなくアセットの管理やデプロイのサポートなど、ゲームを開発するための一連の作業を `プロジェクト` という単位で管理します。
>
> Lumino を使うだけであれば `lumino-rb` コマンドのステップを省略して手動でソースファイルを作成することも可能ですが、作品の配布まで考える場合はプロジェクトの作成を推奨します。

