// luminojs WASM スモークテスト (改善案 #12)。
//
// GoogleTest はすべてデスクトップ (Vulkan) ビルドを検証しており、luminojs が実際に
// ロードする WASM バイナリ (LuminoC.wasm) を検証する自動テストが存在しなかった。
// 本テストは「ブラウザが実際にロードする経路」で WASM を読み込み、GPU に依存しない
// (もしくは WebGPU デバイス初期化までで完結する) API の疎通を検証する。
//
// 採用方式: Playwright + フル Chromium (channel: "chromium")。
//  - LuminoC.mjs は -sENVIRONMENT=web でビルドされており、素の Node では
//    "not compiled for this environment" で instantiate に失敗する。
//  - Runtime.initialize() は内部で LNInstance_Initialize -> WebGPU デバイス生成を
//    行うため WebGPU が必須。Node には WebGPU が無いが、フル Chromium なら
//    (ハードウェア、もしくは --enable-unsafe-swiftshader によるソフトウェア実装で)
//    WebGPU デバイスを生成できる。
//
// 実行前提はこのディレクトリの README.md を参照。

import { test, expect } from "@playwright/test";
import fs from "node:fs";
import path from "node:path";
import { fileURLToPath } from "node:url";
import { startStaticServer } from "./static-server.mjs";
import { makePng } from "./make-png.mjs";

// このファイルは test/smoke/ にあるため、../../ が luminojs パッケージルート。
const packageRoot = path.resolve(fileURLToPath(new URL("../../", import.meta.url)));

// コンパイル済みシェーダ (.lcsh)。GoogleTest と同じテストデータを使い、
// WebGPU (WGSL) ターゲットのカスタムシェーダ経路を検証する。
const COMPILED_SHADER = fs.readFileSync(
    path.resolve(packageRoot, "../LuminoC/test/Data/Unlit.lcsh"));

// 既知のピクセル値を持つ 2x2 PNG。上から下、左から右の順に並ぶ RGBA8。
//   (0,0)=赤  (1,0)=緑
//   (0,1)=青  (1,1)=白
const TEST_PNG = makePng(
    2,
    2,
    Buffer.from([
        255, 0, 0, 255, /**/ 0, 255, 0, 255,
        0, 0, 255, 255, /**/ 255, 255, 255, 255,
    ]),
);

// WebGPU が使えない環境で initialize (WebGPU 必須) の検証をどう扱うかのフラグ。
//   false (既定): skip しない。WebGPU が使えない環境ではこのテストは「失敗」する。
//                 -> どの環境で WebGPU が使える/使えないかに気づくための設定。
//   true        : WebGPU アダプタが取得できない環境では initialize の検証を自動 skip する。
//                 -> GPU 無しの CI 等で GPU 非依存の 4 項目だけを回したいときに切り替える。
// この定数を書き換えるだけで ON/OFF を切り替えられる。
// 一時的に切り替えたい場合は環境変数 LUMINO_SMOKE_SKIP_NO_WEBGPU=1 でも true にできる。
const SKIP_INITIALIZE_WHEN_NO_WEBGPU =
    false || process.env.LUMINO_SMOKE_SKIP_NO_WEBGPU === "1";

// beforeAll で WASM を一度だけロードして初期化し、各 API の結果をここに集約する。
// こうすることで WASM ロード (-O0 -g3 のため数秒かかる) を 1 回に抑えつつ、
// 各検証項目を独立した test() として明快に表現できる。
let result;
let server;

test.beforeAll(async ({ browser }) => {
    server = await startStaticServer(packageRoot);

    const page = await browser.newPage();
    // ブラウザ側のログを Node 側へ中継 (失敗時の診断用)。
    page.on("console", (msg) => console.log(`[browser:${msg.type()}]`, msg.text()));
    page.on("pageerror", (err) => console.log("[browser:pageerror]", err.message));

    await page.goto(server.baseURL + "/");

    result = await page.evaluate(async ({ pngBytes, shaderBytes }) => {
        const res = {
            moduleLoaded: false,
            loadError: null,
            webgpuAvailable: false,
            initialize: {},
            helloTest: {},
            buildTimestamp: {},
            decodeImage: {},
            profilerExport: {},
            profiler: { attempted: false },
            sharedShader: { attempted: false },
            deviceLost: { attempted: false },
        };

        // WebGPU アダプタの有無を先に調べる (skip 判定に使う)。
        // これは「WebGPU が使えるか」の素の指標で、initialize の成否とは分けて扱う。
        // (アダプタはあるのに initialize が落ちる = 実装バグは skip させず検出したいため)。
        try {
            res.webgpuAvailable =
                "gpu" in navigator && !!(await navigator.gpu.requestAdapter());
        } catch {
            res.webgpuAvailable = false;
        }

        let Runtime;
        try {
            ({ Runtime } = await import(location.origin + "/lib/luminojs.mjs"));
            res.moduleLoaded = true;
        } catch (e) {
            res.loadError = String(e && e.stack ? e.stack : e);
            return res;
        }

        // 1. Runtime.initialize()
        //    = WASM ロード + 全 C-API バインド + 構造体レイアウト照合 (#10) +
        //      LNInstance_Initialize (WebGPU デバイス生成)。
        //    構造体サイズ不一致があれば _verifyStructLayouts が例外を投げるため、
        //    initialize が成功すること自体が #10 の照合成功を意味する。
        try {
            await Runtime.initialize();
            res.initialize = { ok: true, initialized: Runtime.initialized };
        } catch (e) {
            res.initialize = { ok: false, error: String(e && e.message ? e.message : e) };
        }

        // 2. LNHelloTest(42) == 42。
        //    公開ラッパは無いが、Runtime.module 経由で実バイナリのエクスポートを直接叩く。
        try {
            const hello = Runtime.module.cwrap("LNHelloTest", "number", ["number"]);
            res.helloTest = { ok: true, value: hello(42) };
        } catch (e) {
            res.helloTest = { ok: false, error: String(e && e.message ? e.message : e) };
        }

        // 3. getBuildTimestamp() が空でない文字列を返す。
        try {
            const ts = Runtime.getBuildTimestamp();
            res.buildTimestamp = { ok: true, value: ts };
        } catch (e) {
            res.buildTimestamp = { ok: false, error: String(e && e.message ? e.message : e) };
        }

        // 4. 画像デコード (GPU 非依存の純 CPU 経路)。
        try {
            const decoded = Runtime.decodeImage(new Uint8Array(pngBytes));
            res.decodeImage = {
                ok: true,
                width: decoded.width,
                height: decoded.height,
                length: decoded.pixels.length,
                firstPixel: Array.from(decoded.pixels.slice(0, 4)),
            };
        } catch (e) {
            res.decodeImage = { ok: false, error: String(e && e.message ? e.message : e) };
        }

        // 5. LNDebug_GetGraphicsProfiler が WASM にエクスポートされている (GPU 非依存)。
        //    この関数は元々 __EMSCRIPTEN__ ガードの内側にあり WASM に存在しなかったため、
        //    「実バイナリから呼べること」を GPU の有無に関わらず検証する。
        //    無効ハンドルを渡すと LN_ERROR_INVALID_HANDLE (-4) が返るので、
        //    WebGPU デバイスが無い環境でもエクスポートの疎通を確認できる。
        try {
            const m = Runtime.module;
            const getStructSize = m.cwrap("LNDebug_GetStructSize", "number", ["string", "number"]);
            const getProfiler = m.cwrap("LNDebug_GetGraphicsProfiler", "number", ["number", "number"]);

            const outPtr = m._malloc(16);
            try {
                // 実バイナリが報告する sizeof(LNGraphicsProfiler)。
                const sizeRc = getStructSize("LNGraphicsProfiler", outPtr);
                const wasmSize = new Uint32Array(m.HEAPU8.buffer, outPtr, 1)[0];

                // 無効ハンドル (0) での呼び出し。エクスポートされていなければ
                // cwrap の時点で例外になる。
                const invalidRc = getProfiler(0, outPtr);

                res.profilerExport = { ok: true, sizeRc, wasmSize, invalidRc };
            } finally {
                m._free(outPtr);
            }
        } catch (e) {
            res.profilerExport = { ok: false, error: String(e && e.message ? e.message : e) };
        }

        // 6. GraphicsContext.getProfiler()。
        //    LNDebug_GetGraphicsProfiler の WASM 経路での疎通と、スプライトの
        //    バッチングが drawCallCount に反映されることを検証する。
        if (res.initialize.ok && res.webgpuAvailable) {
            res.profiler.attempted = true;
            try {
                const { GraphicsContext, Camera, Material, Matrix4x4, LoadOp } =
                    await import(location.origin + "/lib/luminojs.mjs");

                const canvas = document.createElement("canvas");
                canvas.id = "lumino_profiler_canvas";
                canvas.width = 64;
                canvas.height = 64;
                document.body.appendChild(canvas);

                const ctx = await GraphicsContext.createFromCanvas("#lumino_profiler_canvas");
                const camera = Camera.create();
                camera.setOrthographic2D(64, 64, -100, 100);
                const material = Material.createUnlit();
                const nextFrame = () => new Promise((r) => requestAnimationFrame(r));

                // 同一マテリアルのスプライトを spriteCount 枚描画し、endFrame 後の
                // プロファイラ値を返す。
                const renderSprites = async (spriteCount) => {
                    const f = ctx.beginFrame();
                    if (!f) throw new Error("beginFrame returned null");
                    f.renderer.beginRenderPass(ctx, {
                        colorAttachments: [{ clearColor: [0, 0, 0, 1], loadOp: LoadOp.Clear }],
                    }, camera);
                    for (let i = 0; i < spriteCount; i++) {
                        f.renderer.drawSprite(
                            material, 0,
                            Matrix4x4.makeTranslation(i, 0, 0),
                            0, 0, 1, 1, 0.5, 0.5,
                            0, 0, 1, 1,
                            1, 1, 1, 1);
                    }
                    f.renderer.endRenderPass();
                    ctx.endFrame();
                    // 計測値は endFrame の後に読む (fps / lastFrameTimeMs は
                    // endFrame の中で更新される)。
                    const p = ctx.getProfiler();
                    await nextFrame();
                    return p;
                };

                // 1 フレーム目は初期化コストで計測値が安定しないため捨てる。
                await renderSprites(1);
                const one = await renderSprites(1);
                const many = await renderSprites(16);

                res.profiler = {
                    attempted: true,
                    ok: true,
                    one,
                    many,
                    keys: Object.keys(one).sort(),
                };
            } catch (e) {
                res.profiler = {
                    attempted: true,
                    ok: false,
                    error: String(e && e.stack ? e.stack : e),
                };
            }
        }

        // 6b. Shader の共有 (WebGPU / WGSL 経路)。
        //     コンパイル済みシェーダ (.lcsh) から Shader を 1 つ作り、そこから
        //     Material を複数作っても GPU シェーダモジュール / パイプラインレイアウトが
        //     増えないことを shaderPassCount で検証する。
        if (res.initialize.ok && res.webgpuAvailable) {
            res.sharedShader.attempted = true;
            try {
                const { GraphicsContext, Camera, Material, Shader, Matrix4x4, LoadOp } =
                    await import(location.origin + "/lib/luminojs.mjs");

                const canvas = document.createElement("canvas");
                canvas.id = "lumino_sharedshader_canvas";
                canvas.width = 64;
                canvas.height = 64;
                document.body.appendChild(canvas);

                const ctx = await GraphicsContext.createFromCanvas("#lumino_sharedshader_canvas");
                const camera = Camera.create();
                camera.setOrthographic2D(64, 64, -100, 100);
                const nextFrame = () => new Promise((r) => requestAnimationFrame(r));

                // 渡された材料をすべて描画して 1 フレーム進め、shaderPassCount を返す。
                // GPU リソースは初回描画時に遅延生成されるため、必ず描画してから測る。
                const drawAndCount = async (materials) => {
                    const f = ctx.beginFrame();
                    if (!f) throw new Error("beginFrame returned null");
                    f.renderer.beginRenderPass(ctx, {
                        colorAttachments: [{ clearColor: [0, 0, 0, 1], loadOp: LoadOp.Clear }],
                    }, camera);
                    const xf = Matrix4x4.makeTranslation(0, 0, 0);
                    for (const mat of materials) {
                        f.renderer.drawSprite(
                            mat, 0, xf,
                            0, 0, 8, 8, 0.5, 0.5,
                            0, 0, 1, 1,
                            1, 1, 1, 1);
                    }
                    f.renderer.endRenderPass();
                    ctx.endFrame();
                    const count = ctx.getProfiler().shaderPassCount;
                    await nextFrame();
                    return count;
                };

                const shader = Shader.createFromCompiledShader(new Uint8Array(shaderBytes));

                // 1 つ目の Material を描画 -> ここでシェーダモジュールが作られる。
                const shared = [Material.createFromShader(shader)];
                const countAfterFirst = await drawAndCount(shared);

                // 同じ Shader から Material を 4 つ追加。パラメータも個別に設定する。
                for (let i = 0; i < 4; i++) {
                    const mat = Material.createFromShader(shader);
                    mat.setColor(i / 4, 1 - i / 4, 0.5, 1);
                    shared.push(mat);
                }
                const countAfterShared = await drawAndCount(shared);

                // 比較: createFromCompiledShader は Material ごとに新規生成する。
                const standalone = [];
                for (let i = 0; i < 4; i++) {
                    standalone.push(Material.createFromCompiledShader(new Uint8Array(shaderBytes)));
                }
                const countAfterStandalone = await drawAndCount(standalone);

                res.sharedShader = {
                    attempted: true,
                    ok: true,
                    countAfterFirst,
                    countAfterShared,
                    countAfterStandalone,
                };
            } catch (e) {
                res.sharedShader = {
                    attempted: true,
                    ok: false,
                    error: String(e && e.stack ? e.stack : e),
                };
            }
        }

        // 7. デバイスロスト自動復旧 (docs/plans/device-lost-design.md フェーズ B3)。
        //    実 WebGPU デバイスを wgpuDeviceDestroy で破棄し (deep シミュレーション)、
        //    フレームループを回すだけで自動復旧して描画が再開できることを検証する。
        if (res.initialize.ok && res.webgpuAvailable) {
            res.deviceLost.attempted = true;
            try {
                const { GraphicsContext, Camera, LoadOp } =
                    await import(location.origin + "/lib/luminojs.mjs");

                const canvas = document.createElement("canvas");
                canvas.id = "lumino_devicelost_canvas";
                canvas.width = 64;
                canvas.height = 64;
                document.body.appendChild(canvas);

                const ctx = await GraphicsContext.createFromCanvas("#lumino_devicelost_canvas");
                const camera = Camera.create();

                const renderOnce = (frameInfo) => {
                    frameInfo.renderer.beginRenderPass(ctx, {
                        colorAttachments: [{ clearColor: [0, 0, 1, 1], loadOp: LoadOp.Clear }],
                    }, camera);
                    frameInfo.renderer.endRenderPass();
                    ctx.endFrame();
                };
                const nextFrame = () => new Promise((r) => requestAnimationFrame(r));

                // 正常フレームを数回描画する (前提確認)
                for (let i = 0; i < 3; i++) {
                    const f = ctx.beginFrame();
                    if (!f) throw new Error("beginFrame returned null before simulate");
                    renderOnce(f);
                    await nextFrame();
                }

                // デバイスロストをシミュレートする (deep: 実際にデバイスを破棄する)
                const simulate = Runtime.module.cwrap(
                    "LNDebug_SimulateDeviceLost", "number", ["number"]);
                const simRc = simulate(1);

                // フレームループを回し続けるだけで自動復旧することを確認する。
                // アダプタ/デバイスの再取得はブラウザのイベントループ経由で解決される
                // ため、フレーム間で rAF により制御を返す。
                let restoredCount = 0;
                ctx.onDeviceRestored = () => restoredCount++;
                let nullFrames = 0;
                let recovered = false;
                for (let i = 0; i < 600; i++) {
                    const f = ctx.beginFrame();
                    if (f) {
                        renderOnce(f);
                        recovered = true;
                        break;
                    }
                    nullFrames++;
                    await nextFrame();
                }

                // 復旧後も継続して描画できること
                let postFrames = 0;
                for (let i = 0; i < 3; i++) {
                    await nextFrame();
                    const f = ctx.beginFrame();
                    if (f) {
                        renderOnce(f);
                        postFrames++;
                    }
                }

                res.deviceLost = {
                    attempted: true,
                    ok: true,
                    simRc,
                    nullFrames,
                    recovered,
                    restoredCount,
                    postFrames,
                };
            } catch (e) {
                res.deviceLost = {
                    attempted: true,
                    ok: false,
                    error: String(e && e.stack ? e.stack : e),
                };
            }
        }

        return res;
    }, { pngBytes: Array.from(TEST_PNG), shaderBytes: Array.from(COMPILED_SHADER) });

    await page.close();
});

test.afterAll(async () => {
    if (server) {
        await new Promise((resolve) => server.server.close(resolve));
    }
});

test("WASM モジュール (LuminoC.mjs/.wasm) がブラウザでロードできる", () => {
    expect(result.loadError, "module load error").toBeNull();
    expect(result.moduleLoaded).toBe(true);
});

test("Runtime.initialize() が成功する (WASM ロード + LNInstance_Initialize + ABI照合#10)", () => {
    // SKIP_INITIALIZE_WHEN_NO_WEBGPU が true かつ WebGPU アダプタが無い環境でのみ skip。
    // 既定 (false) では skip せず、WebGPU が使えない環境では「失敗」させて気づけるようにする。
    test.skip(
        SKIP_INITIALIZE_WHEN_NO_WEBGPU && !result.webgpuAvailable,
        "WebGPU アダプタが利用できない環境のため skip (SKIP_INITIALIZE_WHEN_NO_WEBGPU=true)");

    // 構造体サイズ照合 (#10) は initialize 内で走る。不一致なら例外になるため、
    // ここで error が null であることが照合成功の確認を兼ねる。
    expect(result.initialize.error ?? null, "initialize error").toBeNull();
    expect(result.initialize.ok).toBe(true);
    expect(result.initialize.initialized).toBe(true);
});

test("LNHelloTest(42) が 42 を返す", () => {
    expect(result.helloTest.error ?? null, "helloTest error").toBeNull();
    expect(result.helloTest.value).toBe(42);
});

test("getBuildTimestamp() が空でない文字列を返す", () => {
    expect(result.buildTimestamp.error ?? null, "buildTimestamp error").toBeNull();
    expect(typeof result.buildTimestamp.value).toBe("string");
    expect(result.buildTimestamp.value.length).toBeGreaterThan(0);
});

test("decodeImage() が小さな PNG を期待どおりの RGBA ピクセルにデコードする", () => {
    expect(result.decodeImage.error ?? null, "decodeImage error").toBeNull();
    expect(result.decodeImage.width).toBe(2);
    expect(result.decodeImage.height).toBe(2);
    // RGBA8 なので幅 * 高さ * 4 バイト。
    expect(result.decodeImage.length).toBe(2 * 2 * 4);
    // 先頭ピクセル (左上) は赤。
    expect(result.decodeImage.firstPixel).toEqual([255, 0, 0, 255]);
});

// GPU 非依存。WebGPU が使えない環境でも常に実行される。
test("LNDebug_GetGraphicsProfiler が WASM にエクスポートされている", () => {
    expect(result.profilerExport.error ?? null, "profilerExport error").toBeNull();
    expect(result.profilerExport.ok).toBe(true);

    // LNDebug_GetStructSize が LNGraphicsProfiler を認識する (LN_OK = 0)。
    expect(result.profilerExport.sizeRc).toBe(0);
    // wasm32 での sizeof(LNGraphicsProfiler) = int32 + float + float + int32。
    // types.ts の SIZEOF_GRAPHICS_PROFILER と一致していなければならない。
    expect(result.profilerExport.wasmSize).toBe(16);

    // 無効ハンドルでの呼び出しが LN_ERROR_INVALID_HANDLE (-4) を返す
    // = 関数が実バイナリに存在し、ブラウザから呼び出せている。
    expect(result.profilerExport.invalidRc).toBe(-4);
});

test("getProfiler() が WASM (WebGPU) 経路で drawCallCount / fps / lastFrameTimeMs を返す", () => {
    test.skip(
        SKIP_INITIALIZE_WHEN_NO_WEBGPU && !result.webgpuAvailable,
        "WebGPU アダプタが利用できない環境のため skip (SKIP_INITIALIZE_WHEN_NO_WEBGPU=true)");

    expect(result.profiler.attempted, "profiler scenario not attempted").toBe(true);
    expect(result.profiler.error ?? null, "profiler error").toBeNull();
    expect(result.profiler.ok).toBe(true);

    // LNGraphicsProfiler の全項目が公開されている (Object.keys().sort() で比較)。
    expect(result.profiler.keys).toEqual(
        ["drawCallCount", "fps", "lastFrameTimeMs", "shaderPassCount"]);

    const { one, many } = result.profiler;

    // スプライトを 1 枚描いたフレームでは 1 回以上のドローコールが記録される。
    expect(Number.isInteger(one.drawCallCount)).toBe(true);
    expect(one.drawCallCount).toBeGreaterThan(0);

    // endFrame の後に読んでいるので、フレーム時間と FPS は正の実測値になる。
    expect(one.lastFrameTimeMs).toBeGreaterThan(0);
    expect(one.fps).toBeGreaterThan(0);

    // 同一マテリアルのスプライト 16 枚がバッチングされ、ドローコールが
    // 枚数に比例して増えないこと (クライアントが確認したかった性質)。
    expect(many.drawCallCount).toBeLessThan(16);
});

test("1 つの Shader から作った Material 群がシェーダモジュールを共有する (WebGPU)", () => {
    test.skip(
        SKIP_INITIALIZE_WHEN_NO_WEBGPU && !result.webgpuAvailable,
        "WebGPU アダプタが利用できない環境のため skip (SKIP_INITIALIZE_WHEN_NO_WEBGPU=true)");

    expect(result.sharedShader.attempted, "sharedShader scenario not attempted").toBe(true);
    expect(result.sharedShader.error ?? null, "sharedShader error").toBeNull();
    expect(result.sharedShader.ok).toBe(true);

    const { countAfterFirst, countAfterShared, countAfterStandalone } = result.sharedShader;

    // コンパイル済みシェーダのパスが実際に構築されている (WGSL ターゲットが選ばれている)。
    expect(countAfterFirst).toBeGreaterThan(0);

    // 同一 Shader から Material を 4 つ追加してもシェーダパスは増えない
    // = シェーダモジュールとパイプラインレイアウトが共有されている。
    expect(countAfterShared).toBe(countAfterFirst);

    // 対照: Material.createFromCompiledShader は Material ごとに新規生成する。
    expect(countAfterStandalone).toBeGreaterThan(countAfterShared);
});

test("デバイスロスト後、フレームループを回すだけで自動復旧して描画が再開できる", () => {
    test.skip(
        SKIP_INITIALIZE_WHEN_NO_WEBGPU && !result.webgpuAvailable,
        "WebGPU アダプタが利用できない環境のため skip (SKIP_INITIALIZE_WHEN_NO_WEBGPU=true)");

    expect(result.deviceLost.attempted, "deviceLost scenario not attempted").toBe(true);
    expect(result.deviceLost.error ?? null, "deviceLost error").toBeNull();
    expect(result.deviceLost.ok).toBe(true);
    // LNDebug_SimulateDeviceLost が成功している (LN_OK = 0)
    expect(result.deviceLost.simRc).toBe(0);
    // ロスト直後は少なくとも 1 フレームは null (復旧待ち) が観測される
    expect(result.deviceLost.nullFrames).toBeGreaterThan(0);
    // 有限フレーム内に自動復旧して FrameInfo が返る
    expect(result.deviceLost.recovered).toBe(true);
    // onDeviceRestored フックが一度だけ呼ばれる
    expect(result.deviceLost.restoredCount).toBe(1);
    // 復旧後も継続して描画できる
    expect(result.deviceLost.postFrames).toBeGreaterThan(0);
});

// TODO(#12): デバイスロスト復旧以外の GPU (WebGPU) レンダリング経路のスモーク
// (DrawSprite / DrawMesh / Capture 等の描画結果検証) は未対応。
// 将来 headless での実描画が安定して確認できたら、RenderTarget への描画 +
// ピクセル読み戻しによる最小の実描画スモークをここに追加する。
