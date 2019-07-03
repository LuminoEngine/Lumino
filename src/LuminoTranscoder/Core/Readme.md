内部実装
==========

`Draft`

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


型情報
----------
Object 型は、Manage 側でいくつかのグローバルな情報を持っておく必要がある。

- Native 側で new された Object を get したとき、対応する Manage-object を new するためのファクトリ関数
- オーバーライド実装のための関数群
- イベントハンドラ実装のための関数群

Native 側は TypeInfo にこれらの情報の Id を持たせておく。

