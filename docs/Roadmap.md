Roadmap
====================

1.0.0 の後
--------------------

- ランタイムで様々なアセットを動的生成する機能
- 動画再生
- 各プラットフォームへの正式対応
- GUI 機能の強化
- 各種通信 (HTTP, TCP/UDP, UART)
- 各種デバイス対応 (マルチタッチ, ペンタブ, カメラ, AR/VR)
- 各種エフェクト (パーティクル,フリップアニメーション)


1.0.0
--------------------

0.10.0 までで実装した機能を正式に仕上げていく。

### Graphics
- シーンレンダリング
- 各種ポストエフェクト
- glTF/VRM 対応
- SkinnedMesh
- GUI

### Audio
- ゲーム用サウンドユーティリティ
- オーディオノードグラフ

### Input
- タッチ、マウス、キーボード、ゲームパッド
- ゲーム用キーコンフィグ

### その他
- 3D 物理演算
- アセット管理
- シーン遷移管理
- 描画速度の改善

0.10.0
--------------------
3Dゲーム開発に最低限必要な機能をトライアルできるレベルまで実装する。

### Rendering module
- Material
- RenderingContext
- CommandBuffer
- SceneRenderer
- RenderFeature (BasicShape, Text, Sprite...)
- ImageEffect

### Graphics module
- DirectX driver

### Input module
- Input
- InputGesture

### Language bindings
- HSP3


0.9.0
--------------------
複数プログラミング言語対応のためのトランスコンパイラを作成する。

- ~~C#(.NET)~~
- ~~Python~~
- Ruby


0.8.0
--------------------
LowLevel なグラフィックス機能を実装する。
まだ使いやすいとは言いづらいけど描画の基本機能を完成させる。

### Graphics module
- SwapChain
- VertexBuffer
- VertexDeclaration
- IndexBuffer
- Texture2D
- ~~Texture3D~~
- RenderTargetTexture
- RenderingThread
- GraphicsContext
- GeometryStructs
- Bitmap
- Vulkan driver

0.7.0
--------------------
HLSL を入力としたシェーダのトランスコンパイラを作成する。

### Graphics module
- ~~DirectX driver~~

### Shader module
- Shader


0.6.0
--------------------
コマンドラインツールでを使って、各プラットフォームで動作する C++ プロジェクトを作れるようにする。
動作としてはビューポート全体のクリアまで。

### Platform
- Html5
- Android
- iOS
- Windows
- Mac
- ~~Linux~~

### Framework
- Application

### UI
- UIMainWindow

### Graphics module
- OpenGL driver

### Tool
- lumino (command line tool)


0.5.0
--------------------
Core ライブラリの主要機能の移行作業。

### Base module
- Assertion
- Logger
- String
- CRC hash
- Typesafe string formatting
- Enum flags
- Environment
- List
- Locale
- RefObject
- ElapsedTimer
- Event
- Optional
- Result

### Text module
- TextEncoding
- ASCIIEncoding
- UTF8Encoding
- UTF16Encoding
- UTF32Encoding
- Win32CodePageEncoding

### IO module
- Path
- FileSystem
- FileStream
- MemoryStream
- TextReader
- StreamReader
- StringReader
- TextWriter
- StreamWriter
- StringWriter
- CommandLineParser

### Json module
- Json(SAX)
- Json(DOM)

### Math module
- Math
- AttitudeTransform
- Matrix
- Plane
- Quaternion
- Random
- Vector2
- Vector3
- Vector4
- ViewFrustum

### Serialization module
- Archive (Json)

### Release
- Nuget
- Doxygen reference
- docfx homepage





