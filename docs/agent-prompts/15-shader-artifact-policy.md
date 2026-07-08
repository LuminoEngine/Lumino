# 依頼: シェーダ成果物の生成/コミット方針統一 (改善案 #15)

推奨モデル: Opus / 目安: 半日-1日 / 人間確認: コミット方針の最終承認

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) のシェーダビルドフローを整備します。
まず `packages/LuminoCore/CMakeLists.txt` (135-165行のシェーダ生成)、`docs/shader-conventions.md`、
`docs/architecture-review-2026-07.md` の改善案 #15 を読んでください。

## 背景

現状のシェーダ成果物 (.lcsh / .lcsh.inl) の扱いは3通りが混在しています:
1. 組み込みシェーダ (packages/LuminoCore/shaders): デスクトップ CMake ビルド時に luminosc で自動生成。
   ただし WASM ビルドでは生成されず、コミット済みの .lcsh.inl に依存する設計 (現在は未コミットで矛盾)
2. examples (packages/LuminoC/examples/assets): 手動で luminosc を実行してコミットする運用。
   実際に SSR.slang だけコミットされ SSR.lcsh がコミット漏れしている
3. .lcsh にはソース .slang との対応を検証する情報 (ハッシュ等) が無く、乖離を検知できない

## タスク

1. examples のシェーダにも自動生成を導入する:
   - `packages/LuminoC/examples` 配下の .slang (SSR.slang, Shader1 系等) に対し、LuminoCore と同様の
     `add_custom_command` + `luminosc` 呼び出しを CMake に追加する (デスクトップビルド時)
   - luminosc が無条件に出力する .lcsh.inl が不要な example では散らからないよう、出力の扱いを整理する
     (luminosc に .inl 抑制オプションを追加するのも可。その場合は packages/luminosc/Main.cpp に
     `--no-inl` を実装する)
2. コミット方針を文書化する (`docs/shader-conventions.md` に節を追加):
   - コミットする: WASM ビルドが #include する組み込みシェーダの .lcsh.inl (と対応する .lcsh)、
     examples がランタイムロードする .lcsh
   - コミットしない: --dump 出力等の一時ファイル
   - .slang を変更したら成果物も再生成してコミットする、という運用ルールを明記
3. 鮮度チェックスクリプトを作る (`scripts/check_shader_artifacts.py`):
   - まず luminosc の出力が決定的 (同一入力から同一バイナリ) かを実験で確認する
   - 決定的なら: リポジトリ内の全 .slang を再コンパイルし、コミット済み .lcsh とバイト比較して
     乖離があれば exit 1 (CI 組み込みを想定)
   - 非決定的なら: 原因 (タイムスタンプ等) を特定して報告し、可能なら luminosc 側を決定的に修正する。
     困難なら「.slang の git 更新日時 vs .lcsh の更新日時」比較などの次善策を実装し限界を明記する
4. 検証: デスクトップフルビルドで examples の .lcsh が自動生成されること、
   チェックスクリプトが「最新状態で緑 / .slang を触ると赤」になること

## 制約

- どのファイルをコミットするかの最終判断は人間が行う。スクリプトと CMake とドキュメントの整備、
  および「コミットすべきファイル一覧」の報告までを行うこと (git add/commit はしない)
- C++/CMake の編集は既存スタイルに従う。C++ は UTF-8 BOM 付き・LF、コメント日本語
