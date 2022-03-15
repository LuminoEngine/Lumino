// see: https://emscripten.org/docs/api_reference/Filesystem-API.html
Module["preRun"] = function () {
    FS.createFolder(
        "/",
        "Assets",
        true,
        false);
    FS.createPreloadedFile(
        "/",
        "Assets.lna",
        "/Assets.lna",
        true,
        false);
};