Lumino
==========

Lumino is a open source library for building interactive applications such as games and digital art.
It provides an simple API that runs on multiple platforms and is available from multiple programming languages.

Lumino はゲームやデジタルアートなどのインタラクティブアプリケーションを構築するためのオープンソースライブラリです。
マルチプラットフォームで動作し複数のプログラミング言語から利用できる、シンプルな API を提供します。

Build and Unit Tests
----------

[![Package](https://github.com/LuminoEngine/Lumino/workflows/Package/badge.svg)](https://github.com/LuminoEngine/Lumino/actions)


Get started
----------

[Website](https://luminoengine.github.io/)

[Roadmap](https://github.com/LuminoEngine/Lumino/wiki/ProjectRoadmap) `変更予定`

[How to build](docs/HowToBuild.md)

[Contributing](.github/CONTRIBUTING.md)

[Blog](http://luminolog.hatenablog.com/)


開発状況と目標
----------

`目標整理中…`

### これまでのこと

v0.10.0 ではひとまず 3D ゲームエンジンとしておおよそ必要な機能を実装し終え、ゲームをリリースしユーザーの下で安定動作できるようになりました。

ここからはより多くのタイトルに対応できるように汎用化を進めていく・・・という計画をこれまでは進めていましたが、
おそらく "ゲームエンジン" として積極的に開発していくのはこのバージョンが最後になると思います。

理由は大きく次の2つです。

- ゲームを作ることが目的である人に対して、強力なアセット管理やエディタを持たないゲームエンジンを勧める理由が無い。
- ゲームプログラミングを学ぶ人に対しては他に競合ライブラリが多くあるが、それらと比べて明確なコンセプトが無い。

Lumino をゲームエンジンとして使用しても、それ以外の手段で得られる色々な意味での "良さ" に達するのは難しいだろう、という結論に至りました。

### これからのこと

ゲームエンジンからは離れますが、その描画性能や特徴を活かして "インタラクティブアプリケーション" という方向に持っていこうとしています。

この分野だと "メディアアート" や "クリエイティブコーディング" といったキーワードがありますが、おそらくそういったものよりも、ここ数年でユーザーと情報の距離が一気に近くなったことで高まってきた "情報の可視化" と "高速なプロトタイピング" のニーズを意識していくと思います。

私の本業でもいくつかプロトタイプを作ろうとしたときに、コードファーストで 3D シーンを瞬時に作れる Lumino が適しているようなケースに何度か遇うことがありました。当面はそんなユースケースを考えています。

#### 直近で目指したいこと

- アセットの動的生成・更新 (外部リソースに頼りすぎず、コードだけで高速にシーンを構築する)
- 標準化された外部デバイスを扱う仕組み (カメラ、マイク、VR 等)
- 各種通信
- VRM

#### 目指さないこと

- グラフィックのリアリティ
- ゲームアプリ向けの機能 (キーコンフィグなど)

Screenshots
--------
参考のため、開発の中で撮れた画像を置いておきます。

#### Scene rendering

![](docs/Gallery/33-scene.jfif)

![](docs/Gallery/32-scene.png)

#### VRM + BVH

![](docs/Gallery/31-vmr.gif)

#### Soft Shadow

![](docs/Gallery/30-softshadow.gif)

#### Depth of field

![](docs/Gallery/29-posteffect.gif)

### Character Controller

![](docs/Gallery/28-moving.gif)

### Mirror

![](docs/Gallery/27-mirror.gif)

### Screen Space Reflection

![](docs/Gallery/26-ssr.gif)

### Sky

![](docs/Gallery/25-sky.gif)

### Particle Rain

![](docs/Gallery/24-patricle.gif)

### Transition Effect

![](docs/Gallery/23-transition.gif)

### 3D Tilemap

![](docs/Gallery/22-3d-tilemap.gif)

### Post Effect (ToneChange, Bloom)

![](docs/Gallery/21-posteffect.png)

### 3D Text and Emoji

![](docs/Gallery/20-3d-text-and-emoji.png)

### Fantastic Ocean

![](docs/Gallery/18-ocean.gif)

![](docs/Gallery/19-ocean.gif)

### Voxel Map

![](docs/Gallery/17-3d-voxelmap.gif)

### Web Platform

![](docs/Gallery/16-web.gif)

### Effekseer

![](docs/Gallery/15-effekseer.png)

### 2D Action
![](docs/Gallery/14-2d-action.png)

### Tilemap Editor

![](docs/Gallery/13-tilemap-editor.png)

### Materials

![](docs/Gallery/12-materials.png)

### Fog

![](docs/Gallery/11-fog.jpg)

### Tilemap

![](docs/Gallery/10-tilemap.png)

### SoftBody

![](docs/Gallery/9-softbody.png)

### 3D Sprites

![](docs/Gallery/8-3d-sprites.png)

### Morphing

![](docs/Gallery/7-morphing.png)

### MMD Model

![](docs/Gallery/6-mmd.png)

### Multi Lights

![](docs/Gallery/5-multi-light.png)

### Atmosphere

![](docs/Gallery/4-atmosphere.jpg)

### Mirror

![](docs/Gallery/3-mirror.jpg)

### 3D Sprite Character

![](docs/Gallery/2-sprite-character.png)

### Pixel Shader

![](docs/Gallery/1-pixel-shader.png)


