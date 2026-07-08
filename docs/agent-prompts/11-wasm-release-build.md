# 依頼: WASM Release ビルド経路の追加 (改善案 #11)

推奨モデル: Sonnet / 目安: 半日 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) の WASM ビルド改善を行います。
まず `scripts/build_wasm.py` と `packages/LuminoC/CMakeLists.txt` を読んでください。

## 背景

現在の WASM ビルドは最適化フラグが `-O0 -g3` にハードコードされており
(packages/LuminoC/CMakeLists.txt:53-54 付近)、build_wasm.py も `CMAKE_BUILD_TYPE=Debug` 固定です。
Release 相当のビルド経路が存在せず、配布サイズも実性能も評価できません。
FFI オーバーヘッド計測 (改善案 #21) の前提条件でもあります。

## タスク

1. `packages/LuminoC/CMakeLists.txt`: ハードコードの `-O0 -g3` を CMAKE_BUILD_TYPE 連動に変更する
   - Debug: 現状同等 (`-O0 -g3`)
   - Release: `-O2` (まず -O2 で安定を確認。-O3/-flto はコメントで将来候補として記す)
   - `-sASYNCIFY=1` などの機能フラグは両構成で維持する
2. `scripts/build_wasm.py`:
   - `--release` フラグを追加し、configure の CMAKE_BUILD_TYPE とビルドディレクトリを切り替える
     (例: `build/lumino-wasm` と `build/lumino-wasm-release` の分離)
   - luminojs/lib へのコピー先は従来と同じ (どちらの構成でビルドしたかをログで明示する)
   - ビルド完了時に `LuminoC.wasm` / `LuminoC.mjs` のファイルサイズを Debug/Release 比較できる形で表示する
3. README.md のビルド手順に `--release` の説明を追記する
4. 検証:
   - `python scripts/build_wasm.py` (従来 Debug) と `python scripts/build_wasm.py --release` の両方が
     成功し、Release の .wasm サイズが Debug より顕著に小さいことを確認して数値を報告する
   - Release ビルドで `packages/luminojs-examples` のいずれかのサンプル (clear_screen 等) を
     `npm run dev` でブラウザ起動し、描画されることを確認する (環境的に不可能なら手順を報告)

## 制約

- 既定の挙動 (フラグなし = Debug) は変えない。開発フローを壊さないこと
- Release で実行時エラーが出た場合は、最適化で顕在化した未定義動作の可能性がある。
  原因調査の初手として改善案 #1 (sortMode) が適用済みかを確認し、状況を報告する
- git commit は作らない
