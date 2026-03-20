addToLibrary({
    LuminoGLSubmitCommandList: function(ptr) {
        console.log("LuminoGLSubmitCommandList", ptr);
        console.log("  Module2", Module);
        console.log("  Module[LuminoGLSubmitCommandList2]", Module["LuminoGLSubmitCommandList2"]);
        Module["LuminoGLSubmitCommandList2"](ptr);
    },
});

