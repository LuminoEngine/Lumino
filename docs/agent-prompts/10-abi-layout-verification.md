# [done] 依頼: ABI レイアウト同期検証の仕組み (改善案 #10)

推奨モデル: Opus / 目安: 0.5-1日 / 人間確認: 不要 (追加のみの変更)

**前提: 改善案 #1 (sortMode 修正) が完了していること。**

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) に、C API と TypeScript
バインディング間の構造体レイアウト同期を機械検証する仕組みを実装します。
まず `AGENTS.md` と `docs/architecture-review-2026-07.md` の 3.1 節・改善案 #10 を読んでください。

## 背景

lumino.h の構造体レイアウトと luminojs の SIZEOF_* 定数/シリアライズ処理は手動同期であり、
実際に LNRenderPassDesc への sortMode 追加が未反映のままヒープ外読み取りを起こしていました
(改善案 #1 で修正済み)。再発を構造的に防ぎます。

## 重要な設計上の注意

構造体サイズはプラットフォームで異なります。`LNRenderPassDesc` は `const char*` を含むため、
wasm32 (ポインタ4バイト) では 224 バイト、x64 デスクトップでは異なるサイズになります。
したがって:
- TS 側の検証は「実行中の WASM バイナリに問い合わせる」実行時チェックが本命
- C 側の static_assert は `#ifdef __EMSCRIPTEN__` ガード付きで wasm32 の期待値を固定する

## タスク

1. C 側 (`packages/LuminoC`):
   - `LNDebug_GetStructSize(const char* structName, uint32_t* outSize)` を追加する。
     対応する構造体: LNInstanceInitializeSettings, LNColorAttachmentDesc,
     LNDepthStencilAttachmentDesc, LNRenderPassDesc, LNVertex, LNSubMesh, LNTransform,
     LNMatrix, LNSpriteCommand, LNGraphicsProfiler (typedef 名は現行ヘッダに合わせる)。
     未知の名前は LN_ERROR (適切な既存コード) を返す
   - `LuminoAPI.cpp` (または専用の .cpp) に `#ifdef __EMSCRIPTEN__` ガード付きで
     `static_assert(sizeof(LNRenderPassDesc) == 224, "...")` などの表明を置き、
     「構造体を拡張したら TS 側 (packages/luminojs/src/types.ts) も更新すること」を
     コメントで明記する
2. TS 側 (`packages/luminojs`):
   - `Runtime.ts` に `LNDebug_GetStructSize` をバインドし、`Runtime.initialize()` の最後で
     `types.ts` の SIZEOF_* 定数群と WASM 側の実サイズを照合する検証関数を呼ぶ。
     不一致の場合は構造体名・期待値・実際値を含むメッセージで例外を投げる
   - 検証対象は最低限 SIZEOF_RENDER_PASS_DESC, SIZEOF_COLOR_ATTACHMENT_DESC と、
     types.ts に存在する他の SIZEOF_* 全部
3. テスト:
   - `packages/LuminoC/test` に LNDebug_GetStructSize の基本テストを追加
     (既知名でサイズ > 0、未知名でエラー)。デスクトップではサイズの具体値は表明しない
4. 検証: デスクトップビルド + テスト、WASM ビルド、luminojs ビルドがすべて通ること。
   可能ならブラウザまたは改善案 #12 のスモークテストで Runtime.initialize() の照合が
   実際に走ることを確認 (環境がなければ手順を報告)

## 制約

- 公開ヘッダに追加する関数は既存の命名規約・ドキュメントコメント様式に従う
- C++ ファイルは UTF-8 BOM 付き・LF。コメントは日本語、`—` `–` `〜` 禁止
- git commit は作らない
