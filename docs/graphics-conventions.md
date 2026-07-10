# グラフィックス基本仕様

Lumino でグラフィックスプログラムを書く際に理解しておくべき基本仕様をまとめます。

基本的には [glTF](https://github.com/khronosgroup/gltf) 及びそのエコシステムに従います。

## 座標系

Lumino は **右手座標系** を採用しています。

- **X 軸**: 右方向が正
- **Y 軸**: 上方向が正
- **Z 軸**: 手前方向が正（カメラはデフォルトで -Z 方向を見る）

これは OpenGL や Godot と同じ規約です。

> glTF:
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#coordinate-system-and-units
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#projection-matrices

## 面の表裏（ワインディングオーダー）

**反時計回り (Counter-Clockwise, CCW)** で定義された面が正面です。

- デフォルト: `FrontFace = CCW`, `CullMode = Back`
- 右手座標系において、右ねじの法則に従って法線が手前を向く方向が正面となります

三角形の頂点を反時計回りに並べると、その面がカメラに向かって表示されます。

> glTF:
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#instantiation

## NDC（正規化デバイス座標）

Lumino の NDC は以下の範囲です:

| 軸 | 範囲 |
|---|---|
| X | -1（左）〜 +1（右） |
| Y | -1（下）〜 +1（上） |
| Z | 0（近）〜 1（遠） |

### バックエンドごとの差異吸収

上記に従うよう、バックエンドの API やシェーダコードのインジェクションを使い、調整を行います。例えば Vulkan はデフォルトで Y 軸が下向き正ですが、ビューポートの高さを負にすることで Y 軸を反転させています。（そのため動作環境は VK_KHR_maintenance1 のサポートが必要です）

これにより、クライアントコードやシェーダーはバックエンド (Vulkan / WebGPU / D3D12 等) の違いを意識する必要がありません。

## 深度（デプス）

- **深度範囲**: 0（近クリップ面）〜 1（遠クリップ面）
- **デフォルトクリア値**: `1.0f`（遠クリップ面でクリア）

## スクリーン座標とテクスチャ座標

### スクリーン座標

- **原点**: 左上
- X は右方向、Y は下方向に増加します

### テクスチャ座標 (UV)

- **原点**: 左上
- U は右方向（0〜1）、V は下方向（0〜1）に増加します

```
(0,0)-----(1,0)
  |         |
  |         |
(0,1)-----(1,1)
```

> glTF:
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#images

## 行列

### メモリレイアウト

**列優先 (Column-major)** で格納されます。

```
// メモリ上の並び: m[0]〜m[15]
// 論理的には以下の配置
| m[0] m[4] m[8]  m[12] |
| m[1] m[5] m[9]  m[13] |
| m[2] m[6] m[10] m[14] |
| m[3] m[7] m[11] m[15] |
```

主な計算は [glm](https://github.com/g-truc/glm) と同様の結果になるようテストされています。

> glTF:
> - https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#data-alignment

> **CPU 側メモリレイアウトとシェーダレジスタパッキングの関係**
>
> [shader-conventions.md](shader-conventions.md) では「シェーダ側は行優先 (row-major)」と
> 説明していますが、これはこのページの「列優先」と矛盾していません。同じバイト列を
> 2つの異なる視点 (CPU 配列の並び / GPU レジスタへの詰め方) から説明しているだけです。
>
> - **CPU 側 (このページの主題)**: `Matrix4x4` の `m[16]` 配列上での値の並び方。列優先
>   (`m[0]` から `m[3]` が第1列)。
> - **シェーダ側**: Slang コンパイラがコンスタントバッファ内で行列を GPU レジスタへ
>   詰める際の規約。Lumino は `SLANG_MATRIX_LAYOUT_COLUMN_MAJOR` を指定していますが、
>   Slang は HLSL 由来の命名を使うため、生成される SPIR-V の装飾は `RowMajor` になります。
>
> **転置は行われません**。CPU 側で書き込んだ列優先のバイト列は、そのまま (バイト単位で)
> GPU に渡され、GPU 側はそれを「行優先」の詰め方として解釈します。「列優先」「行優先」は
> 同じデータに対する別の慣習上のラベルです。詳細な経緯は
> [LuminoShader/ARCHITECTURE.md](../packages/LuminoShader/ARCHITECTURE.md#slang_matrix_layout_column_major-について)
> を参照してください。

### 乗算順序

**左から乗算** する規約です。変換は右から左へ適用されます。

```
// C++ 側
viewProj = projection * view;

// シェーダー側 (Slang/HLSL)
output.position = mul(viewProj, worldPos);  // matrix * vector
```
