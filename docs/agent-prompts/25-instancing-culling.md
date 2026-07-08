# 依頼: メッシュインスタンシング + ビューカリング (改善案 #25)

推奨モデル: Opus / 目安: 2-4日 / 人間確認: 着手判断 (改善案 #21 の計測結果を見てから)

**前提: 改善案 #21 段階1 (計測) の結果、メッシュ描画がボトルネックになる規模が確認されてから
着手すること。計測前に依頼された場合は、まずその旨を確認してください。**

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) の描画性能改善として、
BatchRenderingPlan.md の未実装フェーズ (Phase 2: インスタンシング、Phase 3: ビューカリング) を
実装します。
まず以下を読んでください:
- リポジトリ直下 `BatchRenderingPlan.md` (元計画。ただし Phase 1 は計画と異なり Renderer 内部に
  統合実装済みなので、現状コードを正とする)
- `packages/LuminoCore/src/Graphics/Batch.cpp` (現状: flushSubMeshGroup は 1 サブメッシュ 1 draw)
- `packages/LuminoCore/include/LuminoCore/Graphics/rhi/Rhi.hpp` (RHI 抽象)
- `docs/architecture-review-2026-07.md` 改善案 #25

## タスク

### Phase 2: メッシュインスタンシング
1. RHI にインスタンス描画を追加する:
   - RenderPass::draw 系にインスタンス数/インスタンスバッファの概念を追加
     (WebGPU: vertex buffer の step mode instance / Vulkan: VkVertexInputRate)
   - 両バックエンド (webgpu/, vulkan/) に実装。インスタンスデータはまずワールド行列 (float4x4) のみ
2. Batch に同一 (メッシュ, サブメッシュ, マテリアル) の DrawCommand を集約する経路を追加し、
   2件以上まとまる場合にインスタンス描画へ切り替える。SortKey による描画順を壊さないこと
   (連続する同一キーのみマージする、現行スプライトバッチのマージ規則と同じ考え方)
3. シェーダ対応: lumino.slang の VSInput / ln_World の扱いにインスタンス行列を統合する方法を設計する
   (per-instance 属性か、instance index + ストレージバッファか。WebGPU/Vulkan 両対応で決める)。
   既存シェーダの互換性への影響を必ず報告する

### Phase 3: ビューカリング
4. DrawCommand に AABB (メッシュ生成時に計算してキャッシュ) を持たせ、flush 時にカメラの
   視錐台 (平行投影/透視の両方) との判定で棄却する。カメラなしパスではスキップ
5. カリングの ON/OFF を制御できるようにする (RenderPassDesc かグローバル設定。ABI 変更を伴う場合は
   改善案 #10 の検証値の更新も忘れないこと)

### 検証
- `packages/LuminoCore/test/Test_BatchRendering.cpp` にインスタンス集約とカリング判定の
  ユニットテストを追加 (Batch は GPU 非依存でテスト可能な設計になっている)
- ビジュアル回帰: Test_Graphics に「多数メッシュがインスタンシング経路でも同じ絵になる」テストを追加
- 性能確認: examples (BatchRendering 等) で draw call 数の減少を LNDebug_GetGraphicsProfiler で確認

## 制約

- 既存の描画結果 (ピクセル) を変えないこと。最適化のみ
- C++ は UTF-8 BOM 付き・LF、コメント日本語、`—` `–` `〜` 禁止
- git commit は作らない。フェーズごとに結果を報告
