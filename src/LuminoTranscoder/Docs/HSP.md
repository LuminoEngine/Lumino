HPS
==========


enum
----------

`#const global` へ変換されます。

```
#const global LN_PIXEL_FORMAT_UNKNOWN 0
#const global LN_PIXEL_FORMAT_A8 1
#const global LN_PIXEL_FORMAT_RGBA8 2
#const global LN_PIXEL_FORMAT_RGB8 3
#const global LN_PIXEL_FORMAT_RGBA32F 4
```

struct
----------

構造体は `ストレージコアファンクション` によって、新しい型として登録されます。

> `ストレージコアファンクション` は HSP に新しい方を登録し、メモリ管理を HSP のランタイムに任せることができる。

> Note: Object はサブクラスをサポートするため、新しい型として登録することはできない。
> ```
> LnSprite_Create ..., sprite1
> LnWorldObject_SetPosition sprite1, ...   ; ここで型変換エラーになる
> ```

