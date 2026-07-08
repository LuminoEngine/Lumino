# 依頼: FFI 描画経路の再設計 - 段階1: 計測 (改善案 #21)

推奨モデル: 段階1 (計測) = Opus / 段階2 (設計判断) = Fable 5 メインセッション / 段階3 (実装) = Opus
目安: 全体 2-5日 (段階1は1日以内) / 人間確認: 段階1の結果を見て投資判断 (最重要の意思決定)

**前提: 改善案 #11 (WASM Release ビルド) が完了していること。Debug ビルドでの計測は無意味です。**
**注意: 改善案 #7 (deprecated API 削除) より先に本計測を行うこと (削除対象の一括投入 API が比較基準になるため)。**

このプロンプトは段階1 (計測) のみを依頼するものです。計測結果の報告で停止してください。

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) で、WASM 環境における
FFI (JS から C API) 呼び出しオーバーヘッドの定量計測を行います。
まず `AGENTS.md` の「古い実装と教訓」節と `docs/architecture-review-2026-07.md` の 2.2 節 R4・改善案 #21 を読んでください。

## 背景

AGENTS.md には旧 Lumino の教訓として「API 関数の呼び出し回数を可能な限り抑え、描画コマンドは
JS 側でバイナリ構築して一括フラッシュしたい」と明記されていますが、現行実装は
draw 1回 = FFI 1回 (LNRenderer_DrawSprite は約17引数) です。LYRIDRA のタイルマップは
タイル1枚ごとに drawSprite を呼ぶため、大規模マップでのスケーラビリティが未知数です。
この計測結果が「バイナリコマンドリスト方式へ投資するか」という本リポジトリ最大の
設計判断の材料になります。

## タスク

1. `packages/luminojs-examples` に計測用サンプル `bench_sprites` を追加する:
   - 同一マテリアルのスプライトを N 枚 drawSprite で描画 (N = 1,000 / 5,000 / 20,000 を切替可能)
   - 計測項目 (フレームあたり、直近120フレームの平均と p95):
     a. JS 側の drawSprite ループ所要時間 (performance.now)
     b. beginFrame から endFrame までの JS 側総時間
     c. LNDebug_GetGraphicsProfiler の drawCallCount / lastFrameTimeMs
   - 画面に数値をオーバーレイ表示し、コンソールにも表形式で出力する
2. 比較基準の計測 (可能な範囲で):
   - deprecated の `LNDrawCommandBuffer_DrawSprites` (LNSpriteCommand 配列の一括投入) が
     luminojs から呼べるなら、同条件で一括投入方式の時間も計測する。
     バインドされていない場合は cwrap を計測用に一時追加して構わない (本実装ではない)
   - これにより「FFI 呼び出し回数 N 回 vs 1 回」の差分が直接得られる
3. 実行条件: Release WASM (`python scripts/build_wasm.py --release`) + Chrome。
   Debug WASM でも参考値として1条件だけ計測し、Release との差を示す
4. レポート作成: 以下を含む計測報告を返す
   - 環境 (CPU / ブラウザ / ビルド構成)
   - N ごとの各計測値の表
   - FFI 1呼び出しあたりの推定コスト (一括投入との差分から算出)
   - 所見: 「LYRIDRA の想定規模 (例: 100x100 タイルマップ = 数千〜1万スプライト/フレーム) で
     FFI オーバーヘッドがフレーム予算 (16.6ms) の何割を占めるか」
   - **推奨はするが決定はしない**: バイナリコマンドリスト化の要否について事実ベースの所見を述べ、
     設計判断は依頼者と Fable 5 セッションに委ねる

## 制約

- 本体コード (luminojs / LuminoC) への変更は計測に必要な最小限にとどめ、変更点をすべて報告する
  (計測用の一時変更は原則サンプル内に閉じ込める)
- git commit は作らない

---

## 参考: 段階2以降の見通し (このプロンプトでは実施しない)

計測の結果、投資が正当化される場合の設計方針 (Fable 5 セッションで詳細化):
- JS 側で Uint8Array にスプライトコマンドを蓄積し、`LNRenderer_FlushSprites(ptr, count)` 一発で送る
- フォーマットには バージョン/count/境界検証を含め、改善案 #10 の ABI 検証と整合させる
- luminojs の drawSprite は API 互換のまま内部をバッファ蓄積に差し替える
- 旧 LNDrawCommandBuffer_DrawSprites の設計 (LNSpriteCommand 配列) を Renderer 統合型として再設計する
