Serialization
====================

Lumino の Serialization モジュールは、任意のデータ型を文字列としての表現と可逆的に変換します。

現在サポートされているフォーマットは JSON 文字列だけです。


クイックスタート (オブジェクトと JSON 文字列のシリアライズ)
--------------------

### serialize メソッドを追加する
シリアライズしたいクラスでどのフィールドをシリアライズするかを決めるために、serialize メソッドを実装します。

```cpp
struct MyData
{
    String name;
    int value;

    void serialize(Archive& ar)
    {
        ar & LN_NVP(name);
        ar & LN_NVP(value);
    }
};
```

### オブジェクトを文字列に変換する
JSON 文字列にシリアライズするには、JsonSerializer::serialize メソッドを使用します。

```cpp
MyData data1;
data1.name = "example";
data1.value = 100;

String json = JsonSerializer::serialize(data1);
json    // => {"name":"example","value":100}
```

### 文字列をオブジェクトに変換する
JSON 文字列からオブジェクトへデシリアライズするには、JsonSerializer::deserialize メソッドを使用します。

```cpp
MyData data2;
JsonSerializer::deserialize(json, data2);

data2.name  // => "example"
data2.value // => 100
```




