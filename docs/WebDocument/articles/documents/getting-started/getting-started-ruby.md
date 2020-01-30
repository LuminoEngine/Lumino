Ruby で Lumino をはじめる
==========

必要な環境
----------

- Windows 10 64bit
- Ruby 2.6+

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


最初のプログラム
----------

```ruby
require 'lumino'

class App < Application
  def on_init
    texture = Texture2D.load_emoji("🌱")
    sprite1 = Sprite.new(texture)
    self.world.add(sprite1)
  end
end

App.new.run
```

