# 依頼: WASM 非対応 API の契約明記 (改善案 #8)

推奨モデル: Sonnet / 目安: 1h / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) の C API 整合性修正を行います。
まず `docs/architecture-review-2026-07.md` の改善案 #8 を読んでください。

## 背景

`packages/LuminoC/include/LuminoC/lumino.h` は次の3関数を無条件に宣言していますが、
実装 (`packages/LuminoC/src/LuminoAPI.cpp:438-479` 付近) は `#ifndef __EMSCRIPTEN__` で
囲まれており、WASM ビルドにはシンボル自体が存在しません:
- `LNGraphicsContext_WaitIdle`
- `LNGraphicsContext_RequestCaptureBackbuffer`
- `LNGraphicsContext_CaptureBackbuffer`

ヘッダに制約の記載がないため、ヘッダだけを見たクライアントがリンクエラーや誤解に遭遇します。

## タスク

1. `packages/LuminoC/include/LuminoC/lumino_types.h` の `LNResult` enum に
   `LN_ERROR_NOT_SUPPORTED` を追加する (既存のエラーコード値と重複しない負値。既存の命名規則に従う)
2. `LuminoAPI.cpp` を変更し、WASM ビルドでも3関数のシンボルを提供する:
   - `#ifndef __EMSCRIPTEN__` で実装全体を消すのではなく、関数は常に定義し、
     Emscripten 時は本体を `return LN_ERROR_NOT_SUPPORTED;` にする (out 引数は安全な値でゼロ初期化)
3. lumino.h の3関数のドキュメントコメントに「Web (WASM) ビルドでは LN_ERROR_NOT_SUPPORTED を返す」
   旨を追記する。他にもプラットフォーム依存の挙動をもつ API (LNWindow_Create は Web でどうなるか、
   LNWindow_CreateFromCanvas はデスクトップでどうなるか) のコメントも確認し、記載が無ければ揃える
4. 検証:
   - デスクトップビルドが通ること (挙動不変)
   - WASM ビルド (`python scripts/build_wasm.py`) が通ること。可能ならビルド成果物に
     3シンボルが含まれることを確認 (LuminoC.mjs 内の文字列検索等)

## 制約

- C++ ファイルは UTF-8 BOM 付き・LF を維持。コメントは日本語、`—` `–` `〜` 禁止
- luminojs 側は変更不要 (これらの API は未バインドのため)
- git commit は作らない。変更と検証結果を報告して終了
