Ruby で Lumino をはじめる
==========

必要な環境
----------

> [!WARNING]
> 現在 Ruby パッケージは Windows でのみ利用できます。

### Windows

[RubyInstaller] (https://rubyinstaller.org/) などで Ruby をインストールしてください。

推奨バージョンは 2.6 以降です。


インストール
----------

> [!WARNING]
> 安定版はまだリリースされていません。

[ダウンロードページの「現在開発中の最新ビルド」](download.md#現在開発中の最新ビルド) から「LuminoEngine-RubyGemPackage.zip」をダウンロードします。

```
gem install lumino --pre
```


最初のプログラム
----------

```ruby
require 'lumino'
include Lumino

Engine.initialize

texture1 = Texture2D.load("logo.png")
sprite1 = Sprite.new(texture1, 2, 2)

while Engine.update do
end

Engine.finalize
```




