# Lumino アーキテクチャレビュー (2026-07-06)

- 対象: ブランチ `dev-lyric` (HEAD: `e99ee15d9` "Support sort order")
- 方法: 5系統の並列調査 (LuminoCore描画基盤 / C API+JSバインディング / シェーダツールチェーン / ビルド・テスト・リポジトリ衛生 / LYRIDRA統合側) + 重大指摘のレビュアーによる一次ソース裏取り
- ゴール: LYRIDRA への組み込みと HD-2D 表現 (平行投影でのライティング/シャドウ/SSAO/SSR) に向けて、設計・実装が持続可能かを評価し、改善計画を立てる
- 本レビューはコードを一切変更していない。計画書 (本ファイル) の新規作成のみ

---

## 1. 構造マップ

### 1.1 レイヤー構造

```
[LYRIDRA (上位リポジトリ packages/engine)]
  GraphicsBackend 抽象 ("pixi" | "lumino")
    _LuminoGraphicsBackend (リソース管理)
    _LuminoGraphicsPipeline (Viewport合成, RPGツクール互換スコープ)
    _LuminoRenderTargetPool (RT プール + 遅延GC)
        | import * as LUMINO from "luminojs" (file:../../externals/luminojs)
        v
[luminojs (TypeScript, 手書きバインディング)]
  Runtime (cwrap で 38/66 関数をバインド, ASYNCIFY 初期化)
  クラスラッパー (Texture/Material/Mesh/Camera/Renderer)
  ResidencyManager (GPUリソース遅延生成 + 自動evict = JS側GC)
  構造体を SIZEOF_* 定数で WASM 線形メモリへ手動シリアライズ
        | FFI (draw 1回 = C関数呼び出し1回)
        v
[LuminoC (C API / FFI境界)]
  lumino.h (66関数, LN<Type>_<Verb>, LNResult + outハンドル)
  LuminoAPI.cpp (1535行, 完全手書きの橋渡し)
        v
[LuminoCore (12,271行)]
  Runtime/ObjectRegistry (世代付きハンドル, 上限65,534)
  Graphics/Renderer (837行: パス管理, 動的UBO, BindGroupキャッシュ, ステンシル)
  Graphics/Batch (SortKey, スプライト頂点生成, マテリアル境界分割)
  Graphics/Material・PipelineCache・DynamicUniformAllocator
  rhi/ (WebGPUスタイルのRHI抽象, 545行)
    rhi/vulkan (~2,700行, デスクトップ既定)
    rhi/webgpu (~1,900行, WASM/ブラウザ + デスクトップDawn)
        v
[LuminoBase] RefObject/Ref, Result(tl::expected), Logger, math
[LuminoShader] ShaderCompiler2 (Slang, 4ターゲット同時), UnifiedShaderSerializer2 (.lcsh "lcs2" v2)
```

### 1.2 パッケージ一覧

| パッケージ | 役割 | 規模 | 状態 |
|---|---|---|---|
| LuminoBase | 参照カウント/Result/数学 | 小 | 安定。テストは数学のみ |
| LuminoShader | .lcsh 形式 + Slangコンパイラ | ~1,600行 | 良質。フォーマットはバージョン管理済み (v1→v2 移行実績あり) |
| LuminoCore | RHI + 描画中核 | 12,271行 | 実装は充実。Renderer.cpp に責務集中 |
| LuminoC | C API (FFI境界) | lumino.h 1,159行 + 実装1,535行 | 手書き。deprecated 3系統が残存 |
| luminojs | TSバインディング | ~27ファイル | 手書き同期。**HEADで同期崩れバグあり (後述)** |
| luminosc | シェーダコンパイラCLI | 111行 (本体はLuminoShader) | 健全 |
| luminosc-npm / -x64-windows | npm配布 | - | Windowsのみ。.gitignore 事故でコミット不能状態 |
| luminojs-examples | Viteサンプル6種 | 小 | 自動テストなし |

### 1.3 シェーダパイプライン

```
*.slang --(luminosc / Slang)--> *.lcsh ("lcs2" v2: SPIRV+DXIL+WGSL+Metal 同梱 + リフレクション)
                                *.lcsh.inl (C++埋め込み用hex配列)
- 組み込みシェーダ (Unlit/BasicLit/StencilMask): デスクトップCMakeで自動生成
- WASMビルド: 生成ステップなし。コミット済み .lcsh.inl に依存 (現在未コミット = 構造矛盾)
- examples: 手動生成・手動コミット運用 (SSR.lcsh が実際にコミット漏れ中)
```

### 1.4 LYRIDRA への配線 (更新フロー)

```
.build/Lumino (手動clone) -> 手動ビルド (build_wasm.py, 常に -O0 -g3)
  -> npm run copy-lumino (手動実行, ビルドパイプライン外)
  -> externals/luminojs にビルド済みバイナリをコミット (バージョン照合なし)
シェーダ: compileShader.ts が .build/Lumino のパスを直接参照 (Windows固定)
  -> 存在しない環境では警告のみで静かにスキップ -> 実行時は直描きフォールバック
```

---

## 2. 総合評価

### 2.1 良い点 (この設計は維持すべき)

1. RHI が WebGPU 準拠の素直な抽象で、Vulkan/WebGPU 両実装済み。新バックエンド (コンシューマ機) 追加が現実的
2. 世代付きハンドル + ObjectRegistry によるダングリング検出。ハンドル採用理由もコメントで明文化
3. PipelineCache / RenderPass ラッパのライフサイクル安全設計 (破棄コールバック + alive ガード)
4. .lcsh フォーマットのバージョニングが実装され、実際に v1→v2 移行を経験済み
5. 実GPU描画 + 画像比較のビジュアル回帰テスト基盤 (計89ケース、うち13が実描画)
6. 設計判断の理由がコメント・ドキュメントに残る文化 (SortKey採用理由、Slangのトラブル記録等)
7. luminojs の ResidencyManager と WASM メモリ成長への追従処理は堅実
8. LYRIDRA 側もバックエンド抽象/合成パイプライン/RTプールの責務分離が明確

### 2.2 重大リスク Top 5

| # | リスク | 根拠 |
|---|---|---|
| R1 | **C API と luminojs の手動同期が既に壊れている** (sortMode 追加が未反映、毎フレームのヒープ外読み取り = 未定義動作)。検証機構がないため再発必至 | 検証済み (3.1) |
| R2 | **CI 不在**。テスト無効化 (#if 0)、成果物コミット漏れ、無意味化した回帰テストが誰にも検知されず蓄積している | 検証済み (3.2, 3.3) |
| R3 | **クリーンチェックアウトでビルドが壊れる構造**。WASMビルドが未コミットの .lcsh.inl に依存。.gitignore の [Bb]in/ が npm パッケージのソースを無視 | 調査で確認 |
| R4 | **AGENTS.md の設計教訓「JSでバイナリコマンドリスト構築→一括フラッシュ」が未実装**。現状は draw 1回 = FFI 1回で、旧Luminoで問題化したオーバーヘッド構造が形を変えて残存 | 調査で確認 |
| R5 | **HD-2D の残り工数は上位層に集中**。RHI/マテリアル/マルチパスは準備済みだが、Scene UBO 固定小構造・シャドウマップ機構ゼロ・ポストチェーン管理層なし | 調査で確認 |

---

## 3. 検証済みの問題 (レビュアーが一次ソースで確認したもの)

### 3.1 sortMode ABI 不整合 (現在進行形の未定義動作)

- `packages/LuminoC/include/LuminoC/lumino.h:181` で `LNRenderPassDesc` 末尾に `LNSortMode sortMode` が追加され、実装 `LuminoAPI.cpp` は `desc->sortMode` を読む (構造体は 224 バイト)
- `packages/luminojs/src/types.ts:161` は `SIZEOF_RENDER_PASS_DESC = 220` のままで、`Renderer.ts _serializeDesc()` は sortMode を書き込まない
- 結果: C++ 側は JS が `_malloc(220)` した領域の外 (オフセット220-223) を毎パス読む。`toLnSortMode()` の default フォールバックで偶然動いているだけ
- JS から新機能 (同一zIndex内の深度ソート) は利用不能

### 3.2 無効化されたテスト群 (#if 0)

- `packages/LuminoBase/test/Test_Matrix4x4.cpp:139` の `#if 0` から 409 行目の `#endif` まで、レイアウト検証・回転・逆行列など約24テストケースが丸ごとコンパイル除外
- 実行されている行列テストは6件のみ (バイナリで確認: 全体で10テスト)
- 無効化ブロック内の `RowMajorLayout` テストは、生きている `translate` テストおよび docs/graphics-conventions.md (列優先) と矛盾する期待値を持つ。過去に失敗したテストを修正せず黙らせた形跡

### 3.3 無意味化した回帰テスト

- `packages/LuminoC/test/Test_Graphics.cpp:425` の `TwoSprites` が `captureAndCompare(..., forceUpdate=true)` を渡しており、`VisualTestHelper.cpp` の実装上、常に参照画像を上書き保存して true を返す。リグレッションを検出できない

---

## 4. 改善候補一覧 (27件)

優先度: S = 直ちに / A = 数週間以内 / B = 順次 / C = 機会があれば
分類: [QW] 今日すぐ作るべき Quick Win / [OS] 今後ずっと効く業務OS改善 / [挑戦] Fable 5 の能力がある今こそ挑戦すべき高難度タスク

| # | 分類 | 改善案 | 効果 | 難易度 | 所要時間 | 優先度 |
|---|---|---|---|---|---|---|
| 1 | QW | sortMode ABI 不整合の修正 | 高 | 低 | 30分 | S |
| 2 | QW | #if 0 テスト群の整理・復活 | 中 | 低 | 1-2h | A |
| 3 | QW | TwoSprites forceUpdate 修正 | 中 | 低 | 15分 | A |
| 4 | QW | .gitignore 修正 + 未追跡ファイル棚卸し | 高 | 低 | 1-2h | S |
| 5 | QW | WebGPU-distribution のバージョン固定 | 中 | 低 | 15分 | A |
| 6 | QW | build_wasm.py の emsdk 案内文修正 | 低 | 低 | 10分 | B |
| 7 | QW | 公開 ABI の掃除 (typo/deprecated 削除) | 中 | 低 | 2-3h | A |
| 8 | QW | WASM 非対応 API の契約明記 | 低 | 低 | 1h | B |
| 9 | OS | CI 導入 (GitHub Actions) | 最高 | 中 | 1-2日 | S |
| 10 | OS | ABI レイアウト同期検証の仕組み | 最高 | 中 | 0.5-1日 | S |
| 11 | OS | WASM Release ビルド経路の追加 | 高 | 低-中 | 半日 | A |
| 12 | OS | WASM スモークテスト | 高 | 中 | 1-2日 | A |
| 13 | OS | C API エラー詳細の伝搬 | 中 | 低-中 | 半日 | A |
| 14 | OS | 所有/借用ハンドル契約の明文化と防御 | 中 | 中 | 半日 | B |
| 15 | OS | シェーダ成果物の生成/コミット方針統一 | 高 | 中 | 半日-1日 | A |
| 16 | OS | vcpkg 未使用依存の削減 (9件) | 中 | 低 | 1-2h | B |
| 17 | OS | ドキュメント再整備 | 中 | 低 | 半日 | A |
| 18 | OS | LYRIDRA 連携フロー強化 (上位リポジトリ側) | 高 | 中 | 半日-1日 | A |
| 19 | OS | ctest 粒度改善 (gtest_discover_tests) | 低 | 低 | 30分 | C |
| 20 | OS | luminojs 単体テスト導入 (vitest) | 中 | 中 | 1日 | B |
| 21 | 挑戦 | FFI 描画経路の再設計 (バイナリコマンドリスト) | 最高 | 高 | 2-5日 | S(設計) |
| 22 | 挑戦 | HD-2D ライティング基盤 (Scene UBO + シャドウマップ) | 最高 | 高 | 3-7日 | A |
| 23 | 挑戦 | ポストエフェクトチェーン管理層 | 高 | 高 | 3-5日 | A |
| 24 | 挑戦 | lumino.h からのバインディング自動生成 | 高 | 高 | 2-4日 | B |
| 25 | 挑戦 | メッシュインスタンシング + ビューカリング | 中 | 中-高 | 2-4日 | B |
| 26 | 挑戦 | デバイスロスト対応戦略 | 中 | 高 | 3-5日 | C |
| 27 | 挑戦 | LYRIDRA バックエンド機能パリティ解消 | 高 | 中-高 | 3-5日 | A |

---

## 5. 改善案の詳細

### A. 今日すぐ作るべき Quick Win

#### 1. sortMode ABI 不整合の修正
- 解決する課題: 3.1 の未定義動作 (毎パスのヒープ外読み取り)、JS から sortMode 機能が使えない
- 期待効果: プロトタイプの信頼性回復。LYRIDRA で深度ソートが使用可能に
- 作るもの: `types.ts` の `SIZEOF_RENDER_PASS_DESC = 224`、`RenderPassDesc` インターフェースに `sortMode?: SortMode`、`Renderer.ts _serializeDesc()` にオフセット220への書き込み、`SortMode` enum の追加。Renderer.ts:295 のコメント (216バイト) も修正
- 実装難易度: 低 / 所要時間: 30分
- リスク: ほぼなし。既存呼び出しは sortMode 未指定 = 0 (Stable) で挙動不変
- 優先度: S

#### 2. #if 0 テスト群の整理・復活
- 解決する課題: 3.2。約24の数学テストが無効化されており、行列規約 (列優先) の回帰を検出できない
- 期待効果: 数学ライブラリの実効カバレッジ回復。row-major 前提の誤ったテストの排除
- 作るもの: `Test_Matrix4x4.cpp:139-409` を精査し、(a) docs/graphics-conventions.md の列優先規約に合うよう期待値を修正して復活、(b) 規約と矛盾するもの (RowMajorLayout 等) は削除。全テストがパスすることを ctest で確認
- 実装難易度: 低 (機械的だが1件ずつ規約照合が必要) / 所要時間: 1-2h
- リスク: テスト修正中に「実装側が間違っている」ケースを発見する可能性あり。その場合は実装修正前に報告・相談する
- 優先度: A

#### 3. TwoSprites forceUpdate 修正
- 解決する課題: 3.3。常にパスする無意味なテスト + 実行のたびに参照画像を上書き
- 期待効果: 回帰検出の回復。参照画像の意図しない書き換え防止
- 作るもの: `Test_Graphics.cpp:425` の第5引数 true を削除 (既定 false)。参照画像を一度正しい状態で再生成しコミット
- 実装難易度: 低 / 所要時間: 15分
- リスク: 現在の描画結果が本当に正しいかの目視確認が必要 (人間確認ポイント)
- 優先度: A

#### 4. .gitignore 修正 + 未追跡ファイル棚卸し
- 解決する課題: R3。クリーンチェックアウトで WASM ビルド不能。npm パッケージのソース (luminosc.js) がコミット不能。24.8MB の DLL が誤コミット待ちの状態
- 期待効果: 「clone すればビルドできる」状態の回復。誤コミットによるリポジトリ肥大の防止
- 作るもの:
  - .gitignore: `!packages/luminosc-npm/bin/` の例外追加、`packages/luminosc-x64-windows/bin/` (バイナリ) は明示的に無視、`packages/luminojs-examples/public/`、`.claude/` を追加
  - コミットする: `docs/coding-guidelines.md`、`packages/LuminoCore/shaders/*.lcsh(.inl)` (WASMビルドの前提)、`packages/LuminoC/examples/assets/SSR.lcsh`、`luminosc-npm` のソース部分 (package.json, bin/luminosc.js)、計画書類 (BatchRenderingPlan.md 等は docs/plans/ へ移動を推奨)
  - コミットしない: *.exe / *.dll、examples の public/ コピー、TODO.txt (内容は Issue 化)
- 実装難易度: 低 / 所要時間: 1-2h
- リスク: 何をコミットするかの判断は必ず人間が最終確認する (特にバイナリ成果物のコミット方針)
- 優先度: S

#### 5. WebGPU-distribution のバージョン固定
- 解決する課題: 唯一バージョン無指定の外部依存 (README.md:39)。将来 Dawn のバージョンが勝手に変わる
- 期待効果: ビルド再現性の確保
- 作るもの: 現在使用中のコミット (17dcd42a, 2025-09-03) をタグ/コミット指定で README とセットアップスクリプトに明記
- 実装難易度: 低 / 所要時間: 15分
- リスク: なし
- 優先度: A

#### 6. build_wasm.py の emsdk 案内文修正
- 解決する課題: エラーメッセージが `./emsdk` を案内するが実際の探索先は `build/emsdk` (scripts/build_wasm.py:61-64 vs :28)
- 期待効果: 新環境セットアップ時の混乱防止
- 作るもの: メッセージの修正 (README の手順と一致させる)
- 実装難易度: 低 / 所要時間: 10分 / リスク: なし / 優先度: B

#### 7. 公開 ABI の掃除
- 解決する課題: `LNGraphicsProfilering` という typo が公開 ABI に露出。deprecated 3系統 (LNDrawCommandBuffer/LNBatchProcessor/LNMaterial_CreateUnlit) がヘッダに残存し、luminojs が deprecated API を現役使用
- 期待効果: 「FFI の安定化」を掲げる以上、破壊的変更はプロトタイプの今しかできない。ヘッダの見通し改善
- 作るもの: typo 改名、deprecated API の実装ごと削除、luminojs を `LNMaterial_CreateFromBuiltinShader` へ移行 (バインド追加含む)、C++ examples の追随
- 実装難易度: 低-中 / 所要時間: 2-3h
- リスク: ABI 破壊。LYRIDRA 側の WASM 更新 (copy-lumino) と同期が必要。実施タイミングは人間が判断
- 優先度: A

#### 8. WASM 非対応 API の契約明記
- 解決する課題: lumino.h は Capture 系/WaitIdle を無条件宣言するが、WASM ビルドにはシンボルが存在しない (LuminoAPI.cpp の #ifndef __EMSCRIPTEN__)
- 期待効果: ヘッダだけを見たクライアントのリンクエラー/誤解の防止
- 作るもの: `LN_ERROR_NOT_SUPPORTED` を追加し、WASM でもシンボルは提供してこのエラーを返す方式へ変更。ヘッダコメントにプラットフォーム制約を明記
- 実装難易度: 低 / 所要時間: 1h / リスク: なし / 優先度: B

### B. 今後ずっと効く業務OS改善

#### 9. CI 導入 (GitHub Actions)
- 解決する課題: R2。今回発見した問題の大半 (テスト無効化、成果物コミット漏れ、ABI 同期崩れ、ドキュメント乖離) は CI があれば機械的に検出できた
- 期待効果: AI エージェント主体で高速開発する本プロジェクトの安全網。以後のすべての改善が「戻らない」ようになる
- 作るもの (段階導入):
  - Phase 1: Windows ランナーで `LUMINO_USE_WEBGPU=OFF` のデスクトップビルド (Dawn 回避で時間短縮) + GPU 不要テスト (LuminoBase/LuminoShader/LuminoCore の CPU テスト) + vcpkg キャッシュ
  - Phase 2: emsdk での WASM ビルド + luminojs の tsc/rollup ビルド
  - Phase 3: 成果物同期チェック (.slang と .lcsh の鮮度、ABI サイズ照合 #10)
  - GPU 実描画テストはローカル実行運用のまま (CI ランナーに GPU がないため)。lavapipe/SwiftShader の導入は将来課題
- 実装難易度: 中 / 所要時間: 1-2日
- リスク: vcpkg 初回ビルドが長い (キャッシュで緩和)。private リポジトリの場合は Actions 無料枠を確認 (人間確認ポイント)
- 優先度: S

#### 10. ABI レイアウト同期検証の仕組み
- 解決する課題: R1。lumino.h と luminojs の構造体レイアウト/関数シグネチャの同期が完全手動で、検証手段がない
- 期待効果: #1 の類のバグの再発を構造的に防止。以後 C API を拡張するたびの安心感
- 作るもの:
  - C 側: `LNDebug_GetTypeLayout(名前, outSize)` のような、主要構造体の sizeof (将来的にはフィールドオフセット) を返す関数を追加
  - luminojs 側: `Runtime.initialize()` 時に SIZEOF_* 定数と照合し、不一致なら即例外 (開発ビルドのみでも可)
  - C++ 側: `static_assert(sizeof(LNRenderPassDesc) == 224)` のようなサイズ固定表明をヘッダ近傍に置き、拡張時に意識させる
- 実装難易度: 中 / 所要時間: 0.5-1日
- リスク: なし (追加のみ)。将来 #24 (自動生成) に発展可能
- 優先度: S

#### 11. WASM Release ビルド経路の追加
- 解決する課題: WASM が常に -O0 -g3 のデバッグ固定 (LuminoC/CMakeLists.txt:53-54)。配布物のサイズ・性能が未知数
- 期待効果: LYRIDRA での実性能評価が正しくできる。FFI オーバーヘッド計測 (#21) の前提
- 作るもの: CMAKE_BUILD_TYPE に応じた最適化フラグ切り替え、build_wasm.py に --release、デバッグ/リリースの出力先分離、サイズレポート
- 実装難易度: 低-中 / 所要時間: 半日
- リスク: 最適化で顕在化する未定義動作 (まさに #1 のような) がありうる。#1 と #10 を先に
- 優先度: A

#### 12. WASM スモークテスト
- 解決する課題: luminojs が実際にロードする WASM バイナリを検証する自動テストがゼロ (GTest は全てデスクトップビルド)
- 期待効果: 「デスクトップで通った = WASM でも動く」という誤った安心の解消
- 作るもの: node (または Playwright + headless Chrome の WebGPU) で LuminoC.mjs をロードし、LNHelloTest / Instance_Initialize / テクスチャ生成 / シリアライズ関数の往復を検証する最小テスト。CI (#9 Phase 2) に組み込み
- 実装難易度: 中 (WebGPU の headless 実行が絡むと難度上昇。まずは GPU 不要な API のみで開始) / 所要時間: 1-2日
- リスク: headless WebGPU は環境依存が強い。GPU 不要部分から段階導入
- 優先度: A

#### 13. C API エラー詳細の伝搬
- 解決する課題: C++ 内部の Error.message が C API 境界で LN_ERROR_UNKNOWN に丸められ、JS からはログを見るしかない
- 期待効果: LYRIDRA 側でのデバッグ効率向上。エラー文化の一貫性
- 作るもの: `LNGetLastErrorMessage()` (スレッドローカル or シングルスレッド前提の静的バッファ)、LuminoAPI.cpp の変換ヘルパーでメッセージを保存、luminojs は失敗時にメッセージ付き例外を投げる
- 実装難易度: 低-中 / 所要時間: 半日 / リスク: なし / 優先度: A

#### 14. 所有/借用ハンドル契約の明文化と防御
- 解決する課題: BeginFrame が返す renderer/colorBuffer 等 (借用) を誤って LNObject_Release すると内部参照が壊れてダングリング化する。C API 契約として区別がない
- 期待効果: 将来の他言語バインディング/コンシューマ機移植での事故防止
- 作るもの: ObjectRegistry に「借用フラグ」を追加し借用ハンドルの Release を LN_ERROR で拒否。lumino.h に所有権規約の章を追記。Test_Object.cpp にネガティブテスト追加
- 実装難易度: 中 / 所要時間: 半日 / リスク: 低 / 優先度: B

#### 15. シェーダ成果物の生成/コミット方針統一
- 解決する課題: 組み込みは CMake 自動生成 (デスクトップのみ)、examples は手動運用で、SSR.lcsh のコミット漏れが実際に発生。.lcsh にソース対応を検証する情報がない
- 期待効果: 「.slang を書き換えたのに .lcsh が古い」事故の根絶
- 作るもの: (a) examples にも add_custom_command を追加、(b) .lcsh コミット方針の明文化 (WASM 用の組み込みシェーダはコミット、それ以外は生成)、(c) CI で .slang より .lcsh が古ければ失敗するチェック、(d) 将来: .lcsh にソースハッシュを埋める (フォーマット v3)
- 実装難易度: 中 / 所要時間: 半日-1日 / リスク: なし / 優先度: A

#### 16. vcpkg 未使用依存の削減
- 解決する課題: zlib, libpng, libogg, libvorbis, freetype, glad, glslang, spirv-cross, openal-soft の9件が find_package されておらず legacy 残骸の可能性が高い。初回ビルド時間を浪費
- 期待効果: セットアップ時間短縮、依存の意図の明確化
- 作るもの: vcpkg.json から9件を削除しフルビルドで確認。将来使う予定のもの (Audio 用 ogg/vorbis 等) はコメントで意図を残すか、必要になった時に戻す
- 実装難易度: 低 / 所要時間: 1-2h + ビルド確認
- リスク: 推移的依存で実は必要なものがある可能性 (ビルドで検証)。stb/tinyobjloader/tinygltf は要個別確認
- 優先度: B

#### 17. ドキュメント再整備
- 解決する課題: ARCHITECTURE.md が17バイトの空殻。AGENTS.md に未実装の将来構想 (QuickJS, Audio/Input) が現状として記述。LuminoShader/ARCHITECTURE.md と luminosc/README.md に旧 v1 フォーマットの記述が残存。LuminoC に README なし
- 期待効果: AI エージェントに開発を依頼する本プロジェクトでは、ドキュメントの正確さがそのまま生成コードの品質になる。誤った文書は誤ったコードを生む
- 作るもの: 本レビューの構造マップを ARCHITECTURE.md へ転記・整形。AGENTS.md の現状/将来の区別を明記。stale な v1 記述の削除。LuminoC/luminojs の README 追加
- 実装難易度: 低 / 所要時間: 半日 / リスク: なし / 優先度: A

#### 18. LYRIDRA 連携フロー強化 (上位リポジトリ側の作業)
- 解決する課題: copy-lumino が手動でビルドパイプライン外。バージョン照合なし。compileShader.ts は .build/Lumino 直参照 (Windows 固定) で、無い環境では静かにスキップ
- 期待効果: 「古い WASM で動いていた」「シェーダが実は更新されていない」事故の防止
- 作るもの: (a) `Runtime.getBuildTimestamp()` を LYRIDRA 起動時に呼びログ出力 (既存 API の活用)、(b) copy-lumino 時に Lumino のコミットハッシュを externals/luminojs/package.json 等へ記録、(c) シェーダコンパイルのスキップを警告からエラーに格上げするオプション、(d) 将来: luminosc-npm 配布への切り替え
- 実装難易度: 中 / 所要時間: 半日-1日 / リスク: なし / 優先度: A

#### 19. ctest 粒度改善
- 作るもの: `gtest_discover_tests` への置き換え (4エントリ → 89ケース個別表示)
- 難易度: 低 / 時間: 30分 / リスク: なし / 優先度: C

#### 20. luminojs 単体テスト導入
- 解決する課題: TS 側の純ロジック (シリアライズ、ResidencyManager、Matrix4x4) に自動テストがない
- 作るもの: vitest 導入。WASM 不要でテストできる部分 (シリアライズのオフセット計算、ResidencyManager の evict 判定等) から開始。#10 のサイズ照合テストもここに載る
- 難易度: 中 / 時間: 1日 / リスク: なし / 優先度: B

### C. Fable 5 の能力がある今こそ挑戦すべき高難度タスク

#### 21. FFI 描画経路の再設計 (バイナリコマンドリスト)
- 解決する課題: R4。AGENTS.md の教訓 (旧Luminoの失敗要因) である「API 呼び出し回数の最小化」が現構造 (draw 1回 = FFI 1回) では実現されていない。タイルマップ等の大量スプライトで JS→WASM 呼び出しオーバーヘッドが線形に増加。LYRIDRA の Tilemap3 はまさにタイル1枚ごとに drawSprite を呼んでいる
- 期待効果: HD-2D で想定される数千-数万スプライト/フレームでの性能余裕。設計思想と実装の一致
- 作るもの (3段階):
  1. 計測: Release WASM (#11) 上で 5,000/20,000 スプライト時の FFI オーバーヘッドをプロファイル。**閾値を決めてから設計する** (計測の結果、問題にならなければ大工事はしない判断もあり)
  2. 設計: JS 側で Uint8Array にスプライトコマンドを蓄積し、`LNRenderer_FlushSpriteBuffer(ptr, count)` 一発で C++ に渡す方式の設計。既存の deprecated LNDrawCommandBuffer_DrawSprites (一括投入) の設計を参考に、Renderer 統合型として再設計
  3. 実装: luminojs の drawSprite をバッファ蓄積に差し替え (API 互換のまま内部変更)、C++ 側にバイナリパーサ + バージョン/境界検証を実装
- 実装難易度: 高 (フォーマット設計は #10 の ABI 検証と整合させる必要) / 所要時間: 2-5日
- リスク: 計測せずに着手すると過剰設計になる。段階1で止まる勇気を持つ。設計判断は Fable 5 直轄、実装は分担可能
- 優先度: S (ただし着手は段階1の計測から)

#### 22. HD-2D ライティング基盤 (Scene UBO 再設計 + シャドウマップ)
- 解決する課題: R5。SceneParamsUBO が { lightDir, lightColor, ambientColor } の固定小構造で、複数ライト・シャドウ行列を格納できない。シャドウマップ機構が存在しない
- 期待効果: HD-2D の核心 (平行投影での動的ライティング/シャドウ)。プロジェクトの差別化価値そのもの
- 作るもの:
  1. SceneParamsUBO v2 の設計: 平行光源 + シャドウ行列 (単一カスケードから開始) + ポイントライト N 灯。シェーダ側 SceneParams との同期方法も設計 (手動一致契約の脆さを繰り返さない)
  2. "Shadow" パス: 深度専用 RT へのレンダリング (パス名選択機構は実装済みなので載るだけ)、深度比較サンプラーの RHI 対応確認
  3. BasicLit へのシャドウ受け実装 + 平行投影シーンでの検証サンプル (luminojs-examples に追加)
  - legacy (約1,470ファイル) に旧実装のシャドウ/ライティングコードがあり、アルゴリズムの参考として活用できる
- 実装難易度: 高 / 所要時間: 3-7日
- リスク: WebGPU/Vulkan 両対応の比較サンプラー・深度フォーマットの差異。ビジュアル回帰テストを先に用意して進める
- 優先度: A (設計は Fable 5、実装は工程分割して分担)

#### 23. ポストエフェクトチェーン管理層
- 解決する課題: R5。SSAO/SSR/ブルーム/トーンマップの連鎖に必要な RT ピンポン・中間バッファプール・パス順序管理をエンジンが持たず、クライアントが手配線 (SSR サンプルは手組み3パス)
- 期待効果: LYRIDRA から「エフェクトを並べるだけ」で HD-2D 画面が組める
- 作るもの: まず設計判断 (人間確認ポイント): この層を Lumino C++ 側に置くか、luminojs (TS) 側に置くか。LYRIDRA には既に _LuminoRenderTargetPool (TS) があり、AGENTS.md の方針 (メモリは JS 側管理を推奨) とも整合するため **TS 側実装を推奨**。その上で PostEffectChain クラス (パス列 + RT プール + フルスクリーン描画) を luminojs に追加し、SSR サンプルを移植して実証
- 実装難易度: 高 / 所要時間: 3-5日
- リスク: LYRIDRA 側の合成パイプラインとの責務境界を先に決めないと二重実装になる
- 優先度: A (#22 と並行可能だが、#22 の後の方が要件が明確)

#### 24. lumino.h からのバインディング自動生成
- 解決する課題: R1 の根本解決。66関数 (今後増加) の cwrap バインド・TS 型定義・SIZEOF 定数がすべて手書き
- 期待効果: C API 拡張のたびの手作業と同期ミスをゼロに
- 作るもの: lumino.h をパースする生成スクリプト (Python)。出力は (a) cwrap バインド表、(b) TS の型定義、(c) 構造体レイアウト定数、(d) C 側 static_assert 群。既存の手書きコードと一致することをテストで確認してから切り替え
- 実装難易度: 高 (C ヘッダのパースは単純化した規約に依存させる。lumino.h は整形済みで一貫性が高いので現実的) / 所要時間: 2-4日
- リスク: 生成器自体のバグ。#10 の検証機構を先に入れておけば安全網になる
- 優先度: B (#10 で当面の安全を確保した後の発展形)

#### 25. メッシュインスタンシング + ビューカリング
- 解決する課題: BatchRenderingPlan の Phase 2/3 が未実装。メッシュは 1 サブメッシュ 1 draw
- 期待効果: HD-2D で 3D メッシュ (木・建物・小物) を大量配置する場合の性能
- 作るもの: RHI へのインスタンスバッファ対応、Batch の同一メッシュ+マテリアル集約、AABB ベースの視錐台カリング
- 実装難易度: 中-高 / 所要時間: 2-4日
- リスク: 必要になるシーン規模が来るまでは投資対効果が不明。#21 の計測結果を見てから
- 優先度: B

#### 26. デバイスロスト対応戦略
- **ステータス: 実装済み (2026-07-17)**。設計と実装の詳細は
  `docs/plans/device-lost-design.md` を参照。検知 (Vulkan/WebGPU 両対応)、
  LN_ERROR_DEVICE_LOST の伝搬、BeginFrame ポンプ型の自動復旧、luminojs の
  自動再アップロード、gtest / vitest / playwright smoke によるテストまで完了
- 解決する課題: WebGPU はログのみ、Vulkan は検知なし。ブラウザのタブ復帰やドライバ更新で全損する
- 期待効果: 長時間動作するエディタ (LYRIDRA) としての堅牢性
- 作るもの: 検知とエラー伝搬 (LN_ERROR_DEVICE_LOST)、JS 側へのコールバック通知、ResidencyManager がソースデータから全リソースを再構築するフロー (AGENTS.md の「ビットマップは JS 側管理」方針はこのための布石であり、設計は既に半分できている)
- 実装難易度: 高 / 所要時間: 3-5日
- リスク: テストが難しい (デバイスロストの再現)。エディタの実運用が始まる前までに
- 優先度: C (ただしコンシューマ機移植前には必須)

#### 27. LYRIDRA バックエンド機能パリティ解消 (上位リポジトリ側)
- 解決する課題: Lumino バックエンド選択時に、Sprite の colorHue/brightness/blendColor/blendMode が無効 (opacity のみ有効)、TilingSprite/TransitionSprite が描画されない、captureBackbuffer がスタブ、canvas セレクタ "#main-canvas" 固定でエディタの複数キャンバスに未対応
- 期待効果: 「pixi と切り替えても見た目が同じ」状態への到達。エディタでの Lumino 常用開始
- 作るもの: (a) canvas セレクタの引数化 (Lumino 側 API は対応済み、LYRIDRA 側の配線のみ)、(b) DrawSprite の頂点カラーへの blendColor/brightness 反映、colorHue はシェーダ対応 (#22 のマテリアル拡張と合流)、(c) TilingSprite の onRender 実装 (DrawSprite の offset 機構が既にタイリング向けに設計済み)、(d) captureBackbuffer の実装 (Lumino の Capture API は WASM 未対応のため、RT 経由の読み戻しか、最終パスを RT に描く方式の設計判断が必要)
- 実装難易度: 中-高 / 所要時間: 3-5日
- リスク: captureBackbuffer は WebGPU の readback 制約と絡む。設計判断が必要
- 優先度: A

---

## 6. 最初に着手する3件

| 順 | 項目 | 理由 |
|---|---|---|
| 1 | **#1 sortMode ABI 不整合の修正** | 現在進行形の未定義動作で、LYRIDRA プロトタイプの土台を今この瞬間も蝕んでいる。30分で直り、リスクゼロ。修正前に他の作業をする理由がない |
| 2 | **#10 ABI レイアウト同期検証** | #1 は「起きたバグ」の修正であり、#10 は「この種のバグが二度と黙って起きない」ようにする恒久対策。C API は今後 HD-2D 機能 (#22, #23) で拡張が続くため、先に安全網を張ることが全ての後続作業の速度を上げる。半日-1日で完了 |
| 3 | **#9 CI 導入 (Phase 1)** | 今回のレビューで見つかった問題の大半 (無効化テスト、無意味化した回帰テスト、成果物コミット漏れ、ドキュメント乖離) は「CI があれば発生時点で検知できた」もの。AI エージェント主体の高速開発では人間のレビューが追いつかないため、機械的な検証こそが品質の主防衛線になる。#2, #3 のテスト修正と #4 の衛生改善は、この CI に載せて初めて「戻らない改善」になる |

この3件は「出血を止める → 傷口を塞ぐ → 免疫をつける」の順序であり、いずれも HD-2D への機能投資 (#21, #22, #23) を安全に始めるための前提整備である。機能面の最初の一手は #21 の段階1 (FFI オーバーヘッド計測) を推奨する。計測結果が出れば、バイナリコマンドリストへの投資判断 (このリポジトリ最大の設計判断) を事実に基づいて下せる。

---

## 7. QA 結果

### 実務で本当に使えるか
- 全27項目に対象ファイルパス・具体的な作業内容・時間見積もりを付した。Quick Win 8件はすべて本日中に着手可能
- 最重要指摘3件 (sortMode UB / #if 0 テスト / forceUpdate) は調査エージェントの報告を鵜呑みにせず、レビュアーが一次ソースとテストバイナリ実行で裏取り済み
- 未検証の推測 (クリーンチェックアウトでの WASM ビルド失敗、LYRIDRA エディタでの canvas 取得失敗など) は本文中で「構造上ほぼ確実だが未実行」と明示した

### 出力形式が安定しているか
- 各改善案は「解決する課題 / 期待効果 / 作るもの / 実装難易度 / 所要時間 / リスク / 優先度」の統一フォーマット
- 一覧表 (4章) と詳細 (5章) の番号は一対一対応。進捗管理はこの番号で行う

### 人間確認が必要な箇所 (実施前に必ず判断を仰ぐ)
1. #4: 何をコミットし何を無視するか (特に .lcsh バイナリと npm パッケージの扱い)
2. #3: TwoSprites の現在の描画結果が「正」であるかの目視確認
3. #7: ABI 破壊的変更 (typo 改名・deprecated 削除) の実施タイミング
4. #9: GitHub Actions の課金/無料枠の確認
5. #21: 計測結果を見た上でのバイナリコマンドリスト投資判断
6. #23: ポストエフェクトチェーンを C++ 側と TS 側のどちらに置くか
7. #2: テスト復活作業中に「実装側の誤り」を発見した場合の扱い
8. ブランチ整理 (main が2024年で停止、origin の既定ブランチが main のまま) は本計画の対象外としたが、方針判断を推奨

### 破壊・流出リスクの確認
- 本レビューの全調査は読み取り専用で実施した (テストバイナリの実行1回を含むが、参照画像を書き換える LuminoC_test は意図的に実行していない)
- 本計画書は新規ファイルであり既存ファイルを変更していない
- 本文書に認証情報・個人情報・外部送信を伴う内容は含まれない
- 今後の実装作業でも、#3 (参照画像再生成)、#4 (git 操作)、#7 (ABI 破壊) は事前確認必須とマークした

---

## 8. 補足: 分類の全体観

- Quick Win (8件) は合計でも1日弱。まとめて片付けて CI (#9) で固定するのが最短経路
- 業務OS改善 (12件) の核心は #9 CI と #10 ABI 検証。この2つだけで「AI エージェントに安心して任せられる」度合いが質的に変わる
- 高難度タスク (7件) はすべて「設計判断が半分」の仕事。設計 (要件整理・API 形状・責務境界) を Fable 5 が担い、実装を工程分割して Opus/Sonnet に分担する進め方が適する。着手順は #21(計測) → #22 → #23/#27 → #24/#25 を推奨
