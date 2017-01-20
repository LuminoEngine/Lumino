using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
    /// <summary>
    /// アプリケーション全体にかかわる処理を行います。
    /// </summary>
    public static class Engine
    {
        /// <summary>
        /// エンジンの初期化処理を行います。
        /// </summary>
        public static void Initialize()
        {
            
            var result = API.LNEngine_Initialize();
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// エンジンの終了処理を行います。
        /// </summary>
        public static void Terminate()
        {
            
            var result = API.LNEngine_Terminate();
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// 1フレーム分の更新処理を行います。
        /// </summary>
        /// <returns>
        /// アプリケーションの終了が要求されている場合は false を返します。
        /// </returns>
        /// <remarks>
        /// この関数はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。
        /// </remarks>
        public static bool Update()
        {
            bool outReturn;
            var result = API.LNEngine_Update(out outReturn);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            return outReturn;
        }

    }
    /// <summary>
    /// 音声の再生、制御を行います。
    /// </summary>
    public  class Sound
        : Object
    {
        /// <summary>
        /// この音声の音量を取得します。
        /// この音声の音量を設定します。
        /// </summary>
        public float Volume
        {
            get
            {
                float outReturn;
                var result = API.LNSound_GetVolume(Handle, out outReturn);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outReturn;
            }
            set
            {
                
                var result = API.LNSound_SetVolume(Handle, value);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                
            }
        }

        /// <summary>
        /// この音声のピッチ (音高) を取得します。
        /// この音声のピッチ (音高) を設定します。
        /// </summary>
        public float Pitch
        {
            get
            {
                float outReturn;
                var result = API.LNSound_GetPitch(Handle, out outReturn);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outReturn;
            }
            set
            {
                
                var result = API.LNSound_SetPitch(Handle, value);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                
            }
        }

        /// <summary>
        /// ループ再生の有無を設定します。
        /// </summary>
        public bool LoopEnabled
        {
            set
            {
                
                var result = API.LNSound_SetLoopEnabled(Handle, value);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                
            }
        }

        /// <summary>
        /// ループ再生が有効かを確認します。
        /// </summary>
        public bool oopEnabled
        {
            get
            {
                bool outReturn;
                var result = API.LNSound_IsLoopEnabled(Handle, out outReturn);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outReturn;
            }
        }

        /// <summary>
        /// ループ範囲を設定します。
        /// </summary>
        /// <param name="begin">
        /// ループ範囲の開始サンプル
        /// </param>
        /// <param name="length">
        /// ループ範囲のサンプル数
        /// </param>
        /// <remarks>
        /// MIDI の場合、ループ範囲はミュージックタイム単位 (四分音符ひとつ分を 768 で表す) で指定します。
        /// </remarks>
        public  void SetLoopRange(int begin, int length)
        {
            
            var result = API.LNSound_SetLoopRange(Handle, begin, length);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// この音声の再生を開始します。
        /// </summary>
        public  void Play()
        {
            
            var result = API.LNSound_Play(Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// この音声の再生を停止します。
        /// </summary>
        public  void Stop()
        {
            
            var result = API.LNSound_Stop(Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// この音声の再生を一時停止します。
        /// </summary>
        public  void Pause()
        {
            
            var result = API.LNSound_Pause(Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// 一時停止中の再生を再開します。
        /// </summary>
        public  void Resume()
        {
            
            var result = API.LNSound_Resume(Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// 音量のフェード中であるかを確認します。
        /// </summary>
        public Sound(string filePath) : base(_LNInternal.InternalBlock)
        {
            IntPtr outSound;
            var result = API.LNSound_Initialize(filePath, out outSound);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            InternalManager.RegisterWrapperObject(this, outSound); API.LNObject_Release(outSound);
        }

        internal Sound(_LNInternal i) : base(i) {}

    }
    /// <summary>
    /// ゲームアプリケーションを表します。
    /// </summary>
    public  class GameApplication
        : Object
    {
        /// <summary>
        /// アプリケーションを実行します。
        /// </summary>
        public  void Run(GameScene initialScene)
        {
            
            var result = API.LNGameApplication_Run(Handle, initialScene.Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        public GameApplication() : base(_LNInternal.InternalBlock)
        {
            IntPtr outGameApplication;
            var result = API.LNGameApplication_Initialize(out outGameApplication);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            InternalManager.RegisterWrapperObject(this, outGameApplication); API.LNObject_Release(outGameApplication);
        }

        internal GameApplication(_LNInternal i) : base(i) {}

    }
    /// <summary>
    /// ゲームシーンのベースクラスです。
    /// </summary>
    /// <remarks>
    /// ゲームシーンはタイトル画面やゲームオーバー画面などの画面遷移の単位です。
    /// </remarks>
    public  class GameScene
        : Object
    {
        /// <summary>
        /// 開始処理
        /// </summary>
        public virtual void OnStart()
        {
            
            var result = API.LNGameScene_OnStart_VirtualBase(Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        private static ResultCode OnStart_OverrideCallback(IntPtr gamescene)
        {
        	var obj = InternalManager.GetWrapperObject<GameScene>(gamescene);
        	try
        	{
        		obj.OnStart();
        	}
        	catch (Exception)
        	{
        		return ResultCode.ErrorUnknown; 
        	}
        	return ResultCode.OK;
        }

        public GameScene() : base(_LNInternal.InternalBlock)
        {
            IntPtr outGameScene;
            var result = API.LNGameScene_Initialize(out outGameScene);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            InternalManager.RegisterWrapperObject(this, outGameScene); API.LNObject_Release(outGameScene);
        }

        internal GameScene(_LNInternal i) : base(i) {}

    }



    internal class TypeInfo
    {
        public delegate Object ReferenceObjectFactory(IntPtr handle);

        public ReferenceObjectFactory Factory;

        private static List<TypeInfo> _typeInfos = new List<TypeInfo>();

        public static void Register()
        {
        	// [0] はダミー
            _typeInfos.Add(null);
            
            var _Sound = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new Sound(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_Sound);
            LNSound_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _GameApplication = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new GameApplication(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_GameApplication);
            LNGameApplication_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _GameScene = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new GameScene(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_GameScene);
            LNGameScene_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));


        }

        public static TypeInfo GetTypeInfoByHandle(IntPtr handle)
        {
            int index = (int)LNObject_GetBindingTypeData(handle);
            return _typeInfos[index];
        }

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern IntPtr LNObject_GetBindingTypeData(IntPtr handle);
        
        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNSound_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNGameApplication_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNGameScene_SetBindingTypeInfo(IntPtr data);


    }
}
