# 依頼: LYRIDRA Lumino バックエンドの機能パリティ解消 (改善案 #27)

推奨モデル: Opus / 目安: 3-5日 (サブタスク分割推奨) / 人間確認: captureBackbuffer の方式決定
**作業リポジトリ: 上位リポジトリ `c:\Proj\dragon-baby-engine` (一部 Lumino 本体も関与)**

このタスクは独立性の高い4つのサブタスクで構成されます。1エージェントに順に依頼しても、
サブタスクごとに別エージェントへ分割依頼してもかまいません (a と b は Sonnet でも可)。

---

あなたは LYRIDRA (dragon-baby-engine) の Lumino グラフィックスバックエンドを、pixi バックエンドと
同等の見た目になるよう補完します。
まず `.build/Lumino/docs/architecture-review-2026-07.md` の改善案 #27 と、上位リポジトリの
`packages/engine/src/browser/graphics/backend-lumino/` 一式、`docs/plans/lumino-viewport-compositing.md`
を読んでください。

## 背景 (確認済みのギャップ)

Lumino バックエンド選択時 (`graphicsBackend: "lumino"`)、以下が pixi と非等価です:
- a. canvas セレクタが `"#main-canvas"` 固定 (LuminoGraphicsBackend.ts:70 付近) で、エディタが
  独自に渡す canvas (id なし) を掴めない
- b. Sprite の colorHue / brightness / blendColor / blendMode が無視される
  (graphics/Sprite.ts の onRender は色を常に 1,1,1,opacity で渡す。BaseSprite._updateColorFilter は
  pixi 専用フィルタにしか反映しない)
- c. TilingSprite / TransitionSprite に onRender 実装がなく、Lumino では何も描画されない
- d. captureBackbuffer がスタブ (LuminoGraphicsBackend.ts:103-110、空テクスチャを返す)。
  フェード/ワイプ遷移が機能しない

## サブタスク

### a. canvas セレクタの引数化 (Sonnet 可)
- `GraphicsContext.createFromCanvas` は CSS セレクタを受けるので、Lumino 側の対応は不要。
  LYRIDRA 側で options.canvas から一意なセレクタを解決する (canvas に id が無ければ採番して付与する等)
- エディタ経路 (GameWorldCanvas.ts 等の複数キャンバス) で初期化が通ることを確認。
  ただし「同時に複数の GraphicsContext を作れるか」は Lumino 側の制約を調査し、
  未対応なら制約として報告する (無理に対応しない)

### b. Sprite の色調・ブレンド反映 (Sonnet 可)
- blendColor / brightness: drawSprite の頂点カラー引数 (colorR/G/B/A) に合成して渡す。
  pixi 版 (_PixiColorFilter) の計算式を確認し、同じ見た目になる式にすること
- blendMode: VisualObject の blendMode を Lumino の Material.setBlendMode (Normal/Alpha/Add 等) に
  マッピングする。マテリアルはブレンドモード別にキャッシュが必要か調査して適切に実装
- colorHue (色相回転): 頂点カラーでは表現できない。対応方法 (専用シェーダ or カラーマトリクス
  uniform) を検討し、工数が大きければ本サブタスクでは「未対応の明示 (警告ログ)」に留めて
  改善案 #22/#23 のシェーダ拡張と合流する提案を報告する

### c. TilingSprite の実装 (Opus)
- `graphics/TilingSprite.ts` に Lumino 経路の onRender を実装する。
  Lumino の drawSprite は「transform 共有 + offset で複数矩形を軽量配置」する設計
  (lumino.h の DrawSprite コメント参照) になっており、タイリングに適する
- UV リピートの扱い (uvX/Y/W/H で 1 超の値が使えるか = サンプラーの wrap 設定) を Lumino 側で
  調査し、不可なら矩形分割で実装する
- TransitionSprite は d に依存するため、ここでは調査のみ (実装は d の後)

### d. captureBackbuffer の設計と実装 (Opus, 人間確認あり)
- Lumino の LNGraphicsContext_CaptureBackbuffer は WASM 未対応 (Emscripten で除外) のため使えない。
  代替案を比較して提案し、承認を得てから実装する:
  1. 最終合成を常にオフスクリーン RT に描き、それを capture 用テクスチャとして直接使う
     (読み戻し不要。GPU 内で完結。LuminoGraphicsPipeline は既に RT 合成構造を持つので親和性が高い)
  2. WebGPU の buffer readback を Lumino に実装して CPU 経由で返す (遅いが汎用)
- 遷移 (TransitionSprite) が要求するのは「直前フレームの絵をテクスチャとして使う」ことなので、
  案1 が本命であることをレビューは示唆している。設計提案 → 承認 → 実装の順で進めること

## 検証

- pixi バックエンドと Lumino バックエンドで同一シーンを表示し、スクリーンショット比較で
  色・ブレンド・タイリングが一致することを確認 (`packages/game` の起動手順は Bootstrapper.ts 参照)

## 制約

- 上位リポジトリの規約に従う。pixi 側の挙動を正として合わせる (後方互換が目的)
- Lumino 本体 (.build/Lumino) の変更が必要になった場合は、変更内容を明確に分離して報告する
- git commit は作らない。サブタスクごとに結果を報告
