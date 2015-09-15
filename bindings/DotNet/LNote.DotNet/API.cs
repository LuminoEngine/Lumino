using System;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ExceptionCallback(Result errorCode);

    /// <summary>
    /// 結果・エラーコード
    /// </summary>
    public enum Result
    {
        /// <summary>
        /// 成功
        /// </summary>
        OK = 0,

        /// <summary>
        /// 不明なエラー
        /// </summary>
        Unknown = -1,

        /// <summary>
        /// メモリ確保に失敗
        /// </summary>
        OutOfMemory = -2,

        /// <summary>
        /// その他のIOエラー
        /// </summary>
        Io = -3,

        /// <summary>
        /// ストリームの末尾を越えてアクセスしようとした
        /// </summary>
        EndOfStream = -4,

        /// <summary>
        /// 引数が無効
        /// </summary>
        Argument = -5,

        /// <summary>
        /// オブジェクトの現在の状態に対して無効な呼び出しが行われた
        /// </summary>
        InvalidOperation = -6,

        /// <summary>
        /// サポートされない機能を呼び出そうとした
        /// </summary>
        NotSupported = -7,

        /// <summary>
        /// ファイルにアクセスしようとして失敗した
        /// </summary>
        FileNotFound = -8,

        /// <summary>
        /// 無効なディレクトリにアクセスしようとした
        /// </summary>
        DirectoryNotFound = -9,

        /// <summary>
        /// ファイルや文字列等の形式が不正
        /// </summary>
        InvalidFormat = -10,

        /// <summary>
        /// 未実装の機能を呼び出した
        /// </summary>
        NotImplemented = -11,

        /// <summary>
        /// C/C++ランタイム API でエラーが発生した
        /// </summary>
        Runtime = -12,

        /// <summary>
        /// 文字コードの変換中、マッピングできない文字または不正シーケンスが見つかった
        /// </summary>
        EncodingFallback = -13,

        /// <summary>
        /// WindowsAPI のエラー
        /// </summary>
        Win32 = -14,

        /// <summary>
        /// COM のエラー
        /// </summary>
        Com = -15,

    }

    /// <summary>
    /// Variant の型識別情報
    /// </summary>
    public enum VariantType
    {
        /// <summary>
        /// 不明な型
        /// </summary>
        Unknown = 0,

        /// <summary>
        /// Boolean
        /// </summary>
        Bool = 1,

        /// <summary>
        /// 32bit 整数
        /// </summary>
        Int = 2,

        /// <summary>
        /// 単精度浮動小数
        /// </summary>
        Float = 3,

        /// <summary>
        /// 文字列
        /// </summary>
        String = 4,

        /// <summary>
        /// 配列
        /// </summary>
        List = 5,

        /// <summary>
        /// オブジェクト
        /// </summary>
        Object = 6,

        /// <summary>
        /// LNSizeF 構造体
        /// </summary>
        SizeF = 7,

    }

    /// <summary>
    /// ファイルへのアクセス制限
    /// </summary>
    public enum FileAccessPriority
    {
        /// <summary>
        /// アーカイブ優先
        /// </summary>
        ArchiveFirst = 0,

        /// <summary>
        /// ディレクトリ優先
        /// </summary>
        DirectoryFirst = 1,

        /// <summary>
        /// アーカイブのみ
        /// </summary>
        ArchiveOnly = 2,

    }

    /// <summary>
    /// ファイルオープンモード
    /// </summary>
    public enum FileType
    {
        /// <summary>
        /// バイナリモードで開く
        /// </summary>
        Binary = 0,

        /// <summary>
        /// テキストモードで開く
        /// </summary>
        Text = 1,

    }

    /// <summary>
    /// ファイルを開く方法または作成する方法
    /// </summary>
    public enum FileMode
    {
        /// <summary>
        /// 既存のファイルを開く
        /// </summary>
        Open = 0,

        /// <summary>
        /// 新しいファイルを作成する。既に存在する場合は上書きされる
        /// </summary>
        Create = 1,

        /// <summary>
        /// ファイルを開き、ファイルの末尾をシークする。存在しない場合は新しいファイルを作成する
        /// </summary>
        Append = 2,

    }

    /// <summary>
    /// ファイルにアクセスする方法
    /// </summary>
    public enum FileAccess
    {
        /// <summary>
        /// 読み取りアクセス
        /// </summary>
        Read = 0,

        /// <summary>
        /// 読み取り及び書き込みアクセス
        /// </summary>
        Readwrite = 1,

        /// <summary>
        /// 書き込みアクセス
        /// </summary>
        Write = 2,

    }

    /// <summary>
    /// シーク位置移動の起点
    /// </summary>
    public enum SeekOrigin
    {
        /// <summary>
        /// ファイルの先頭
        /// </summary>
        Begin = 0,

        /// <summary>
        /// 現在のシーク位置
        /// </summary>
        Current = 1,

        /// <summary>
        /// ファイルの末尾
        /// </summary>
        End = 2,

    }

    /// <summary>
    /// 非同期処理の状態
    /// </summary>
    public enum AsyncIOState
    {
        /// <summary>
        /// 待機状態 (初期状態)
        /// </summary>
        Idle = 0,

        /// <summary>
        /// 実行可能状態 (実際の処理開始を待っている)
        /// </summary>
        Ready = 1,

        /// <summary>
        /// 実行状態 (処理中)
        /// </summary>
        Processing = 2,

        /// <summary>
        /// 終了状態 (成功)
        /// </summary>
        Completed = 3,

        /// <summary>
        /// 終了状態 (処理終了後・処理に失敗した)
        /// </summary>
        Failed = 4,

    }

    /// <summary>
    /// 回転順序
    /// </summary>
    public enum RotationOrder
    {
        /// <summary>
        /// X → Y → Z
        /// </summary>
        XYZ = 0,

        /// <summary>
        /// X → Z → Y
        /// </summary>
        XZY = 1,

        /// <summary>
        /// Y → X → Z
        /// </summary>
        YXZ = 2,

        /// <summary>
        /// Y → Z → X
        /// </summary>
        YZX = 3,

        /// <summary>
        /// Z → X → Y
        /// </summary>
        ZXY = 4,

        /// <summary>
        /// Z → Y → X
        /// </summary>
        ZYX = 5,

    }

    /// <summary>
    /// キーボードのキーコード
    /// </summary>
    public enum KeyCode
    {
        Unknown = 0,

        A = 1,

        B = 2,

        C = 3,

        D = 4,

        E = 5,

        F = 6,

        G = 7,

        H = 8,

        I = 9,

        J = 10,

        K = 11,

        L = 12,

        M = 13,

        N = 14,

        O = 15,

        P = 16,

        Q = 17,

        R = 18,

        S = 19,

        T = 20,

        U = 21,

        V = 22,

        W = 23,

        X = 24,

        Y = 25,

        Z = 26,

        F10 = 46,

        F11 = 47,

        F12 = 48,

        Space = 49,

        Escape = 50,

        Down = 52,

        Left = 53,

        Right = 54,

        Lshift = 55,

        Rshift = 56,

        Lctrl = 57,

        Rctrl = 58,

        Lalt = 59,

        Ralt = 60,

        Tab = 61,

        Enter = 62,

        Backspace = 63,

        Insert = 64,

        Delete = 65,

        Pageup = 66,

        Pagedown = 67,

        Home = 68,

        End = 69,

        /// <summary>
        /// :	[非推奨 GLFWでは;]
        /// </summary>
        Colon = 70,

        /// <summary>
        /// ;	[非推奨 GLFWでは=]
        /// </summary>
        Semicolon = 71,

        /// <summary>
        /// ,
        /// </summary>
        Comma = 72,

        /// <summary>
        /// .
        /// </summary>
        Period = 73,

        /// <summary>
        /// /
        /// </summary>
        Slash = 74,

        /// <summary>
        /// -
        /// </summary>
        Minus = 75,

        /// <summary>
        /// \(バックスラッシュ) [非推奨 GLFWではNOWORD2]
        /// </summary>
        Backslash = 76,

        /// <summary>
        /// \(BaskSpaceキーの左の\キー) [非推奨 GLFWではI]
        /// </summary>
        Yen = 77,

        /// <summary>
        /// ^ [非推奨 GLFWではGLFW_KEY_APOSTROPHE(')]
        /// </summary>
        Caret = 78,

        /// <summary>
        /// [
        /// </summary>
        Lbracket = 79,

        /// <summary>
        /// ]
        /// </summary>
        Rbracket = 80,

        /// <summary>
        /// Android '+'
        /// </summary>
        AndroidPlus = 81,

    }

    /// <summary>
    /// アクセラレータキー
    /// </summary>
    public enum AcceleratorKey
    {
        /// <summary>
        /// フルスクリーン切り替えを行うアクセラレータキー
        /// </summary>
        ToggleFullscreen = 0,

        /// <summary>
        /// デバッグ用の情報を表示切替を行うアクセラレータキー
        /// </summary>
        ShowDebugStatistics = 1,

    }

    /// <summary>
    /// DirectMusic の初期化方法
    /// </summary>
    public enum DirectMusicMode
    {
        /// <summary>
        /// DirectMusic を使用しない
        /// </summary>
        NotUse = 0,

        /// <summary>
        /// 通常
        /// </summary>
        Normal = 1,

        /// <summary>
        /// 別スレッドで初期化して、再生時に未完了の場合は待つ
        /// </summary>
        ThreadWait = 2,

        /// <summary>
        /// 別スレッドで初期化して、再生時に未完了の場合は再生を予約する
        /// </summary>
        ThreadRequest = 3,

    }

    /// <summary>
    /// サウンド再生方法
    /// </summary>
    public enum SoundPlayingMode
    {
        /// <summary>
        /// 不明な再生方法
        /// </summary>
        Unknown = 0,

        /// <summary>
        /// 自動選択 (デフォルトではデコード後のサイズが 10000 バイト以上になる場合はストリーミング、そうでない場合はオンメモリ再生になります)
        /// </summary>
        Auto = 1,

        /// <summary>
        /// オンメモリ
        /// </summary>
        Onmemory = 2,

        /// <summary>
        /// ストリーミング
        /// </summary>
        Streaming = 3,

        /// <summary>
        /// SMF
        /// </summary>
        Midi = 4,

    }

    /// <summary>
    /// サウンドの再生状態
    /// </summary>
    public enum SoundPlayingState
    {
        /// <summary>
        /// 停止中
        /// </summary>
        Stopped = 0,

        /// <summary>
        /// 再生中
        /// </summary>
        Playing = 1,

        /// <summary>
        /// 一時停止中
        /// </summary>
        Pausing = 2,

    }

    /// <summary>
    /// 音量フェード完了時の動作
    /// </summary>
    public enum SoundFadeBehavior
    {
        /// <summary>
        /// 継続(なにもしない)
        /// </summary>
        Continue = 0,

        /// <summary>
        /// 停止する (stop())
        /// </summary>
        Stop = 1,

        /// <summary>
        /// 停止して、音量を元の値に戻す
        /// </summary>
        StopReset = 2,

        /// <summary>
        /// 一時停止する (pause( true ))
        /// </summary>
        Pause = 3,

        /// <summary>
        /// 一時停止して、音量を元の値に戻す
        /// </summary>
        PauseReset = 4,

    }

    /// <summary>
    /// GameAudio 内部 Sound
    /// </summary>
    public enum InternalGameSound
    {
        /// <summary>
        /// BGM
        /// </summary>
        BGM = 0,

        /// <summary>
        /// BGS
        /// </summary>
        BGS = 1,

        /// <summary>
        /// ME
        /// </summary>
        ME = 2,

    }

    /// <summary>
    /// ウィンドウとバックバッファのリサイズモード
    /// </summary>
    public enum BackbufferResizeMode
    {
        /// <summary>
        /// バックバッファは初期化時のサイズのまま、ウィンドウサイズに合わせて拡縮を行う
        /// </summary>
        Scaling = 0,

        /// <summary>
        /// バックバッファは初期化時のサイズのまま、レターボックスを表示してアスペクト比を保ち拡縮を行う
        /// </summary>
        ScalingWithLetterBox = 1,

        /// <summary>
        /// バックバッファをウィンドウサイズに合わせてリサイズする
        /// </summary>
        Resize = 2,

    }

    /// <summary>
    /// テクスチャフォーマット
    /// </summary>
    public enum TextureFormat
    {
        /// <summary>
        /// 不明なフォーマット
        /// </summary>
        Unknown = 0,

        /// <summary>
        /// 32 ビットのアルファ付きフォーマット
        /// </summary>
        A8R8G8B8 = 1,

        /// <summary>
        /// 32 ビットのアルファ無しフォーマット
        /// </summary>
        X8R8G8B8 = 2,

        /// <summary>
        /// 64 ビットの浮動小数点フォーマット
        /// </summary>
        A16B16G16R16F = 3,

        /// <summary>
        /// 128 ビットの浮動小数点フォーマット
        /// </summary>
        A32B32G32R32F = 4,

        /// <summary>
        /// 32 ビットの Z バッファフォーマット
        /// </summary>
        D24S8 = 5,

    }

    /// <summary>
    /// 合成方法
    /// </summary>
    public enum BlendMode
    {
        /// <summary>
        /// 通常 (アルファ無視)
        /// </summary>
        Normal = 0,

        /// <summary>
        /// アルファブレンド (デフォルト)
        /// </summary>
        Alpha = 1,

        /// <summary>
        /// 加算
        /// </summary>
        Add = 2,

        /// <summary>
        /// 加算（アルファチャンネル無視）
        /// </summary>
        AddAlphaDisable = 3,

        /// <summary>
        /// 減算
        /// </summary>
        Sub = 4,

        /// <summary>
        /// 減算（アルファチャンネル無視）
        /// </summary>
        SubAlphaDisable = 5,

        /// <summary>
        /// 乗算（アルファチャンネル無視）
        /// </summary>
        Mul = 6,

        /// <summary>
        /// スクリーン（アルファチャンネル無視）
        /// </summary>
        Screen = 7,

        /// <summary>
        /// リバース
        /// </summary>
        Reverse = 8,

    }

    /// <summary>
    /// カリング方法
    /// </summary>
    public enum CullingMode
    {
        /// <summary>
        /// 前面表示
        /// </summary>
        Front = 0,

        /// <summary>
        /// 裏面表示
        /// </summary>
        Back = 1,

        /// <summary>
        /// 両面表示
        /// </summary>
        Double = 2,

    }

    /// <summary>
    /// テキストアライン
    /// </summary>
    public enum TextAlign
    {
        /// <summary>
        /// 左揃え
        /// </summary>
        Left = 0,

        /// <summary>
        /// 中央揃え
        /// </summary>
        Center = 1,

        /// <summary>
        /// 右揃え
        /// </summary>
        Right = 2,

    }

    /// <summary>
    /// ノードの配置方法
    /// </summary>
    public enum PlacementType
    {
        /// <summary>
        /// 通常の配置方法
        /// </summary>
        Normal = 0,

        /// <summary>
        /// 常にカメラの方向を向くビルボード (回転は Z 軸のみ有効)
        /// </summary>
        Billboard = 1,

        /// <summary>
        /// Y 軸固定
        /// </summary>
        YFixed = 2,

    }

    /// <summary>
    /// 3D 空間での基準方向を表す値
    /// </summary>
    public enum FrontAxis
    {
        /// <summary>
        /// X+ 方向 (右向き)
        /// </summary>
        X = 0,

        /// <summary>
        /// Y+ 方向 (上向き)
        /// </summary>
        Y = 1,

        /// <summary>
        /// Z+ 方向 (奥向き)
        /// </summary>
        Z = 2,

        /// <summary>
        /// X- 方向 (左向き)
        /// </summary>
        Rx = 3,

        /// <summary>
        /// Y- 方向 (下向き)
        /// </summary>
        Ry = 4,

        /// <summary>
        /// Z- 方向 (手前向き)
        /// </summary>
        Rz = 5,

    }

    /// <summary>
    /// メッシュ生成オプション
    /// </summary>
    [Flags]
    public enum MeshCreateFlags
    {
        /// <summary>
        /// ソフトウェアスキニング用のモデル (動的な頂点、インデックスバッファを作成)
        /// </summary>
        SoftwareSkinning = 0x01,

        /// <summary>
        /// 物理演算のメッシュボディとして扱うメッシュを生成する
        /// </summary>
        CollisionMesh = 0x02,

        /// <summary>
        /// マテリアルに含まれるテクスチャファイルが見つからなくても無視する
        /// </summary>
        IgnoreTextureNotFound = 0x04,

    }

    /// <summary>
    /// カメラ姿勢の設定方法
    /// </summary>
    public enum CameraType
    {
        /// <summary>
        /// 位置と注視点から算出する (3Dカメラのデフォルト。2Dカメラでは無効)
        /// </summary>
        PositionAndLookat = 0,

        /// <summary>
        /// 位置と各軸の回転角度から算出する (2D カメラのデフォルト)
        /// </summary>
        PositionAndAngle = 1,

        /// <summary>
        /// 行列を使用し、正面(0,0,1) と 上方向(0,1,0) を変換する
        /// </summary>
        TransformMatrix = 2,

    }

    /// <summary>
    /// 座標系
    /// </summary>
    public enum Coord
    {
        /// <summary>
        /// 2D 空間
        /// </summary>
        Space2D = 0,

        /// <summary>
        /// 3D 空間
        /// </summary>
        Space3D = 1,

    }

    /// <summary>
    /// ハンドルオブジェクトの種類
    /// </summary>
    public enum ObjectType
    {
        Sound = 0,

    }


    public class API
    {
    
#if DEBUG
    	internal const string DLLName = "LNote_d.dll";
#else
    	internal const string DLLName = "LNote.dll";
#endif
        internal const CharSet DLLCharSet = CharSet.Ansi;
        internal const CallingConvention DefaultCallingConvention = CallingConvention.Cdecl;
        
        /// <summary>
        /// オブジェクトを解放します。
        /// </summary>
        /// <param name="hadnleObject">オブジェクトハンドル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObject_Release( IntPtr hadnleObject);

        /// <summary>
        /// オブジェクトの参照カウントをインクリメントします。
        /// </summary>
        /// <param name="hadnleObject">オブジェクトハンドル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObject_AddRef( IntPtr hadnleObject);

        /// <summary>
        /// オブジェクトの参照カウント取得します。
        /// </summary>
        /// <param name="hadnleObject">オブジェクトハンドル</param>
        /// <param name="count">参照カウントを格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNObject_GetRefCount( IntPtr hadnleObject, out int count);

        /// <summary>
        /// デバッグ用のログファイルの出力有無を設定します。(初期値:LN_FALSE)
        /// </summary>
        /// <param name="enabled">LN_TRUE:出力する / LN_FALSE:出力しない</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNConfig_SetApplicationLogEnabled( bool enabled);

        /// <summary>
        /// 標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(初期値:LN_FALSE)
        /// </summary>
        /// <param name="enabled">LN_TRUE:割り当てる / LN_FALSE:割り当てない</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNConfig_SetConsoleEnabled( bool enabled);

        /// <summary>
        /// ユーザー定義のウィンドウハンドルを設定します。(初期値:NULL)
        /// </summary>
        /// <param name="windowHandle">ユーザー定義のウィンドウハンドル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNConfig_SetUserWindowHandle( IntPtr windowHandle);

        /// <summary>
        /// サウンドオブジェクトのキャッシュサイズの設定
        /// </summary>
        /// <param name="count">キャッシュできるサウンドオブジェクトの最大数 (初期値:32)</param>
        /// <param name="memorySize">サウンドオブジェクトのキャッシュが使用できる最大メモリサイズ (初期値:0)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNConfig_SetSoundCacheSize( int count,  int memorySize);

        /// <summary>
        /// DirectMusic の初期化方法を設定します。(初期値:LN_DIRECTMUSICMODE_NOT_USE)
        /// </summary>
        /// <param name="mode">DirectMusic の初期化方法</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNConfig_SetDirectMusicInitializeMode( DirectMusicMode mode);

        /// <summary>
        /// DirectMusic のリバーブエフェクトの強さを設定します。(初期値:70)
        /// </summary>
        /// <param name="level">リバーブの強さ (0 ～ 100)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNConfig_SetDirectMusicReverbLevel( int level);

        /// <summary>
        /// アプリケーションを初期化します。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNApplication_Initialize();

        /// <summary>
        /// アプリケーションを初期化します。音声機能のみを使用する場合に呼び出します。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNApplication_InitializeAudio();

        /// <summary>
        /// フレームを更新します。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNApplication_Update();

        /// <summary>
        /// 画面の更新タイミングをリセットします。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNApplication_ResetFrameDelay();

        /// <summary>
        /// アプリケーションを終了するべきかを確認します。
        /// </summary>
        /// <param name="requested">終了要求の有無を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNApplication_IsEndRequested(out bool requested);

        /// <summary>
        /// LightNote の終了処理を行います。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LNApplication_Terminate();

        /// <summary>
        /// 2D ベクトルの長さを計算します。
        /// </summary>
        /// <param name="vec">処理の基になる 2D ベクトル</param>
        /// <param name="outLength">ベクトルの長さを格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector2_GetLength(ref Vector2 vec, out float outLength);

        /// <summary>
        /// 2D ベクトルの長さの2乗を計算します。
        /// </summary>
        /// <param name="vec">処理の基になる 2D ベクトル</param>
        /// <param name="outLength">ベクトルの長さの2乗を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector2_GetSquareLength(ref Vector2 vec, out float outLength);

        /// <summary>
        /// 2D ベクトルを初期化します。
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="vec">値を格納する 2D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector2_Create( float x,  float y, out Vector2 vec);

        /// <summary>
        /// 2D ベクトルを正規化します。
        /// </summary>
        /// <param name="vec">処理の基になる 2D ベクトル</param>
        /// <param name="outVec">演算結果を格納する 2D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector2_Normalize(ref Vector2 vec, out Vector2 outVec);

        /// <summary>
        /// 2D ベクトルを正規化します。
        /// </summary>
        /// <param name="vec">処理対象の 2D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector2_NormalizeV(ref Vector2 vec);

        /// <summary>
        /// 3Dベクトルの長さを計算する
        /// </summary>
        /// <param name="vec">処理の基になるベクトル</param>
        /// <param name="outLength">ベクトルの長さを格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_GetLength(ref Vector3 vec, out float outLength);

        /// <summary>
        /// 3D ベクトルの長さの2乗を計算する
        /// </summary>
        /// <param name="vec">処理の基になる Vector3</param>
        /// <param name="outLength">ベクトルの長さの2乗を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_GetSquareLength(ref Vector3 vec, out float outLength);

        /// <summary>
        /// 3Dベクトルを初期化します。
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        /// <param name="vec">値を格納する 3Dベクトル 変数のアドレス</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_Create( float x,  float y,  float z, out Vector3 vec);

        /// <summary>
        /// 2D ベクトル と Z値 を指定して、3Dベクトルを初期化します。
        /// </summary>
        /// <param name="vec2">2D ベクトル</param>
        /// <param name="z">Z 値</param>
        /// <param name="vec">値を格納する 3Dベクトル 変数のアドレス</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_CreateVZ(ref Vector2 vec2,  float z, out Vector3 vec);

        /// <summary>
        /// 3D ベクトルを正規化する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_Normalize(ref Vector3 vec, out Vector3 outVec);

        /// <summary>
        /// 3D ベクトルを正規化します。
        /// </summary>
        /// <param name="vec">処理対象の 2D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_NormalizeV(ref Vector3 vec);

        /// <summary>
        /// 2 つの 3D ベクトルの内積を計算する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル</param>
        /// <param name="dot">ベクトルの内積を格納する変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_Dot(ref Vector3 vec1, ref Vector3 vec2, out float dot);

        /// <summary>
        /// 2 つの 3D ベクトルの外積を計算する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル</param>
        /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_Cross(ref Vector3 vec1, ref Vector3 vec2, out Vector3 outVec);

        /// <summary>
        /// 指定された法線で反射させたベクトルを計算する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="normal">面方向を表す法線 3D ベクトル</param>
        /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_Reflect(ref Vector3 vec, ref Vector3 normal, out Vector3 outVec);

        /// <summary>
        /// 指定された法線と方向から滑りベクトルを計算する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="normal">面方向を表す法線 3D ベクトル</param>
        /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_Slide(ref Vector3 vec, ref Vector3 normal, out Vector3 outVec);

        /// <summary>
        /// 2 つの 3D ベクトル間を線形補間する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
        /// <param name="vec2">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
        /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
        /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_Lerp(ref Vector3 vec1, ref Vector3 vec2,  float t, out Vector3 outVec);

        /// <summary>
        /// 指定された 3D ベクトルを Catmull-Rom 補間する
        /// </summary>
        /// <param name="vec1">処理の基になる 3D ベクトル</param>
        /// <param name="vec2">処理の基になる 3D ベクトル ( t = 0.0 のときの値 )</param>
        /// <param name="vec3">処理の基になる 3D ベクトル ( t = 1.0 のときの値 )</param>
        /// <param name="vec4">処理の基になる 3D ベクトル</param>
        /// <param name="t">補間値 ( 0.0 ～ 1.0 )</param>
        /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_CatmullRom(ref Vector3 vec1, ref Vector3 vec2, ref Vector3 vec3, ref Vector3 vec4,  float t, out Vector3 outVec);

        /// <summary>
        /// 指定された行列を使用して座標変換する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="mat">処理の基になる 行列</param>
        /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_Transform(ref Vector3 vec, ref Matrix mat, out Vector4 outVec);

        /// <summary>
        /// 指定された行列を使用して座標変換し、結果を w = 1 に射影する
        /// </summary>
        /// <param name="vec">処理の基になる 3D ベクトル</param>
        /// <param name="mat">処理の基になる 行列</param>
        /// <param name="outVec">演算結果を格納する 3D ベクトル</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector3_TransformCoord(ref Vector3 vec, ref Matrix mat, out Vector3 outVec);

        /// <summary>
        /// 4Dベクトルを初期化します。
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        /// <param name="w">W 値</param>
        /// <param name="vec">値を格納する4Dベクトル変数のアドレス</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNVector4_Create( float x,  float y,  float z,  float w, out Vector4 vec);

        /// <summary>
        /// 右方向を示す 3D ベクトルの取得
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        /// <param name="outVec">結果を格納する Vector3 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_GetRight(ref Matrix mat, out Vector3 outVec);

        /// <summary>
        /// 上方向を示す 3D ベクトルの取得
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        /// <param name="outVec">結果を格納する Vector3 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_GetUp(ref Matrix mat, out Vector3 outVec);

        /// <summary>
        /// 正面方向を示す 3D ベクトルの取得
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        /// <param name="outVec">結果を格納する Vector3 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_GetFront(ref Matrix mat, out Vector3 outVec);

        /// <summary>
        /// 位置を示す 3D ベクトルの取得
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        /// <param name="outVec">結果を格納する Vector3 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_GetPosition(ref Matrix mat, out Vector3 outVec);

        /// <summary>
        /// 単位行列を作成する
        /// </summary>
        /// <param name="mat">結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_Identity(out Matrix mat);

        /// <summary>
        /// 行列を平行移動する
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="x">X 方向の移動量</param>
        /// <param name="y">Y 方向の移動量</param>
        /// <param name="z">Z 方向の移動量</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_Translate(ref Matrix mat,  float x,  float y,  float z);

        /// <summary>
        /// 行列を平行移動する
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="vec">移動量</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_TranslateVec3(ref Matrix mat, ref Vector3 vec);

        /// <summary>
        /// X 軸を回転軸にして行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="radian">回転角度 ( 軸方向に対して反時計回り )</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_RotateX(ref Matrix mat,  float radian);

        /// <summary>
        /// Y 軸を回転軸にして行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="radian">回転角度</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_RotateY(ref Matrix mat,  float radian);

        /// <summary>
        /// Z 軸を回転軸にして行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="radian">回転角度</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_RotateZ(ref Matrix mat,  float radian);

        /// <summary>
        /// 行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="xRad">X 軸を回転軸とした角度</param>
        /// <param name="yRad">Y 軸を回転軸とした角度</param>
        /// <param name="zRad">Z 軸を回転軸とした角度</param>
        /// <param name="rotOrder">回転順序の指定</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_Rotate(ref Matrix mat,  float xRad,  float yRad,  float zRad,  RotationOrder rotOrder = RotationOrder.XYZ);

        /// <summary>
        /// 行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="vec">各軸の回転角度</param>
        /// <param name="rotOrder">回転順序の指定</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_RotateVec3(ref Matrix mat, ref Vector3 vec,  RotationOrder rotOrder = RotationOrder.XYZ);

        /// <summary>
        /// 任意の軸を回転軸にして行列を回転する ( ラジアン単位 )
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="axis">回転軸を示す Vector3</param>
        /// <param name="radian">回転角度</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_RotateAxis(ref Matrix mat, ref Vector3 axis,  float radian);

        /// <summary>
        /// クォータニオンを使って行列を回転する
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="qua">処理の基になる Quaternion</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_RotateQuaternion(ref Matrix mat, ref Quaternion qua);

        /// <summary>
        /// 行列をスケーリングする
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="xyz">X Y Z 全ての方向に適用する拡大率 (X Y Z を同じ値で拡大)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_Scale(ref Matrix mat,  float xyz);

        /// <summary>
        /// 行列をスケーリングする
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="x">X 方向の拡大率</param>
        /// <param name="y">Y 方向の拡大率</param>
        /// <param name="z">Z 方向の拡大率</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_ScaleXYZ(ref Matrix mat,  float x,  float y,  float z);

        /// <summary>
        /// 行列をスケーリングする
        /// </summary>
        /// <param name="mat">演算結果を格納する行列</param>
        /// <param name="scale">各方向の拡大率</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_ScaleVec3(ref Matrix mat, ref Vector3 scale);

        /// <summary>
        /// 行列を乗算する
        /// </summary>
        /// <param name="mat1">処理の基になる行列</param>
        /// <param name="mat2">処理の基になる行列</param>
        /// <param name="matOut">結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_Multiply(ref Matrix mat1, ref Matrix mat2, out Matrix matOut);

        /// <summary>
        /// 逆行列を求める
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        /// <param name="matOut">演算結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_Inverse(ref Matrix mat, out Matrix matOut);

        /// <summary>
        /// 転置行列を求める
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        /// <param name="matOut">演算結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_Transpose(ref Matrix mat, out Matrix matOut);

        /// <summary>
        /// 左手座標系ビュー行列を作成する
        /// </summary>
        /// <param name="pos">視点の位置を示す Vector3</param>
        /// <param name="lookAt">注視点を示す Vector3</param>
        /// <param name="upDir">上方向を示す Vector3</param>
        /// <param name="matOut">演算結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_ViewTransformLH(ref Vector3 pos, ref Vector3 lookAt, ref Vector3 upDir, out Matrix matOut);

        /// <summary>
        /// 右手座標系ビュー行列を作成する
        /// </summary>
        /// <param name="pos">視点の位置を示す Vector3</param>
        /// <param name="lookAt">注視点を示す Vector3</param>
        /// <param name="upDir">上方向を示す Vector3</param>
        /// <param name="matOut">演算結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_ViewTransformRH(ref Vector3 pos, ref Vector3 lookAt, ref Vector3 upDir, out Matrix matOut);

        /// <summary>
        /// 左手座標系射影行列の作成
        /// </summary>
        /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
        /// <param name="aspect">アスペクト比</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        /// <param name="matOut">演算結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_PerspectiveFovLH( float fovY,  float aspect,  float nearZ,  float farZ, out Matrix matOut);

        /// <summary>
        /// 右手座標系射影行列の作成
        /// </summary>
        /// <param name="fovY">Y 方向への視野角 (ラジアン単位)</param>
        /// <param name="aspect">アスペクト比</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        /// <param name="matOut">演算結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_PerspectiveFovRH( float fovY,  float aspect,  float nearZ,  float farZ, out Matrix matOut);

        /// <summary>
        /// 左手座標系正射影行列の作成
        /// </summary>
        /// <param name="width">ビューの幅</param>
        /// <param name="height">ビューの高さ</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        /// <param name="matOut">演算結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_OrthoLH( float width,  float height,  float nearZ,  float farZ, out Matrix matOut);

        /// <summary>
        /// 右手座標系正射影行列の作成
        /// </summary>
        /// <param name="width">ビューの幅</param>
        /// <param name="height">ビューの高さ</param>
        /// <param name="nearZ">近くのビュー平面の Z 値</param>
        /// <param name="farZ">遠くのビュー平面の Z 値</param>
        /// <param name="matOut">演算結果を格納する行列</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_OrthoRH( float width,  float height,  float nearZ,  float farZ, out Matrix matOut);

        /// <summary>
        /// 回転行列からオイラー角を計算する
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        /// <param name="outVec">各軸の回転角度(ラジアン)を格納する Vector3 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_GetEulerAngles(ref Matrix mat, out Vector3 outVec);

        /// <summary>
        /// 行列をスケーリング、回転、移動成分に分解する
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        /// <param name="scale">各軸のスケーリング量を格納する Vector3 変数</param>
        /// <param name="rot">各軸の回転情報を格納する Quaternion 変数</param>
        /// <param name="trans">各軸の平行移動量を格納する Vector3 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNMatrix_Decompose(ref Matrix mat, out Vector3 scale, out Quaternion rot, out Vector3 trans);

        /// <summary>
        /// クォータニオンを初期化します。
        /// </summary>
        /// <param name="x">X 値</param>
        /// <param name="y">Y 値</param>
        /// <param name="z">Z 値</param>
        /// <param name="w">W 値</param>
        /// <param name="qua">値を格納する Quaternion 変数のアドレス</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNQuaternion_Create( float x,  float y,  float z,  float w, out Quaternion qua);

        /// <summary>
        /// 単位クォータニオンを作成する
        /// </summary>
        /// <param name="qua">結果を格納する Quaternion 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNQuaternion_Identity(out Quaternion qua);

        /// <summary>
        /// 任意の軸を回転軸にして回転するクォータニオンを作成する
        /// </summary>
        /// <param name="axis">回転軸を示す Vector3 変数</param>
        /// <param name="r">回転角度 (ラジアン)</param>
        /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNQuaternion_RotationAxis(ref Vector3 axis,  float r, out Quaternion outQua);

        /// <summary>
        /// 回転行列からクォータニオンを作成する
        /// </summary>
        /// <param name="mat">処理の基になる行列</param>
        /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNQuaternion_RotationMatrix(ref Matrix mat, out Quaternion outQua);

        /// <summary>
        /// ヨー、ピッチ、およびロールを指定してクォータニオンを作成する
        /// </summary>
        /// <param name="yaw">y 軸を中心とするヨー (ラジアン単位)</param>
        /// <param name="pitch">x 軸を中心とするピッチ (ラジアン単位)</param>
        /// <param name="roll">z 軸を中心とするロール (ラジアン単位)</param>
        /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNQuaternion_RotationYawPitchRoll( float yaw,  float pitch,  float roll, out Quaternion outQua);

        /// <summary>
        /// クォータニオンを正規化する
        /// </summary>
        /// <param name="qua">処理の基になるクォータニオン</param>
        /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNQuaternion_Normalize(ref Quaternion qua, out Quaternion outQua);

        /// <summary>
        /// クォータニオンの共役を計算する
        /// </summary>
        /// <param name="qua">処理の基になるクォータニオン</param>
        /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNQuaternion_Conjugate(ref Quaternion qua, out Quaternion outQua);

        /// <summary>
        /// 2 つのクォータニオンの積を計算する
        /// </summary>
        /// <param name="qua1">処理の基になるクォータニオン</param>
        /// <param name="qua2">処理の基になるクォータニオン</param>
        /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNQuaternion_Multiply(ref Quaternion qua1, ref Quaternion qua2, out Quaternion outQua);

        /// <summary>
        /// 2 つのクォータニオンを球面線形補間する
        /// </summary>
        /// <param name="qua1">処理の基になるクォータニオン</param>
        /// <param name="qua2">処理の基になるクォータニオン</param>
        /// <param name="t">補間値</param>
        /// <param name="outQua">演算結果を格納する Quaternion 変数</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNQuaternion_Slerp(ref Quaternion qua1, ref Quaternion qua2,  float t, out Quaternion outQua);

        /// <summary>
        /// BGM を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayBGM( string filePath,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0);

        /// <summary>
        /// メモリ上の音声ファイルデータを使用して BGM を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayBGMMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0);

        /// <summary>
        /// BGM の演奏を停止します。
        /// </summary>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_StopBGM( double fadeTime = 0.0);

        /// <summary>
        /// BGS を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayBGS( string filePath,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0);

        /// <summary>
        /// メモリ上の音声ファイルデータから BGS を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayBGSMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0);

        /// <summary>
        /// BGS の演奏を停止します。、
        /// </summary>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_StopBGS( double fadeTime = 0.0);

        /// <summary>
        /// ME を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayME( string filePath,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// メモリ上の音声ファイルデータから ME を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlayMEMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// ME の演奏を停止します。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_StopME();

        /// <summary>
        /// SE を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlaySE( string filePath,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// SE を演奏します。(3D サウンド)
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="position">3D 空間上の座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlaySE3D( string filePath, ref Vector3 position,  float distance,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// SE を演奏します。(3D サウンド)
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlaySE3DXYZ( string filePath,  float x,  float y,  float z,  float distance,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// メモリ上の音声データから SE を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlaySEMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// メモリ上の音声ファイルデータから SE を演奏します。 (3D サウンド)
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="position">3D 空間上の座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlaySE3DMem( byte[] data,  int dataSize, ref Vector3 position,  float distance,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// メモリ上の音声ファイルデータから SE を演奏します。(3D サウンド)
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_PlaySE3DMemXYZ( byte[] data,  int dataSize,  float x,  float y,  float z,  float distance,  int volume = 100,  int pitch = 100);

        /// <summary>
        /// すべての SE の演奏を停止します。
        /// </summary>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_StopSE();

        /// <summary>
        /// 再生中のBGMの音量を設定します。(フェードアウト中は無効)
        /// </summary>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_SetBGMVolume( int volume,  double fadeTime = 0.0);

        /// <summary>
        /// 再生中のBGSの音量を設定します。(フェードアウト中は無効)
        /// </summary>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNAudio_SetBGSVolume( int volume,  double fadeTime = 0.0);

        /// <summary>
        /// 3D音声のリスナーの位置を設定します。
        /// </summary>
        /// <param name="position">3D 空間上の座標</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetPosition(ref Vector3 position);

        /// <summary>
        /// 3D音声のリスナーの正面方向を設定します。
        /// </summary>
        /// <param name="direction">向き</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetDirection(ref Vector3 direction);

        /// <summary>
        /// 3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
        /// </summary>
        /// <param name="direction">上方向</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetUpDirection(ref Vector3 direction);

        /// <summary>
        /// 3D音声のリスナーの速度を設定します。
        /// </summary>
        /// <param name="velocity">速度</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetVelocity(ref Vector3 velocity);

        /// <summary>
        /// 3D音声のリスナーの位置を設定します。
        /// </summary>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetPositionXYZ( float x,  float y,  float z);

        /// <summary>
        /// 3D音声のリスナーの正面方向を設定します。
        /// </summary>
        /// <param name="x">向きの X 成分</param>
        /// <param name="y">向きの Y 成分</param>
        /// <param name="z">向きの Z 成分</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetDirectionXYZ( float x,  float y,  float z);

        /// <summary>
        /// 3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
        /// </summary>
        /// <param name="x">向きの X 成分</param>
        /// <param name="y">向きの Y 成分</param>
        /// <param name="z">向きの Z 成分</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetUpDirectionXYZ( float x,  float y,  float z);

        /// <summary>
        /// 3D音声のリスナーの速度を設定します。
        /// </summary>
        /// <param name="x">速度の X 成分</param>
        /// <param name="y">速度の Y 成分</param>
        /// <param name="z">速度の Z 成分</param>
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static Result LNSoundListener_SetVelocityXYZ( float x,  float y,  float z);



        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_Open( string filePath,  string password);

        [DllImport(DLLName, CharSet = CharSet.Unicode, CallingConvention = DefaultCallingConvention)]
        public extern static bool LArchiveMaker_AddFile( string filePath,  string accessName);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LArchiveMaker_Close();

        // 文字列の取得に使用する
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrStringLength(IntPtr strPtr, out int len);

        // 文字列の取得に使用する
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static void LCSInternal_GetIntPtrString(IntPtr strPtr, StringBuilder outBuffer);
    }
}
