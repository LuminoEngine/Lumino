ビルド方法
========

必要なパッケージをインストールする
--------
### Windows

#### Visual Studio
Visual Studio 2019 をインストールしてください。

また、Visual Studio Installer では次のワークロードをインストールしてください。
* C++ によるデスクトップ開発
* .NET Core クロスプラットフォームの開発

#### Chocolatey

Chocolatey をインストールしてください。
― [Chocolatey installation](https://chocolatey.org/docs/installation)

続いて Windows PowerShell(管理者) で各パッケージをインストールします。 
- [Win + X] > [Windows PowerShell(管理者)]

```
choco install -y git
choco install -y cmake --installargs 'ADD_CMAKE_TO_PATH=System'
choco install -y nuget.commandline
choco install -y docfx
choco install -y doxygen.install
choco install -y python3
choco install -y wixtoolset
choco install -y vulkan-sdk
choco install -y mingw
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

Windows (MSVC):
```
dotnet run -- BuildLocalPackage MSVC2019-x64-MT
```

macOS:
```
dotnet run -- BuildLocalPackage macOS
```

Visual Studio や Xcode のプロジェクトファイルは `build/ターゲット名` フォルダ内に作られます。これを開いて、各 IDE で開発できます。

また、上記コマンドは特定ターゲットのみのビルドを行います。Web や Android, iOS ターゲットなども含めてフルビルドする場合は次のコマンドを使用します。

```
dotnet run
```

なお、このコマンドは Lumino 本体のビルド、ドキュメントのビルド、リリースパッケージの作成までの一連のタスクを実行します。

より細かい単位でタスクを実行したい場合は次のコマンドを使用します。

```
dotnet run -- <タスク名>
```

<タスク名> は以下のフォルダ内の .cs ファイルに定義されているクラス名です。
- tools/LuminoBuild/Rules
- tools/LuminoBuild/Tasks

