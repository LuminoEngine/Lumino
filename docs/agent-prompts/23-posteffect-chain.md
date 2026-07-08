# 依頼: ポストエフェクトチェーン管理層 (改善案 #23)

推奨モデル: フェーズA (設計) = Fable 5 メインセッション / フェーズB (実装) = Opus
目安: 3-5日 / 人間確認: 「C++ 側か TS 側か」の責務境界の決定 (設計フェーズで提案し承認を得る)

このプロンプトはフェーズA (設計) から開始し、設計ドキュメントの提出で一度停止する構成です。
改善案 #22 (ライティング基盤) の後に着手するのが望ましい (要件が明確になるため)。

---

あなたは Lumino に SSAO/SSR/ブルーム/トーンマップ等を連鎖させるポストエフェクトチェーンの
管理層を設計・実装します。
まず以下を読んでください:
- `docs/architecture-review-2026-07.md` 改善案 #23
- `packages/LuminoC/examples/SSR/Main.cpp` (現状の手組み3パス構成)
- `packages/luminojs-examples/src/ssr/main.ts` (TS 版の手組み)
- `packages/luminojs/src` (Texture.createRenderTargetEx, Renderer.drawScreenRect, Material)
- AGENTS.md の「コアモジュールのメモリ使用の注意点」(リソースは JS 側管理を推奨)
- 上位リポジトリの `packages/engine/src/browser/graphics/backend-lumino/LuminoRenderTargetPool.ts`
  (LYRIDRA 側に既に RT プールが存在する)

## 背景

エフェクトの部品 (オフスクリーン RT、名前付きパス、drawScreenRect、setNamedTexture) は揃っており
SSR サンプルは動きますが、RT のピンポン・中間バッファのプール・パス順序の管理は全てクライアントの
手配線です。HD-2D で「SSAO → ライティング合成 → SSR → ブルーム → トーンマップ」を実用化するには
管理層が必要です。

## フェーズA: 設計 (このプロンプトの範囲)

設計ドキュメント `docs/plans/posteffect-chain-design.md` を作成して停止する。含めるべき内容:

1. **配置の提案 (最重要の要判断事項)**: 管理層を C++ (LuminoCore) と TS (luminojs) の
   どちらに置くか。レビューでは以下の理由から TS 側を暫定推奨している:
   - AGENTS.md の方針 (WASM の32bitメモリ制約のため、リソース管理は JS 側推奨)
   - LYRIDRA に既に _LuminoRenderTargetPool (TS) があり、二重実装を避けたい
   - FFI 呼び出しはパスあたり数回で済み、オーバーヘッドが問題にならない
   両案の比較表 (メモリ管理 / 性能 / C API だけ使う他言語クライアントへの再利用性 / 保守性) を作り、
   推奨案を提示すること
2. API 設計 (TS 案の場合): `PostEffectChain` クラスの形。
   - エフェクトの登録 (Material + 入出力宣言) とパス順序
   - RT プール (サイズ/フォーマット別の貸出、ピンポン、リサイズ時の flush)。
     LYRIDRA の _LuminoRenderTargetPool との関係 (luminojs に汎用プールを移管し LYRIDRA が
     それを使う形にできるか) を調査して方針を出す
   - 入力: シーンカラー / 深度 / (将来) GBuffer。改善案 #22 のライティングとの接続点
   - エフェクト無効時にゼロコストになる設計 (LYRIDRA 側の「効果が無ければコストゼロ」原則と整合)
3. 標準エフェクトの提供範囲: 第1弾は SSR (既存サンプルの移植) + ブルーム + トーンマップを提案。
   各エフェクトの .slang とパラメータ定義
4. 実装フェーズ分割と完了条件 (B1: チェーン骨格 + SSR 移植、B2: ブルーム/トーンマップ、
   B3: LYRIDRA 統合の接続方針)

## フェーズB: 実装 (設計承認後、別途依頼)

- luminojs-examples に「チェーンでエフェクトを並べ替えられる」デモを追加し実証する
- 既存 SSR サンプルをチェーン API に移植して手組みコードと同じ絵が出ることを確認する

## 制約

- Lumino のスコープを守る: パス名の意味付けやシーングラフは上位エンジンの責務 (README.md の方針)。
  チェーン層は「RT とパスの機械的な管理」に徹する
- git commit は作らない
