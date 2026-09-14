// luminojs WASM スモークテストの Playwright 設定。
//
// WebGPU を使うため、Playwright 既定の headless_shell ではなくフル Chromium
// (channel: "chromium") を使用する。headless_shell は requestAdapter() が null を
// 返し WebGPU デバイスを生成できないことを確認済み。
// --enable-unsafe-swiftshader は、GPU の無い環境 (CI 等) でソフトウェア実装の
// WebGPU へフォールバックすることを許可する (GPU がある環境ではハードウェアを使う)。

import { defineConfig } from "@playwright/test";

// 環境検証用に、Chromium 起動フラグを外から追加できるようにする。
// 例: LUMINO_SMOKE_CHROMIUM_ARGS="--disable-gpu" で「WebGPU が使えない環境」を再現し、
//     この環境でテストがどう振る舞うか (どの項目が落ちる/skip されるか) を確認できる。
const extraArgs = (process.env.LUMINO_SMOKE_CHROMIUM_ARGS ?? "")
    .split(/\s+/)
    .filter(Boolean);

// Playwright が同梱する Chromium ではなく、環境側に用意された Chromium を使いたい
// 場合のための逃げ道 (CI コンテナにブラウザが事前インストールされている場合など)。
const executablePath = process.env.LUMINO_SMOKE_CHROMIUM_EXECUTABLE || undefined;

export default defineConfig({
    // このファイルと同じディレクトリの *.spec.mjs を対象にする。
    testDir: ".",
    testMatch: /.*\.spec\.mjs$/,

    // WASM 初期化を含むため直列実行で安定させる。
    fullyParallel: false,
    workers: 1,
    retries: 0,

    // WASM は -O0 -g3 でロードに時間がかかるため、やや長めのタイムアウト。
    timeout: 60_000,

    forbidOnly: !!process.env.CI,
    reporter: [["list"]],

    use: {
        // フル Chromium を使う (headless_shell では WebGPU が使えない)。
        // executablePath を指定した場合は channel より優先される。
        channel: executablePath ? undefined : "chromium",
        headless: true,
        launchOptions: {
            executablePath,
            // --enable-unsafe-webgpu: GPU の無い Linux の headless Chromium は
            // 既定で WebGPU を公開しない (requestAdapter() が null になる) ため必要。
            args: [
                "--enable-unsafe-webgpu",
                "--enable-unsafe-swiftshader",
                ...extraArgs,
            ],
        },
    },
});
