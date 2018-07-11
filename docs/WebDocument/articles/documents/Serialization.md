オブジェクトのシリアライズ
====================

Lumino の Serialization モジュールは、任意のデータ型と文字列表現を可逆的に変換します。

> [!WARNING]
> 現在サポートされているフォーマットは JSON 文字列だけです。


オブジェクトと JSON 文字列のシリアライズ
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

Lumino で定義されているオブジェクトのシリアライズ
--------------------
List や Path など、Lumino で定義されているオブジェクトは Serialization モジュールによるシリアライズをサポートしています。

> [!WARNING]
> 0.5.0 時点では、特によく使うクラスを優先的にサポートしています。
> 今はサポートしていないクラスについても、今後のバージョンアップでサポートしていく予定です。

```
struct Documents
{
    String caption;
    List<Path> fileList;

    void serialize(Archive& ar)
    {
        ar & LN_NVP(caption);
        ar & LN_NVP(fileList);
    }
};

Documents docs;
docs.caption = "note";
docs.fileList.add("file1.md");
docs.fileList.add("file2.md");

String json = JsonSerializer::serialize(docs);
// {"caption":"note","fileList":["file1.md","file2.md"]}
```



