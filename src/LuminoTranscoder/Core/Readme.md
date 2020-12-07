内部実装
==========


[2020/8/19] SetPrototype の API はどのクラスに属する？
----------

例えば

- Component::OnRender()
- SpriteComponent::OnRender()

があり、ユーザープログラムで SpriteComponent::OnRender() をオーバーライドしたい場合、次のどちらがいいだろう？

```cpp
LnSpriteComponent_Create(&obj);

LnComponent_SetPototype_OnRender(obj, callback);        // (A)
LnSpriteComponent_SetPototype_OnRender(obj, callback);  // (B)
```

(B) の方がいいかもしれない。というのも、基本的に callback の実装は次のように、ベースクラスのメソッドを呼び出すことになる。

```cpp
LnResult callback(LNHandle self, LNHandle context) {
    LnSpriteComponent_CallSuper_OnRender(self, context);
    retun LN_OK;
}
```

ここで LnComponent_CallSuper_OnRender を呼び出すのは基本的に NG だし、
LnSpriteComponent_SetPototype_OnRender と LnSpriteComponent_CallSuper_OnRender は対応してるんだよ、という意思表示のために、(B) の方がいいだろう。

使用イメージはこんな感じかな↓

```cpp
LnSpriteOnUpdateHandler_Create(func, &callback);
LnSprite_Create(&obj);
LnSprite_SetPototype_OnUpdate(obj, callback);
```

今は ~Handler がほとんど同じシグネチャ持っててあんまり良くない気がしてるけど、共通化するなら↓の感じか。

```cpp
LnWorldObjectOnUpdateHandler_Create(func, &callback);
LnSprite_Create(&obj);
LnSprite_SetPototype_OnUpdate(obj, callback);
```

読むときのリズムが崩れそうなので、将来的に共通化するにしても、マクロとかで LnSpriteOnUpdateHandler_Create は残した方がいい気がする。


Strong reference について
----------

次のように、内部で参照を持たないため、Ref を値で返しているものが該当する。

```
static Ref<Texture2D> load(...);
```

このようなものは、FlatC ではユーザープログラムでインスタンスを Release しなければならない。

```
LnTexture2D_Load(..., &tex);
LnObject_Release(tex);
```

Strong reference は、これをマークするために戻り値型などの QualType が持つ属性。







`Draft`




[2019/11/28]
----------

### Manage 側を強参照リストで持つ場合

- あるオブジェクトが、リストからだけ参照されているのか、ロカール変数などからも参照されているのかがわからない。
    このため、たとえ内部・外部問わず参照カウントが1だからといって、本当に不要であるか判断することはできない。
- もしこの方式で行くなら、Dispose が必須になる。

### Manage 側を弱参照リストで持つ場合

- Native-object 生存中に Manager-object のデストラクトを許可することになるので、ユーザーが派生クラスを作ったりできなくなる。
  `button.AddChild(new MyControl());` や `Physics::EmitCollision(..., new UserData());` など。
- Component や UIElement を継承して実装することは Lumino の標準的なワークフローなので、この制約は致命的。

### ハイブリッドにしてみる

Manage 側のリストアイテムを次のようにする。

```.cs
struct {
    WeakReference<LnObject> weakRef;
    LnObject strongRef;
}
```

#### AccessorCache に該当しない Set 操作が発生するケース 1

※ UIElement.AddChild() は List の AccessorCache ではなく、透過的に子要素を追加するための仮想関数。

まず、Native-object の参照カウントの操作があったときに Manage 側のコールバックを呼んでもらうようにする。

```
{
    var button = new MyButton();    // Ref -> 1     作られたときは weakRef 参照のみ
    window.AddChild(button);        // Ref++ -> 2   ここでコールバック
}
```

Create 以外で Ref++ が行われたら、weakRef を strongRef に代入して、強参照にしておく。

次に削除。

```
window.RemoveAllChildren();         // Ref-- -> 1   ここでコールバック
```

コールバックで Ref=1 つまり Manage-object からしか参照していなかったら削除する。


#### すぐに消えるケース

```
{
    var button = new MyButton();    // Ref -> 1     作られたときは weakRef 参照のみ
}
```

weakRef のみなのでそのうち消える。


#### Native 側で Create されたものを Get する

```
var window = Engine.MainWindow      // Ref++ -> 2   return する前に wrap するが、そこで強参照にする。
```

#### 内部で Native-object を作って参照するパターン

```.cs
CollisionManager.EmitCollision(pos, size, time, new UserData(this));
```

```.cpp
void CollisionManager::emitCollision(..., userData)
{
    auto emitter = new Emitter();
    ...
    emitter->userData = userData;       // ref++ -> 2  ここでコールバック。強参照へ。
    world->addPhysicsObject(emitter);
}
```



### 整理

基本的に Ref=1 は Manage 側からの参照であることを担保としてオブジェクトの寿命を管理する。

### 制限事項

- キャッシュに入ることができなくなる (Native 側で if (getReferenceCount() == 1) みたいな感じで寿命管理しているもの)
    - 現時点では以下が該当
        - RenderTarget
        - DepthBuffer
        - FontCore
        - AudioDecoder
        - Effect
    - 監視して削除するのではなく、明示的に削除するような場合は良い。(UIEventArgs の Pool とか)





[2019/11/14] 参照の管理
----------
- Manage 側を強参照リストで持つ場合
    - あるオブジェクトが、リストからだけ参照されているのか、ロカール変数などからも参照されているのかがわからない。
      このため、たとえ内部・外部問わず参照カウントが1だからといって、本当に不要であるか判断することはできない。
    - もしこの方式で行くなら、Dispose が必須になる。


なぜ C++11 の属性構文を使わないのか？
----------

未知の属性は clang を使って取得できない。


次のように、言語規格で定義されたものであれば Decl::attrs() で取得できる。

```
class [[deprecated("please use new_func() function"]] Engine
```

しかし次のように、未知 (ユーザー定義) の属性を使うためには、clang 本体に手を入れる必要があるかもしれない。（未調査）

```
class [[ln_loca_attr(a=10, b="")]] Engine
```

clang を拡張して使うとしたら、clang::attr::Kind の enum を新しく登録したりする必要がありそう。




参照の管理
----------

v0.4.0 までのプロト版は、WeekReference の List ですべてのオブジェクトを管理していた。
そのため、特に get プロパティが Object を返す場合、誰かが Manage 側のオブジェクトの参照を持ってなければならなかったため、
たくさんのフィールドを自動生成していた。

新しいシステムでは、普通の List で参照を持てないだろうか？

`※結論としては、無理。` 以下、メモ書き。

基本ルールとしては、
- Native-object が存在している間、対応する Manage-object は必ず存在していなければならない。
- Native-object は必ずしも対応する Manage-object があるわけではない。
- Manage-object は必ず対応する Native-object がある。

これを守らないと、

```
// C#
var texture = sprite.Texture
```

などと書くたびに、Texture の Manage-object インスタンスを new する必要が出てきてパフォーマンス悪いし、Imutable な感じではないので気を付けることが多くなる。

Manage 側のグローバルな List (Manage-object-list)ですべての Manage-object を管理することになるので、特に「いつ解放するのか？」が重要となる。
基本としては、「Native-object が解放されたら Manage-object は Manage-object-list から外す」になる。アプリ実行中は、Manage 側で明示的にManage-object-list から外すことはない。
必ず、Native-object (RefObject) の release で参照カウントが 0 になるタイミングをフックすることになる。

しかし基本ルールに則って、Manage-object は Native-object を強参照でラップするものでなければならないため、循環参照が発生して解放できなくなってしまう。
また、Engine の内部では Sound 周りをはじめとして、管理用リストをイテレートして参照カウントが 1 であれば、他からの参照は無いと考えて解放する、といったように参照カウントの具体的な数値に依存する処理もある。

### 対策案 (没)
対策として、Windows の COM 実装のような、第2参照カウント (InternalReferenceCount) を導入してみたらどうだろう？
※カウントと言っているが、基本は Manager-object からの参照有無を判断するもので、0 or 1　になる。

・・・だめかも。

C# で new Texture() とだけした時、内部的には
1. newObject<ln::Texture>()
2. InternalReference++
3. Reference--
ということをする。

3 ではカウントが 0 になるので、Manage-object-list からの除外処理が走る。
コンストラクトの時だけ特別扱いするというのあアリかもしれないが・・。

他にも、

```cs
class Actor {
    Texture _texture = new Texture("test.png");
    Sprite _sprite;
    Actor() {
        _sprite = new Sprite(_texture);
    }
    void ChangeImage() {
        _sprite.Dispose();  // ここで ln::Texture の ReferenceCount=0 になり、Manage-object-list から除外される
        _sprite = new Sprite(_texture);
        ...
        var t = _sprite.Texture;    // get するとき Native-object が持っているインデックスをもとに Manage-object-list を探してしまう
        t.DrawText("img2");
    }
}
```

これを解決するには、C# 側で「Manage-object-list からだけ参照されているか？」という判断が必要になるが、WeakReference 無しでそれは不可能。
あるいは C# なのにスマートポインタのような仕組みを作る必要があるが、現実的ではない。

そもそもやっぱり C# 側ではスコープを外れたら GC で解放されるようにしたいし、強参照な(ユーザーからは見えない)Listで持っていると Dispose の明示的な呼び出しが必須になるし、`使いやすさ的な意味でも Manage-object-list を強参照リストにするべきではないのかもしれない。`


### キャッシュされるオブジェクトの参照問題

EventArgs など。

キャッシュから取り出されたとき、できれば古い Manage-object も再利用したいが、Manage-object-list が弱参照だとそれができない。

Ruby であれば mark 駆使で何とかできそうだが、できれば言語特有の GC 機能を利用する方法は避けたい。

WeakReference もやや言語ごとに使い勝手違うとかあるし・・・。


### 対策案2

- 第2参照カウント を導入する
- Mangae-object-list は強参照にする
- `定期的に Mangae-object-list の GC を実行する` (Reference が 0 で、InternalReference が 1 なら消す)

`Native-object が生きている間、一度作成された対応する Managed-object は絶対に消えない。`

これが安定してできると、以前やっていた次のようなものが不要になり、シンプルに実装できる。
- 参照保持のためのフィールドを Managed-object に持たせる
- テンプレートコレクションについて、Native 側と Managed 側の同期

また、ファンク他を受け取るようなメソッドを作るときに、Managed 側の関数オブジェクトをラップするオブジェクトが必要になるが、
そういった寿命の短いオブジェクトを簡単に作れるようになる。
※特に Task のように、登録したメソッドを後で呼び出す仕組みの時に、Managed-object の寿命を気にする必要が無い。




型情報
----------
Object 型は、Manage 側でいくつかのグローバルな情報を持っておく必要がある。

- Native 側で new された Object を get したとき、対応する Manage-object を new するためのファクトリ関数
- オーバーライド実装のための関数群
- イベントハンドラ実装のための関数群

Native 側は TypeInfo にこれらの情報の Id を持たせておく。


非同期処理
----------
多くのスクリプト言語はスレッドサポートが無いので、非同期処理自体は C++ 側で行い、それの終了をディスパッチするような仕組みを作っておきたい。

C# のタスクの仕組み↓
- https://ufcpp.net/study/csharp/misc_task.html
- https://qiita.com/takutoy/items/d45aa736ced25a8158b3

Unity の非同期読み込みは Task ではなく ResourceRequest というクラスを使う仕組み。
- http://kan-kikuchi.hatenablog.com/entry/Resources_LoadAsync
https://docs.unity3d.com/jp/460/ScriptReference/ResourceRequest.html

C#とかは、スレッドサポートがあるけど、Lumino としては C++ 側処理の待ち受け機能のみの提供としておく、で十分だと思う。
C#標準の方がリッチな機能持ってるし、Lumino 側で似たような機能公開する必要もない。

```.cs
Promise promise = Asset.LoadMeshAsync("...");
promise.Then((mesh) => { textbox.Text = "Done."; });    // メインスレッドでコールされる (Javascript 参考)
.Catch((e) => { /* error */ });
```

```.c
lnHandle promise1, promise2;
lnAsset_LoadMeshAsync(u"...", &promise1);
lnPromise_SetThen(primise1, [](lnHandle mesh) { ... }, &promise2);
lnPromise_SetCache(primise2, [](lnHandle e) { ... }, nullptr);
```

Javascript のように、then() で２つのハンドラを受け取るようにはしない。
そうすることで、setter として考えられるようになり、ハンドラを Manage 側のメンバに持たせることが容易となる。
（言語ごとに特殊クラスを独自定義しなくても、自動生成に任せられる。ただ、set戻り値対応が必要だが…）



テンプレートコレクション
----------

「対策案2」が実装されれば、従来やっていた ObjectList<T> のようなラッパーが必要なくなる。

Ref<ln::Collection<Ref<Sprite>>> は SpriteCollection というクラスに自動展開できそう。


