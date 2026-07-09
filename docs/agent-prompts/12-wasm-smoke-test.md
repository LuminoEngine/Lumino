# [done] 依頼: WASM スモークテストの導入 (改善案 #12)

推奨モデル: Opus / 目安: 1-2日 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) に、luminojs が実際にロードする
WASM バイナリを検証する自動テストを導入します。
まず `docs/architecture-review-2026-07.md` の改善案 #12 と、`packages/luminojs/src/Runtime.ts`、
`packages/LuminoC/CMakeLists.txt` (Emscripten リンクフラグ) を読んでください。

## 背景

GoogleTest は全てデスクトップ (Vulkan) ビルドを検証しており、WASM バイナリ (LuminoC.wasm) を
検証する自動テストがゼロです。「デスクトップで通った = WASM でも動く」は保証されません
(コンパイル条件も ASYNCIFY 等のフラグも異なるため)。

## 重要な技術的制約

LuminoC.mjs は `-sENVIRONMENT=web` でビルドされており、**素の Node.js ではロードできない可能性が
高い**です。方式を最初に検証してから本実装に入ること:
- 方式A (推奨): Playwright (Chromium) でテストページをロードし、ブラウザ内で API を呼ぶ。
  GPU 不要の API のみなら headless で安定して動く
- 方式B: テスト用ビルドだけ `-sENVIRONMENT=web,node` にして Node (vitest) で直接ロードする。
  ビルドフラグの分岐が増えるのが欠点
- どちらを採るかは、まず最小実験 (LNHelloTest を呼ぶだけ) で両方式の成立性を確認して決める

## タスク

1. 上記の方式検証を行い、採用方式と理由を報告に含める
2. スモークテストを実装する (置き場所は `packages/luminojs` に `npm run test:smoke` として追加):
   - Runtime.initialize() が成功する (= WASM ロード + LNInstance_Initialize)
   - `LNHelloTest(42)` が 42 を返す
   - `Runtime.getBuildTimestamp()` が空でない文字列を返す
   - 画像デコード (LNImage_DecodeFromMemory 相当の TS API): 小さな PNG をデコードして
     期待サイズのピクセルが返る (GPU 不要の純 CPU 経路)
   - 改善案 #10 実装済みの場合: 構造体サイズ照合が initialize 内で走りエラーにならないこと
   - GPU (WebGPU) が必要な API はスコープ外とする (headless GPU は環境依存が強いため。
     将来の拡張ポイントとしてテスト内に TODO コメントを残す)
3. テストの実行前提 (WASM がビルド済みであること等) を README または package.json コメントに明記
4. 検証: テストがローカルで安定して2回連続グリーンになること

## 制約

- CI (改善案 #9) への組み込みは、9 が導入済みならジョブ追加まで行う。未導入ならローカル実行のみで良い
- git commit は作らない。採用方式・テスト内容・実行結果を報告して終了
