文字列の取り扱い
==========

Lumino は文字列を環境に依存せず透過的に使えるようにするため、Unicode 文字列を扱う String クラスを提供しています。

String クラスは文字列を UTF-16 コードで表現し、大文字小文字の変更、部分文字列の参照、検索など、様々な種類の文字列操作を行うことができます。

文字は Char 型で表される 2Byte 値です。65535を 超えるコード値の文字は、サロゲートペア、つまり2つの連続した文字で格納されます。エンディアンは、アプリケーションが動作しているプラットフォームに適した方法で格納されます。


文字列の初期化
----------
最も高速で安全な方法は、String クラスのコンストラクタに char16_t 文字列を指定することです。

```cpp
String str = u"Hello, World!";
```

また、std::string と同様の方法で初期化することもできます。

```cpp
const Char data[] = u"abcd";
String str(data, 2);    // "ab" で初期化
```

```cpp
String str(5, u'a');    // "aaaaa" で初期化
```


コードの変換
----------
String から std::string など他の文字列型や、その他のエンコーディングと相互変換することができます。

C++ 標準の型 (char, wchar_t, std::string, std::wstring) との変換に使われるエンコーディングは環境によって異なります。例えば wchar_t はそのサイズによって、UTF-16 または UTF-32 とみなします。

char, wchar_t 型のエンコーディングは、それぞれ TextEncoding::systemMultiByteEncoding(), TextEncoding::wideCharEncoding() で確認できます。

### String と std::string の相互変換

```cpp
std::string stdstr = "abc";

// std::string -> ln::String
String str = String::fromStdString(stdstr);

// ln::String -> std::string
stdstr = str.toStdString();
```

### String と std::wstring の相互変換

```cpp
std::wstring stdstr = L"abc";

// std::string -> ln::String
String str = String::fromStdString(stdstr);

// ln::String -> std::string
stdstr = str.toStdWString();
```

### char から String への変換

```cpp
const char* astr = "abc";
String str = String::fromCString(astr);
```

### wchar_t から String への変換

```cpp
const wchar_t* wstr = L"abc";
String str = String::fromCString(wstr);
```

### String から UTF-8 バイト列への変換

```cpp
String str = u"abc";
Ref<TextEncoding> encoding = TextEncoding::utf8Encoding();
std::vector<byte_t> utf8Data = encoding.encode(str);
```


char 型文字列からの暗黙的な初期化
----------
String クラスは char 型文字列からの暗黙的な変換をサポートしています。

```cpp
String str = "Hello, World!";
```

これは変換を簡単に記述するための機能ですが、前項と同様の変換処理が行われるため、char16_t 文字列を直接指定するよりもオーバーヘッドがある点に注意してください。

暗黙的な変換を無効にするには、Lumino の各ヘッダーファイルを include する前に `#define LN_STRING_STRICT_CONVERSION` を定義します。
