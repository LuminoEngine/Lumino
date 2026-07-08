# 依頼: HD-2D ライティング基盤 - Scene UBO 再設計 + シャドウマップ (改善案 #22)

推奨モデル: フェーズA (設計) = Fable 5 メインセッション / フェーズB-D (実装) = Opus
目安: 3-7日 / 人間確認: フェーズA の設計ドキュメント承認後に実装へ進む

このプロンプトはフェーズA (設計) から開始し、設計ドキュメントの提出で一度停止する構成です。
実装フェーズは設計承認後に、この続きとして依頼してください。

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) に HD-2D 表現の核となる
動的ライティング/シャドウマップ基盤を設計・実装します。
まず以下を読んでください:
- `docs/architecture-review-2026-07.md` 2.2 節 R5 と改善案 #22
- `packages/LuminoCore/include/LuminoCore/Graphics/Material.hpp` の SceneParamsUBO (36-40行付近)
- `packages/LuminoCore/shaders/BasicLit.slang` と `packages/LuminoShader/shaders/lumino.slang`
- `docs/shader-conventions.md` (パス選択機構: shaderPassName)
- `packages/LuminoC/examples/SSR` (マルチパス構成の実例)

## 背景と現状

- 実行時ライティングは1平行光源 + 環境光のみ。`SceneParamsUBO = { lightDir, lightColor, ambientColor }`
  の固定小構造で、シャドウ行列や複数ライトを格納できない
- シャドウマップ機構は存在しない (legacy/ に旧実装の参考コードあり)
- 一方で土台は整っている: 名前付きマルチパス (shaderPassName で "Shadow" パスを選択可能)、
  オフスクリーン RT、深度専用テクスチャ (LNTexture2D_CreateDepthStencil)、MRT、
  View UBO に invViewProj 等が既に存在
- ターゲットは HD-2D: 平行投影カメラのシーンに対する指向性ライト1灯のシャドウ (最優先) +
  ポイントライト数灯 (シャドウなしで可)

## フェーズA: 設計 (このプロンプトの範囲)

設計ドキュメント `docs/plans/hd2d-lighting-design.md` を作成して停止する。含めるべき内容:

1. SceneParamsUBO v2 の構造: 指向性ライト (方向/色/強度) + シャドウ行列 (まず単一カスケード) +
   シャドウマップ解像度/バイアスパラメータ + ポイントライト N 灯 (個数上限と根拠)。
   std140/WGSL の uniform レイアウト規則 (vec3 のパディング等) を明記した C++ 構造体と
   lumino.slang 側 SceneParams の対応表
2. C++/シェーダ間のレイアウト同期方法: 現状の「手動一致契約」の脆さ (Material.hpp:35 コメント) を
   繰り返さない仕組み (static_assert + オフセット表、あるいはシェーダリフレクションとの照合)
3. シャドウパスのフロー: C API 利用者 (luminojs) から見た手順
   (シャドウ用深度 RT 作成 → "Shadow" パスで描画 → メインパスでシャドウマップを参照)。
   ライトカメラ (平行投影) の設定 API、シャドウマップのバインド方法 (Material 経由か Scene 経由か) の決定
4. 深度比較サンプラー (comparison sampler) の RHI 対応状況の調査結果と、WebGPU/Vulkan 両対応の方針
5. C API の追加/変更一覧 (例: LNRenderer_SetDirectionalLight, LNRenderer_SetShadowParams 等の案)
6. テスト計画: ビジュアル回帰テスト (Test_Graphics に追加) のシーン構成
7. 実装フェーズ分割 (B: SceneUBO v2 + ライト API、C: シャドウパス、D: サンプル + テスト) と
   各フェーズの完了条件

設計上の疑問点があれば、勝手に決めずに「要判断事項」として列挙すること。

## フェーズB-D: 実装 (設計承認後、別途依頼)

- legacy/ のシャドウ実装 (検索して特定する) はアルゴリズムの参考として読んでよいが、コードは流用せず
  現行の RHI/Renderer 設計に合わせて書き直す
- luminojs へのバインド追加と、luminojs-examples への平行投影シャドウのサンプル追加まで含む
- 各フェーズ完了時にビルド + 全テスト + ビジュアル確認

## 制約

- C++ は UTF-8 BOM 付き・LF、コメント日本語、`—` `–` `〜` 禁止 (AGENTS.md)
- 既存の Forward 描画の互換性を壊さない (ライト未設定時は従来と同じ見た目)
- git commit は作らない
