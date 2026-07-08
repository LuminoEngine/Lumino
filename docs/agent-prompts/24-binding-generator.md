# 依頼: lumino.h からのバインディング自動生成 (改善案 #24)

推奨モデル: Opus / 目安: 2-4日 / 人間確認: 生成物への切り替えタイミング

**前提: 改善案 #10 (ABI 検証) が完了していること (自動生成の安全網になる)。
改善案 #1, #7 が完了し lumino.h が整理済みであることが望ましい。**

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) に、C ヘッダから
TypeScript バインディングを自動生成する仕組みを実装します。
まず `packages/LuminoC/include/LuminoC/lumino.h` と `lumino_types.h`、
`packages/luminojs/src/Runtime.ts` の `_bindAPI()` (手書き cwrap 表)、`types.ts` を読んでください。

## 背景

66個の C API 関数の cwrap バインド・TS enum・構造体レイアウト定数 (SIZEOF_*) がすべて手書きで、
同期ミスが実際に発生しました (sortMode 事件、改善案 #1)。今後 HD-2D 機能で API が増え続けるため、
手作業の同期をゼロにします。

## 方針

- 汎用 C パーサは作らない。lumino.h は整形済みで規約が一貫している (`extern LUMINO_API 戻り値 名前(...)`、
  `typedef struct { ... } 名前;`、`typedef enum { ... } 名前;`) ため、この規約に依存した
  正規表現/行ベースのパーサで十分。**パーサが解釈できない構文に出会ったら黙って無視せず
  エラーで停止する** こと (サイレントな取りこぼしが最悪の failure mode)
- 生成物は git にコミットする方式 (ビルド時生成ではなく、生成スクリプトを回して差分を確認できる形)

## タスク

1. `scripts/generate_bindings.py` を作成する。入力: lumino.h / lumino_types.h。出力:
   a. `packages/luminojs/src/generated/api.ts`: cwrap バインド表 (関数名 / 戻り値型 / 引数型)。
      ポインタは "number"、文字列引数は "string" など、既存 _bindAPI() の型対応規則を踏襲する。
      async 指定が必要な関数 (LNInstance_Initialize 等) はスクリプト内の明示リストで管理する
   b. `packages/luminojs/src/generated/enums.ts`: LN 系 enum の TS enum (プレフィックス除去した命名は
      既存 types.ts の対応関係に合わせる)
   c. `packages/luminojs/src/generated/layout.ts`: wasm32 前提の構造体レイアウト
      (サイズと各フィールドのオフセット)。ポインタ/enum は4バイト、アラインメントは natural。
      ネスト構造体・固定長配列 (colorAttachments[8] 等) に対応すること
   d. C 側検証ファイル `packages/LuminoC/src/generated_layout_assert.inl`:
      `#ifdef __EMSCRIPTEN__` ガード付きの static_assert 群 (c. と同じ値)。LuminoAPI.cpp から include
2. 検証を最優先にする:
   - ゴールデンテスト: 生成された関数リスト/enum値/構造体サイズが、現行の手書き
     (`Runtime.ts` の cwrap 表、`types.ts` の enum と SIZEOF_*) と一致することを確認する
     スクリプトまたは vitest テストを書く。**不一致が出たらどちらが正しいか調査して報告する**
     (手書き側のバグ発見の機会でもある)
   - 改善案 #10 の LNDebug_GetStructSize と layout.ts の突き合わせテスト (実行環境があれば)
3. ドキュメント: スクリプトの使い方・再生成手順・「lumino.h を変更したら generate_bindings.py を
   回す」運用を `packages/luminojs/README.md` に追記
4. **切り替えはしない**: 手書きコードを生成物で置き換える作業は、ゴールデンテストの結果を
   人間が確認してから別途行う。本タスクは「生成物と一致検証」までで停止する

## 制約

- Python は既存スクリプト (build_wasm.py 等) のスタイルに合わせる
- git commit は作らない。生成物のサンプルと一致検証の結果を報告して終了
