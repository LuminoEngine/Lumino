
![](img/index-1.png)

環境を準備する
----------

Lumino は複数のプログラミング言語で利用することができます。

言語ごとの準備方法は次のリンクを参照してください。

[
<i class="glyphicon glyphicon-circle-arrow-right"></i>C++
](getting-started-windows.md)

[
<i class="glyphicon glyphicon-circle-arrow-right"></i>Ruby
](getting-started-ruby.md)








開発環境の準備を行う
--------
- [開発環境の準備を行う (Windows で開発を行う場合)](preparing-development-environment-windows.md)
- [開発環境の準備を行う (macOS で開発を行う場合)](preparing-development-environment-macos.md)


プロジェクトを作成する
--------
ターミナル（コマンドプロンプト）を開いて次のコマンドを入力します。

```sh
lumino-cli init HelloLumino
```

- `HelloLumino` の部分はプロジェクト名です。
- カレントディレクトリの下に HelloLumino というディレクトリが作成され、その中に必要なファイルが作成されます。

続いて作成されたディレクトリに移動しておきます。
```sh
cd HelloLumino
```

プロジェクトを実行する
--------
Lumino のプロジェクトディレクトリ(.lnproj ファイルのあるディレクトリ)で次のコマンドを入力します。

```sh
lumino-cli run
```

動作中のプラットフォーム向けのアプリがビルド・実行され、ウィンドウが表示されます。


その他のプラットフォーム固有の情報は以下のページを参照してください。
- [Windows アプリ](../platform-specific/windows.md)
- [macOS アプリ](../platform-specific/macos.md)
- [iOS アプリ](../platform-specific/ios.md)
- [Android アプリ](../platform-specific/android.md)
- [Web アプリ](../platform-specific/web.md)




