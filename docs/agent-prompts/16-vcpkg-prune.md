# [done] 依頼: vcpkg 未使用依存の削減 (改善案 #16)

推奨モデル: Sonnet / 目安: 1-2h + フルビルド時間 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) の依存整理を行います。
まず `vcpkg.json` と、ルートおよび `packages/*/CMakeLists.txt` の `find_package` 呼び出しを確認してください。

## 背景

vcpkg.json に宣言された依存のうち、以下の9件はどの CMakeLists.txt からも find_package されておらず、
ソース中にも対応 API の使用が見つかりません (legacy エンジンの Audio/Font 機能の残骸と推測):
`zlib, libpng, libogg, libvorbis, freetype, glad, glslang, spirv-cross, openal-soft`

これらは vcpkg の初回セットアップ・フルビルド時間を不必要に増大させています。
また `tinyobjloader`, `tinygltf`, `stb`, `glm` は使用状況の個別確認が必要です。

## タスク

1. 確認: 上記9件 + tinyobjloader/tinygltf/stb/glm について、リポジトリ全体 (legacy/ を除く) で
   find_package・ヘッダ include・API 使用を grep し、使用/未使用の判定表を作る。
   特に `packages/LuminoCore/src/Graphics/MeshLoader.cpp` が tinyobjloader/tinygltf/stb を
   使っているかは必ず個別確認する (stb は画像デコードで使用している可能性が高い)
2. vcpkg.json から未使用と判定した依存を削除する。将来使う計画が明確なもの
   (Audio 用の libogg/libvorbis/openal-soft 等) は削除した上で、vcpkg.json の隣にコメントを
   置けないため `docs/` の適切な場所か AGENTS.md に「将来 Audio 実装時に再追加する依存」として
   一覧をメモする
3. 検証 (必須):
   - CMake キャッシュを消した状態で configure からフルビルド + 全テスト:
     ```
     cmake -B build/prune-check -DCMAKE_TOOLCHAIN_FILE=<vcpkgのパス> ...
     cmake --build build/prune-check --config Release --parallel
     ctest --test-dir build/prune-check -C Release --output-on-failure
     ```
     (既存の build/lumino-x64-windows を壊さないよう、検証用の別ビルドディレクトリを使うこと)
   - ビルドエラーが出た依存は「実は推移的に必要だった」ものとして vcpkg.json に戻し、判定表を訂正する
4. 検証で使った一時ビルドディレクトリ (build/prune-check) は最後に削除してよい

## 制約

- vcpkg.json 以外のビルド設定は変更しない
- git commit は作らない。判定表 (依存名 / 使用箇所 or 未使用 / 対応) と検証結果を報告して終了
