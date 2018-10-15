ビルド方法
========


必要なパッケージをインストールする
--------
### Windows

#### Visual Studio
Visual Studio 2017 をインストールしてください。

#### Chocolatey
Chocolatey をインストールしてください。
― [Chocolatey installation](https://chocolatey.org/docs/installation)

続いて Windows PowerShell(管理者) で各パッケージをインストールします。 
- [Win + X] > [Windows PowerShell(管理者)]

```
choco install cmake
choco install nuget.commandline
choco install docfx
choco install doxygen.portable
choco install python3 --pre
choco install wixtoolset
```

### macOS
[dotnet](https://docs.microsoft.com/ja-jp/dotnet/core/macos-prerequisites?tabs=netcore2x) をインストールしてください。

その他のパッケージは homebrew でインストールします。
```
brew install git cmake doxygen docfx openssl python3
```


ビルドする
--------
Lumino のルートフォルダ (build.csproj のあるフォルダ) で次のコマンドを実行してください。

```
dotnet run
```

このコマンドは Lumino 本体のビルド、ドキュメントのビルド、リリースパッケージの作成までの一連のタスクを実行します。

より細かい単位でタスクを実行したい場合は次のコマンドを使用します。

```
dotnet run -- <タスク名>
```

<タスク名> は以下のフォルダ内の .cs ファイルに定義されているクラス名です。
- tools/LuminoBuild/Rules
- tools/LuminoBuild/Tasks


TIPS
--------
Lumino の開発を行う場合、ビルド後、build/<ターゲット名> フォルダ内に生成されている各 IDE 用のプロジェクトを開いてコーディングを行うとスムーズに開発できます。その場合は最初に `dotnet run` を実行してパッケージ一式をビルドしてください。

例えば、Visual Studio のプロジェクト (ソリューションファイル) は build/MSVC2017-x86-MT/Lumino.sln などに生成されます。


After this, please check in the "build/..." folder.


emscripten

emsdk Windows installer
http://kripken.github.io/emscripten-site/docs/getting_started/downloads.html

https://gist.github.com/faithandbrave/9b3d439d135e63abdbe7
