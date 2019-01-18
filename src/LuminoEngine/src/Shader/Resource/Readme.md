
glslang 7.9.2888 時点では、HLSL で include ファイル内でエラーがあったとき、エラー位置が正しく表示されない。
（include 元の #include の行を指してしまう）

そのため事前にヘッダのコンパイルだけ行って、エラーがないことを確認しておく。

```
glslangValidator.exe -o t.spv -V -S vert -D Lumino.fxh
```

リンクまで行ければOK。以下のようなメッセージが表示される。

```
WARNING: Linking vertex stage: Entry point not found
```

