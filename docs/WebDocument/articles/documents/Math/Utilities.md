Math ユーティリティ
====================

Math クラス
----------
Math クラスは主にスカラー値 (float) に対する一般的な演算を行う機能をサポートします。

例えば、clamp メソッドは値を指定された範囲内に制限します。

```cpp
y = Math::clamp(x, 1.0, 2.0); 
```

lerp メソッドは1次元の線形補間を行います。
```cpp
for (float t = 0; t < 1.0; t += 0.1)
{
    y = Math::lerp(0.5, 2.0, t);    // ループ回数によって 0.5 ～ 2.0 の間の値を返す
}
```

Random クラス
----------
Random クラスは Xorshift アルゴリズムによる疑似乱数を生成します。
C言語の標準関数である rand() では精度の要求が満たせない場合の代替として使用できます。

簡単に乱数を取得したい場合は次のようにします。（現在の時間をシード値とします）
```cpp
int value = Random().getInt();
```

値を float で取得したり、任意の範囲の乱数を生成することもできます。
```cpp
Random rnd;
float value = rnd.getFloat();
float value = rnd.getFloatRange(0, 100);    // 0～100までの float 値
```

