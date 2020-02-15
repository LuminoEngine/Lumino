Module['preRun'] = function () {
    FS.createFolder(
        '/',
        'Assets',
        true,
        false);
    FS.createPreloadedFile(
        '/',
        'Assets.lca',
        '/Assets.lca',
        true,
        false);
};