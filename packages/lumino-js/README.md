

| Directory | Description |
|---|---|
| 📁lib | ライブラリの出力フォルダ。dist ではない。Pixi.js を参考にした。 |


## NOTE

npm link
npm link luminoengine


### esbuild は使わないの？

lib 自体そんなに大きくないのと、esbuild はトランスパイルと一緒に型定義を生成してくれない。

### package.json で `type: module` は指定しない

指定すると ESM とみなされ、おもなリンク先プロダクトの electron-mocha で [こちら](https://scrapbox.io/appbirdNotebook-public/ts-mocha%E3%82%92%E4%BD%BF%E3%81%86%E3%81%A8ECMAScript_module%E3%81%AB%E4%BE%9D%E5%AD%98%E3%81%97%E3%81%9F%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E3%81%A7TypeError_ERR_UNKNOWN_FILE_EXTENSION:_Unknown_file_extension_%22.ts%22%E3%81%8C%E5%87%BA%E3%81%A6%E3%81%8F%E3%82%8B%E3%80%82) のようなエラーが発生する。

