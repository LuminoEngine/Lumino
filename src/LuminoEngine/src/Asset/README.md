

- ローカルファイルにフルパスでアクセスできるようにする。
- ローカルファイルとアーカイブファイルを透過的に扱えるようにする。
- Web上から HTTP でダウンロードできるようにする。
- 非同期読み込みできるようにする。
- リロードできるようにする。
- アセットが無いときは警告をレポートして続行し、代替アセットを使えるようにする。

ユーザープログラムへ公開する基本コード
----------

### Idea.1 (没)

```
auto texture = Texture2D::loadAsync("test.png");
if (texture->isLoading()) {
    // 読み込み中
}
if (!texture->isValid()) {
    // 読み込み失敗
}
sprite->setTexture(texture);    // 少なくとも null ではない
```

このパターンだと、全ての AssetObject が [利用可能][読み込み中][失敗] といった状態を持つことになる。
例えばテクスチャサイズに依存して要素の大きさを決めるようなことができない。

直観的ではあるのだが、状態が多くすごく mutable なオブジェクトになるので、取り回しはかなり複雑になってしまう。

### Idea.2

```
Texture2D::loadAsync("test.png")
    ->then([&](auto& texture) {
        if (!texture->isValid()) {
            // 読み込み失敗
        }
        sprite->setTexture(texture);
    });
```

JS の fetch と同じ仕組み。Three.js の GLTFLoader でも同じような仕組みを採用している。

texture->isValid() が false の場合は 代替アセット が使われている。
sprite->setTexture(texture); するとマゼンタテクスチャが表示される。

失敗=代替アセットにしたいので、catch は今のところ不要かも。

ただし場合によっては同期ロードを使いたいこともある。
ちょっとお試しコード書いたり、ラムダ関数を作れない言語へのBindingでは同期ロードをメインで使いたい。

```
auto texture = Texture2D::load("test.png");
```

同期ロードはこれまで通りの考え方。

null と代替アセット
----------

null はそもそもアセットを利用していない状態 (Texture なら、デフォルトの WhiteTexture)

代替アセットは読み込み失敗の状態 (Texture なら、MagentaTexture)


ユーザープログラムから見た、ロードの基本手順
----------

```plantuml
@startuml
(*) --> "XXXX::load()"

(*) --> "XXXX::loadAsync()"
"XXXX::loadAsync()" -> "Task"
"Task" -> "XXXX::load()"

(*) --> "Asset::reload(XXXX)"

"XXXX::load()" --> "Find Cache"

"Find Cache" --> "Create instance"

"Create instance" --> "Asset::reload(XXXX)"

"Asset::reload(XXXX)" --> "Serialize"
@enduml
```

Import までの内部フロー
----------

```plantuml
@startuml
partition "User program interface" {
    (*) --> "XXXX::load()"

    (*) --> "XXXX::loadAsync()"

    (*) --> "Asset::reload(XXXX)"
}

"XXXX::load()" --> "YYYYManager::loadXXXX()"
"XXXX::loadAsync()" --> "YYYYManager::loadXXXXAsync()"
"YYYYManager::loadXXXXAsync()" --> "YYYYManager::loadXXXX()"


note left
AssetObject へ m_assetFilePath と、
派生へ m_sourceFilePath を set。
代替オブジェクト処理はここ。
end note
"YYYYManager::loadXXXX()" --> "AssetObject::reload()"

"Asset::reload(XXXX)" --> "AssetObject::reload()"

note left
呼び出し元から Init or Reload を受け取る。
.png等ならonLoadSourceFile()を呼ぶ。
.ymlならserializeを行った後、onLoadSourceFile()を呼ぶ。
end note
"AssetObject::reload()" --> "XXXX::serialize()"
"AssetObject::reload()" --> "XXXX::onLoadSourceFile()"

"XXXX::onLoadSourceFile()" --> "YYYYManager::importXXXX()"

@enduml
```

#### バイナリデータなどから作成し、名前付き AssetObject として登録するには？

.gltf の埋め込みテクスチャや、プログラム内で生成したテクスチャを登録したいとき。

- 生成と登録は分けておく。
- 新規作成したときは、`YYYYManager::registerXXXX()` で AssetPath と共に登録する。
    - .gltf などファイルを基点に作った場合は / で区切った方がいいかも。
- リロードするときは、
    - まず YYYYManager::loadXXXX() で検索する。
    - 見つかった Object に生成処理を適用して、中身を書き換える。

#### CacheKey に拡張子は含めるべき？

```cpp
auto mesh1 = StaticMesh::load("mesh");      // .yml があればそちらを優先ロード
auto mesh2 = StaticMesh::load("mesh.gltf");
```

.yml をロードしたときも、importer に流れるときは mesh.gltf に解決されるので、埋め込みテクスチャがある場合、どちらも "mesh.gltf/img.png" が Cache に登録される。

でも埋め込みテクスチャを外部で明示的にロードしたいケースはないか…。

ひとまず「含める」で進めてみる。


AssetObject
----------

Object のサブクラス。Texture などのスーパークラス。
Asset file と 1:1 で対応する。

AssetObject::m_assetPath は アセットファイル (.yml) のパス。
.png 等から直接読み込んだ場合は Empty。

実装は Texture::m_sourceFilePath を持つ。
.png 等のパスはここに持ち、reload の時に使用する。

複数 Aseet の生成元となるファイルの reload
----------

glTF 等のファイルは、Mesh, Texture, Material, AnimationClip 等の生成元となる。

Assets::reload(Mesh) したら、Mesh の sourceFile に起因する Asset はすべてリロードする。

例えば .gltf はテクスチャが埋め込まれていることがあるが、それに関わらずすべてリロードする。
ただし、initialize の時のみ。ランタイム時はキャッシュ検索して同一リソースは共有したいため、常にリロードだとそれの障害となる。


