/**
 * Emscripte が生成する Module (このプロジェクトでは NWJSOverlap_Native) がサポートするのは、
 * - .wasm の読み込み (Module["_関数名"] で C 関数にアクセスできる)
 * - これを呼ぶための ccall または cwrap の提供
 * まで。
 * 使いやすいライブラリを考えると、Emscripte が生成する Module を直接公開するのはちょっと難しい。
 * そのためこれをラップするライブラリインターフェースを作る。
 */


const exampleApp = (function(global) {
    let lib = {};
    let Module = {};
    let _onLoadRuntime = null;
    let _onLoadErrorRuntime = null;


    // .wasm は独立したファイルなので、どうしてもサーバから fetch してくる必要がある。
    // https://emscripten.org/docs/compiling/WebAssembly.html#wasm-files-and-compilation
    lib.initRuntime = function(wasmPath, onLoad, onError) {
        _onLoadRuntime = onLoad;
        _onLoadErrorRuntime = onError;
        _initalize_wasm(wasmPath);
    }

    function _onRuntimeInitialized() {
        lib.native = {
            // C Functions
            myFunction: Module.cwrap("myFunction", null, null),
            initLumino: Module.cwrap("initLumino", null, null),
            initApp: Module.cwrap("initApp", null, null),
            renderApp: Module.cwrap("renderApp", null, ["number", "number"]),
        };

        if (_onLoadRuntime) {
            _onLoadRuntime();
        }
    }

    function _initalize_wasm(url) {
        const xhr = new XMLHttpRequest();
        xhr.open('GET', url, true);
        xhr.responseType = "arraybuffer";
        xhr.onload = () => {
            var params = {};
            params.wasmBinary = xhr.response;
            Example_Engine_NWJSOverlap(params).then(function (module) {
                Module = module;
                lib.Module = module;
                _onRuntimeInitialized();
            });
        };
        xhr.onerror = () => {
            _onerrorAssembly();
        };
        xhr.send(null);
    };
    

    return lib;

})(this);

// if (typeof exports !== 'undefined') {
//     exports = effekseer;
// }
