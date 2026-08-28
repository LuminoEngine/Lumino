# 毎フレームアロケーション削減計画 (2026-08-28)

- 対象: `packages/LuminoCore` (ブランチ `dev-lyric`, HEAD `942b3f8e5`)
- 方法: フレームループ (`LNGraphicsContext_BeginFrame` から `EndFrame` まで) から到達する全経路を読み、ヒープ確保と GPU オブジェクト生成の発生箇所を分類
- ゴール: 毎フレーム発生する malloc/free と GPU/JS オブジェクトの再生成を無くす。キャッシュ不足時のみ確保が起きる償却的定数時間のものは現状維持
- 本計画書の作成時点でコードは一切変更していない

## 0. 前提と方針

AGENTS.md の「事前確保された共有メモリを最大限に活用し、API 関数の呼び出し回数を可能な限り抑える」という設計方針に沿って、次の基準で分類した。

| 分類 | 判定 | 扱い |
|---|---|---|
| A | 描画内容にかかわらず毎フレーム必ず発生する確保 | 修正する |
| B | クライアントの使い方次第で毎フレーム発生し、かつ 1 回のコストが大きい | 修正する |
| C | 初回またはキャパシティ不足時のみ確保し、以後フレーム間で再利用される (償却的定数時間) | 現状維持。第 7 節に記録 |
| D | ヒープ確保ではないが毎フレーム発生する無駄なコスト | 修正する (優先度は個別) |

wasm32 では 32bit アドレス空間かつメモリ上限が環境依存で小さくなるため、A/B の対策は「確保を減らす」だけでなく「常駐サイズを増やさない」ことも条件とする。

## 1. 課題一覧

| # | 分類 | 課題 | 発生頻度 | 効果 | 難易度 | 所要時間 | 優先度 |
|---|---|---|---|---|---|---|---|
| B-1 | B | マテリアルパラメータ変更で BindGroup を毎フレーム再生成 | 変更のあるマテリアル数 x フレーム | 最高 | 中 | 半日 | 済 (6dabd6a01) |
| A-1 | A | `RenderPassDesc::colorAttachments` 等が `std::vector` | 4 回 x パス数 x フレーム | 高 | 低 | 2-3h | 済 |
| B-2 | B | `Renderer::m_materialCache` にエビクションが無い | Material 生成数に比例して単調増加 | 高 | 中 | 半日 | A |
| A-2 | A | `DebugPrint::render()` のローカル vector | 3 回 x フレーム (デスクトップのみ) | 中 | 低 | 30分 | A |
| D-1 | D | Vulkan の `vkMapMemory`/`vkUnmapMemory` が毎フレーム | バッファ数 x フレーム | 中 | 低 | 1h | A |
| D-2 | D | `WebGPUBuffer::m_shadow` が未使用のまま常駐リスクを持つ | 常駐 | 中 | 低 | 1h | A |
| D-3 | D | `Material::findPass()` がドローコールごとに文字列ハッシュ | ドローコール数 x フレーム | 低 | 低 | 1-2h | B |
| D-4 | D | `ForwardRenderer::renderFrame()` が `std::vector` を要求する API 形状 | 呼び出し側で 1 回 x フレーム | 低 | 低 | 30分 | C |

---

## 2. A: 無条件に毎フレーム発生するアロケーション

### A-1. RenderPassDesc 系の `std::vector` を SmallVector へ置き換える

**実装済み**。`RenderPassDesc::colorAttachments` と `RenderPassLayoutDesc::colorFormats`、`WebGPURenderPassLayoutKey::colorFormats` を `SmallVector<..., kMaxMultiRenderTargets>` へ変更し、`WebGPUCommandBuffer::beginRenderPass()` の `std::vector<WGPURenderPassColorAttachment>` をスタック配列 + カウンタに置き換えた。`LNRenderer_BeginRenderPass` に `colorAttachmentCount > LN_MAX_COLOR_ATTACHMENTS` の検証を追加し、`Test_Graphics.ColorAttachmentCountOverLimitIsRejected` で確認している。B-1 の積み残しである `BindGroupEntry` の `SmallVector` 化は未着手 (優先度 C のまま)。

- 解決する課題: レンダーパス 1 本を開くたびに、アタッチメント配列のためのヒープ確保と解放が最大 4 回発生する。パス 4 本のフレームで毎フレーム 16 回の malloc/free になる。内訳は次の通り。

  | 場所 | 内容 |
  |---|---|
  | `LuminoC/src/LuminoAPI.cpp:1475` | `rpDesc.colorAttachments.push_back()` (実運用の入口) |
  | `Renderer.cpp:212`, `Renderer.cpp:326` | initializer_list からの vector 代入 |
  | `rhi/webgpu/WebGPUCommandBuffer.cpp:113` | `std::vector<WGPURenderPassColorAttachment>` をサイズ指定で構築 |
  | `rhi/webgpu/WebGPUCommandBuffer.cpp:132` | `layoutDesc.colorFormats.push_back()` |
  | `rhi/webgpu/WebGPUCommandBuffer.cpp:169` | `key.colorFormats = layoutDesc.colorFormats` (キャッシュヒット時も毎回コピー) |

- 期待効果: WebGPU 経路で 1 パスあたり 4 回の malloc/free が 0 になる。Vulkan 経路のキャッシュキー (`RenderPassKey`/`FramebufferKey`) はすでに `SmallVector` で書かれており、そこと実装の一貫性も取れる
- 作るもの:
  - `Rhi.hpp` の `RenderPassDesc::colorAttachments` を `SmallVector<ColorAttachment, kMaxMultiRenderTargets>` へ変更 (`kMaxMultiRenderTargets` = 8)
  - `Rhi.hpp` の `RenderPassLayoutDesc::colorFormats` を `SmallVector<TextureFormat, kMaxMultiRenderTargets>` へ変更
  - `WebGPUCommandBuffer.hpp` の `WebGPURenderPassLayoutKey::colorFormats` も同型へ変更 (`SmallVector::operator==` があるため `operator==` はそのまま動く。ハッシュ関数の range-for もそのまま動く)
  - `WebGPUCommandBuffer.cpp:113` の `std::vector` はサイズ指定構築なので、`WGPURenderPassColorAttachment` のスタック配列 (`[kMaxMultiRenderTargets]`) + カウンタに置き換える
  - `Renderer.cpp:212`, `:326` の initializer_list 代入を `push_back` に書き換える (`SmallVector` は initializer_list 代入を持たない)
  - `LuminoAPI.cpp`, `examples/HelloTriangle/Main.cpp` の `push_back` はそのまま動く。呼び出し前に `colorAttachmentCount <= kMaxMultiRenderTargets` の検証を C API 側に足す (`SmallVector::push_back` は超過時 assert のみで、リリースビルドでは範囲外書き込みになる)
- 実装難易度: 低 / 所要時間: 2-3h
- リスク:
  - `RenderPassDesc` のサイズが 32 バイト前後から 264 バイト前後へ増える。全経路で const 参照渡しなのでコピーコストは増えないが、ローカル変数としてのスタック使用量は増える
  - `SmallVector::push_back` の上限超過はリリースビルドで無検査になる。C API 境界でのバリデーション追加を必ずセットで行う
  - `kMaxMultiRenderTargets` = 8 は RHI が宣言している上限なので、これを容量に採るのが契約として正しい。スタックを削りたい場合は 4 に落とす選択もあるが、その場合は上限宣言側も直す
- 優先度: S

### A-2. DebugPrint のステージングバッファをメンバへ移す

- 解決する課題: `DebugPrint.cpp:89-92` でローカルの `std::vector<Vertex> verts` / `std::vector<uint32_t> idxs` を作って `reserve()` し、`DebugPrint.cpp:146` の `setSubmeshes({{0, ..., 0}})` で一時 `std::vector<SubMesh>` をもう 1 つ作っている。デバッグ文字列を出しているフレームは必ず 3 回の malloc/free が発生する
- 期待効果: FPS オーバーレイを常時表示する開発時の計測ノイズが消える。`BatchProcessor` がステージングバッファをメンバに持っている方針と揃う
- 作るもの:
  - `DebugPrint` に `std::vector<Vertex> m_vertexStaging` / `std::vector<uint32_t> m_indexStaging` / `std::vector<SubMesh> m_submeshStaging` を追加し、`render()` では `clear()` + `push_back` で使い回す。最大サイズは `kMaxChars * kVertsPerChar` で固定なので、`create()` 時に `reserve()` しておけば以後の確保は起きない
  - `setSubmeshes` の一時 vector は `m_submeshStaging` に 1 要素入れて渡す形にする
- 実装難易度: 低 / 所要時間: 30分
- リスク: なし。`__EMSCRIPTEN__` では `DebugPrint` 自体がビルドされないためデスクトップのみの変更
- 優先度: A

---

## 3. B: 条件付きで毎フレーム発生し、影響が大きいもの

### B-1. BindGroup の再生成条件を「UBO の内容変更」から切り離す

**実装済み (6dabd6a01)**。`Material` のバージョンを `paramVersion` (UBO の内容) と `bindingVersion` (テクスチャ/サンプラーの構成) に分離し、`CachedMaterialBind` はスロットごとに「UBO へ書き込み済みの `paramVersion`」を持つようにした。パラメータを毎フレーム更新するマテリアルのコストは `writeBuffer` 1 回だけになる。検証は `Test_Graphics` の `MaterialColorChangeAcrossFrames` ほか 2 件と `Test_Material.ParamAndBindingVersionAreIndependent`。

- 積み残し: `Renderer.cpp` の `std::vector<rhi::BindGroupEntry> entries` は BindGroup 生成時のみの確保になったため優先度は下がったが、`SmallVector<rhi::BindGroupEntry, kMaxBindGroupEntries>` 化と `PipelineLayout::createBindGroup()` のシグネチャ変更 (両バックエンドと `DynamicUniformAllocator::createPage()` の追随が必要) は未着手。優先度: C

### B-2. Renderer のマテリアルキャッシュにエビクションを入れる

- 解決する課題:
  `Renderer::m_materialCache` (`Renderer.hpp:358`) は `MaterialBindKey{Material*, ShaderPass*}` を生ポインタのままキーにしており、Renderer が破棄されるまで 1 件もエビクトされない。`Material.hpp:271-279` は「同一 Shader から Material を何個作っても GPU リソースは増えません。フレーム内で異なるパラメータを使いたい場合は Material を量産してください」と明示的に勧めているため、実際に踏まれる構成である。結果として次の 3 つが起きる。

  1. `m_materialCache[key]` の insert がノード確保を伴う (`Renderer.cpp:706`)。Material を毎フレーム作る使い方では毎フレーム malloc
  2. キャッシュが単調増加する。エントリは `paramBuffers` と `bindGroups` をフレームスロット分保持しているため、GPU リソースごとリークする
  3. Material 解放後に同じアドレスへ新しい Material が確保されると、前の Material の BindGroup を掴む。これはアロケーションの問題ではなく正しさの問題

- 期待効果: 「Material を量産してよい」というドキュメント上の契約が実際に成立する。wasm32 のメモリ上限に対する最大のリーク源が閉じる
- 作るもの:
  - `PipelineCache` が `rhi::RHIObject::addDestroyCallback` + 生存フラグ `std::shared_ptr<bool>` でエビクションを実装済み (`PipelineCache.cpp:166-196`)。同じ仕組みを `Material` / `ShaderPass` 側にも用意する
  - `Object` (または `Material` / `ShaderPass`) に破棄コールバックの登録口を追加し、`Renderer` が `m_materialCache` にエントリを作るときに両オブジェクトを追跡登録する。破棄時に該当エントリを消す
  - `PipelineCache::trackObject`/`evictByObject` と同型の実装になるため、共通化できるならまとめる。ただし `RHIObject` と `Object` は別階層なので、無理に共通化せず素直に 2 つ書く方が短くなる可能性がある。実装時に短い方を採る
- 実装難易度: 中 / 所要時間: 半日
- リスク:
  - `Object` に破棄コールバックを足すと全 `Object` 派生のサイズが増える。`Material` と `ShaderPass` に限定して足す方が影響が小さい
  - コールバック中に Renderer 側の map を触るため、Renderer が先に破棄されるケースの保護が必要。`PipelineCache` の `m_alive` パターンをそのまま流用する
  - 3 の正しさの問題は現時点でも起きうる。エビクションが入るまでの間、Material を使い捨てにしないことを運用上の前提として明記しておく
- 優先度: A

---

## 4. D: アロケーション以外の毎フレームコスト

### D-1. Vulkan のホストビジブルバッファを永続マップにする

- 解決する課題: `VulkanBuffer::map()` は `m_mapped` をキャッシュしているのに `unmap()` が即座に `vkUnmapMemory` してしまう (`VulkanBuffer.cpp:73-84`)。`VulkanDevice::writeBuffer()` は非 device-local バッファに対して map/memcpy/unmap を行うため、UBO ページ 3 本 (view/scene/object) とスプライトの動的頂点・インデックスバッファに対して、毎フレーム map/unmap のドライバ呼び出しが走る
- 期待効果: 毎フレームのドライバ呼び出しが消える。メモリは `HOST_VISIBLE | HOST_COHERENT` で確保しているため flush も不要で、memcpy のみになる
- 作るもの:
  - `VulkanBuffer::unmap()` を no-op にし、マップは `m_mapped` に保持し続ける。`vkFreeMemory` はマップ中のメモリを暗黙にアンマップするため `finalize()` 側の追加処理は不要 (仕様の確認結果をコメントに残す)
  - `map()`/`unmap()` の意味が変わるので、`Rhi.hpp` のコメントを「unmap は永続マップの実装では何もしない」旨に更新する
- 実装難易度: 低 / 所要時間: 1h
- リスク: 低。device-local バッファは `map()` が `nullptr` を返す挙動のままで変わらない。デバイスロスト復旧時にバッファごと作り直しているため、マップの持ち越しによる不整合も起きない
- 優先度: A

### D-2. 使われていない WebGPUBuffer のシャドウバッファを削除する

- 解決する課題: `WebGPUBuffer::map()` はバッファ全サイズの CPU コピー `m_shadow` を確保する (`WebGPUBuffer.cpp:49-57`) が、WebGPU 経路で `map()` を呼ぶコードは存在しない。描画はすべて `writeBuffer` (= `wgpuQueueWriteBuffer`) 経由である。将来この経路を踏むと、wasm32 の線形メモリ上に全バッファの二重持ちが発生する。AGENTS.md の「デバイスロスト時の再アップロード用データは C/C++ 側ではなく JavaScript 側で管理する」という方針にも反する
- 期待効果: 踏んだ瞬間にメモリが倍になる罠を、使われていない今のうちに消せる
- 作るもの:
  - 第一候補: `rhi::Buffer` の `map()`/`unmap()` を公開インタフェースから外す。`LuminoCore` 内で外部から呼んでいる箇所は無く、Vulkan の `writeBuffer` が内部的に使っているだけなので、`VulkanBuffer` のメンバ関数として残せば足りる
  - 第二候補 (RHI の形を保ちたい場合): `WebGPUBuffer::map()` は `nullptr` を返し `unmap()` は no-op にする。device-local な `VulkanBuffer` と同じ「マップ不可」の契約に揃え、`m_shadow` を削除する
- 実装難易度: 低 / 所要時間: 1h
- リスク: 低。第一候補は RHI の仮想関数を減らす変更なのでコンパイルエラーで漏れが検出できる
- 優先度: A

### D-3. ShaderPass の名前引きをドローコールの外へ出す

- 解決する課題: `Renderer::drawSubmesh()` はドローコールごとに `mat->findPass(m_currentShaderPassName)` を呼び、`unordered_map<std::string, Ref<ShaderPass>>` に対する文字列ハッシュと比較を行う (`Renderer.cpp:451`)。ヒープ確保は無いが、1 マテリアルが持つパスは通常 1-3 個なのでハッシュマップは割に合っていない
- 期待効果: ドローコールあたりの固定費が下がる。マテリアル数が多い 2D 描画で効く
- 作るもの:
  - `Material::m_shaderPasses` を `std::unordered_map` から `std::vector<std::pair<std::string, Ref<ShaderPass>>>` に変更し、`findPass()` は長さ比較を先に行う線形走査にする。`shaderPasses()` を返している公開アクセサの型が変わるため、利用箇所の追随が必要
  - さらに踏み込む場合はパス名を整数 ID にインターンし、`beginRenderPass` で ID に解決してから draw に渡す形にする。ただし C API がパス名を文字列で受け取っている以上インターンテーブルが必要になるため、まずは線形走査までで効果を測る
- 実装難易度: 低 / 所要時間: 1-2h
- リスク: `shaderPasses()` の戻り型変更が公開ヘッダに及ぶ。パスが 10 個を超えるシェーダが出てきたら線形走査は不利になるため、その前提をコメントに残す
- 優先度: B

### D-4. ForwardRenderer::renderFrame の引数を span 相当にする

- 解決する課題: `ForwardRenderer::renderFrame()` が `const std::vector<RenderObject>&` を受け取るため、呼び出し側は毎フレーム vector を組み立てることになる
- 期待効果: 呼び出し側の毎フレームアロケーションを設計上不要にできる
- 作るもの: `(const RenderObject*, size_t)` もしくは `std::span` 相当を受ける形に変更する。`ForwardRenderer` は `LuminoCore/examples` 3 本からのみ使われており C API 経路には乗っていないため、影響範囲は examples に限られる
- 実装難易度: 低 / 所要時間: 30分
- リスク: なし
- 優先度: C

---

## 5. 着手順

1. ~~**B-1 (dirty 分割のみ)**~~: 実装済み (6dabd6a01)
2. ~~**A-1**~~: 実装済み
3. **D-1, D-2**: どちらも局所的で独立している。A-1 の途中に挟んでもよい
4. **A-2**: 独立。いつでも入れられる
5. **B-2**: エビクションの仕組みが必要なため最後。着手前に「Material を毎フレーム作る使い方を実際にするのか」を確認し、しないなら優先度を落とす判断もありえる
6. **D-3, D-4**: 効果測定の結果を見てから

## 6. 検証方法

- 数値の確認: `LuminoBase` にフレーム内の malloc 回数を数えるカウンタを一時的に仕込むか、デスクトップビルドで Visual Studio のヒーププロファイラを使う。B-1 は GPU オブジェクトの生成数が指標になるため、`wgpuDeviceCreateBindGroup` / `vkAllocateDescriptorSets` の呼び出し回数をログで数えるのが確実
- 回帰の確認: `ctest --test-dir build/lumino-x64-windows -C Release` の既存の画像比較テストで描画結果が変わらないことを確認する。B-1 は既存テストではカバーされていない可能性が高いため、次の 3 ケースを先に追加する
  1. マテリアルのパラメータを 2 フレーム連続で変更し、両フレームで正しい色が出ること
  2. マテリアルのテクスチャを差し替えたフレームで新しいテクスチャが出ること
  3. 名前付きサンプラー設定を変更したフレームで新しい設定が反映されること
- A-1 は `colorAttachmentCount` が上限を超えた場合に C API がエラーを返すことをテストする

## 7. 対象外: 償却的定数時間として現状維持するもの (C)

いずれもキャパシティがフレーム間で維持され、成長方向のみに確保が起きる。意図された設計と判断し、今回は触らない。

- `DrawCommandBuffer::m_commands`: `clear()` のみでキャパシティ保持 (`Batch.cpp:46`)
- `BatchProcessor::m_vertexStaging` / `m_indexStaging` / `m_submeshStaging`: メンバ化済み。`resize`/`clear` のみ
- `SpriteMeshPool::m_slots`: `resetFrame()` でカーソルを巻き戻し、スロットと GPU バッファはフレーム間で再利用 (`Batch.cpp:106-118`)
- `DynamicUniformAllocator`: ページ追加のみで、`allocate()` はアロケーションフリー (`DynamicUniformAllocator.cpp:52-77`)
- `Mesh::setSubmeshes` / `materials().resize()`: vector のコピー代入でキャパシティ再利用 (`Mesh.cpp:105-116`)
- `Renderer.cpp:797-805` のマテリアル UBO ステージング: 512 バイトまではスタック配列、超過時のみヒープ
- `PipelineCache` / `Renderer::m_samplerPool` / Vulkan の RenderPass・Framebuffer キャッシュ: ヒット時ゼロアロケーション。キーが `SmallVector` である点も正しい

### 記録: 縮まない高水位について

`SpriteMeshPool` と各ステージングバッファは高水位に張り付いたあと縮まない。1 フレームだけ大量のスプライトを描くと、そのサイズの GPU バッファと CPU バッファが常駐し続ける。wasm32 のメモリ上限を考えると将来的には縮小の仕組みが必要になりうるが、現時点では上限が読めないため対処しない。必要になった時点で「N フレーム連続で使用量が容量の半分未満なら縮小する」程度の単純な規則を入れる。
