Development Tips
==========

macOS
----------

```
brew install git-lfs
git lfs install

dotnet run -- BuildExternalProjects macOS
dotnet run -- BuildEngine_macOS
dotnet run -- MakeNativePackage
```

iOS
----------

```
dotnet run -- BuildExternalProjects iOS-OS64
dotnet run -- BuildExternalProjects iOS-SIMULATOR64
dotnet run -- BuildEngine_iOS
dotnet run -- MakeNativePackage
```


Android
----------

```
dotnet run -- BuildExternalProjects Android-x86_64
dotnet run -- BuildExternalProjects Android-x86
dotnet run -- BuildExternalProjects Android-arm64-v8a
dotnet run -- BuildExternalProjects Android-armeabi-v7a
dotnet run -- BuildEngine_Android
dotnet run -- MakeNativePackage
```


Web
----------

```
dotnet run -- BuildExternalProjects Emscripten
dotnet run -- BuildEngine_Emscripten
dotnet run -- MakeNativePackage
```

```
cp build/Emscripten/EngineInstall/lib/libLuminoEngine-static.a build/NativePackage/Engine/Emscripten/lib/libLuminoEngine-static.a
or
cp build/Emscripten/src/LuminoCore/libLuminoCore.a build/Emscripten/emsdk/upstream/emscripten/system/local/LuminoEngine/lib/libLuminoCore.a
cp build/Emscripten/src/LuminoEngine/libLuminoEngine-static.a build/Emscripten/emsdk/upstream/emscripten/system/local/LuminoEngine/lib/libLuminoEngine-static.a
```

```
python -m http.server 8000
```

----------


```
dotnet run -- BuildExternalProjects MSVC2019-x64-MT
dotnet run -- BuildEngine_MSVC MSVC2019-x64-MT
dotnet run -- MakeNativePackage
dotnet run -- MakeInstaller_Win32
```


### 起動できない場合…

Conosle:

```
(index):1 wasm streaming compile failed: TypeError: Failed to execute 'compile' on 'WebAssembly': Incorrect response MIME type. Expected 'application/wasm'.
```
↓こんなかんじの MIME type つけてあげる。ブラウザは Shift+F5 でリロード。
```
# -*- coding: utf-8 -*-
#test on python 3.4 ,python of lower version  has different module organization.
import http.server
from http.server import HTTPServer, BaseHTTPRequestHandler
import socketserver

PORT = 8000

Handler = http.server.SimpleHTTPRequestHandler
Handler.extensions_map={
    '.wasm': 'application/wasm',
    '.manifest': 'text/cache-manifest',
    '.html': 'text/html',
    '.png': 'image/png',
    '.jpg': 'image/jpg',
    '.svg':	'image/svg+xml',
    '.css':	'text/css',
    '.js': 'application/x-javascript',
    '': 'application/octet-stream', # Default
}
httpd = socketserver.TCPServer(("", PORT), Handler)

print("serving at port", PORT)
httpd.serve_forever()
```


```
(index):1 Cannot enlarge memory arrays to size 17014784 bytes (OOM). Either (1) compile with  -s TOTAL_MEMORY=X  with X higher than the current value 16777216, (2) compile with  -s ALLOW_MEMORY_GROWTH=1  which allows increasing the size at runtime, or (3) if you want malloc to return NULL (0) instead of this abort, compile with  -s ABORTING_MALLOC=0 
```
↓



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


PlantUML
----------

```
choco install -y openjdk
choco install -y graphviz
```

```
ext install plantuml
shd101wyy.markdown-preview-enhanced
```

プレビューは Alt+D.


開発中のパッケージを使う
----------

### Windows


```
set LUMINO_PATH=<path to LocalPackage>
set PATH=%LUMINO_PATH%\Tools;%PATH%
```
LUMINO_PATH

