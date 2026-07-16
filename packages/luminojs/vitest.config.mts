import { defineConfig } from "vitest/config";

// WASM (LuminoC.mjs/.wasm) に依存しない純ロジックのみを対象とする。
// test/smoke は Playwright (別コマンド test:smoke) が担当するため、
// vitest のデフォルト include パターンと衝突しないよう test/unit に限定する。
export default defineConfig({
    test: {
        include: ["test/unit/**/*.test.ts"],
    },
});
