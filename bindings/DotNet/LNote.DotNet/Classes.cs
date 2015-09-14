using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
    public partial class ote
    {
    
    
        /// <summary>
        /// デバッグ用のログファイルの出力有無を設定します。(初期値:false)
        /// </summary>
        /// <param name="enabled">true:出力する / false:出力しない</param>
        public static void SetApplicationLogEnabled( bool enabled)
        {
            API.LNConfig_SetApplicationLogEnabled( enabled);
        
        }
        
        /// <summary>
        /// 標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(初期値:false)
        /// </summary>
        /// <param name="enabled">true:割り当てる / false:割り当てない</param>
        public static void SetConsoleEnabled( bool enabled)
        {
            API.LNConfig_SetConsoleEnabled( enabled);
        
        }
        
        /// <summary>
        /// ユーザー定義のウィンドウハンドルを設定します。(初期値:NULL)
        /// </summary>
        /// <param name="windowHandle">ユーザー定義のウィンドウハンドル</param>
        public static void SetUserWindowHandle( IntPtr windowHandle)
        {
            API.LNConfig_SetUserWindowHandle( windowHandle);
        
        }
        
        /// <summary>
        /// サウンドオブジェクトのキャッシュサイズの設定
        /// </summary>
        /// <param name="count">キャッシュできるサウンドオブジェクトの最大数 (初期値:32)</param>
        /// <param name="memorySize">サウンドオブジェクトのキャッシュが使用できる最大メモリサイズ (初期値:0)</param>
        /// <remarks>
        /// count が 0 の場合、キャッシュを使用しません。
        /// 					memorySize が 0 の場合、メモリ使用量に制限を設けません。
        /// </remarks>
        public static void SetSoundCacheSize( int count,  int memorySize)
        {
            API.LNConfig_SetSoundCacheSize( count,  memorySize);
        
        }
        
        /// <summary>
        /// DirectMusic の初期化方法を設定します。(初期値:DirectMusicMode.NotUse)
        /// </summary>
        /// <param name="mode">DirectMusic の初期化方法</param>
        /// <remarks>
        /// DirectMusic の初期化には比較的時間がかかります。
        /// 					これを回避するために初期化専用のスレッドで初期化を行うことが出来ます。
        /// </remarks>
        public static void SetDirectMusicInitializeMode( DirectMusicMode mode)
        {
            API.LNConfig_SetDirectMusicInitializeMode( mode);
        
        }
        
        /// <summary>
        /// DirectMusic のリバーブエフェクトの強さを設定します。(初期値:70)
        /// </summary>
        /// <param name="level">リバーブの強さ (0 ～ 100)</param>
        public static void SetDirectMusicReverbLevel( int level)
        {
            API.LNConfig_SetDirectMusicReverbLevel( level);
        
        }
        
    
    };
    
    /// <summary>
    /// LightNote の初期化や更新等、包括的な処理を行うクラスです。
    /// </summary>
    public partial class Application
    {
    
    
        /// <summary>
        /// アプリケーションを初期化します。
        /// </summary>
        /// <remarks>
        /// ライブラリのすべての機能を使用できるように初期化を行います。
        /// </remarks>
        public static void Initialize()
        {
            InternalManager.Initialize();
            var result = API.LNApplication_Initialize();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// アプリケーションを初期化します。音声機能のみを使用する場合に呼び出します。
        /// </summary>
        public static void InitializeAudio()
        {
            InternalManager.Initialize();
            var result = API.LNApplication_InitializeAudio();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// フレームを更新します。
        /// </summary>
        /// <remarks>
        /// LightNote の状態を更新し、時間を1フレーム分進めます。
        /// 					この関数は必ず1フレームに1度だけ呼び出す必要があります。
        /// </remarks>
        public static void Update()
        {
            var result = API.LNApplication_Update();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 画面の更新タイミングをリセットします。
        /// </summary>
        /// <remarks>
        /// 時間のかかる処理の後にこのメソッドを呼ぶことで、
        /// 					極端なフレームスキップが発生しないようにすることができます。
        /// </remarks>
        public static void ResetFrameDelay()
        {
            var result = API.LNApplication_ResetFrameDelay();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// アプリケーションを終了するべきかを確認します。
        /// </summary>
        /// <remarks>
        /// ウィンドウのクローズボタンが押された場合等、
        /// 					アプリケーションを終了するべき時には false を返します。
        /// </remarks>
        public static bool IsEndRequested()
        {
            var requested = new bool();
            var result = API.LNApplication_IsEndRequested(out requested);
            if (result != Result.OK) throw new LNoteException(result);
            return requested;
        
        }
        
        /// <summary>
        /// LightNote の終了処理を行います。
        /// </summary>
        public static void Terminate()
        {
            API.LNApplication_Terminate();
            InternalManager.Terminate();
        
        }
        
    
    };
    
    /// <summary>
    /// 音声再生のユーティリティクラスです。
    /// </summary>
    public partial class Audio
    {
    
    
        /// <summary>
        /// BGM を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        public static void PlayBGM( string filePath,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_PlayBGM( filePath,  volume,  pitch,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータを使用して BGM を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        public static void PlayBGMMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_PlayBGMMem( data,  dataSize,  volume,  pitch,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// BGM の演奏を停止します。
        /// </summary>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        public static void StopBGM( double fadeTime = 0.0)
        {
            var result = API.LNAudio_StopBGM( fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// BGS を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        public static void PlayBGS( string filePath,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_PlayBGS( filePath,  volume,  pitch,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータから BGS を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        /// <param name="fadeTime">フェードインにかける時間 (秒)</param>
        public static void PlayBGSMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_PlayBGSMem( data,  dataSize,  volume,  pitch,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// BGS の演奏を停止します。、
        /// </summary>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        public static void StopBGS( double fadeTime = 0.0)
        {
            var result = API.LNAudio_StopBGS( fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// ME を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlayME( string filePath,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlayME( filePath,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータから ME を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlayMEMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlayMEMem( data,  dataSize,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// ME の演奏を停止します。
        /// </summary>
        public static void StopME()
        {
            var result = API.LNAudio_StopME();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// SE を演奏します。
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySE( string filePath,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE( filePath,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// SE を演奏します。(3D サウンド)
        /// </summary>
        /// <param name="filePath">ファイルパス</param>
        /// <param name="position">3D 空間上の座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySE3D( string filePath,  Vector3 position,  float distance,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE3D( filePath, ref position,  distance,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
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
        public static void PlaySE3D( string filePath,  float x,  float y,  float z,  float distance,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE3DXYZ( filePath,  x,  y,  z,  distance,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声データから SE を演奏します。
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySEMem( byte[] data,  int dataSize,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySEMem( data,  dataSize,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// メモリ上の音声ファイルデータから SE を演奏します。 (3D サウンド)
        /// </summary>
        /// <param name="data">メモリ上の音声ファイルデータ</param>
        /// <param name="dataSize">データサイズ (バイト単位)</param>
        /// <param name="position">3D 空間上の座標</param>
        /// <param name="distance">減衰距離</param>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="pitch">ピッチ (50 ～ 200)</param>
        public static void PlaySE3DMem( byte[] data,  int dataSize,  Vector3 position,  float distance,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE3DMem( data,  dataSize, ref position,  distance,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
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
        public static void PlaySE3DMem( byte[] data,  int dataSize,  float x,  float y,  float z,  float distance,  int volume = 100,  int pitch = 100)
        {
            var result = API.LNAudio_PlaySE3DMemXYZ( data,  dataSize,  x,  y,  z,  distance,  volume,  pitch);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// すべての SE の演奏を停止します。
        /// </summary>
        public static void StopSE()
        {
            var result = API.LNAudio_StopSE();
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 再生中のBGMの音量を設定します。(フェードアウト中は無効)
        /// </summary>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        public static void SetBGMVolume( int volume,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_SetBGMVolume( volume,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 再生中のBGSの音量を設定します。(フェードアウト中は無効)
        /// </summary>
        /// <param name="volume">ボリューム (0 ～ 100)</param>
        /// <param name="fadeTime">フェードアウトにかける時間 (秒)</param>
        public static void SetBGSVolume( int volume,  double fadeTime = 0.0)
        {
            var result = API.LNAudio_SetBGSVolume( volume,  fadeTime);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
    
    };
    
    /// <summary>
    /// 3D音声のリスナーに関する情報を表します。
    /// </summary>
    public partial class SoundListener
    {
    
        /// <summary>
        /// 3D音声のリスナーの位置
        /// </summary>
        public static Vector3 Position
        {
            set
            {
                var result = API.LNSoundListener_SetPosition(ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// 3D音声のリスナーの正面方向
        /// </summary>
        public static Vector3 Direction
        {
            set
            {
                var result = API.LNSoundListener_SetDirection(ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// 3D音声のリスナーの上方向(正面方向とは直交であること)
        /// </summary>
        public static Vector3 UpDirection
        {
            set
            {
                var result = API.LNSoundListener_SetUpDirection(ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
        /// <summary>
        /// 3D音声のリスナーの速度
        /// </summary>
        public static Vector3 Velocity
        {
            set
            {
                var result = API.LNSoundListener_SetVelocity(ref value);
                if (result != Result.OK) throw new LNoteException(result);
            
            }
            
        }
    
        /// <summary>
        /// 3D音声のリスナーの位置を設定します。
        /// </summary>
        /// <param name="x">3D 空間上の X 座標</param>
        /// <param name="y">3D 空間上の Y 座標</param>
        /// <param name="z">3D 空間上の Z 座標</param>
        public static void SetPositionXYZ( float x,  float y,  float z)
        {
            var result = API.LNSoundListener_SetPositionXYZ( x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 3D音声のリスナーの正面方向を設定します。
        /// </summary>
        /// <param name="x">向きの X 成分</param>
        /// <param name="y">向きの Y 成分</param>
        /// <param name="z">向きの Z 成分</param>
        public static void SetDirection( float x,  float y,  float z)
        {
            var result = API.LNSoundListener_SetDirectionXYZ( x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 3D音声のリスナーの上方向を設定します。(正面方向とは直交であること)
        /// </summary>
        /// <param name="x">向きの X 成分</param>
        /// <param name="y">向きの Y 成分</param>
        /// <param name="z">向きの Z 成分</param>
        public static void SetUpDirection( float x,  float y,  float z)
        {
            var result = API.LNSoundListener_SetUpDirectionXYZ( x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
        /// <summary>
        /// 3D音声のリスナーの速度を設定します。
        /// </summary>
        /// <param name="x">速度の X 成分</param>
        /// <param name="y">速度の Y 成分</param>
        /// <param name="z">速度の Z 成分</param>
        public static void Velocity( float x,  float y,  float z)
        {
            var result = API.LNSoundListener_SetVelocityXYZ( x,  y,  z);
            if (result != Result.OK) throw new LNoteException(result);
        
        }
        
    
    };
    
	

    

    internal enum _LNInternal
    {
        InternalBlock,
    }   

    /// <summary>
    /// 参照カウントを持つオブジェクトをラップする
    /// </summary>
    public class ReferenceObject : IDisposable
    {
        internal IntPtr _handle;

        internal ReferenceObject() {}
        internal ReferenceObject(_LNInternal i) {}

        /// <summary>
        /// ハンドルの取得
        /// </summary>
        public IntPtr Handle { get { return _handle; } }

        /// <summary>
        /// デストラクタ
        /// </summary>
        ~ReferenceObject() { Dispose(); }

        internal virtual void SetHandle(IntPtr handle)
        {
            _handle = handle;
        }       

        /// <summary>
        /// Dispose
        /// </summary>
        public void Dispose()
        {
            if (_handle != IntPtr.Zero)
            {
                if (InternalManager.SystemInitialized)
                {
                    InternalManager.UnregisterWrapperObject(this);
                    API.LNObject_Release(_handle);
                }
                _handle = IntPtr.Zero;
            }
            GC.SuppressFinalize(this);
        }
    }

    internal class TypeInfo
    {
        public delegate ReferenceObject ReferenceObjectFactory(IntPtr handle);

        public ReferenceObjectFactory Factory;

        private static List<TypeInfo> _typeInfos = new List<TypeInfo>();

        public static void Register()
        {

        }

        public static TypeInfo GetTypeInfoByHandle(IntPtr handle)
        {
            int index = (int)LNObject_GetTypeUserData(handle);
            return _typeInfos[index];
        }

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern IntPtr LNObject_GetTypeUserData(IntPtr handle);

    }

    /// <summary>
    /// オブジェクトのコレクション
    /// </summary>
    public class ObjectList<T> : ReferenceObject
        where T : ReferenceObject
    {
        private List<ReferenceObject> _list;
        
        internal ObjectList(_LNInternal i) {}

        internal override void SetHandle(IntPtr handle)
        {
            _list = new List<ReferenceObject>();
            _handle = handle;
            int count = Count;
            for (int i = 0; i < count; i++)
            {
                IntPtr item;
                API.LNObjectList_GetAt(_handle, i, out item);
                var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
                t.SetHandle(item);
                _list.Add(t);
            }
        }
        
        public int Count
        {
            get
            {
                int count;
                API.LNObjectList_GetCount(_handle, out count);
                return count;
            }
        }
        
        public T this[int index]
        {
            get
            {
                SyncItems();
                return (T)_list[index];
            }
            set
            {
                SyncItems();
                _list[index] = value;
                API.LNObjectList_SetAt(_handle, index, value.Handle);
            }
        }
        
        public void Add(T item)
        {
            SyncItems();
            _list.Add(item);
            API.LNObjectList_Add(_handle, item.Handle);
        }
        
        public void Clear()
        {
            _list.Clear();
            API.LNObjectList_Clear(_handle);
        }
        
        public void Insert(int index, T item)
        {
            SyncItems();
            _list.Insert(index, item);
            API.LNObjectList_Insert(_handle, index, item.Handle);
        }
        
        public void Remove(T item)
        {
            SyncItems();
            _list.Remove(item);
            API.LNObjectList_Remove(_handle, item.Handle);
        }
        
        public void RemoveAll(T item)
        {
            SyncItems();
            _list.RemoveAll((i) => i == item);
            API.LNObjectList_RemoveAll(_handle, item.Handle);
        }
        
        public void RemoveAt(int index)
        {
            SyncItems();
            _list.RemoveAt(index);
            API.LNObjectList_RemoveAt(_handle, index);
        }

        private void SyncItems() 
        {
            int count = Count;
            if (_list.Count < count)
            {
                // 足りない分を詰める
                for (int i = 0; i < count - _list.Count; ++i)
		        {
                    _list.Add(null);
                }

                // リスト内容を同期する
                for (int i = 0; i < count; ++i)
		        {
                    IntPtr item;
                    API.LNObjectList_GetAt(_handle, i, out item);
			        if (_list[i] == null || _list[i].Handle != item)
			        {
                        var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
                        t.SetHandle(item);
                        _list.Add(t);
			        }
		        }
            }
        }
    }

	internal class UserData
    {
        public WeakReference     RefObject;
    }

    internal static class InternalManager
    {
        private const int InitialListSize = 1024;
        
        //private static  List<ReferenceObject> _objectList;
        private static  Stack<int> _userDataListIndexStack;
        private static  List<UserData> _userDataList;           // LNObject_SetUserData で登録するのはこのインデックス。0 は無効値 (ユーザーデータ未割り当て)

		
        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
		private static extern void LNObject_SetUserData(IntPtr handle, IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
		private static extern IntPtr LNObject_GetUserData(IntPtr handle);

		// init ～ terminate
        public static bool SystemInitialized = false;

		// 初期化
		public static void Initialize()
		{
			if (!SystemInitialized)
			{
				_userDataListIndexStack = new Stack<int>(InitialListSize);
				_userDataList = new List<UserData>(InitialListSize); 

				// 管理リストの空き番号を詰める
				for (int i = 0; i < InitialListSize; i++)
                {
					_userDataListIndexStack.Push(i);
                    _userDataList.Add(null);
                }
					
				// 型情報の登録
				TypeInfo.Register();

				SystemInitialized = true;
			}
		}

		// 終了処理
		public static void Terminate()
		{
			SystemInitialized = false;
			GC.Collect();
		}

        // ハンドルからラップオブジェクトを返す
        public static T GetWrapperObject<T>(IntPtr handle) where T : ReferenceObject
        {
            int index = (int)LNObject_GetUserData(handle);
            if (index == 0) // 新しく登録する
                return CreateWrapperObject<T>(handle);
            else            // 登録済みのオブジェクトを返す
                return (T)_userDataList[index].RefObject.Target;
        }

        // 新しいハンドルを登録し、ラップオブジェクトを生成する
        public static T CreateWrapperObject<T>(IntPtr handle) where T : ReferenceObject
        {
            var obj = TypeInfo.GetTypeInfoByHandle(handle).Factory(handle);
            RegisterWrapperObject(obj, handle);
            return (T)obj;
        }

        // handle を refObj にセットし、refObj を管理リストに追加する
        //  (ReferenceObject のコンストラクタからも呼ばれる)
        public static void RegisterWrapperObject(ReferenceObject refObj, IntPtr handle)
        {
            // 管理リストが一杯の時は、倍のサイズに拡張する
            if (_userDataListIndexStack.Count == 0)
            {
                int growCount = _userDataList.Count;
                _userDataList.Capacity = growCount + growCount;
                for (int i = 0; i < growCount; i++)
                {
                    _userDataList.Add(null);
                    _userDataListIndexStack.Push(growCount + i);
                }
            }

            // 空きインデックスに new する
            //      new T ではなくちゃんとファクトリから作る。 new T ではサブクラスを考慮できない。
            //      コレクションの取得で Sprite を作りたいのに SceneNode を作ってしまうかもしれない。
            int index = _userDataListIndexStack.Pop();
            refObj.SetHandle(handle);
            _userDataList[index].RefObject = new WeakReference(refObj);

            // 管理配列のインデックスをユーザーデータとして関連付ける
            LNObject_SetUserData(handle, (IntPtr)index);
        }

        // RefObject の Dispose から呼ばれ、管理リストのインデックスを解放する
        //      (ユーザーデータを取得するため、LNObject_Release より前に呼ぶこと)
        public static void UnregisterWrapperObject(ReferenceObject refObj)
        {
            int index = (int)LNObject_GetUserData(refObj.Handle);
            _userDataList[index] = null;
            _userDataListIndexStack.Push(index);
        }
    }

}
