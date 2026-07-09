# [done] 依頼: TwoSprites ビジュアル回帰テストの修正 (改善案 #3)

推奨モデル: Sonnet / 目安: 15分 + テスト実行 / 人間確認: 再生成した参照画像の目視承認が必要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) のテスト修正を行います。
まず `docs/architecture-review-2026-07.md` の 3.3 節を読んでください。

## 背景

`packages/LuminoC/test/Test_Graphics.cpp:425` の `TwoSprites` テストが
`VisualTest::captureAndCompare("Test_Graphics.TwoSprites", data, w, h, TEST_DATA_DIR, true)` と
第5引数 `forceUpdate=true` を渡しています。`VisualTestHelper.cpp` の実装上、forceUpdate=true は
「現在の描画結果を参照画像として上書き保存して無条件に true を返す」ため、このテストは
常にパスし、リグレッションを検出できません。さらに実行のたびに追跡済みの参照画像
`packages/LuminoC/test/Expected/Test_Graphics.TwoSprites.png` を上書きします。

## タスク

1. `Test_Graphics.cpp:425` の第5引数 `true` を削除する (デフォルトの false 比較モードにする)
2. `VisualTestHelper.cpp` を読み、参照画像の更新手段 (LN_UPDATE_REFERENCES 環境変数等) を確認する
3. デスクトップビルドでテストを実行し、参照画像を一度だけ正規の手段で再生成する:
   ```
   cmake --build build/lumino-x64-windows --config Debug --target LuminoC_test --parallel
   # 参照更新 (VisualTestHelper が対応する環境変数を使う):
   LN_UPDATE_REFERENCES=1 ./build/.../LuminoC_test.exe --gtest_filter=Test_Graphics.TwoSprites
   # その後、比較モードでパスすることを確認:
   ./build/.../LuminoC_test.exe --gtest_filter=Test_Graphics.TwoSprites
   ```
   (実行ファイルの正確なパスはビルド後に探すこと。GPU が必要なテストです)
4. 生成された参照画像のパスを報告し、**人間による目視確認を依頼する** (2つのスプライトが
   正しい重なり順で描画されているか)。git status で他の参照画像が意図せず変わっていないことも確認
5. 可能なら検証として、テストコード内のスプライト色を一時的に変えてテストが「失敗する」ことを確認し、
   変更を元に戻す (回帰検出能力の証明)

## 制約

- 他のテストケースの forceUpdate 引数も横断的に grep し、同様の問題があれば報告する (修正は TwoSprites のみ)
- git commit は作らない。変更・実行結果・目視確認してほしい画像のパスを報告して終了
