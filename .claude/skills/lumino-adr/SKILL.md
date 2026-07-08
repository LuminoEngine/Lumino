---
name: lyridra-adr
description: 'Record architecture/design decisions as ADRs in docs/adr. Use when: a design consultation reaches a conclusion, 設計相談の結論, 設計判断, 決定の記録, なぜこの設計, ADR, decision record, 記録に残す.'
---

# ADR 作成スキル

## 目的

設計相談の結論を `docs/adr/` に短い記録として残し、次回以降 AI へ文脈を再説明する手間を減らします。

## いつ提案するか

次の場合、**結論が確定した直後に 1 回だけ**「この決定を ADR に残しますか？」とユーザーに提案してください（会話のたびに繰り返さない）:

- 設計相談で複数の選択肢から 1 つを採択したとき
- アーキテクチャ・データ形式・命名方針など、後の実装に影響する決定をしたとき
- 過去の決定を覆したとき（この場合、旧 ADR の状態を「廃止」に更新する）

自明な実装詳細や暫定対応は提案しません。

## 手順

1. `docs/adr/` の索引（README.md）を確認し、次の連番 NNNN を決める。
2. `docs/adr/template.md` をコピーして `docs/adr/NNNN-<短い英語スラッグ>.md` を作成する。
3. **1 ページ以内**で書く。会話の長い引用はせず、文脈・検討した選択肢・決定・理由・トレードオフの要点のみを書く。
4. `docs/adr/README.md` の索引に 1 行追加する。
5. 作成した ADR をユーザーに提示し、内容の確認を得る。

## あわせて確認すること

- 決定の中に新しい用語や表記の決定（例: 英語名の採択）が含まれる場合は、`docs/guidelines/glossary.md` への追記も提案する。
- 決定が既存ドキュメント（ARCHITECTURE.md、coding-rule 等）と矛盾する場合は、そのドキュメントの更新も提案する。
