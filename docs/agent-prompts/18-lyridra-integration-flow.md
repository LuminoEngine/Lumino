# 依頼: LYRIDRA 連携フローの強化 (改善案 #18)

推奨モデル: Opus / 目安: 半日-1日 / 人間確認: 不要
**作業リポジトリ: 上位リポジトリ `c:\Proj\dragon-baby-engine` (Lumino 本体ではない)**

---

あなたは LYRIDRA (dragon-baby-engine) と Lumino の連携フローを堅牢化します。
まず上位リポジトリの `externals/README.md`、`scripts/copyLumino.ts`、
`packages/engine/scripts/compileShader.ts`、
`.build/Lumino/docs/architecture-review-2026-07.md` の改善案 #18 を読んでください。

## 背景

Lumino の成果物は「.build/Lumino で手動ビルド → `npm run copy-lumino` で externals/luminojs へ
手動コピー → git にバイナリコミット」というフローで LYRIDRA に届きます。現状の問題:
- copy-lumino がビルドパイプライン外の手動実行で、忘れると古い WASM のまま気づかない
- externals/luminojs/package.json の version が 0.0.0 固定で、どの Lumino コミット由来か追跡不能
- luminojs には `Runtime.getBuildTimestamp()` (WASM ビルド時刻を返す確認用 API) があるのに未使用
- compileShader.ts は .build/Lumino のパス (Windows 固定) を直接参照し、無い環境では
  警告のみでシェーダコンパイルを静かにスキップする (実行時に直描きフォールバックし異常に気づけない)

## タスク

1. ビルドスタンプの活用:
   - Lumino バックエンド初期化時 (`Runtime.initialize()` 成功後の適切な場所。Engine.ts または
     LuminoGraphicsBackend.ts) に `Runtime.getBuildTimestamp()` を呼び、`console.info` で
     「Lumino WASM build: <timestamp>」を出力する
2. コピー時のトレーサビリティ:
   - `scripts/copyLumino.ts` を拡張し、コピー実行時に `.build/Lumino` で
     `git rev-parse HEAD` と `git status --porcelain` (dirty 判定) を取得して、
     `externals/luminojs/BUILD_INFO.json` (コミットハッシュ / dirty フラグ / コピー日時 /
     WASM ファイルサイズ) を書き出すようにする。コピー完了ログにも同情報を表示する
3. シェーダコンパイルの静かな失敗の防止:
   - `compileShader.ts` に厳格モード (環境変数 `LUMINO_SHADER_STRICT=1` または `--strict`) を追加し、
     luminosc が見つからない場合に警告ではなくエラー終了できるようにする。既定挙動は変えない
   - スキップ時の警告メッセージに「ポストエフェクトが直描きフォールバックになる」という
     実行時影響を明記する
4. ドキュメント: `externals/README.md` に更新フロー全体 (Lumino のビルド → copy-lumino →
   BUILD_INFO.json の確認方法) を追記する
5. 検証:
   - `npm run copy-lumino` を実行し BUILD_INFO.json が生成されることを確認
     (.build/Lumino のビルド済み成果物が古い場合はその旨を報告し、コピー内容はコミットしない)
   - engine のビルド (`npm run build` 等、上位リポジトリの手順に従う) が通ること

## 制約

- 上位リポジトリのコーディング規約・既存スクリプトのスタイルに従う (TypeScript)
- luminojs パッケージ本体 (.build/Lumino 側) は変更しない
- git commit は作らない。変更ファイルと検証結果を報告して終了
