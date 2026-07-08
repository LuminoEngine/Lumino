# 依頼: 公開 ABI の掃除 (改善案 #7)

推奨モデル: Opus / 目安: 2-3h / 人間確認: 実施タイミング (ABI 破壊のため LYRIDRA 側の WASM 更新と同期が必要)

**注意: 改善案 #21 の FFI オーバーヘッド計測を先に行うこと。本タスクで削除する
LNDrawCommandBuffer_DrawSprites (一括投入 API) は計測の比較基準として有用なため。**

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) の C API 整理を行います。
まず `AGENTS.md` と `docs/architecture-review-2026-07.md` の改善案 #7 を読んでください。
対象は `packages/LuminoC/include/LuminoC/lumino.h` (公開 FFI 契約) とその実装・利用側です。

## 背景

Lumino は「純 C の FFI を安定化する」方針 (README.md) ですが、正式リリース前の今しか
できない破壊的整理が残っています:
- `LNGraphicsProfilering` という typo の構造体タグが公開 ABI に露出 (lumino.h:1120。typedef 名は LNGraphicsProfiler)
- deprecated 3系統がヘッダ・実装に残存: `LNDrawCommandBuffer_*`, `LNBatchProcessor_*`, `LNMaterial_CreateUnlit`
- luminojs が deprecated の `LNMaterial_CreateUnlit` を現役使用しており、後継の
  `LNMaterial_CreateFromBuiltinShader` は cwrap バインドすらされていない
- lumino.h:839 に「使用してくださsい」という typo

## タスク

1. `LNGraphicsProfilering` を `LNGraphicsProfiler` に改名 (タグと typedef を一致させる)。lumino.h:839 の typo も修正
2. deprecated API の削除:
   - lumino.h から `LNDrawCommandBuffer_*` 群、`LNBatchProcessor_*` 群、`LNMaterial_CreateUnlit` の宣言を削除
   - `LuminoAPI.cpp` から対応する実装を削除
   - **重要**: C++ 内部の `DrawCommandBuffer` / `BatchProcessor` クラス自体は Renderer が内部使用している
     可能性が高い (Renderer.hpp:310 の m_commandBuffer 等)。削除するのは C API ラッパーのみで、
     内部クラスは残すこと。削除前に参照を必ず調査する
   - C++ examples (`packages/LuminoC/examples/BatchRendering`, `BatchSprite` 等) が削除対象 API を
     使っている場合は `LNRenderer_DrawMesh` / `LNRenderer_DrawSprite` へ移行するか、例が冗長なら
     example ごと削除を提案 (削除は報告して判断を仰ぐ)
3. luminojs の追随 (`packages/luminojs/src`):
   - `Runtime.ts` の `_bindAPI()` に `LNMaterial_CreateFromBuiltinShader` を追加
   - `types.ts` に `BuiltinShader` enum を追加 (lumino_types.h の LNBuiltinShader と値を一致させる)
   - `Material.ts` の unlit 生成経路を新 API に切り替える。**TS 公開 API (Material.createUnlit()) の
     シグネチャは変えない** (内部実装の差し替えのみ)
4. 検証:
   - デスクトップビルド + 全テスト: `cmake --build build/lumino-x64-windows --config Debug --parallel`
     の後 ctest (GPU テスト含む。失敗したら原因を特定して報告)
   - WASM ビルド: `python scripts/build_wasm.py` (emsdk セットアップ済み前提。失敗時は状況報告)
   - luminojs: `cd packages/luminojs && npm run build`

## 制約

- C++ ファイルは UTF-8 BOM 付き・LF を維持。コメントは日本語、`—` `–` `〜` 禁止 (AGENTS.md)
- ABI 破壊の一覧 (削除した関数名・改名した型名) を報告書に明記すること
- git commit は作らない
