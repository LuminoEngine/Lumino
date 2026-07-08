# 依頼: 所有/借用ハンドル契約の明文化と防御 (改善案 #14)

推奨モデル: Opus / 目安: 半日 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) のハンドル管理を堅牢化します。
まず `packages/LuminoCore/src/Runtime/ObjectRegistry.cpp`(.hpp)、
`packages/LuminoC/src/LuminoAPI.cpp` の `wrapObjectFromCreate` / `wrapObjectFromGet` (106-139行付近)、
`docs/architecture-review-2026-07.md` の改善案 #14 を読んでください。

## 背景

C API には2種類のハンドルがあります:
- 所有ハンドル: `LN*_Create` が返す。`wrapObjectFromCreate` が addRef して所有権をクライアントに渡す。
  クライアントは `LNObject_Release` で解放する義務がある
- 借用ハンドル: `LNGraphicsContext_BeginFrame` が返す renderer / colorBuffer / depthBuffer や
  `LNWindow_GetGraphicsContext` の戻り。`wrapObjectFromGet` は addRef せず内部参照に相乗りする

現状 `LNObject_Release` は区別なく参照カウントを減らすため、借用ハンドルを誤って Release すると
所有者 (GraphicsContext 等) が生きているのにオブジェクトが delete され、以降ダングリングポインタ経由の
未定義動作になります。ヘッダの記述も「Release を呼ぶ必要はありません」であって「呼んではいけない」に
なっていません。luminojs 自身は正しく回避していますが、C API の契約として穴があります。

## タスク

1. 調査: `wrapObjectFromGet` が同一オブジェクトに対して常に同じハンドルを返すのか、
   呼び出しごとに新しいスロットを登録するのかを確認する。また同一オブジェクトが
   所有ハンドルと借用ハンドルの両方を持ちうるケースがあるかを整理する。
   この結果を先に短い設計メモとして報告に含める (実装方針の妥当性が変わるため)
2. 実装: ObjectRegistry のスロットに「借用フラグ」を追加する
   - 登録 API を `register(所有)` / `registerBorrowed(借用)` のように分離する
     (命名は既存規約に合わせる)
   - `LNObject_Release` は借用ハンドルに対して参照カウントを減らさず、
     エラー (適切な既存の LNResult コード、なければ LN_ERROR_INVALID_OPERATION 等を追加) を返す
   - `wrapObjectFromGet` を借用登録に切り替える
3. ドキュメント: `lumino.h` の冒頭 (LNObject セクション付近) に「ハンドルの所有権」の章を
   ドキュメントコメントとして追加する。どの API が所有ハンドルを返し、どれが借用かを一覧化し、
   BeginFrame 等の該当 API のコメントも「Release してはいけない (エラーになる)」に改める
4. テスト: `packages/LuminoC/test/Test_Object.cpp` に追加:
   - BeginFrame で得た renderer を Release するとエラーが返り、その後も renderer が使えること
   - 所有ハンドルの Release は従来通り成功すること
5. 検証: デスクトップビルド + 全テスト (GPU テスト含む) が通ること。luminojs のビルドも通ること
   (JS 側は autoRelease=false で借用を扱っており挙動不変のはずだが確認する)

## 制約

- ObjectRegistry のスレッド安全性 (mutex 保護) を壊さないこと
- C++ ファイルは UTF-8 BOM 付き・LF。コメントは日本語、`—` `–` `〜` 禁止
- git commit は作らない
