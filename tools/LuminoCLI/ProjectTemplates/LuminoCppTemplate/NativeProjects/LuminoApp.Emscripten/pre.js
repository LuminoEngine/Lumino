Module['preRun'] = function () {
    FS.createFolder(
        '/', // 親フォルダの指定
        'Assets', // フォルダ名
        true, // 読み込み許可
        false // 書き込み許可(今回の例はfalseでもよさげ)
    );
    FS.createPreloadedFile(
        '/Assets',// 親フォルダの指定
        'test.txt',// ソース中でのファイル名
        '/Assets/test.txt',  // httpでアクセスする際のURLを指定
        true,
        false
    );
    FS.createPreloadedFile(
        '/Assets',// 親フォルダの指定
        '8_MapBGM2.wav',// ソース中でのファイル名
        '/Assets/8_MapBGM2.wav',  // httpでアクセスする際のURLを指定
        true,
        false
    );
};