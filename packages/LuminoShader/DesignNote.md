「データの種類別」vs「更新頻度別」
データの種類別 (legacy Lumino 方式)

Set 0: すべての UBO (ViewParams, MaterialParams, ObjectParams)
Set 1: すべてのリソース (Texture, Sampler)
更新頻度別 (現在の BasicLit.slang 方式)

Set 0: per-view   (カメラ、ライト - フレームに1回更新)
Set 1: per-material (色、テクスチャ - マテリアル切り替え時)
Set 2: per-object  (ワールド行列 - オブジェクトごと)
更新頻度別が優れている理由
1. BindGroup の再バインド効率

Vulkan/WebGPU/Metal では、あるセットだけを差し替えても、それより番号の小さいセットはそのまま維持されます。更新頻度別に分けると：

フレームの最初に Set 0 をバインド → フレーム中は変わらない
マテリアルが変わったら Set 1 だけ差し替え → Set 0 はそのまま
オブジェクトが変わったら Set 2 だけ差し替え → Set 0, 1 はそのまま
データの種類別だと、オブジェクトが変わるたびに UBO 全体を含む Set 0 を再バインドする必要があり、無駄が生じます。

2. 描画ソートとの親和性

ゲームエンジンでは通常、描画をマテリアル単位でソートします。更新頻度別レイアウトなら、同じマテリアルのオブジェクト群を描画する間、Set 1 は固定で Set 2 だけ回せます。

3. 主要エンジンの実績

Unreal Engine: Global / Per-Pass / Per-Material / Per-Draw の4段階
Unity (SRP): 同様に per-frame / per-camera / per-material / per-object
Godot 4: フレーム → シーン → マテリアル → インスタンス
Filament (Google): per-view / per-renderable / per-material-instance
ほぼ全て更新頻度ベースです。

4. 番号の小さいセットほど変更頻度を低く

これはパフォーマンス上の鉄則です。現在の設計（Set 0=view, Set 1=material, Set 2=object）はこれに正確に従っています。

vk::binding を省略してリフレクションに任せる方針について
この方針は合理的です。Slang のリフレクションで binding/set を取得できるので、シェーダ側はシンプルに書けます：


// vk::binding を省略した場合のイメージ
ConstantBuffer<ViewParams>     viewParams;
ConstantBuffer<MaterialParams> materialParams;
Texture2D                      baseTexture;
SamplerState                   baseSampler;
ConstantBuffer<ObjectParams>   objectParams;
ただし、省略すると Slang が自動的にすべてを1つの set にまとめる 可能性があります。更新頻度別にセットを分けたい場合は、以下のいずれかが必要です：

方法A: ParameterBlock を使う（推奨）
Slang の ParameterBlock<T> は独立した descriptor set にマップされます：


struct ViewData {
    ConstantBuffer<ViewParams> viewParams;
};

struct MaterialData {
    ConstantBuffer<MaterialParams> materialParams;
    Texture2D baseTexture;
    SamplerState baseSampler;
};

struct ObjectData {
    ConstantBuffer<ObjectParams> objectParams;
};

ParameterBlock<ViewData>     view;      // → Set 0
ParameterBlock<MaterialData> material;  // → Set 1
ParameterBlock<ObjectData>   object;    // → Set 2
これなら vk::binding を一切書かずに、リフレクションで set/binding の情報を取得でき、かつ更新頻度別のセット分離も保証されます。

方法B: 規約ベースでコンパイラ側が割り当て
シェーダに [lumino_set(0)] のようなカスタムアトリビュートをつけるか、変数名の規約（view_* → Set 0, material_* → Set 1）で ShaderCompiler 側が binding を注入する。ただしこれは複雑になるので、ParameterBlock の方がシンプルです。

まとめ
データ種類別	更新頻度別 (現在)
再バインド効率	低い	高い
描画ソート親和性	低い	高い
業界標準	-	主要エンジン全般
コード可読性		意図が明確
現在の Set 0/1/2 の設計は正しい方向です。vk::binding を省略するなら、Slang の ParameterBlock を使うのが最もクリーンで、リフレクションとの相性も良いでしょう。


----------------------------

ありがとうございます！
それでは次のように進めたいと思います。

- 更新頻度別
- 方法A: ParameterBlock を使う

実装計画を作成してほしいです！

