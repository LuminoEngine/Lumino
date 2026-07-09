# [done] 依頼: 無効化された行列テスト群の整理・復活 (改善案 #2)

推奨モデル: Opus / 目安: 1-2h / 人間確認: 実装側のバグを発見した場合は修正せず報告

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) の C++ ユニットテスト修復を行います。
まず `AGENTS.md`、`docs/graphics-conventions.md`、`docs/architecture-review-2026-07.md` の 3.2 節を読んでください。

## 背景

`packages/LuminoBase/test/Test_Matrix4x4.cpp` の 139 行目に `#if 0` があり、409 行目の `#endif` までの
約24テストケース (RowMajorLayout, ScaleLayout, RotateX/Y/Z, FromQuaternion系, Inverse系, Transpose系,
LookAtRH系など) が丸ごとコンパイル除外されています。過去に失敗したテストを修正せず無効化した形跡です。

Lumino の行列規約 (docs/graphics-conventions.md で規定、これが正):
- メモリレイアウトは列優先 (column-major)。平行移動成分は m[12], m[13], m[14]
- 右手座標系、mul(M, v) の列ベクトル規約
- glm と同様の結果になることが期待される (テストは GLM_TEST マクロで glm と比較する構造)

無効化ブロック内の `RowMajorLayout` テスト (173-209行) は「平行移動が m[3], m[7], m[11] に入る」という
行優先前提の期待値を持ち、規約および生きている `translate` テスト (54-67行) と矛盾しています。

## タスク

1. `#if 0` ブロック内の全テストを1件ずつ精査し、次のいずれかを行う:
   - 期待値が列優先規約に照らして正しい (または修正可能な) もの: 期待値を修正して復活させる
   - 行優先前提など規約と根本的に矛盾するもの (RowMajorLayout 等): 削除するか、
     列優先の正しい期待値を持つテスト (例: ColumnMajorLayout) に書き換える
   - デバッグ残骸 (コメントアウトされた変数、未使用の memcpy 等) は削除する
2. `#if 0` / `#endif` を除去し、全テストがコンパイル対象になるようにする
3. ビルドして全テストを実行:
   ```
   cmake --build build/lumino-x64-windows --config Debug --target LuminoBase_test --parallel
   ./build/lumino-x64-windows/packages/LuminoBase/test/Debug/LuminoBase_test.exe
   ```
4. 全テストがパスすることを確認し、復活/修正/削除したテストの一覧を報告する

## 重要な注意

- テストを直す過程で「実装 (Matrix4x4) 側が規約と食い違っている」ことを発見した場合は、
  実装を変更せず、その事実を証拠付きで報告して停止すること (実装修正は別途判断する)
- glm は列優先なので、GLM_TEST 有効時の比較コードはそのまま良い参照になる
- このファイルは既存ファイルの編集であり、エンコーディング (UTF-8 BOM) と LF 改行を維持すること
- コメントは日本語。`—` `–` `〜` は使わない (AGENTS.md 準拠)
- git commit は作らない。変更と実行結果 (テスト数・パス数) を報告して終了
