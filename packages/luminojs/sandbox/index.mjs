// Phase 0 smoke test: load the Emscripten module and call LNHelloTest.
// Expected DevTools console output:
//   Lumino HelloTest: 42    <-- from the C printf, routed to console.log
//   LNHelloTest(42) = 42    <-- from the JS console.log below
import LuminoC from "../lib/LuminoC.mjs";

const mod = await LuminoC();
const helloTest = mod.cwrap("LNHelloTest", "number", ["number"]);
console.log("LNHelloTest(42) =", helloTest(42));
