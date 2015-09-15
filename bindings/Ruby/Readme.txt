※ struct のコンストラクタは引数省略可能。C#と同じくデフォルト値。


[2015/9/15]
拡張ライブラリを so 1本にまとめるには MinGW か cygwin で本体をビルドしなければならない。
ちょっと時間無いので2本体制にする。



[Windows]

・Ruby をインストールする/DEVELOPMENT KIT をダウンロードする
http://rubyinstaller.org/downloads/

・DEVELOPMENT KIT を初期化する
> ruby dk.rb init
> ruby dk.rb install
参考：http://qiita.com/tt_yamaguchi/items/c145e1759f9441c22feb


> set PATH=C:\ProgramFiles\DevKit-mingw64-32-4.7.2-20130224-1151-sfx\mingw\bin;C:\ProgramFiles\DevKit-mingw64-32-4.7.2-20130224-1151-sfx\bin;%PATH%
> ruby extconf.rb
※ PATH は前に追加しておく。そうしないと、mingw のものではない実行ファイルを参照してしまうことがある。




拡張ライブラリとリファレンスの生成は cygwin 上で行う。


・インストール
gem install yard

・ドキュメント作成
yardoc lnote_ruby_doc.rb
