このフォルダの fmt ライブラリは、 Lumino を他のアプリに組み込んだ時の ODR 違反などを回避するための対策が施されたものです。

## アップデート手順

1. https://github.com/fmtlib/fmt の目的のバージョンをクローンする
2. include フォルダにある当該のファイルをコピーしてくる
3. マクロ FMT_* を LN_FMT_* に変更する。
4. namespace の調整などを Format.hpp, Format.cpp で行う。

