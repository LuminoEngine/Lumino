import typescript from "@rollup/plugin-typescript";
import { nodeResolve } from "@rollup/plugin-node-resolve";

export default {
    input: "src/index.ts",
    output: {
        file: "lib/luminojs.mjs",
        format: "es",
        sourcemap: true,
    },
    // LuminoC.mjs is an Emscripten-generated module that lives alongside the
    // output in lib/. It must not be bundled by rollup.
    external: ["./LuminoC.mjs"],
    plugins: [
        typescript({
            tsconfig: "./tsconfig.json",
            // Declarations are emitted by tsc during build:typecheck or by the
            // plugin itself; avoid double-emit issues.
            declaration: true,
            declarationDir: "lib",
        }),
        nodeResolve(),
    ],
};
