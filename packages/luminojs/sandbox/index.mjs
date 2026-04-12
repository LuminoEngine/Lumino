// Phase 0 + Phase 1 sandbox driver.
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

import LuminoC from "../lib/LuminoC.mjs";

const logEl = document.getElementById("log");
const btnInit      = document.getElementById("btn-init");
const btnTerminate = document.getElementById("btn-terminate");

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
// Phase 2: CoreInstance::init now creates a WebGPU device internally.
// Because WebGPU adapter/device requests are async, ASYNCIFY is used.
// cwrap with {async:true} returns a Promise for functions that may yield.
const lnInstanceInitialize = mod.cwrap("LNInstance_Initialize", "number", ["number"], {async: true});
const lnInstanceTerminate  = mod.cwrap("LNInstance_Terminate",  null,     [], {async: true});

let initialized = false;

function refreshButtons() {
    btnInit.disabled      = initialized;
    btnTerminate.disabled = !initialized;
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
        await lnInstanceTerminate();
        initialized = false;
        appendLog("LNInstance_Terminate returned");
    } catch (e) {
        appendLog(`LNInstance_Terminate threw: ${e}`);
    }
    refreshButtons();
});

refreshButtons();
appendLog("Module loaded. Click LNInstance_Initialize to start.");
