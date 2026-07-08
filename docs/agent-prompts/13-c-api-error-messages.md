# 依頼: C API エラー詳細の伝搬 (改善案 #13)

推奨モデル: Sonnet / 目安: 半日 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) のエラーハンドリング改善を行います。
まず `packages/LuminoBase/include/LuminoBase/Result.hpp`、`packages/LuminoC/src/LuminoAPI.cpp`、
`docs/architecture-review-2026-07.md` の改善案 #13 を読んでください。

## 背景

C++ 内部は `Result<T> = tl::expected<T, Error>` で `Error.message` に詳細を持ちますが、
C API 境界 (`LuminoAPI.cpp`) で `if (!result) return LN_ERROR_UNKNOWN;` のように丸められ、
呼び出し側 (JS) はエラー詳細を得られません。デバッグはログ頼みです。

## タスク

1. C 側:
   - `LNGetLastErrorMessage()` (戻り値 `const char*`, UTF-8) を lumino.h に追加する。
     直近に失敗した API のエラーメッセージを返し、成功時は空文字列を返す仕様とする。
     「返されたポインタは次の API 呼び出しまで有効」の寿命規約をコメントに明記する
   - 実装: LuminoAPI.cpp に静的な `std::string s_lastErrorMessage` を置き、
     Result からのエラー変換箇所で code とともに message を保存する。
     変換が散在している場合は共通ヘルパー関数 (例: `toLNResult(const ln::Error&)`) に集約し、
     各所をそれで置き換える。成功パスでのクリアはコストとの兼ね合いで「失敗時のみ更新」とし、
     その仕様をヘッダコメントに書く
   - スレッドについて: 現状の Lumino はシングルスレッド前提のため static で良いが、
     その前提をコメントに明記する (将来 thread_local 化する目印)
   - あわせて `ln::ErrorCode` と `LNResult` の対応が UNKNOWN 一辺倒になっている箇所を確認し、
     既存の LNResult コード (INVALID_ARG 等) に自然に対応付くものは対応させる
2. luminojs 側:
   - `Runtime.ts` に `LNGetLastErrorMessage` をバインド (UTF8ToString 経由)
   - 既存の「LNResult をチェックして例外を投げる」共通処理 (あるいは各所の失敗ハンドリング) を
     調査し、例外メッセージに `LNGetLastErrorMessage()` の内容を含めるよう変更する。
     共通ヘルパーが無ければ `Runtime.throwIfFailed(result, apiName)` を新設して主要呼び出しに適用する
3. 検証:
   - デスクトップビルド + 既存テストが通ること
   - `packages/LuminoC/test` に「わざと失敗する呼び出し (不正ハンドル等) の後に
     LNGetLastErrorMessage が非空文字列を返す」テストを追加
   - luminojs ビルドが通ること

## 制約

- C++ ファイルは UTF-8 BOM 付き・LF。コメントは日本語、`—` `–` `〜` 禁止
- git commit は作らない
