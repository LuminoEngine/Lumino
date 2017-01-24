bindings
====================

各種プログラム言語用のライブラリのビルドと、テスト、サンプルの作成を行う一式。

パッケージの生成はここでは行わない。あくまでランタイムをビルドすることに専念する。
ドキュメントやツールをまとめてパッケージを作るのはもっと上層のビルドシステムに任せる。


クラスライブラリの設計方針
--------------------


### WrapperObject をグローバルなリストに弱参照で持つ方法

NativeObject が生きている間、対応する WrapperObject が消えたり、別のものに変わったりする。


shader.SetTexture("_Texture", tex1);

// 別の場所
tex2 = shader.GetTexture("_Texture");

tex1 と tex2 は != になる。Handle は同じ。
ただ、tex1 はきえているので実際にこの比較が行われるようなことは無い。

上記例では、GetTexture でグローバルリストからを検索するとき、tex1 は既に削除されているかもしれない。
WrapperObject のデストラクタで、NativeObject が持っている WrapperObject のインデックスを消しておく必要がある。

デストラクタをサポートしない言語では使えない。


### WrapperObject をグローバルなリストに強参照で持つ方法 (没)

NativeObject が本当に delete されたら、あらかじめ登録した、リストからの削除処理をコールバックする必要がある。

逆にそれだけで WrapperObject の寿命を保証できる。Cache に入れられてもOK。

プロパティの値をフィールドに持つのも必須ではない。
ただ、持たせないとグローバルリストに排他処理付きでアクセスしなければならないため、持たせておくほうがパフォーマンスは良くなる。

・・・が、やっぱりダメ。
WrapperObject には NativeObject の参照を持たせてはならない。
初歩的なところ、


Texture t = Create<Texture>();
LNTexture_Create(32, 32, out t.Handle);
LNObject_Release(t.Handle);
return t


としなければならないということ。すぐ削除されてしまう。





