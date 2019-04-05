Development Tips
==========

Windows
----------

```
dotnet run -- MakeVSProjects
```

macOS
----------
AppData は ~/.config/Lumino/

textlint
----------

VSCode に拡張機能をインストールする。コマンドパレット から、

```
ext install vscode-textlint
```

textlint をインストールする (-g は必須) 。ターミナルから、

```
npm i -g textlint textlint-rule-preset-ja-technical-writing textlint-rule-max-ten textlint-rule-spellcheck-tech-word textlint-rule-no-mix-dearu-desumasu
```

.textlintrc ファイルを作る。(リポジトリルートに置いてあります)

### ルールについて
* textlint-rule-preset-ja-technical-writing
    * 日本語の技術文書用のルール
* textlint-rule-max-ten
    * 一文に利用できる、の数をチェックするルール
* textlint-rule-spellcheck-tech-word
    * [azu/technical-word-rules](https://github.com/azu/technical-word-rules) の辞書で単語チェック
* textlint-rule-no-mix-dearu-desumasu
    * 「ですます」調と「である」調の混在をチェックするルール



開発中のパッケージを使う
----------

### Windows
```
set LUMINO_PATH=<path to LocalPackage>
set PATH=%LUMINO_PATH%\Tools;%PATH%
```


