

Side menu
----------
- Map List
- Scene List
- Prefab List


ジャンル共通
- Asset List (単純な Asset フォルダ以下のブラウザ・・・必要？)
- Audio List
- Plugin List
- Setting


### Map List と Scene List は区別する必要ある？
Map は Scene の Chunk.
とはいえそのフラグが違うだけで、ジャンルとしての Scene の役目で分類しているに過ぎない。
なので一緒にしてしまってもいい。
ただ、中身は一緒でも役目が違うので分けておくと整理しやすい。
その辺の自由度高くするのはほんとにわかりやすさとのトレードオフ。
というか、そいういう自由度欲しければGameMakerStudioとか使えばいい気がする。



Qt vs Imgui
----------

- Both
	- リストとツリービューのフィルタリングと並べ替え
	- コピーアンドペースト
	- Multiple selection
	- ドラッグアンドドロップ
	- アイコン (SVG -> Nanosvg)
	- 高DPI
	- スタイリング

- Qt
	- Undo/Redo (自前実装は難しくはない)
	- Animation (Imgui では難しい、とのこと)
	- Unicode
	- コモンダイアログ
	- ファイルブラウザ (TreeView)
	- DockWidget
	- ショートカットキー（Imgui は実装中：https://github.com/ocornut/imgui/issues/456）

https://blog.johnnovak.net/2016/05/29/cross-platform-gui-trainwreck-2016-edition/
https://www.reddit.com/r/programming/comments/5zrqcn/why_qt_and_not_imgui/

