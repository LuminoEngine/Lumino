﻿Graphics
==========

FAQ
----------

### なぜ Interface パターンのバックエンド実装と、それをラップするクラスを分けているの？

- 言語バインダの実装のため。


### GraphcisContext はどんな人が使うの？

- 普通は RenderingContext で十分。Rendering モジュールを使わない人はこれを使うしかないが、そういう人は少ない想定。
- 主なところでは、並列コンピュートシェーダ使いたい人。
- あとはサードパーティのミドルウェアを使いたい人。VkCommandList とかはここから取り出すことになる。



