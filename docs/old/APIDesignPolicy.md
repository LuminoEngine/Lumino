API design policy
==========

Lumino (特に Core モジュール) は現在、フォーマルなプロジェクトで使用するため積極的にリファクタリングを行っています。

ここではリファクタリング作業のガイドラインや、今後の API 設計のベースとなる方針に付いて説明します。


サイズ・長さの型
----------

符号付きの整数を使用します。

Array::length() や String::length() などが該当します。

### 理由

符号付き整数と符号なし整数との意図しない演算が発生することが非常に多いためです。
C++ に不慣れなプログラマーには、次のコードの問題を見つけることは困難です。
コンパイラによっては警告を出力することもありません。

```cpp
for (int i = 0; i < container.size() - 1; ++i) {
    if (container[i] == container[i + 1]) return true;
}
```

https://google.github.io/styleguide/cppguide.html#Integer_Types
https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1227r1.html




