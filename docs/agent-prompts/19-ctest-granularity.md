# 依頼: ctest 粒度改善 (改善案 #19)

推奨モデル: Sonnet / 目安: 30分 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) のテスト登録を改善します。

## 背景

各テストパッケージの CMakeLists.txt (`packages/LuminoBase/test`, `packages/LuminoShader/test`,
`packages/LuminoCore/test`, `packages/LuminoC/test`) は `add_test(NAME <target> COMMAND <target>)`
形式で登録しており、約89個の GoogleTest ケースが ctest 上は4エントリにしか見えません。
CI 導入時に「どのケースが落ちたか」が ctest レベルで分からない状態です。

## タスク

1. 4つのテスト用 CMakeLists.txt の `add_test` を `gtest_discover_tests` に置き換える:
   ```cmake
   include(GoogleTest)
   gtest_discover_tests(${PROJECT_NAME})
   ```
   - `LuminoC_test` は実 GPU 描画を含むため、`gtest_discover_tests` の `PROPERTIES` や
     `TEST_FILTER` で GPU テスト (Test_Graphics.*) にラベル `gpu` を付けるなど、
     CI から除外しやすい形にできるか検討して可能なら実装する (無理なら現状維持で報告)
   - DLL 依存 (LuminoC.dll, webgpu 等) のあるテストは discover 時 (ビルド直後のテスト列挙実行) に
     実行ファイルが起動できる必要がある。起動失敗する場合は `DISCOVERY_MODE PRE_TEST` を試す
2. 検証:
   ```
   cmake -B build/lumino-x64-windows   # 再configure
   cmake --build build/lumino-x64-windows --config Debug --parallel
   ctest --test-dir build/lumino-x64-windows -C Debug -N   # 一覧に個別ケースが出ること
   ctest --test-dir build/lumino-x64-windows -C Debug --output-on-failure
   ```
   ケース数 (約89) が列挙されること、実行結果が従来と同じであることを確認する

## 制約

- テストコード自体は変更しない (CMake のみ)
- git commit は作らない。ctest -N の出力サマリを報告して終了
