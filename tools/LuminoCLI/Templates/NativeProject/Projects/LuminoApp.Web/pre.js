Module['preRun'] = function () {
    FS.createFolder(
        '/', // 親フォルダの指定
        'Assets', // フォルダ名
        true, // 読み込み許可
        false // 書き込み許可(今回の例はfalseでもよさげ)
    );
    FS.createPreloadedFile(
        '/',// 親フォルダの指定
        'LineWave.lufx',// ソース中でのファイル名
        '/LineWave.lufx',  // httpでアクセスする際のURLを指定
        true,
        false
    );
};