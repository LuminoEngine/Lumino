# コーディングガイドライン

基本的には [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) に従います。
この文書では、 Google C++ Style Guide よりも優先される Lumino 固有のガイドラインについて説明します。

## 命名規則

- 一般的な C++ の慣例に寄せたいフォルダ名は `kebab-case` とします。例えば、パッケージ直下の `include`, `src`, `docs` などです。

## API として公開する型定義

- [MUST] packages\LuminoC\include\LuminoC\lumino_types.h で定義し、 C++ 側でも参照して使います。
  - これは依存レイヤーの逆転で、本来は C++ 側の `enum class` と C_API 側の `enum` を別々に定義するのが綺麗でしょう。しかし定義やドキュメンテーションコメントの多重管理などの保守コストが大きく、小規模メンバーでメンテナンスする Lumino では、定義を一箇所にまとめることにしました。
