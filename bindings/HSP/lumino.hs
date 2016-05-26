;============================================================
; Lumino ヘルプファイル
;============================================================

%dll
Lumino

%ver
0.2.0.0

%date
2016/05/26

%author
lriki

%note
lumino.as をインクルードしてください

%type
拡張命令

%url
http://nnmy.sakura.ne.jp/

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetApplicationLogEnabled
デバッグ用のログファイルの出力有無を設定します。(既定値:LN_FALSE)

%group
エンジン
%prm
enabled
[in] enabled : LN_TRUE:出力する / LN_FALSE:出力しない

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetConsoleEnabled
標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(既定値:LN_FALSE)

%group
エンジン
%prm
enabled
[in] enabled : LN_TRUE:割り当てる / LN_FALSE:割り当てない

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_RegisterArchive
ファイルを開くときにアクセスする暗号化アーカイブを登録します。

%group
エンジン
%prm
filePath, password
[in] filePath : アーカイブファイルパス
[in] password : アーカイブファイルを開くためのパスワード

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetFileAccessPriority
ファイルへのアクセス優先順位を設定します。

%group
エンジン
%prm
priority
[in] priority : 制限方法 (default:LN_FILEACCESSPRIORITY_DIRECTORY_FIRST)
                   LN_FILEACCESSPRIORITY_ARCHIVE_FIRST
                       アーカイブ優先
                   LN_FILEACCESSPRIORITY_DIRECTORY_FIRST
                       ディレクトリ優先
                   LN_FILEACCESSPRIORITY_ARCHIVE_ONLY
                       アーカイブのみ

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetUserWindowHandle
ユーザー定義のウィンドウハンドルを設定します。(既定値:NULL)

%group
エンジン
%prm
windowHandle
[in] windowHandle : ユーザー定義のウィンドウハンドル

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetSoundCacheSize
サウンドオブジェクトのキャッシュサイズの設定

%group
エンジン
%prm
objectCount, memorySize
[in] objectCount : キャッシュできるサウンドオブジェクトの最大数 (既定値:32)
[in] memorySize  : サウンドオブジェクトのキャッシュが使用できる最大メモリサイズ (既定値:0)

%inst
objectCount が 0 の場合、キャッシュを使用しません。
memorySize が 0 の場合、メモリ使用量に制限を設けません。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetDirectMusicMode
DirectMusic の初期化方法を設定します。(既定値:LN_DIRECTMUSICMODE_NOT_USE)

%group
エンジン
%prm
mode
[in] mode : DirectMusic の初期化方法
               LN_DIRECTMUSICMODE_NOT_USE
                   DirectMusic を使用しない
               LN_DIRECTMUSICMODE_NORMAL
                   通常
               LN_DIRECTMUSICMODE_THREAD_WAIT
                   別スレッドで初期化して、再生時に未完了の場合は待つ
               LN_DIRECTMUSICMODE_THREAD_REQUEST
                   別スレッドで初期化して、再生時に未完了の場合は再生を予約する

%inst
DirectMusic の初期化には比較的時間がかかります。
これを回避するために初期化専用のスレッドで初期化を行うことが出来ます。
なお、DirectMusic を使用するためには LNConfig_SetUserWindowHandle でウィンドウハンドルを設定する必要があります。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNConfig_SetDirectMusicReverbLevel
DirectMusic のリバーブエフェクトの強さを設定します。(既定値:0.75)

%group
エンジン
%prm
level
[in] level : リバーブの強さ (0.0 ～ 1.0)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_Initialize
ライブラリを初期化します。

%group
エンジン
%prm
　

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_InitializeAudio
ライブラリを初期化します。音声機能のみを使用する場合に呼び出します。

%group
エンジン
%prm
　

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_UpdateFrame
1フレーム分の更新処理を行います。

%group
エンジン
%prm
　

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
この命令はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_IsEndRequested
アプリケーションを終了するべきかを確認します。

%group
エンジン
%prm
outRequested
[out] outRequested : 終了要求の有無を格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
ウィンドウのクローズボタンが押された場合等、
アプリケーションを終了するべき時には LN_FALSE を返します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNEngine_Terminate
ライブラリの終了処理を行います。

%group
エンジン
%prm
　

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetMajor
メジャーバージョンを取得します。

%group
エンジン
%prm
outMajor
[out] outMajor : バージョン番号を格納する変数

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetMinor
マイナーバージョンを取得します。

%group
エンジン
%prm
outMinor
[out] outMinor : バージョン番号を格納する変数

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetRevision
リビジョンバージョンを取得します。

%group
エンジン
%prm
outRevision
[out] outRevision : バージョン番号を格納する変数

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetBuild
ビルドバージョンを取得します。

%group
エンジン
%prm
outBuild
[out] outBuild : バージョン番号を格納する変数

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_GetString
バージョン文字列を取得します。

%group
エンジン
%prm
outStr
[out] outStr : 文字列へを格納する変数

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVersion_IsAtLeast
指定したバージョン番号と、ライブラリファイルのコンパイルバージョン番号を比較します。

%group
エンジン
%prm
major, minor, revision, outResult
[in]  major     : メジャーバージョン
[in]  minor     : マイナーバージョン
[in]  revision  : リビジョンバージョン
[out] outResult : 結果を格納する変数

%inst
指定バージョン >= コンパイルバージョン である場合、LN_TRUE となります。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNError_GetLastErrorCode
最後に発生したエラーのエラーコードを取得します。

%group
基本機能
%prm
　

stat : エラーコード
    LN_OK
        成功
    LN_ERROR_UNKNOWN
        不明なエラー
    LN_ERROR_VERIFY
        前提条件の検証エラー
    LN_ERROR_ARGUMENT
        引数が無効
    LN_ERROR_INVALID_OPERATION
        オブジェクトの現在の状態に対して無効な呼び出しが行われた
    LN_ERROR_NOT_IMPLEMENTED
        未実装の機能を呼び出した
    LN_ERROR_OUT_OF_MEMORY
        メモリ確保に失敗
    LN_ERROR_OUT_OF_RANGE
        値が有効な範囲に存在しない
    LN_ERROR_KEY_NOT_FOUND
        検索に使用したキーが存在しない
    LN_ERROR_OVERFLOW
        数値演算によるオーバーフローが発生した
    LN_ERROR_IO
        その他のIOエラー
    LN_ERROR_FILE_NOT_FOUND
        ファイルにアクセスしようとして失敗した
    LN_ERROR_DIRECTORY_NOT_FOUND
        無効なディレクトリにアクセスしようとした
    LN_ERROR_INVALID_FORMAT
        ファイルや文字列等の形式が不正
    LN_ERROR_END_OF_STREAM
        ストリームの末尾を越えてアクセスしようとした
    LN_ERROR_ENCODING
        文字コードの変換中、マッピングできない文字または不正シーケンスが見つかった
    LN_ERROR_WIN32
        WindowsAPI のエラー
    LN_ERROR_COM
        COM のエラー


%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNError_GetLastErrorMessage
最後に発生したエラーのエラーメッセージを取得します。

%group
基本機能
%prm
outStr
[out] outStr : メッセージ文字列を格納する変数。

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNObject_Release
オブジェクトを解放します。

%group
基本機能
%prm
hadnleObject
[in] hadnleObject : オブジェクトハンドル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
指定されたオブジェクトの参照を解放します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNObject_AddRef
オブジェクトの参照カウントをインクリメントします。

%group
基本機能
%prm
hadnleObject
[in] hadnleObject : オブジェクトハンドル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
この命令は「参照カウント」という概念について十分に理解した上で使用してください。
参照カウントは LNObject_AddRef でインクリメントされ、LNObject_Release でデクリメントされます。
Release 漏れがあるとメモリリークの原因になります。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNObject_GetRefCount
オブジェクトの参照カウント取得します。

%group
基本機能
%prm
hadnleObject, count
[in]  hadnleObject : オブジェクトハンドル
[out] count        : 参照カウントを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_GetLength
2D ベクトルの長さを取得します。

%group
算術機能
%prm
vec, outLength
[in]  vec       : 処理の基になる 2D ベクトル
[out] outLength : ベクトルの長さを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_GetSquareLength
2D ベクトルの長さの2乗を取得します。

%group
算術機能
%prm
vec, outLength
[in]  vec       : 処理の基になる 2D ベクトル
[out] outLength : ベクトルの長さの2乗を格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_Set
2D ベクトルに値を設定します。

%group
算術機能
%prm
vec, x, y
[in] vec : 値を格納する 2D ベクトル変数
[in] x   : X 値
[in] y   : Y 値

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_Normalize
2D ベクトルを正規化します。

%group
算術機能
%prm
vec, outVec
[in]  vec    : 処理の基になる 2D ベクトル
[out] outVec : 演算結果を格納する 2D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector2_NormalizeV
2D ベクトルを正規化します。

%group
算術機能
%prm
vec
[in] vec : 処理対象の 2D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_GetLength
3Dベクトルの長さを取得します。

%group
算術機能
%prm
vec, outLength
[in]  vec       : 処理の基になるベクトル
[out] outLength : ベクトルの長さを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_GetSquareLength
3D ベクトルの長さの2乗を計算する

%group
算術機能
%prm
vec, outLength
[in]  vec       : 処理の基になる Vector3
[out] outLength : ベクトルの長さの2乗を格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Set
3Dベクトルに値を設定します。

%group
算術機能
%prm
vec, x, y, z
[in] vec : 値を格納する 3D ベクトル変数
[in] x   : X 値
[in] y   : Y 値
[in] z   : Z 値

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_SetVZ
2D ベクトル と Z値 を指定して、3Dベクトルに値を設定します。

%group
算術機能
%prm
vec, vec2, z
[out] vec  : 値を格納する 3Dベクトル 変数
[in]  vec2 : 2D ベクトル
[in]  z    : Z 値

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Normalize
3D ベクトルを正規化します。

%group
算術機能
%prm
vec, outVec
[in]  vec    : 処理の基になる 3D ベクトル
[out] outVec : 演算結果を格納する 3D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_NormalizeV
3D ベクトルを正規化します。

%group
算術機能
%prm
vec
[in] vec : 処理対象の 2D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Dot
2 つの 3D ベクトルの内積を計算します。

%group
算術機能
%prm
vec1, vec2, dot
[in]  vec1 : 処理の基になる 3D ベクトル
[in]  vec2 : 処理の基になる 3D ベクトル
[out] dot  : ベクトルの内積を格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Cross
2 つの 3D ベクトルの外積を計算します。

%group
算術機能
%prm
vec1, vec2, outVec
[in]  vec1   : 処理の基になる 3D ベクトル
[in]  vec2   : 処理の基になる 3D ベクトル
[out] outVec : 演算結果を格納する 3D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Reflect
指定された法線で反射させたベクトルを計算します。

%group
算術機能
%prm
vec, normal, outVec
[in]  vec    : 処理の基になる 3D ベクトル
[in]  normal : 面方向を表す法線 3D ベクトル
[out] outVec : 演算結果を格納する 3D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Slide
指定された法線と方向から滑りベクトルを計算します。

%group
算術機能
%prm
vec, normal, outVec
[in]  vec    : 処理の基になる 3D ベクトル
[in]  normal : 面方向を表す法線 3D ベクトル
[out] outVec : 演算結果を格納する 3D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Lerp
2 つの 3D ベクトル間を線形補間します。

%group
算術機能
%prm
vec1, vec2, t, outVec
[in]  vec1   : 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
[in]  vec2   : 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
[in]  t      : 補間値 ( 0.0 ～ 1.0 )
[out] outVec : 演算結果を格納する 3D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_CatmullRom
指定された 3D ベクトルを Catmull-Rom 補間します。

%group
算術機能
%prm
vec1, vec2, vec3, vec4, t, outVec
[in]  vec1   : 処理の基になる 3D ベクトル
[in]  vec2   : 処理の基になる 3D ベクトル ( t = 0.0 のときの値 )
[in]  vec3   : 処理の基になる 3D ベクトル ( t = 1.0 のときの値 )
[in]  vec4   : 処理の基になる 3D ベクトル
[in]  t      : 補間値 ( 0.0 ～ 1.0 )
[out] outVec : 演算結果を格納する 3D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_Transform
指定された行列を使用して座標変換します。

%group
算術機能
%prm
vec, mat, outVec
[in]  vec    : 処理の基になる 3D ベクトル
[in]  mat    : 処理の基になる 行列
[out] outVec : 演算結果を格納する 3D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
ベクトルを ( x, y, z, 1 ) として座標変換します。
結果の w は出力されません。<br>
結果を w = 1 に射影する (x y z を w で除算) 場合は
Vector3TransformCoord() を使用してください。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector3_TransformCoord
指定された行列を使用して座標変換し、結果を w = 1 に射影します。

%group
算術機能
%prm
vec, mat, outVec
[in]  vec    : 処理の基になる 3D ベクトル
[in]  mat    : 処理の基になる 行列
[out] outVec : 演算結果を格納する 3D ベクトル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
ベクトルを ( x, y, z, 1 ) として座標変換し、
結果を w = 1 に射影 (x y z を w で除算) します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNVector4_Set
4Dベクトルに値を設定します。

%group
算術機能
%prm
vec, x, y, z, w
[in] vec : 値を格納する4Dベクトル変数
[in] x   : X 値
[in] y   : Y 値
[in] z   : Z 値
[in] w   : W 値

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetRight
右方向を示す 3D ベクトルを取得します。

%group
算術機能
%prm
mat, outVec
[in]  mat    : 処理の基になる行列
[out] outVec : 結果を格納する Vector3 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetUp
上方向を示す 3D ベクトルを取得します。

%group
算術機能
%prm
mat, outVec
[in]  mat    : 処理の基になる行列
[out] outVec : 結果を格納する Vector3 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetFront
正面方向を示す 3D ベクトルを取得します。

%group
算術機能
%prm
mat, outVec
[in]  mat    : 処理の基になる行列
[out] outVec : 結果を格納する Vector3 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetPosition
位置を示す 3D ベクトルを取得します。

%group
算術機能
%prm
mat, outVec
[in]  mat    : 処理の基になる行列
[out] outVec : 結果を格納する Vector3 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Identity
単位行列を作成します。

%group
算術機能
%prm
mat
[out] mat : 結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Translate
行列を平行移動します。

%group
算術機能
%prm
mat, x, y, z
[in] mat : 演算結果を格納する行列
[in] x   : X 方向の移動量
[in] y   : Y 方向の移動量
[in] z   : Z 方向の移動量

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数から平行移動行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_TranslateVec3
行列を平行移動します。

%group
算術機能
%prm
mat, vec
[in] mat : 演算結果を格納する行列
[in] vec : 移動量

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数から平行移動行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateX
X 軸を回転軸にして行列を回転します。 (ラジアン単位)

%group
算術機能
%prm
mat, radian
[in] mat    : 演算結果を格納する行列
[in] radian : 回転角度 ( 軸方向に対して反時計回り )

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数から回転行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateY
Y 軸を回転軸にして行列を回転します。 (ラジアン単位)

%group
算術機能
%prm
mat, radian
[in] mat    : 演算結果を格納する行列
[in] radian : 回転角度

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数から回転行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateZ
Z 軸を回転軸にして行列を回転します。 (ラジアン単位)

%group
算術機能
%prm
mat, radian
[in] mat    : 演算結果を格納する行列
[in] radian : 回転角度

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数から回転行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Rotate
行列を回転します。 (ラジアン単位)

%group
算術機能
%prm
mat, xRad, yRad, zRad, rotOrder
[in] mat                            : 演算結果を格納する行列
[in] xRad                           : X 軸を回転軸とした角度
[in] yRad                           : Y 軸を回転軸とした角度
[in] zRad                           : Z 軸を回転軸とした角度
[in] rotOrder(LN_ROTATIONORDER_XYZ) : 回転順序の指定
                                         LN_ROTATIONORDER_XYZ
                                             X → Y → Z
                                         LN_ROTATIONORDER_XZY
                                             X → Z → Y
                                         LN_ROTATIONORDER_YXZ
                                             Y → X → Z
                                         LN_ROTATIONORDER_YZX
                                             Y → Z → X
                                         LN_ROTATIONORDER_ZXY
                                             Z → X → Y
                                         LN_ROTATIONORDER_ZYX
                                             Z → Y → X

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数から回転行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateVec3
行列を回転します。 (ラジアン単位)

%group
算術機能
%prm
mat, vec, rotOrder
[in] mat                            : 演算結果を格納する行列
[in] vec                            : 各軸の回転角度
[in] rotOrder(LN_ROTATIONORDER_XYZ) : 回転順序の指定
                                         LN_ROTATIONORDER_XYZ
                                             X → Y → Z
                                         LN_ROTATIONORDER_XZY
                                             X → Z → Y
                                         LN_ROTATIONORDER_YXZ
                                             Y → X → Z
                                         LN_ROTATIONORDER_YZX
                                             Y → Z → X
                                         LN_ROTATIONORDER_ZXY
                                             Z → X → Y
                                         LN_ROTATIONORDER_ZYX
                                             Z → Y → X

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数から回転行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateAxis
任意の軸を回転軸にして行列を回転します。 (ラジアン単位)

%group
算術機能
%prm
mat, axis, radian
[in] mat    : 演算結果を格納する行列
[in] axis   : 回転軸を示す Vector3
[in] radian : 回転角度

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数から回転行列を作り、現在の行列に乗算します。<br>
与えられた軸が正規化されていない場合は正規化したものを使って計算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_RotateQuaternion
クォータニオンを使って行列を回転します。

%group
算術機能
%prm
mat, qua
[in] mat : 演算結果を格納する行列
[in] qua : 処理の基になる Quaternion

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
クォータニオンから回転行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Scale
行列をスケーリングします。

%group
算術機能
%prm
mat, xyz
[in] mat : 演算結果を格納する行列
[in] xyz : X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数からスケーリング行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_ScaleXYZ
行列をスケーリングします。

%group
算術機能
%prm
mat, x, y, z
[in] mat : 演算結果を格納する行列
[in] x   : X 方向の拡大率
[in] y   : Y 方向の拡大率
[in] z   : Z 方向の拡大率

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数からスケーリング行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_ScaleVec3
行列をスケーリングします。

%group
算術機能
%prm
mat, scale
[in] mat   : 演算結果を格納する行列
[in] scale : 各方向の拡大率

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた引数からスケーリング行列を作り、現在の行列に乗算します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Multiply
行列を乗算します。

%group
算術機能
%prm
mat1, mat2, matOut
[in]  mat1   : 処理の基になる行列
[in]  mat2   : 処理の基になる行列
[out] matOut : 結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Inverse
逆行列を求めます。

%group
算術機能
%prm
mat, matOut
[in]  mat    : 処理の基になる行列
[out] matOut : 演算結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Transpose
転置行列を求めます。

%group
算術機能
%prm
mat, matOut
[in]  mat    : 処理の基になる行列
[out] matOut : 演算結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_ViewTransformLH
左手座標系ビュー行列を作成します。

%group
算術機能
%prm
pos, lookAt, upDir, matOut
[in]  pos    : 視点の位置を示す Vector3
[in]  lookAt : 注視点を示す Vector3
[in]  upDir  : 上方向を示す Vector3
[out] matOut : 演算結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_ViewTransformRH
右手座標系ビュー行列を作成します。

%group
算術機能
%prm
pos, lookAt, upDir, matOut
[in]  pos    : 視点の位置を示す Vector3
[in]  lookAt : 注視点を示す Vector3
[in]  upDir  : 上方向を示す Vector3
[out] matOut : 演算結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_PerspectiveFovLH
左手座標系射影行列を作成します。

%group
算術機能
%prm
fovY, aspect, nearZ, farZ, matOut
[in]  fovY   : Y 方向への視野角 (ラジアン単位)
[in]  aspect : アスペクト比
[in]  nearZ  : 近くのビュー平面の Z 値
[in]  farZ   : 遠くのビュー平面の Z 値
[out] matOut : 演算結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_PerspectiveFovRH
右手座標系射影行列作成します。

%group
算術機能
%prm
fovY, aspect, nearZ, farZ, matOut
[in]  fovY   : Y 方向への視野角 (ラジアン単位)
[in]  aspect : アスペクト比
[in]  nearZ  : 近くのビュー平面の Z 値
[in]  farZ   : 遠くのビュー平面の Z 値
[out] matOut : 演算結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_OrthoLH
左手座標系正射影行列作成します。

%group
算術機能
%prm
width, height, nearZ, farZ, matOut
[in]  width  : ビューの幅
[in]  height : ビューの高さ
[in]  nearZ  : 近くのビュー平面の Z 値
[in]  farZ   : 遠くのビュー平面の Z 値
[out] matOut : 演算結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_OrthoRH
右手座標系正射影行列作成します。

%group
算術機能
%prm
width, height, nearZ, farZ, matOut
[in]  width  : ビューの幅
[in]  height : ビューの高さ
[in]  nearZ  : 近くのビュー平面の Z 値
[in]  farZ   : 遠くのビュー平面の Z 値
[out] matOut : 演算結果を格納する行列

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_GetEulerAngles
回転行列からオイラー角を計算します。

%group
算術機能
%prm
mat, outVec
[in]  mat    : 処理の基になる行列
[out] outVec : 各軸の回転角度(ラジアン)を格納する Vector3 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNMatrix_Decompose
行列をスケーリング、回転、移動成分に分解します。

%group
算術機能
%prm
mat, scale, rot, trans
[in]  mat   : 処理の基になる行列
[out] scale : 各軸のスケーリング量を格納する Vector3 変数
[out] rot   : 各軸の回転情報を格納する Quaternion 変数
[out] trans : 各軸の平行移動量を格納する Vector3 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Set
クォータニオンに値を設定します。

%group
算術機能
%prm
qua, x, y, z, w
[in] qua : 値を格納する Quaternion 変数
[in] x   : X 値
[in] y   : Y 値
[in] z   : Z 値
[in] w   : W 値

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Identity
単位クォータニオンを作成します。

%group
算術機能
%prm
qua
[out] qua : 結果を格納する Quaternion 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_RotationAxis
任意の軸を回転軸にして回転するクォータニオンを作成します。

%group
算術機能
%prm
axis, r, outQua
[in]  axis   : 回転軸を示す Vector3 変数
[in]  r      : 回転角度 (ラジアン)
[out] outQua : 演算結果を格納する Quaternion 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
与えられた軸が正規化されていない場合は正規化された軸を使用します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_RotationMatrix
回転行列からクォータニオンを作成します。

%group
算術機能
%prm
mat, outQua
[in]  mat    : 処理の基になる行列
[out] outQua : 演算結果を格納する Quaternion 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_RotationYawPitchRoll
ヨー、ピッチ、およびロールを指定してクォータニオンを作成します。

%group
算術機能
%prm
yaw, pitch, roll, outQua
[in]  yaw    : y 軸を中心とするヨー (ラジアン単位)
[in]  pitch  : x 軸を中心とするピッチ (ラジアン単位)
[in]  roll   : z 軸を中心とするロール (ラジアン単位)
[out] outQua : 演算結果を格納する Quaternion 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Normalize
クォータニオンを正規化します。

%group
算術機能
%prm
qua, outQua
[in]  qua    : 処理の基になるクォータニオン
[out] outQua : 演算結果を格納する Quaternion 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Conjugate
クォータニオンの共役を計算します。

%group
算術機能
%prm
qua, outQua
[in]  qua    : 処理の基になるクォータニオン
[out] outQua : 演算結果を格納する Quaternion 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Multiply
2 つのクォータニオンの積を計算します。

%group
算術機能
%prm
qua1, qua2, outQua
[in]  qua1   : 処理の基になるクォータニオン
[in]  qua2   : 処理の基になるクォータニオン
[out] outQua : 演算結果を格納する Quaternion 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNQuaternion_Slerp
2 つのクォータニオンを球面線形補間します。

%group
算術機能
%prm
qua1, qua2, t, outQua
[in]  qua1   : 処理の基になるクォータニオン
[in]  qua2   : 処理の基になるクォータニオン
[in]  t      : 補間値
[out] outQua : 演算結果を格納する Quaternion 変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayBGM
BGM を演奏します。

%group
音声機能
%prm
filePath, volume, pitch, fadeTime
[in] filePath      : ファイルパス
[in] volume(1.0f)  : ボリューム (0.0～1.0)
[in] pitch(1.0f)   : ピッチ (0.5～2.0)
[in] fadeTime(0.0) : フェードインにかける時間 (秒)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
BGM はひとつだけ演奏することができ、ループ再生します。
新しい BGM を演奏ようとした場合、再生中の BGM は停止します。
フェードイン時間を指定した場合はクロスフェードしながら音声を切り替えます。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayBGMMem
メモリ上の音声ファイルデータを使用して BGM を演奏します。

%group
音声機能
%prm
data, dataSize, volume, pitch, fadeTime
[in] data          : メモリ上の音声ファイルデータ
[in] dataSize      : データサイズ (バイト単位)
[in] volume(1.0f)  : ボリューム (0.0～1.0)
[in] pitch(1.0f)   : ピッチ (0.5～2.0)
[in] fadeTime(0.0) : フェードインにかける時間 (秒)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_StopBGM
BGM の演奏を停止します。

%group
音声機能
%prm
fadeTime
[in] fadeTime(0.0) : フェードアウトにかける時間 (秒)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayBGS
BGS を演奏します。

%group
音声機能
%prm
filePath, volume, pitch, fadeTime
[in] filePath      : ファイルパス
[in] volume(1.0f)  : ボリューム (0.0～1.0)
[in] pitch(1.0f)   : ピッチ (0.5～2.0)
[in] fadeTime(0.0) : フェードインにかける時間 (秒)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
BGS は風や滝の音などの環境音です。
BGM と同様、ひとつだけ演奏することができ、ループ再生します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayBGSMem
メモリ上の音声ファイルデータから BGS を演奏します。

%group
音声機能
%prm
data, dataSize, volume, pitch, fadeTime
[in] data          : メモリ上の音声ファイルデータ
[in] dataSize      : データサイズ (バイト単位)
[in] volume(1.0f)  : ボリューム (0.0～1.0)
[in] pitch(1.0f)   : ピッチ (0.5～2.0)
[in] fadeTime(0.0) : フェードインにかける時間 (秒)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_StopBGS
BGS の演奏を停止します。、

%group
音声機能
%prm
fadeTime
[in] fadeTime(0.0) : フェードアウトにかける時間 (秒)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayME
ME を演奏します。

%group
音声機能
%prm
filePath, volume, pitch
[in] filePath     : ファイルパス
[in] volume(1.0f) : ボリューム (0.0～1.0)
[in] pitch(1.0f)  : ピッチ (0.5～2.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
ME は レベルアップやアイテム取得時のファンファーレ等の効果音楽です。
ME はひとつだけ演奏することができ、ループ再生せずに一度だけ演奏されます。
BGM が再生中であれば一時停止し、ME 終了後に再開させます。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlayMEMem
メモリ上の音声ファイルデータから ME を演奏します。

%group
音声機能
%prm
data, dataSize, volume, pitch
[in] data         : メモリ上の音声ファイルデータ
[in] dataSize     : データサイズ (バイト単位)
[in] volume(1.0f) : ボリューム (0.0～1.0)
[in] pitch(1.0f)  : ピッチ (0.5～2.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_StopME
ME の演奏を停止します。

%group
音声機能
%prm
　

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE
SE を演奏します。

%group
音声機能
%prm
filePath, volume, pitch
[in] filePath     : ファイルパス
[in] volume(1.0f) : ボリューム (0.0～1.0)
[in] pitch(1.0f)  : ピッチ (0.5～2.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
SE はゲーム中の効果音です。 SE は同時に複数再生することができ、同じ音声を演奏した場合は 重ねがけされて演奏されます。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE3D
SE を演奏します。(3D サウンド)

%group
音声機能
%prm
filePath, position, distance, volume, pitch
[in] filePath     : ファイルパス
[in] position     : 3D 空間上の座標
[in] distance     : 減衰距離
[in] volume(1.0f) : ボリューム (0.0～1.0)
[in] pitch(1.0f)  : ピッチ (0.5～2.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE3DXYZ
SE を演奏します。(3D サウンド)

%group
音声機能
%prm
filePath, x, y, z, distance, volume, pitch
[in] filePath     : ファイルパス
[in] x            : 3D 空間上の X 座標
[in] y            : 3D 空間上の Y 座標
[in] z            : 3D 空間上の Z 座標
[in] distance     : 減衰距離
[in] volume(1.0f) : ボリューム (0.0～1.0)
[in] pitch(1.0f)  : ピッチ (0.5～2.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySEMem
メモリ上の音声データから SE を演奏します。

%group
音声機能
%prm
data, dataSize, volume, pitch
[in] data         : メモリ上の音声ファイルデータ
[in] dataSize     : データサイズ (バイト単位)
[in] volume(1.0f) : ボリューム (0.0～1.0)
[in] pitch(1.0f)  : ピッチ (0.5～2.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE3DMem
メモリ上の音声ファイルデータから SE を演奏します。 (3D サウンド)

%group
音声機能
%prm
data, dataSize, position, distance, volume, pitch
[in] data         : メモリ上の音声ファイルデータ
[in] dataSize     : データサイズ (バイト単位)
[in] position     : 3D 空間上の座標
[in] distance     : 減衰距離
[in] volume(1.0f) : ボリューム (0.0～1.0)
[in] pitch(1.0f)  : ピッチ (0.5～2.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_PlaySE3DMemXYZ
メモリ上の音声ファイルデータから SE を演奏します。(3D サウンド)

%group
音声機能
%prm
data, dataSize, x, y, z, distance, volume, pitch
[in] data         : メモリ上の音声ファイルデータ
[in] dataSize     : データサイズ (バイト単位)
[in] x            : 3D 空間上の X 座標
[in] y            : 3D 空間上の Y 座標
[in] z            : 3D 空間上の Z 座標
[in] distance     : 減衰距離
[in] volume(1.0f) : ボリューム (0.0～1.0)
[in] pitch(1.0f)  : ピッチ (0.5～2.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_StopSE
すべての SE の演奏を停止します。

%group
音声機能
%prm
　

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_SetBGMVolume
再生中のBGMの音量を設定します。(フェードアウト中は無効)

%group
音声機能
%prm
volume, fadeTime
[in] volume        : ボリューム (0.0～1.0)
[in] fadeTime(0.0) : フェードアウトにかける時間 (秒)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNGameAudio_SetBGSVolume
再生中のBGSの音量を設定します。(フェードアウト中は無効)

%group
音声機能
%prm
volume, fadeTime
[in] volume        : ボリューム (0.0～1.0)
[in] fadeTime(0.0) : フェードアウトにかける時間 (秒)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetPosition
3D音声のリスナーの位置を設定します。

%group
音声機能
%prm
position
[in] position : 3D 空間上の座標

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetDirection
3D音声のリスナーの正面方向を設定します。

%group
音声機能
%prm
direction
[in] direction : 向き

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetUpDirection
3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)

%group
音声機能
%prm
direction
[in] direction : 上方向

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetVelocity
3D音声のリスナーの速度を設定します。

%group
音声機能
%prm
velocity
[in] velocity : 速度

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetPositionXYZ
3D音声のリスナーの位置を設定します。

%group
音声機能
%prm
x, y, z
[in] x : 3D 空間上の X 座標
[in] y : 3D 空間上の Y 座標
[in] z : 3D 空間上の Z 座標

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetDirectionXYZ
3D音声のリスナーの正面方向を設定します。

%group
音声機能
%prm
x, y, z
[in] x : 向きの X 成分
[in] y : 向きの Y 成分
[in] z : 向きの Z 成分

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetUpDirectionXYZ
3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)

%group
音声機能
%prm
x, y, z
[in] x : 向きの X 成分
[in] y : 向きの Y 成分
[in] z : 向きの Z 成分

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSoundListener_SetVelocityXYZ
3D音声のリスナーの速度を設定します。

%group
音声機能
%prm
x, y, z
[in] x : 速度の X 成分
[in] y : 速度の Y 成分
[in] z : 速度の Z 成分

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Create
ファイルからサウンドオブジェクトを作成します。

%group
音声機能
%prm
filePath, sound
[in]  filePath : 音声ファイルのパス
[out] sound    : 作成されたサウンドオブジェクトのハンドルを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_CreateMem
メモリ上の音声ファイルデータからサウンドオブジェクトを作成します。

%group
音声機能
%prm
data, dataSize, sound
[in]  data     : メモリ上の音声データへ
[in]  dataSize : データサイズ (バイト単位)
[out] sound    : 作成されたサウンドオブジェクトのハンドルを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetVolume
サウンドのボリュームを取得します。

%group
音声機能
%prm
sound, outVolume
[in]  sound     : サウンドハンドル
[out] outVolume : ボリュームを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetVolume
サウンドのボリュームを設定します。

%group
音声機能
%prm
sound, volume
[in] sound  : サウンドハンドル
[in] volume : ボリューム (0.0～1.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetPitch
サウンドのピッチを取得します。

%group
音声機能
%prm
sound, outPitch
[in]  sound    : サウンドハンドル
[out] outPitch : ピッチを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetPitch
サウンドのピッチを設定します。

%group
音声機能
%prm
sound, pitch
[in] sound : サウンドハンドル
[in] pitch : ピッチ (0.5～2.0)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_IsLoopEnabled
サウンドのループ再生が有効であるかを確認します。

%group
音声機能
%prm
sound, outEnabled
[in]  sound      : サウンドハンドル
[out] outEnabled : ループ再生の有無状態を格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetLoopEnabled
サウンドのループ再生の有無を設定します。

%group
音声機能
%prm
sound, loopEnable
[in] sound      : サウンドハンドル
[in] loopEnable : LN_TRUE = ループ再生する / LN_FALSE = しない

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Is3DEnabled
サウンドが 3D 音源であるかを確認します。

%group
音声機能
%prm
sound, outEnabled
[in]  sound      : サウンドハンドル
[out] outEnabled : 状態を格納する変数 (LN_TRUE = 3D音声 / LN_FALSE = 非 3D)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Set3DEnabled
サウンドを 3D 音源であるかを設定します。

%group
音声機能
%prm
sound, enabled
[in] sound   : サウンドハンドル
[in] enabled : LN_TRUE = 3D音声 / LN_FALSE = 非3D

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetPlayingMode
サウンド再生時の音声データの再生方法を取得します。

%group
音声機能
%prm
sound, outMode
[in]  sound   : サウンドハンドル
[out] outMode : 再生方法を格納する変数
                   LN_SOUNDPLAYINGMODE_UNKNOWN
                       不明な再生方法
                   LN_SOUNDPLAYINGMODE_AUTO
                       自動選択 (デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります)
                   LN_SOUNDPLAYINGMODE_ONMEMORY
                       オンメモリ
                   LN_SOUNDPLAYINGMODE_STREAMING
                       ストリーミング
                   LN_SOUNDPLAYINGMODE_MIDI
                       SMF

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetPlayingMode
サウンド再生時の音声データの再生方法を設定します。

%group
音声機能
%prm
sound, mode
[in] sound : サウンドハンドル
[in] mode  : 読み込み方法
                LN_SOUNDPLAYINGMODE_UNKNOWN
                    不明な再生方法
                LN_SOUNDPLAYINGMODE_AUTO
                    自動選択 (デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります)
                LN_SOUNDPLAYINGMODE_ONMEMORY
                    オンメモリ
                LN_SOUNDPLAYINGMODE_STREAMING
                    ストリーミング
                LN_SOUNDPLAYINGMODE_MIDI
                    SMF

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetPlayingState
サウンドの再生状態を取得します。

%group
音声機能
%prm
sound, outState
[in]  sound    : サウンドハンドル
[out] outState : 状態を格納する変数
                    LN_SOUNDPLAYINGSTATE_STOPPED
                        停止中
                    LN_SOUNDPLAYINGSTATE_PLAYING
                        再生中
                    LN_SOUNDPLAYINGSTATE_PAUSING
                        一時停止中

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetPlayedSamples
サウンドの再生したサンプル数を取得します。

%group
音声機能
%prm
sound, outSamples
[in]  sound      : サウンドハンドル
[out] outSamples : 再生したサンプル数を格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetTotalSamples
サウンドの音声データ全体のサンプル数を取得します。

%group
音声機能
%prm
sound, outSamples
[in]  sound      : サウンドハンドル
[out] outSamples : 音声データ全体のサンプル数を格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_GetSamplingRate
サウンドのサンプリングレートを取得します。

%group
音声機能
%prm
sound, outRate
[in]  sound   : サウンドハンドル
[out] outRate : サンプリングレートを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterPosition
サウンドの 3D 音源としての位置を設定します。

%group
音声機能
%prm
sound, position
[in] sound    : サウンドハンドル
[in] position : 3D 空間上の座標

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterVelocity
サウンドの 3D 音源としての速度を設定します。

%group
音声機能
%prm
sound, velocity
[in] sound    : サウンドハンドル
[in] velocity : 速度

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterMaxDistance
サウンドの 3D 音源の減衰距離 (聴こえなくなる距離) を設定します。(default:100)

%group
音声機能
%prm
sound, distance
[in] sound    : サウンドハンドル
[in] distance : 距離

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetLoopRange
サウンドのループ再生の範囲を設定します。

%group
音声機能
%prm
sound, begin, length
[in] sound  : サウンドハンドル
[in] begin  : ループ領域の先頭位置 (サンプル数単位)
[in] length : ループ領域長さ (サンプル数単位)

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
begin と length に 0 を指定すると、全体をループ領域として設定します。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Play
サウンドを再生します。

%group
音声機能
%prm
sound
[in] sound : サウンドハンドル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Stop
サウンドの再生を停止します。

%group
音声機能
%prm
sound
[in] sound : サウンドハンドル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Pause
サウンドの再生を一時停止します。

%group
音声機能
%prm
sound
[in] sound : サウンドハンドル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_Resume
サウンドの再生を一時停止します。

%group
音声機能
%prm
sound
[in] sound : サウンドハンドル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_FadeVolume
サウンド音量のフェード操作を行います。

%group
音声機能
%prm
sound, targetVolume, time, behavior
[in] sound        : サウンドハンドル
[in] targetVolume : 変更先の音量
[in] time         : フェードにかける時間 (秒)
[in] behavior     : フェード完了後の動作の指定
                       LN_SOUNDFADEBEHAVIORBEHAVIOR_CONTINUE
                           継続(なにもしない)
                       LN_SOUNDFADEBEHAVIOR_STOP
                           停止する
                       LN_SOUNDFADEBEHAVIOR_STOP_RESET
                           停止して、音量を元の値に戻す
                       LN_SOUNDFADEBEHAVIOR_PAUSE
                           一時停止する
                       LN_SOUNDFADEBEHAVIOR_PAUSE_RESET
                           一時停止して、音量を元の値に戻す

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterPositionXYZ
サウンドの 3D 音源としての位置を設定します。

%group
音声機能
%prm
sound, x, y, z
[in] sound : サウンドハンドル
[in] x     : 3D 空間上の X 座標
[in] y     : 3D 空間上の Y 座標
[in] z     : 3D 空間上の Z 座標

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSound_SetEmitterVelocityXYZ
サウンドの 3D 音源としての速度を設定します。

%group
音声機能
%prm
sound, x, y, z
[in] sound : サウンドハンドル
[in] x     : 速度の X 成分
[in] y     : 速度の Y 成分
[in] z     : 速度の Z 成分

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNTexture_GetSize
テクスチャのサイズを取得します。

%group
グラフィックス機能
%prm
texture, outSize
[in]  texture : テクスチャハンドル
[out] outSize : サイズを格納する変数のアドレス

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNTexture2D_Create
サイズを指定して2Dテクスチャを作成します。

%group
グラフィックス機能
%prm
width, height, format, mipmap, outTexture2D
[in]  width                   : テクスチャの幅 (ピクセル単位)
[in]  height                  : テクスチャの高さ (ピクセル単位)
[in]  format(LN_FMT_A8R8G8B8) : テクスチャのピクセルフォーマット
                                   LN_FMT_UNKNOWN
                                       不明なフォーマット
                                   LN_FMT_A8R8G8B8
                                       32 ビットのアルファ付きフォーマット
                                   LN_FMT_X8R8G8B8
                                       32 ビットのアルファ無しフォーマット
                                   LN_FMT_A16B16G16R16F
                                       64 ビットの浮動小数点フォーマット
                                   LN_FMT_A32B32G32R32F
                                       128 ビットの浮動小数点フォーマット
                                   LN_FMT_D24S8
                                       32 ビットの Z バッファフォーマット
[in]  mipmap(LN_FALSE)        : ミップマップの有無
[out] outTexture2D            : 作成された2Dテクスチャのハンドルを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNTexture2D_CreateFromFile
ファイルから2Dテクスチャを作成します。

%group
グラフィックス機能
%prm
filePath, outTexture2D
[in]  filePath     : 画像ファイルのパス
[out] outTexture2D : 作成された2Dテクスチャのハンドルを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst
全てのプラットフォームでサポートされているファイルフォーマットは .png です。
グラフィックスAPI に DirectX9 を使用している場合は MSDN の D3DXCreateTextureFromFileEx を参照してください。

%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSceneNode_SetPosition
ノードの位置を設定します。

%group
シーングラフ
%prm
sceneNode, position
[in] sceneNode : シーンノードハンドル
[in] position  : 座標

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSprite_GetTexture
スプライトに設定されているテクスチャを取得します。

%group
シーングラフ
%prm
sprite, outTexture
[in]  sprite     : スプライトハンドル
[out] outTexture : テクスチャハンドルを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSprite_SetTexture
スプライトにテクスチャを設定します。

%group
シーングラフ
%prm
sprite, texture
[in] sprite  : スプライトハンドル
[in] texture : テクスチャハンドル

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSprite2D_Create
2Dスプライトオブジェクトを作成します。

%group
シーングラフ
%prm
outSprite2D
[out] outSprite2D : 作成された2Dスプライトハンドルを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

;-----------------------------------------------------------
;
;-----------------------------------------------------------
%index
LNSprite2D_CreateFromTexture
2Dスプライトオブジェクトを作成します。

%group
シーングラフ
%prm
texture, outSprite2D
[in]  texture     : 2Dスプライトが表示するテクスチャのハンドル
[out] outSprite2D : 作成された2Dスプライトハンドルを格納する変数

stat : エラーコード (エラーコードについては LNError_GetLastErrorCode を参照してください)

%inst


%href

