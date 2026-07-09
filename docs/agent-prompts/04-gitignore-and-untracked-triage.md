# [done] 依頼: .gitignore 修正と未追跡ファイルの棚卸し (改善案 #4)

推奨モデル: Sonnet / 目安: 1-2h / 人間確認: 必須 (コミット対象の最終承認)

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) のリポジトリ衛生改善を行います。
まず `docs/architecture-review-2026-07.md` の改善案 #4 と 2.2 節 R3 を読んでください。

## 背景

- `.gitignore:37` 付近の `[Bb]in/` (Visual Studio テンプレート由来) が、npm パッケージの
  ソースディレクトリ `packages/luminosc-npm/bin/` を誤って無視しており、手書きのランチャー
  `bin/luminosc.js` がコミットできない状態
- WASM ビルドが依存する `packages/LuminoCore/shaders/*.lcsh.inl` が未追跡 (クリーンチェックアウトでビルド不能)
- `packages/luminosc-x64-windows/bin/slang-compiler.dll` (24.8MB) が誤コミット待ちの状態で放置
- その他多数の未追跡ファイルが「コミットすべき/無視すべき」の判断がないまま溜まっている

## タスク

1. `.gitignore` を修正する:
   - `!packages/luminosc-npm/bin/` の例外を追加 (luminosc.js をコミット可能に)
   - `packages/luminosc-x64-windows/bin/` を明示的に無視 (exe/dll はコミットしない)
   - `packages/luminojs-examples/public/` と `packages/luminojs-examples/src/ssr/assets/` を無視
     (build_wasm.py がコピーする成果物)
   - `.claude/` を無視
   - `*.slang.dump/` `*.dump2/` (luminosc --dump の出力) を無視
2. 修正後、次で意図通りかを検証して結果を報告する:
   ```
   git check-ignore -v packages/luminosc-npm/bin/luminosc.js        # 無視されないこと
   git check-ignore -v packages/luminosc-x64-windows/bin/slang-compiler.dll  # 無視されること
   git status --short                                                # 残る未追跡一覧
   ```
3. 残った未追跡ファイル全件について、以下の推奨判断で「コミット提案リスト」を作って報告する
   (git add / commit は実行しないこと):
   - コミット推奨: `docs/coding-guidelines.md`、`packages/LuminoCore/shaders/*.lcsh` と `*.lcsh.inl`
     (WASM ビルドの前提)、`packages/LuminoC/examples/assets/SSR.lcsh`(.inl)、
     `packages/luminosc-npm/` のソース部分 (package.json, bin/luminosc.js)、
     `packages/luminosc-x64-windows/package.json` (バイナリ以外)、`scripts/package_luminosc.py`
   - docs/plans/ へ移動してコミット推奨: `BatchRenderingPlan.md`, `WasmTexturePlan.md` (移動もまだ実行しない)
   - コミット非推奨 (理由付き): TODO.txt (内容1行は Issue 化を提案)、その他あれば

## 制約

- **git add / git commit / git mv は一切実行しない**。.gitignore の編集と検証、提案リストの作成まで
- 判断に迷うファイルは無理に分類せず「要判断」として列挙する
- 報告フォーマット: ファイル(群)ごとに「推奨アクション / 理由」の表
