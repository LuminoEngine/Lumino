import { RollupOptions, Plugin, RenderedChunk, NormalizedOutputOptions } from "rollup";
import typescript from "@rollup/plugin-typescript";
import esbuild from "rollup-plugin-esbuild";


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

const configs: RollupOptions[] = [
    {
        input: "src/index.ts",
        output: {
            name: "lumino",
            // dir: "packages/game/game/scripts/engine",
            format: "es",
            sourcemap: true,
            file: "dist/lumino.mjs",
        },
        plugins: [
            typescript({
                declaration: true,
                declarationDir: "dist",
            }),
            //esbuild(esbuildConfig),
        ],
    },
];
export default configs;

// https://zenn.dev/no4_dev/articles/74f80c4243919ea2a247-2

// npx rollup src/main/main.ts --file bundle.js --format iife
// npx tsc src/renderer/core/Application.ts
// npx esformatter ./dist/rollup/index.js
