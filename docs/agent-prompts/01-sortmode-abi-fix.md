# [done] 依頼: sortMode ABI 不整合の修正 (改善案 #1)

推奨モデル: Sonnet / 目安: 30分 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`C:\Proj\dragon-baby-engine\.build\Lumino`) の TypeScript バインディング修正を行います。
まず `AGENTS.md` と `docs/architecture-review-2026-07.md` の 3.1 節と改善案 #1 を読んでください。

## 背景

直近のコミットで C API の `LNRenderPassDesc` (packages/LuminoC/include/LuminoC/lumino.h:164-182) の末尾に
`LNSortMode sortMode` フィールドが追加され、wasm32 での構造体サイズが 220 から 224 バイトになりました。
しかし luminojs 側が未追随のため、C++ 側 (LuminoAPI.cpp の LNRenderer_BeginRenderPass) が
JS の確保した 220 バイト領域の外 (オフセット 220-223) を読む未定義動作が毎パス発生しています。

## タスク

1. `packages/luminojs/src/types.ts`
   - `SIZEOF_RENDER_PASS_DESC` を 220 から 224 に変更 (types.ts:161)
   - `LNSortMode` に対応する `SortMode` enum を追加 (Stable = 0, FrontToBack = 1, BackToFront = 2)。
     値と意味は lumino.h の LNSortMode 定義 (lumino.h:151-158) と一致させること
   - `RenderPassDesc` インターフェース (types.ts:120-133 付近) に `sortMode?: SortMode` を追加し、
     lumino.h のコメントに準じたドキュメントコメントを書く (未指定 = Stable = 投入順)
2. `packages/luminojs/src/Renderer.ts`
   - `_serializeDesc()` (311行付近) でオフセット 220 に `v.setUint32(220, desc.sortMode ?? 0, true)` を書き込む
   - 構造体レイアウトを説明するコメント (295行付近の「合計 216 バイト」など) を実際のレイアウトに合わせて修正
3. `packages/luminojs/src/index.ts` から `SortMode` をエクスポート
4. 他に 220 をハードコードしている箇所がないか `packages/luminojs/src` 全体を検索して確認

## 検証

- `cd packages/luminojs && npm run build` (tsc --noEmit + rollup) が成功すること
- 変更後の `_serializeDesc` のオフセット合計が 224 になることをコメントの計算とともに確認
  (colorAttachmentCount 4 + colorAttachments 8x24=192 + depthStencil 20 + shaderPassName 4 + sortMode 4 = 224)

## 制約

- C++ 側 (lumino.h / LuminoAPI.cpp) は変更しない (既に正しい)
- 既存 API の互換性を壊さない (sortMode はオプショナル、未指定時は従来挙動 = Stable)
- git commit は作らない。変更ファイル一覧と検証結果を報告して終了
