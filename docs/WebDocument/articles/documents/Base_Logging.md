ログ出力
====================

Lumino はアプリケーションの利用状況やプログラムの実行状況を記録するための簡易的なロギング機能を持っています。

この機能の目的は、ログを解析することでアプリケーションが正常に動作していることを確認したり、不具合発生時に問題の原因を特定するための補助を効率的に行うことです。

Lumino 内部では主にエラー発生時にこの機能を使用して、問題を記録します。

なお現時点では、この機能は長期的または大量のデータを記録することは想定していません。
特にゲームアプリケーションでは、メインループの内部でデバッグ目的以外で連続的にログを出力するような使い方は避けてください。


使い方
--------------------

ログ出力には以下のマクロを使います。

```cpp
LN_LOG_FATAL
LN_LOG_ERROR
LN_LOG_WARNING
LN_LOG_INFO
LN_LOG_DEBUG
LN_LOG_VERBOSE
```

これらのマクロは std::cout のように `<<` 演算子に続けて文字列を指定します。

また、ログの出力先を指定するために GlobalLogger クラスを使います。

```cpp
GlobalLogger::addStdErrAdapter("log.txt");  // "log.txt" というファイルへ出力する
GlobalLogger::addStdErrAdapter();           // 標準エラー出力へ出力する
LN_LOG_INFO << "Running test.";
LN_LOG_INFO << "Start title game scene.";
LN_LOG_WARNING << "UserId: " << 0;
LN_LOG_INFO << "End title game scene.";
LN_LOG_ERROR << "File not found. " << "save1.dat";
```

この例は次のような出力結果となります。

```
2018/04/10 23:10:10 I [3171][main(173)] Running test.
2018/04/10 23:10:10 I [3171][main(174)] Start title game scene.
2018/04/10 23:10:10 W [3171][main(175)] UserId: 0
2018/04/10 23:10:10 I [3171][main(176)] End title game scene.
2018/04/10 23:10:10 E [3171][main(177)] File not found. save1.dat
```

出力される情報は次の通りです。

* 日付
* 時間
* 重要度
* スレッドID
* LN_LOG_ マクロの書かれた関数名と行番号
* ログ文字列
