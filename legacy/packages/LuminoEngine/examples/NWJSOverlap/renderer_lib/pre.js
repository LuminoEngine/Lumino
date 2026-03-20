// see: https://emscripten.org/docs/api_reference/Filesystem-API.html
Module["preRun"] = function () {
    FS.createPreloadedFile(
        "/",
        "Assets.lna",
        "/dist/Assets.lna",
        true,
        false);
};