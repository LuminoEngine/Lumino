// Phase 0 + Phase 1 + Phase 3 sandbox driver.
//
// Phase 0: load the Emscripten module and call LNHelloTest to confirm the
//          wasm pipeline is wired up. Expected DevTools console output:
//            Lumino HelloTest: 42    <-- from the C printf
//            LNHelloTest(42) = 42    <-- from the JS console.log below
//
// Phase 1: two buttons call LNInstance_Initialize / LNInstance_Terminate
//          through the real ln::CoreInstance. The CoreInstance::init/dispose
//          path logs via LuminoBase's Logger, which WasmAPI.cpp routes to
//          stdout so everything lands in both the on-page log and the
//          browser console.
//
// Phase 3: LNWindow_CreateFromCanvas creates a Window + GraphicsContext
//          from an HTML <canvas>.

import LuminoC from "../lib/LuminoC.mjs";

const logEl = document.getElementById("log");
const btnInit          = document.getElementById("btn-init");
const btnTerminate     = document.getElementById("btn-terminate");
const btnCreateWindow  = document.getElementById("btn-create-window");
const btnGetCtx        = document.getElementById("btn-get-ctx");
const btnReleaseWindow = document.getElementById("btn-release-window");

function appendLog(line) {
    logEl.textContent += line + "\n";
    logEl.scrollTop = logEl.scrollHeight;
    console.log(line);
}

// Emscripten's Module.print / printErr fires once per line from C printf /
// std::fprintf(stderr, ...). Route both into the on-page log so the user can
// see CoreInstance log output without opening DevTools.
const mod = await LuminoC({
    print:    (text) => appendLog("[stdout] " + text),
    printErr: (text) => appendLog("[stderr] " + text),
});

// ─── Phase 0 smoke test ─────────────────────────────────────────────────
const helloTest = mod.cwrap("LNHelloTest", "number", ["number"]);
const helloResult = helloTest(42);
appendLog(`LNHelloTest(42) = ${helloResult}`);

// ─── Phase 1 + Phase 2: LNInstance_Initialize / LNInstance_Terminate ────
const lnInstanceInitialize = mod.cwrap("LNInstance_Initialize", "number", ["number"], {async: true});
const lnInstanceTerminate  = mod.cwrap("LNInstance_Terminate",  null,     [], {async: true});

// ─── Phase 3: LNWindow_CreateFromCanvas / LNWindow_GetGraphicsContext ───
const lnWindowCreateFromCanvas   = mod.cwrap("LNWindow_CreateFromCanvas",   "number", ["string", "number", "number", "number"], {async: true});
const lnWindowGetGraphicsContext = mod.cwrap("LNWindow_GetGraphicsContext", "number", ["number", "number"]);
const lnObjectRelease            = mod.cwrap("LNObject_Release",            "number", ["number"]);

// Allocate a persistent 4-byte slot for out-parameter pointers.
const ptrOut = mod._malloc(4);

let initialized = false;
let windowHandle = 0;
let gfxCtxHandle = 0;

function refreshButtons() {
    btnInit.disabled          = initialized;
    btnTerminate.disabled     = !initialized;
    btnCreateWindow.disabled  = !initialized || windowHandle !== 0;
    btnGetCtx.disabled        = windowHandle === 0;
    btnReleaseWindow.disabled = windowHandle === 0;
}

btnInit.addEventListener("click", async () => {
    appendLog("--- click: LNInstance_Initialize ---");
    try {
        const rc = await lnInstanceInitialize(0 /* settings = nullptr */);
        appendLog(`LNInstance_Initialize returned ${rc}`);
        if (rc === 0) initialized = true;
    } catch (e) {
        appendLog(`LNInstance_Initialize threw: ${e}`);
    }
    refreshButtons();
});

btnTerminate.addEventListener("click", async () => {
    appendLog("--- click: LNInstance_Terminate ---");
    try {
        if (windowHandle !== 0) {
            lnObjectRelease(windowHandle);
            windowHandle = 0;
            gfxCtxHandle = 0;
        }
        await lnInstanceTerminate();
        initialized = false;
        appendLog("LNInstance_Terminate returned");
    } catch (e) {
        appendLog(`LNInstance_Terminate threw: ${e}`);
    }
    refreshButtons();
});

btnCreateWindow.addEventListener("click", async () => {
    appendLog("--- click: LNWindow_CreateFromCanvas ---");
    try {
        mod.HEAPU32[ptrOut >> 2] = 0;
        const rc = await lnWindowCreateFromCanvas("#my_canvas", 640, 480, ptrOut);
        windowHandle = mod.HEAPU32[ptrOut >> 2];
        appendLog(`LNWindow_CreateFromCanvas returned rc=${rc}, handle=${windowHandle}`);
    } catch (e) {
        appendLog(`LNWindow_CreateFromCanvas threw: ${e}`);
    }
    refreshButtons();
});

btnGetCtx.addEventListener("click", () => {
    appendLog("--- click: LNWindow_GetGraphicsContext ---");
    try {
        mod.HEAPU32[ptrOut >> 2] = 0;
        const rc = lnWindowGetGraphicsContext(windowHandle, ptrOut);
        gfxCtxHandle = mod.HEAPU32[ptrOut >> 2];
        appendLog(`LNWindow_GetGraphicsContext returned rc=${rc}, gfxCtx=${gfxCtxHandle}`);
    } catch (e) {
        appendLog(`LNWindow_GetGraphicsContext threw: ${e}`);
    }
});

btnReleaseWindow.addEventListener("click", () => {
    appendLog("--- click: LNObject_Release (window) ---");
    try {
        const rc = lnObjectRelease(windowHandle);
        appendLog(`LNObject_Release returned rc=${rc}`);
        windowHandle = 0;
        gfxCtxHandle = 0;
    } catch (e) {
        appendLog(`LNObject_Release threw: ${e}`);
    }
    refreshButtons();
});

refreshButtons();
appendLog("Module loaded. Click LNInstance_Initialize to start.");
