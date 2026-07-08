# 依頼: CI 導入 (GitHub Actions) (改善案 #9)

推奨モデル: Opus / 目安: 1-2日 (試行錯誤含む) / 人間確認: Actions の課金/無料枠の事前確認、初回は PR として提案

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) に CI を導入します。
まず `AGENTS.md`、`README.md` (ビルド手順)、`docs/architecture-review-2026-07.md` の改善案 #9 を読んでください。

## 背景

リポジトリに CI が存在せず (.github/ には prompts/ のみ)、テストの無効化・成果物のコミット漏れ・
ABI 同期崩れなどが機械的に検知されない状態です。AI エージェント主体の開発のため、
機械的検証が品質の主防衛線になります。

## 構成方針 (段階導入)

`.github/workflows/ci.yml` を新規作成し、push / pull_request で起動する2ジョブ構成にする:

### Job 1: desktop-build-test (windows-latest)
- vcpkg のセットアップ。リポジトリは `-b 2026.03.18` の vcpkg を build/vcpkg にクローンする方式
  (README.md 準拠)。**vcpkg のビルドキャッシュが最重要**: `VCPKG_BINARY_SOURCES` の GitHub Actions
  キャッシュ (`x-gha`) または actions/cache を必ず設定する (初回は長時間、2回目以降を数分にする)
- CMake configure は Dawn を回避して時間短縮する:
  `cmake -B build/ci -DLUMINO_USE_WEBGPU=OFF -DLUMINO_BUILD_EXAMPLES=OFF -DCMAKE_TOOLCHAIN_FILE=...`
- ビルド (Release) 後、GPU 不要のテストのみ実行:
  `LuminoBase_test`, `LuminoShader_test`, `LuminoCore_test` を直接実行する
  (`LuminoC_test` は実 GPU 描画があるため CI では除外。除外理由をワークフローにコメントで明記)
- 注意: LUMINO_USE_WEBGPU=OFF 構成がそもそもビルド可能かをまずローカルで確認し、
  壊れていたら CMake 側を修正すること (このオプションは普段使われていない可能性がある)

### Job 2: wasm-build (ubuntu-latest)
- emsdk 5.0.5 のセットアップ (mymindstorm/setup-emsdk 等の action か、README 手順の再現。キャッシュ推奨)
- `python scripts/build_wasm.py` 一式 (configure/build)。
  注意: WASM ビルドは `packages/LuminoCore/shaders/*.lcsh.inl` がコミット済みであることに依存する。
  未コミットでビルドが失敗する場合は、その事実を報告し、改善案 #4 (棚卸し) の完了を前提条件とする
- 続けて `packages/luminojs` で `npm ci && npm run build` (tsc + rollup)

## タスク

1. 上記2ジョブの ci.yml を作成する
2. 可能な範囲でローカル検証する (LUMINO_USE_WEBGPU=OFF ビルド、テスト直接実行、build_wasm.py)。
   Actions 上の動作は push しないと確認できないため、ワークフローの構文チェックと
   ローカルで再現できるステップの検証までを行い、残る不確定要素を報告する
3. README.md に CI バッジとステータスの説明を追加する (任意)

## 制約

- ワークフローは最初から完璧を狙わず「確実に緑になる最小構成」を優先する。
  GPU テスト・ビジュアル回帰・成果物同期チェック (改善案 #10, #15) は後続フェーズとして
  ci.yml 内に TODO コメントで居場所だけ用意する
- git commit / push は作らない。ファイル作成と検証結果、残課題の報告で終了
