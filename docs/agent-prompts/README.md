# AIエージェント依頼プロンプト集

`docs/architecture-review-2026-07.md` の改善案 #1-#27 を、それぞれ独立した AI エージェントに依頼するためのプロンプトです。
各ファイルの内容をそのままプロンプトとして渡してください (エージェントは前提知識ゼロで開始する想定で書かれています)。

## 推奨モデルの使い分け

- **Sonnet**: 作業内容が完全に特定されている機械的なタスク (修正箇所・期待結果が明記済み)
- **Opus**: 複数ファイル横断・調査を伴う実装、CI などの試行錯誤が必要なタスク
- **Fable 5 (メインセッション)**: 設計判断そのもの。該当プロンプトは「設計フェーズで停止して人間+Fable 5 のレビューを受ける」構成になっています

## 一覧

| # | ファイル | 内容 | 推奨モデル | 目安 | 人間確認 |
|---|---|---|---|---|---|
| 1 | 01-sortmode-abi-fix.md | sortMode ABI 不整合修正 | Sonnet | 30分 | - |
| 2 | 02-revive-disabled-matrix-tests.md | #if 0 テスト群の整理・復活 | Opus | 1-2h | 実装バグ発見時 |
| 3 | 03-fix-twosprites-visual-test.md | TwoSprites 回帰テスト修正 | Sonnet | 15分+ | 参照画像の目視 |
| 4 | 04-gitignore-and-untracked-triage.md | .gitignore 修正 + 棚卸し | Sonnet | 1-2h | コミット対象の承認 |
| 5 | 05-pin-webgpu-distribution.md | WebGPU-distribution 固定 | Sonnet | 15分 | - |
| 6 | 06-fix-build-wasm-emsdk-message.md | build_wasm.py 案内文修正 | Sonnet | 10分 | - |
| 7 | 07-public-abi-cleanup.md | 公開 ABI の掃除 | Opus | 2-3h | 実施タイミング |
| 8 | 08-wasm-unsupported-api-contract.md | WASM 非対応 API の契約明記 | Sonnet | 1h | - |
| 9 | 09-ci-github-actions.md | CI 導入 | Opus | 1-2日 | Actions 課金枠 |
| 10 | 10-abi-layout-verification.md | ABI レイアウト同期検証 | Opus | 0.5-1日 | - |
| 11 | 11-wasm-release-build.md | WASM Release ビルド経路 | Sonnet | 半日 | - |
| 12 | 12-wasm-smoke-test.md | WASM スモークテスト | Opus | 1-2日 | - |
| 13 | 13-c-api-error-messages.md | C API エラー詳細伝搬 | Sonnet | 半日 | - |
| 14 | 14-handle-ownership-contract.md | 所有/借用ハンドル契約 | Opus | 半日 | - |
| 15 | 15-shader-artifact-policy.md | シェーダ成果物方針統一 | Opus | 半日-1日 | コミット方針 |
| 16 | 16-vcpkg-prune.md | vcpkg 未使用依存削減 | Sonnet | 1-2h+ビルド | - |
| 17 | 17-docs-refresh.md | ドキュメント再整備 | Sonnet | 半日 | - |
| 18 | 18-lyridra-integration-flow.md | LYRIDRA 連携フロー強化 | Opus | 半日-1日 | - |
| 19 | 19-ctest-granularity.md | ctest 粒度改善 | Sonnet | 30分 | - |
| 20 | 20-luminojs-vitest.md | luminojs 単体テスト | Sonnet | 1日 | - |
| 21 | 21-ffi-command-buffer.md | FFI 描画経路再設計 (段階1=計測) | Opus (計測) / Fable 5 (設計判断) | 2-5日 | 投資判断 |
| 22 | 22-hd2d-lighting.md | HD-2D ライティング基盤 | Fable 5 (設計) / Opus (実装) | 3-7日 | 設計承認 |
| 23 | 23-posteffect-chain.md | ポストエフェクトチェーン | Fable 5 (設計) / Opus (実装) | 3-5日 | 責務境界の決定 |
| 24 | 24-binding-generator.md | バインディング自動生成 | Opus | 2-4日 | 切替判断 |
| 25 | 25-instancing-culling.md | インスタンシング+カリング | Opus | 2-4日 | #21計測後に判断 |
| 26 | 26-device-lost.md | デバイスロスト対応 | Fable 5 (設計) / Opus (実装) | 3-5日 | 設計承認 |
| 27 | 27-lyridra-backend-parity.md | LYRIDRA 機能パリティ | Opus | 3-5日 | capture方式の決定 |

## 依存関係と推奨着手順

```
#1 (sortMode修正)
  -> #10 (ABI検証。#1修正後の正しいサイズ224を基準値にする)
  -> #9 (CI Phase1。#2, #3 のテスト修正を先に済ませるとCIが最初から緑になる)
#11 (Releaseビルド) -> #21 段階1 (計測。Releaseビルドがないと計測が無意味)
#21 の計測は #7 (deprecated削除) より先に行うこと
  (削除対象の LNDrawCommandBuffer_DrawSprites が一括投入方式の比較基準として有用なため)
#22 (ライティング) -> #23 (ポストチェーン) の順を推奨 (要件が明確になる)
#10 -> #24 (自動生成は検証機構がある状態で切り替える)
```

## 全プロンプト共通の前提

各プロンプトに個別に記載済みですが、依頼時の共通事項:

- 作業リポジトリ: `c:\Proj\dragon-baby-engine\.build\Lumino` (#18, #27 のみ上位リポジトリ `c:\Proj\dragon-baby-engine`)
- エージェントは最初に `AGENTS.md` と `docs/architecture-review-2026-07.md` の該当節を読む
- C++ ファイルの新規作成・編集は UTF-8 BOM 付き (BOM が無いと /WX + C4819 でビルド失敗)
- 特に指示しない限り git commit は作らせない (変更と検証結果の報告で終了)
