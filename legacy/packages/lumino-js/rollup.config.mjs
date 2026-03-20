//import { RollupOptions } from "rollup";
import typescript from "@rollup/plugin-typescript";
import esbuild from "rollup-plugin-esbuild";
import { nodeResolve } from '@rollup/plugin-node-resolve';
import commonjs from '@rollup/plugin-commonjs';

const bundleTarget = "es2017";
const moduleTarget = "es2020";
const esbuildConfig = {
    target: moduleTarget,
    //minifySyntax: true,
    // define: {
    //     "process.env.VERSION": `'${repo.version}'`,
    //     "process.env.DEBUG": "true",
    // },
    //treeShaking: true,
    //tsconfigRaw: '{"compilerOptions":{"useDefineForClassFields":false}}',
};

/** @type RollupOptions[] */
const configs = [
    // NOTE: Output は ESModule と CommonJS の 2 つを出力します。
    // 通常は ESModule だけで十分ですが、例えば electron-mocha は renderer プロセスであっても
    // .test.ts を読み込む際に CommonJS が必要になります。
    // https://scrapbox.io/appbirdNotebook-public/ts-mocha%E3%82%92%E4%BD%BF%E3%81%86%E3%81%A8ECMAScript_module%E3%81%AB%E4%BE%9D%E5%AD%98%E3%81%97%E3%81%9F%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E3%81%A7TypeError_ERR_UNKNOWN_FILE_EXTENSION:_Unknown_file_extension_%22.ts%22%E3%81%8C%E5%87%BA%E3%81%A6%E3%81%8F%E3%82%8B%E3%80%82

    // ESModule
    {
        input: "src/index.ts",
        output: {
            name: "lumino",
            // dir: "packages/game/game/scripts/engine",
            // 出力ファイル名は index.mjs ではなく lumino.mjs にする。
            // こうしておかないと単体で配布するときに、他ライブラリと名前が衝突する可能性がある。
            file: "lib/lumino.mjs",
            format: "es",
            sourcemap: true,
        },
        plugins: [
            typescript(),
            nodeResolve(),
            commonjs({
                extensions: ['.js', '.ts'],
                transformMixedEsModules: true,
            }),
        ],
    },
    // CommonJS
    {
        input: "src/index.ts",
        output: {
            name: "lumino",
            file: "lib/lumino.js",
            format: "cjs",
            sourcemap: true,
        },
        plugins: [
            typescript(),
            nodeResolve(),
            commonjs({
                extensions: ['.js', '.ts'],
                transformMixedEsModules: true,
            }),
        ],
    },
];
export default configs;

// https://zenn.dev/no4_dev/articles/74f80c4243919ea2a247-2

// npx rollup src/main/main.ts --file bundle.js --format iife
// npx tsc src/renderer/core/Application.ts
// npx esformatter ./dist/rollup/index.js
