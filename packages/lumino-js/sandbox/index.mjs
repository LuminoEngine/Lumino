import * as luuu from "./../dist/lumino.mjs";


console.log("luuu", luuu);
luuu.Runtime.initialize().then(() => {
    console.log("Lumino initialized");
});

const Lumino = {

};

// const promise = LuminoFFI();
// promise.then((module) => {
//     console.log("loaded 1", module);


//     Lumino.API = {
//         LNRuntime_Initialize: module.cwrap("LNRuntime_Initialize", "number", []),
//     };


//     const result = Lumino.API.LNRuntime_Initialize();
//     console.log("Lumino initialized", result);
// });

