# 依頼: luminojs 単体テスト導入 (vitest) (改善案 #20)

推奨モデル: Sonnet / 目安: 1日 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) の TypeScript バインディング
`packages/luminojs` に単体テストを導入します。
まず `packages/luminojs/src` の構成 (Runtime.ts, Renderer.ts, types.ts, Matrix4x4.ts,
ResidencyManager.ts) と `docs/architecture-review-2026-07.md` の改善案 #20 を読んでください。

## 背景

luminojs には自動テストが一切なく (package.json に test スクリプトなし)、動作確認は
ブラウザでのサンプル目視のみです。WASM に依存しない純ロジックだけでもテスト可能な部分が
多くあります。WASM 本体の検証は別タスク (改善案 #12 スモークテスト) の範囲です。

## タスク

1. vitest を devDependencies に追加し、`npm test` で実行できるようにする
   (tsconfig/rollup と干渉しない最小構成。テストは `packages/luminojs/test/` または `src/*.test.ts`、
   既存のビルド (`npm run build`) に影響しないことを確認する)
2. WASM 不要でテストできる範囲のテストを書く:
   - `types.ts`: SIZEOF_* 定数が構造体レイアウトの手計算と一致すること
     (LNRenderPassDesc = 4 + 8*24 + 20 + 4(ポインタ) + 4(sortMode) = 224 など、
     計算式をテスト内にコメントで残す)
   - `Matrix4x4.ts`: makeTranslation / makeScale / makeRotationZ / multiply / makeOrthographic 等の
     数学検証。列優先レイアウト (平行移動が m[12..14]) であることを明示的に検証する
     (C++ 側 docs/graphics-conventions.md と同じ規約)
   - シリアライズ関数: Renderer の `_serializeDesc` / `_serializeTransform` / `_serializeMatrix` は
     Runtime (WASM ヒープ) に依存している。テスト容易性のため、DataView への書き込みロジックを
     純関数として切り出せるか検討し、過度なリファクタなしで可能なら切り出してオフセットを検証する。
     大工事になるなら見送り、理由を報告する
   - `ResidencyManager.ts`: evict 判定 (未使用フレーム数のしきい値) をモックリソースで検証
3. Runtime に依存する部分のモック方針 (vi.mock 等) をテストコードのコメントで説明する
4. 検証: `npm test` が全件パス、`npm run build` が従来通り成功すること

## 制約

- 公開 API を変えない。テスト容易性のためのリファクタは最小限にとどめ、変更点を報告する
- git commit は作らない。追加したテストの一覧と実行結果を報告して終了
