ログ出力
====================

> [!Note]
> Draft


Lumino はアプリケーションの利用状況やプログラムの実行状況を記録するための簡易的なロギング機能を持っています。


```cpp
LN_LOG_INFO << "Running test.";
LN_LOG_INFO << "Start title game scene.";
LN_LOG_WARNING << "UserId: " << 0;
LN_LOG_INFO << "End title game scene.";
LN_LOG_ERROR << "File not found. " << "save1.dat";
```

```
2018/04/10 23:10:10 Info    [3171][main(173)] Running test.
2018/04/10 23:10:10 Info    [3171][main(174)] Start title game scene.
2018/04/10 23:10:10 Warning [3171][main(175)] UserId: 0
2018/04/10 23:10:10 Info    [3171][main(176)] End title game scene.
2018/04/10 23:10:10 Error   [3171][main(177)] File not found. save1.dat
```
