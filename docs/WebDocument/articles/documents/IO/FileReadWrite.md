ファイルの読み書き
==========

テキストファイルから 1 行ずつ読み取る
----------

```cpp
StreamReader reader("file.txt");
String line;
while (reader.readLine(&line))
{
    std::cout << line << std::endl;
}
```

ファイルは UTF-8 テキストとして読み込まれます。

エンコーディングを明示するには次のようにします。

```cpp
StreamReader reader("file.txt", TextEncoding::utf16Encoding());
```


テキストファイルへ 1 行ずつ書き込む
----------

```cpp

```


