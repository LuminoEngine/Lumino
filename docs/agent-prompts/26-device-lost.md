# 依頼: デバイスロスト対応戦略 (改善案 #26)

推奨モデル: フェーズA (設計) = Fable 5 メインセッション / フェーズB (実装) = Opus
目安: 3-5日 / 人間確認: 設計承認後に実装へ

このプロンプトはフェーズA (設計) から開始し、設計ドキュメントの提出で一度停止する構成です。
優先度 C (エディタ実運用開始前まで、コンシューマ機移植前には必須) のタスクです。

---

あなたは Lumino のデバイスロスト (GPU デバイス喪失) 対応を設計します。
まず以下を読んでください:
- `packages/LuminoCore/src/Graphics/rhi/webgpu/WebGPUDevice.cpp` のデバイスロストコールバック
  (177-186行付近。現状ログのみ)
- `packages/LuminoBase/include/LuminoBase/Result.hpp` (ErrorCode::DeviceLost は定義済みだが未使用)
- `packages/luminojs/src/ResidencyManager.ts` と Texture/Material/Mesh の ResidentResource 実装
  (JS 側にソースデータを保持し GPU リソースを遅延生成する既存レイヤー)
- `AGENTS.md` の「コアモジュールのメモリ使用の注意点」(再アップロード用データは JS 側管理を推奨)
- `docs/architecture-review-2026-07.md` 改善案 #26

## 背景

- WebGPU: デバイスロストコールバックはあるがログを出すだけで復旧しない
- Vulkan: 検知すら無い (VK_ERROR_DEVICE_LOST のハンドリングなし)
- ブラウザのタブ復帰・GPU ドライバ更新・OS のGPUリセットで全損する
- 好材料: luminojs の ResidencyManager は「ソースデータから GPU リソースを再生成する」構造を
  既に持っており、設計の半分はできている

## フェーズA: 設計 (このプロンプトの範囲)

設計ドキュメント `docs/plans/device-lost-design.md` を作成して停止する。含めるべき内容:

1. 検知: WebGPU (device lost callback) / Vulkan (VK_ERROR_DEVICE_LOST の伝搬経路) それぞれの
   検知ポイントの列挙。現行コードの該当箇所を file:line で示す
2. 伝搬: C API へどう伝えるか。案の比較:
   - LNResult に LN_ERROR_DEVICE_LOST を追加し、BeginFrame 等が返す (ポーリング型)
   - コールバック登録 API (プッシュ型)。WASM (Emscripten) でのコールバックの扱いも検討
3. 復旧プロトコル: クライアント (luminojs) 側の手順を状態遷移図で定義する。
   例: DeviceLost 検知 → 全ハンドル無効化の扱い (ObjectRegistry との整合) →
   GraphicsContext 再作成 → ResidencyManager が全リソース再生成 → 描画再開。
   「どのハンドルが生き残り、どれが無効になるか」の契約を明確にすること (最重要)
4. 各リソース種別の再生成可否の表: Texture (ソース保持あり = 可) / RenderTarget (揮発 = 再作成のみ) /
   Mesh / Material / 動的メッシュ。JS 側にソースが無い C++ 直クライアント (将来のコンシューマ機) の
   扱いも一言触れる
5. テスト戦略: 実デバイスロストの再現は困難なため、
   - WebGPU: device.destroy() を使った擬似ロストのテストが可能か調査
   - 「強制ロスト」デバッグ API (LNDebug_SimulateDeviceLost) の追加を提案に含めるか検討
6. 実装フェーズ分割 (B1: 検知+エラー伝搬、B2: luminojs 復旧フロー、B3: テスト) と完了条件

設計上の疑問点は勝手に決めず「要判断事項」として列挙すること。

## 制約

- フェーズA ではコードを変更しない (調査のための読み取りのみ)
- git commit は作らない

## ユーザー要望

デバイスロスト検出した場合、 BeginFrame は何らかのエラーを返すようにしつつ、 LuminoCore 側で復旧フローに入れるようにしたい。
