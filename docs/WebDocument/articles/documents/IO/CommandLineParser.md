コマンドライン引数の解析
==========

コマンドライン引数は通常、文字列のリストとしてアプリケーションに渡されますが、これを解析してアプリケーションとして意味のある値を取り出したり、ユーザーにとってわかりやすい使い方を示すことは容易ではありません。

CommandLineParser は一連のオプションを定義し、コマンドライン引数を解析し、オプションの値を簡単に取得するためのインターフェイスを提供します。


クイックスタート
----------
次のプログラムは、オプションと位置引数を使用する例です。

コピー元とコピー先2つのファイルパスを受け取り、ファイルコピーを行います。必要に応じて上書きするオプションを指定します。

```cpp
int main(int argc, char** argv)
{
    CommandLineParser parser;
    parser.addHelpOption();

    auto* forceOpt = parser.addFlagOption(u"f", u"force", u"Overwrite file.");
    auto* sourceArg = parser.addPositionalArgument(u"SOURCE", u"source filename.");
    auto* destArg = parser.addPositionalArgument(u"DEST", u"destination filename.");

    if (parser.process(argc, argv))
    {
        FileSystem::copyFile(sourceArg->value(), destArg->value(), parser.has(forceOpt));
        return 0;
    }
    else
    {
        return 1;
    }
}
```

"--help" や "-h" オプションを指定することで、コマンドの使い方が表示されます。

```
$ MyApp --help
usage:
  MyApp [options...] SOURCE DEST

options:
  -h, --help    Display this help.
  -f, --force   Overwrite file.

arguments:
  SOURCE        source filename.
  DEST          destination filename.
```

このアプリケーションを使うには次のようなコマンドを入力します。

```bash
# ファイル file1.txt を file2.txt へ上書きコピーする
$ MyApp -f file1.txt file2.txt
```

使い方の流れ
----------

1. CommandLineParser のインスタンスへ基本的な情報を設定する
2. コマンドライン引数の情報を設定する
3. コマンドライン引数の解析を行う
4. 2 で設定したコマンドライン引数情報から値を取り出す

### 1. CommandLineParser のインスタンスへ基本的な情報を設定する

必要に応じて、コマンドラインアプリケーションとして一般的に使用される機能の設定を行います。

```cpp
CommandLineParser parser;

// "-h" と "--help" オプションでヘルプを表示できるようにする
parser.addHelpOption();

// "-v" と "--version" オプションでバージョンを表示できるようにする
parser.addVersionOption(u"1.2.0");

// ヘルプにアプリケーションの説明を表示する
parser.setApplicationDescription(u"sample application.");
```

### 2. コマンドライン引数の情報を設定する

CommandLineParser::addFlagOption() や CommandLineParser::addPositionalArgument() を使用して、CommandLineParser のインスタンスに引数情報を登録します。

```cpp
CommandLineOption* forceOpt = parser.addFlagOption(u"f", u"force", u"Overwrite file.");
CommandLinePositionalArgument* sourceArg = parser.addPositionalArgument(u"SOURCE", u"source filename.");
```

### 3. コマンドライン引数の解析を行う

CommandLineParser::process() は main() などから受け取ったコマンドライン引数をそのまま渡して解析できます。

また、引数として --help など手順 1 で設定したビルトインオプションが入力されていれば標準出力します。

解析の結果、値を取り出すことができる場合、CommandLineParser::process() は true を返します。

```cpp
if (parser.process(argc, argv))
{
    // 解析成功。値を取り出すことができる。
}
else if (!parser.hasError())
{
    // --help などを表示した。解析失敗ではないが、値を取り出すことはできない。
    return 0;
}
else
{
    // 解析失敗
    return 1;
}
```


### 4. 2 で設定したコマンドライン引数情報から値を取り出す

オプションの有無を確認するには CommandLineParser::has() を使います。

値を取得するには、CommandLineOption や CommandLinePositionalArgument クラスの value() を使います。
値は文字列として取得できます。

```cpp
if (parser.process(argc, argv))
{
    if (parser.has(forceOpt))
    {
        // --force が指定されている
    }

    // 値を取り出す
    String source = sourceArg->value();
}
```
