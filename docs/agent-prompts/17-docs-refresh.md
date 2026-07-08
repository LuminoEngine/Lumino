# 依頼: ドキュメント再整備 (改善案 #17)

推奨モデル: Sonnet / 目安: 半日 / 人間確認: 不要 (AGENTS.md の将来構想の書きぶりのみ最後に確認)

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) のドキュメント整備を行います。
まず `docs/architecture-review-2026-07.md` 全体 (特に 1 章の構造マップと改善案 #17) を読んでください。
このプロジェクトは AI エージェントに開発を依頼する運用のため、ドキュメントの正確さが
そのまま生成コードの品質になります。「実装と一致していること」を最優先にしてください。

## タスク

1. `ARCHITECTURE.md` (現在17バイトの空殻) を本文化する:
   - `docs/architecture-review-2026-07.md` 1章の構造マップ (レイヤー図・パッケージ表・
     シェーダパイプライン) を転記・整形する。レビュー固有の指摘 (バグや行数) は含めず、
     恒常的な構造の説明に絞る
   - 各パッケージの README や docs/ への相互リンクを張る
2. `AGENTS.md` の現状と将来構想を区別する:
   - 「デスクトップ: Vulkan + QuickJS」「コンシューマゲーム機: Vulkan-like + QuickJS」の QuickJS、
     および Audio/Input への言及は未実装の将来構想である。「現状」と「将来構想」が
     読み分けられるよう見出しや注記を追加する (内容の削除はしない)
3. stale な旧フォーマット記述の掃除:
   - `packages/LuminoShader/ARCHITECTURE.md` の「以下、v1の時の情報」以降 (lcsh.bl. 等の旧シグネチャ、
     UnifiedShaderSerializer 無印のクラス名) を、現行実装 (lcs2 マジック、UnifiedShaderSerializer2) に
     合わせて更新するか、「歴史的記録」であることを明確にした付録に移す
   - `packages/luminosc/README.md` のフォーマット記述 (lcsh.bl. 等) を現行の lcs2 仕様に修正する。
     正となる情報源は `packages/LuminoShader/src/UnifiedShaderSerializer2.cpp` と
     `packages/LuminoShader/README.md` (こちらは正しい)
4. README の欠落を埋める:
   - `packages/LuminoC/README.md` を新規作成 (パッケージの役割、lumino.h が公開契約であること、
     examples の場所、ハンドル/LNResult の基本)
   - `packages/luminojs/README.md` (現在6行) にビルド方法・Runtime.initialize の使い方・
     WASM 成果物の入手方法 (build_wasm.py) を追記
   - `packages/LuminoBase/README.md`, `packages/LuminoCore/README.md` (各3行) に提供機能の
     概要を追記。LuminoCore の「Audio、Input」への言及は実装に合わせて修正する
5. 行列規約の説明の混乱を解消する:
   - `docs/graphics-conventions.md` は「列優先」、`docs/shader-conventions.md` は
     「シェーダ側は行優先」と書かれており、初見では矛盾に見える。
     `packages/LuminoShader/src/ShaderCompiler2.cpp` の SLANG_MATRIX_LAYOUT_COLUMN_MAJOR 設定と
     `packages/LuminoShader/ARCHITECTURE.md` の補足を確認した上で、両文書に
     「CPU 側メモリレイアウトとシェーダレジスタパッキングの関係」を1つの囲み説明として追記し、
     相互参照させる

## 制約

- 事実 (コード) と食い違う記述を新たに作らないこと。書く前に必ず該当ソースを確認する
- 文体・見出しスタイルは既存ドキュメントに合わせる。`—` `–` `〜` は使わない
- git commit は作らない。変更ファイル一覧と、確認が必要な記述 (もしあれば) を報告して終了
