import * as Lumino from "./../dist/lumino.mjs";


Lumino.Runtime.initialize().then(() => {
    console.log("Lumino initialized");

    Lumino.API.LNRuntime_Terminate();

    const canvas = document.getElementById("maincanvas");
    var gl = canvas.getContext("webgl");

    console.log("canvas", canvas);
    console.log("Lumino.Runtime.module.GL", Lumino.Runtime.module.GL);
    const emGLContextHandle = Lumino.Runtime.module.GL.registerContext(gl, {
        // option は Effekseer を参考にしています。
        // Emscripte の生成するコードではコンテキスト情報として保持するものの参照はされていないようなので、仮の値を設定しておきます。
        // ※設定しておかないと ReferenceError が発生します。
        majorVersion: 1,
        minorVersion: 0,
        enableExtensionsByDefault: true,
    });
    console.log("emGLContextHandle", emGLContextHandle);

    
    gl.clearColor(0.0, 0.0, 1.0, 1.0);
    gl.clear(gl.COLOR_BUFFER_BIT);

    /*
    if (!canvas.getContextSafariWebGL2Fixed) {
        canvas.getContextSafariWebGL2Fixed = canvas.getContext;
        function fixedGetContext(ver, attrs) {
            var gl = canvas.getContextSafariWebGL2Fixed(ver, attrs);
            return ((ver == 'webgl') == (gl instanceof WebGLRenderingContext)) ? gl : null;
        }
        canvas.getContext = fixedGetContext;
    }

      var ctx =
        (canvas.getContext("webgl", webGLContextAttributes)
          // https://caniuse.com/#feat=webgl
          );

      if (!ctx) return 0;
      */
});


// const promise = LuminoFFI();
// promise.then((module) => {
//     console.log("loaded 1", module);


//     Lumino.API = {
//         LNRuntime_Initialize: module.cwrap("LNRuntime_Initialize", "number", []),
//     };


//     const result = Lumino.API.LNRuntime_Initialize();
//     console.log("Lumino initialized", result);
// });

