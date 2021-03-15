
Module['preRun'] = function () {
    console.log("prerun3");
    FS.createFolder(
        '/', // 親フォルダの指定
        'data', // フォルダ名
        true, // 読み込み許可
        true // 書き込み許可(今回の例はfalseでもよさげ)
    );

    //create your directory where we keep our persistent data
    FS.mkdir('/IDBFS'); 
 
    //mount persistent directory as IDBFS
    FS.mount(IDBFS, {}, '/IDBFS');

    
    FS.syncfs(true, function (err) {
        assert(!err);
    }); // sync FROM backing store

    console.log("prerun done.");
};
// python -m SimpleHTTPServer 8000

/*
https://kripken.github.io/emscripten-site/docs/porting/files/file_systems_overview.html
Note: デフォルトの MEMFS はインメモリファイルシステム。再起動するとファイルは全部消える。

http://wlog.flatlib.jp/archive/1/2014-05


https://stackoverflow.com/questions/23376234/emscripten-and-persistence
*/
