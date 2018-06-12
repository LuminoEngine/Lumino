ファイル、ディレクトリの操作
==========

ファイルやディレクトリの基本的な操作は FileSystem クラスにまとめられています。

ファイルをコピーする
----------

```cpp
// file1.txt を file2.txt へコピーする
FileSystem::copyFile("file1.txt", "file2.txt");

// 上書きコピーする場合は FileCopyOption::Overwrite を指定する
FileSystem::copyFile("file1.txt", "file2.txt", FileCopyOption::Overwrite);
```


