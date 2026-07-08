# 依頼: build_wasm.py の emsdk エラーメッセージ修正 (改善案 #6)

推奨モデル: Sonnet / 目安: 10分 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) の小規模修正を行います。

## 背景

`scripts/build_wasm.py` の 28 行目付近で `EMSDK_ROOT` は `build/emsdk` を指しているのに、
61-64 行目付近の FileNotFoundError のエラーメッセージは `git clone -b 5.0.5 ... ./emsdk`
(リポジトリ直下) を案内しており、README.md の正しい手順 (`./build/emsdk`) と矛盾しています。
新環境のセットアップ時に、誤った場所へ emsdk をクローンさせてしまいます。

## タスク

1. `scripts/build_wasm.py` のエラーメッセージを、実際の探索先 (`build/emsdk`) および
   README.md の手順と一致するように修正する
2. スクリプト内の他のエラーメッセージ・コメントにも同種のパス不整合がないか一読して確認する
3. `python scripts/build_wasm.py --help` (または引数なし実行) が構文エラーなく動くことを確認する
   (実際の WASM ビルドまでは実行しなくてよい)

## 制約

- 動作ロジックは変更しない (メッセージ文字列の修正のみ)
- git commit は作らない。変更内容を報告して終了
