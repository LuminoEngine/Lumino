---
agent: agent
description: 'packages/LuminoC/include/LuminoC/lumino.h のドキュメントコメントを packages/luminojs/src/ の TypeScript ラッパーに転記する。'
---

# C-API コメントを TypeScript ラッパーへ転記する

`packages/luminojs/src/` 配下の TypeScript コードは `packages/LuminoC/include/LuminoC/lumino.h` で宣言される C-API の薄いラッパーです。
現在の Lumino は **C-API よりも TypeScript クライアントの DX を優先** したいため、
ヘッダーに記載された Doxygen コメントを TypeScript 側に **JSDoc として転記**してください。
（リファレンスサイトは未整備のため、エディタ上のホバーで読めるコメントが第一級のドキュメントになります）

## 入力

- **転記元 (Source of truth)**: `packages/LuminoC/include/LuminoC/lumino.h`
  - 必要に応じて `packages/LuminoC/include/LuminoC/lumino_types.h` も参照（enum / 型定義のドキュメント）
- **転記先**: `packages/luminojs/src/*.ts`
  - `LuminoObject.ts` `Runtime.ts` `LuminoC.mjs.d.ts` `index.ts` は対象外（純粋に TS 側都合の基盤コード）
  - `types.ts` は `LNVertex` / `LNSubMesh` / `LNTransform` 等の値型の対応関係がある場合のみ対象

## 対応関係の探し方

TypeScript のメンバーごとに、対応する C 関数を以下の規則で特定する:

| TS                                   | C 関数                                  |
|--------------------------------------|----------------------------------------|
| `class Foo`                          | `LNFoo_*` シリーズ全体                  |
| `static Foo.create(...)`             | `LNFoo_Create(...)`                    |
| `foo.bar(...)` (インスタンスメソッド) | `LNFoo_Bar(handle, ...)`               |
| `get foo.bar()` / `set foo.bar(v)`   | `LNFoo_GetBar` / `LNFoo_SetBar`        |
| `interface FooDesc`                  | `typedef struct LNFooDesc`             |
| `interface FrameInfo` 等の戻り値型    | 対応する関数の `out*` パラメータ群       |

該当する C 関数が見つからない場合は、**そのメンバーには手を加えないこと**。
（TS 側だけに存在する補助メソッド・residency 管理 (`ensure` `evict`) などはそのまま残す）

## 転記の規則

### 1. 必ず転記するもの

- 関数 / 構造体冒頭の説明文
- `@note` で書かれた挙動の注意点
- パラメータの値域・単位（ピクセル、ラジアン、`0.0-1.0` 等）
- NULL 許容や「内部で〇〇が呼ばれる」「次の呼び出しまで有効」等のライフサイクル説明

### 2. 適応的に書き換えるもの

| C-API                                 | TypeScript                                                 |
|--------------------------------------|------------------------------------------------------------|
| `@param[in] handle ...`              | 削除（`this` に置き換わるため）                              |
| `@param[in] paramName ...`           | `@param paramName ...`                                     |
| `@param[out] outFoo ...`             | `@returns ...`（複数 out の場合は戻り値型のフィールドコメント） |
| `LN_NULL_HANDLE`                     | `null` または `undefined`                                  |
| `LNHandle`                           | 対応する TS クラス名                                       |
| `LNBool` (`LN_TRUE` / `LN_FALSE`)    | `boolean` (`true` / `false`)                              |
| `const char*` (UTF-8)                | `string`                                                  |
| `LNResult` のエラーコード返却         | 「失敗時は例外を投げる」                                   |
| `LNObject_Release` を呼ぶ説明         | `dispose()` を呼ぶ説明                                    |

### 3. 残すもの・触らないもの

- 既存の JSDoc に書かれている **TS 側固有の情報**（residency、async ラッパーの挙動、`@internal` 等）は削除しない。
  C コメントとマージする場合は、TS 固有の情報を先頭または末尾に残し、C 由来の本文を本体に置く。
- 既存コメントが C のコメントより詳しい場合は、上書きせずに不足分のみ追記する。
  既存 TS コメントが英語の場合は、日本語の説明を追加で併記する形にする（既存英文は消さない）。
- メソッドの実装コードは一切変更しない。

### 4. 書かないこと

- C-API の存在自体への言及（「これは `LNFoo_Bar` のラッパーです」のような文）。
  ユーザーから見れば実装詳細であり、リネーム時に rot する。
- `extern LUMINO_API` 等の C 装飾。
- ハンドルを直接扱う旨の説明（TS 側ではクラスインスタンスを介す）。

## 進め方

1. まず `packages/luminojs/src/` のファイル一覧を取得し、対象ファイルを列挙する。
2. 各 TS ファイルについて：
   1. ファイル全体を読む
   2. `lumino.h` から対応する `LN<ClassName>_*` セクションを grep / read で抽出
   3. メンバーごとに対応付けし、上記規則に沿って JSDoc を生成・適用
   4. 1 ファイルの編集が終わったら、変更内容を簡潔に要約して報告
3. すべて完了したら、転記できなかったメンバー（対応する C 関数なし、既に十分なコメントあり、等）を一覧で報告する。

## 出力フォーマット

各ファイルの編集後、以下の形式で報告:

```
### packages/luminojs/src/<File>.ts
- 転記: <メンバー名> <- <LN関数名>
- 転記: ...
- スキップ (理由): <メンバー名>
```

## 制約

- 一度に編集するファイルは 1 つずつ。差分を最小に保つ。
- コメント以外（import、シグネチャ、実装）は変更しない。
- TS の型注釈やデフォルト値は変更しない（C 側の `(デフォルト: ...)` は文中で説明として残すだけ）。
- 不明な対応関係を推測で書かない。判断に迷ったらスキップして報告する。
