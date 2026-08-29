import typescript from "@rollup/plugin-typescript";
import { nodeResolve } from "@rollup/plugin-node-resolve";

export default {
    input: "src/index.ts",
    output: {
        file: "lib/luminojs.mjs",
        format: "es",
        sourcemap: true,
    },
    // LuminoC.mjs は Emscripten が生成するモジュールで、出力と同じ lib/ に置かれる。
    // rollup でバンドルしてはいけない。
    external: ["./LuminoC.mjs"],
    plugins: [
        typescript({
            tsconfig: "./tsconfig.json",
            // 型宣言は build:typecheck の tsc かこのプラグイン自身が出力する。
            // 二重出力の問題を避けること。
            declaration: true,
            declarationDir: "lib",
        }),
        nodeResolve(),
    ],
};
