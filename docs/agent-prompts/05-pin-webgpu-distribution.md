# [done] 依頼: WebGPU-distribution のバージョン固定 (改善案 #5)

推奨モデル: Sonnet / 目安: 15分 / 人間確認: 不要

---

あなたは Lumino リポジトリ (`c:\Proj\dragon-baby-engine\.build\Lumino`) のビルド再現性改善を行います。

## 背景

README.md のセットアップ手順のうち、WebGPU-distribution だけがバージョン無指定です:

```
git clone https://github.com/eliemichel/WebGPU-distribution.git ./build/webgpu
```

vcpkg (`-b 2026.03.18`) や emsdk (`-b 5.0.5`) はタグ固定されているのに対し、これだけ main 追従に
なっており、将来 Dawn のバージョンが暗黙に変わるリスクがあります。現在ローカルで使用中のものは
コミット `17dcd42a` (2025-09-03, main) で、内部の dawn-git-tag は `chromium/7187` です。

## タスク

1. `build/webgpu` で `git log -1 --format="%H %ad"` を実行し、現在使用中の正確なコミットを確認する
2. README.md のクローン手順を、そのコミット (またはリポジトリに適切なタグがあればタグ) を
   チェックアウトする2段階の手順に書き換える:
   ```
   git clone https://github.com/eliemichel/WebGPU-distribution.git ./build/webgpu
   git -C ./build/webgpu checkout <コミットハッシュ>
   ```
3. `scripts/build_wasm.py` に `build/webgpu` の存在チェックがあるか確認し、あればエラーメッセージにも
   同じ固定手順を反映する。存在チェックがなければ、期待コミットと異なる場合に警告を出す軽量チェックの
   追加を検討する (必須ではない。大掛かりになるならスキップして報告)
4. AGENTS.md のビルド手順にも同種の記述があれば整合させる

## 制約

- `build/webgpu` 自体の checkout を変更しない (現状のまま。手順文書の固定が目的)
- git commit は作らない。変更ファイルと確認したコミットハッシュを報告して終了
