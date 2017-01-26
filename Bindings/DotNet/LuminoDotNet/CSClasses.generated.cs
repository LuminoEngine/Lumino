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
        /// ループ再生が有効かを確認します。
        /// ループ再生の有無を設定します。
        /// </summary>
        public bool IsLoopEnabled
        {
            get
            {
                bool outReturn;
                var result = API.LNSound_IsLoopEnabled(Handle, out outReturn);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outReturn;
            }
            set
            {
                
                var result = API.LNSound_SetLoopEnabled(Handle, value);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                
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
        public void SetLoopRange(int begin, int length)
        {
            
            var result = API.LNSound_SetLoopRange(Handle, begin, length);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// この音声の再生を開始します。
        /// </summary>
        public void Play()
        {
            
            var result = API.LNSound_Play(Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// この音声の再生を停止します。
        /// </summary>
        public void Stop()
        {
            
            var result = API.LNSound_Stop(Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// この音声の再生を一時停止します。
        /// </summary>
        public void Pause()
        {
            
            var result = API.LNSound_Pause(Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        /// <summary>
        /// 一時停止中の再生を再開します。
        /// </summary>
        public void Resume()
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
    /// テクスチャのクラスです。
    /// </summary>
    public  class Texture
        : GraphicsResourceObject
    {
        internal Texture(_LNInternal i) : base(i) {}

    }
    /// <summary>
    /// 2D テクスチャのクラスです。
    /// </summary>
    public  class Texture2D
        : Texture
    {
        /// <summary>
        /// 指定した色でテクスチャ全体を塗りつぶします。
        /// </summary>
        public Texture2D(int width, int height, TextureFormat format = TextureFormat.R8G8B8A8, bool mipmap = false) : base(_LNInternal.InternalBlock)
        {
            IntPtr outTexture2D;
            var result = API.LNTexture2D_Initialize(width, height, format, mipmap, out outTexture2D);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            InternalManager.RegisterWrapperObject(this, outTexture2D); API.LNObject_Release(outTexture2D);
        }

        public Texture2D(string filePath, TextureFormat format = TextureFormat.R8G8B8A8, bool mipmap = false) : base(_LNInternal.InternalBlock)
        {
            IntPtr outTexture2D;
            var result = API.LNTexture2D_InitializeFFM(filePath, format, mipmap, out outTexture2D);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            InternalManager.RegisterWrapperObject(this, outTexture2D); API.LNObject_Release(outTexture2D);
        }

        internal Texture2D(_LNInternal i) : base(i) {}

    }
    /// <summary>
    /// グラフィックスモジュールの基本的なリソースオブジェクトのベースクラスです。デバイスの状態変化を通知する機能を実装します。
    /// </summary>
    public  class GraphicsResourceObject
        : Object
    {
        internal GraphicsResourceObject(_LNInternal i) : base(i) {}

    }
    public  class SceneNode
        : Component
    {
        /// <summary>
        /// 可視状態を取得します。
        /// 可視状態を設定します。false の場合、ノードの描画自体行われません。(default: true)
        /// </summary>
        public bool IsVisible
        {
            get
            {
                bool outReturn;
                var result = API.LNSceneNode_IsVisible(Handle, out outReturn);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return outReturn;
            }
            set
            {
                
                var result = API.LNSceneNode_SetVisible(Handle, value);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                
            }
        }

        internal SceneNode(_LNInternal i) : base(i) {}

    }
    public  class VisualNode
        : SceneNode
    {
        internal VisualNode(_LNInternal i) : base(i) {}

    }
    /// <summary>
    /// スプライトの抽象クラスです。スプライトオブジェクトを作成するには Sprite2D または Sprite3D クラス使用します。
    /// </summary>
    public  class Sprite
        : VisualNode
    {
        internal Sprite(_LNInternal i) : base(i) {}

    }
    /// <summary>
    /// 2D 空間に配置されるスプライトのクラスです。
    /// </summary>
    public  class Sprite2D
        : Sprite
    {
        /// <summary>
        /// テクスチャを指定してスプライトを作成します。
        /// </summary>
        /// <param name="texture">
        /// スプライトが表示するテクスチャ@detail		作成されたスプライトは、デフォルトの 2D シーングラフ に追加されます。スプライトのサイズは、指定したテクスチャのサイズとなります。
        /// </param>
        public Sprite2D() : base(_LNInternal.InternalBlock)
        {
            IntPtr outSprite2D;
            var result = API.LNSprite2D_Initialize(out outSprite2D);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            InternalManager.RegisterWrapperObject(this, outSprite2D); API.LNObject_Release(outSprite2D);
        }

        public Sprite2D(Texture texture) : base(_LNInternal.InternalBlock)
        {
            IntPtr outSprite2D;
            var result = API.LNSprite2D_InitializeT(texture.Handle, out outSprite2D);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            InternalManager.RegisterWrapperObject(this, outSprite2D); API.LNObject_Release(outSprite2D);
        }

        internal Sprite2D(_LNInternal i) : base(i) {}

    }
    /// <summary>
    /// 3D 空間に配置されるスプライトのクラスです。
    /// </summary>
    public  class Sprite3D
        : Sprite
    {
        internal Sprite3D(_LNInternal i) : base(i) {}

    }
    /// <summary>
    /// 
    /// </summary>
    public  class Component
        : Object
    {
        internal Component(_LNInternal i) : base(i) {}

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
        public void Run(GameScene initialScene = null)
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
        protected virtual void OnStart()
        {
            
            var result = API.LNGameScene_OnStart_CallVirtualBase(Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            
        }

        internal static ResultCode OnStart_OverrideCallback(IntPtr gamescene)
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

        protected GameScene() : base(_LNInternal.InternalBlock)
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

            var _Texture = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new Texture(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_Texture);
            LNTexture_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _Texture2D = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new Texture2D(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_Texture2D);
            LNTexture2D_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _GraphicsResourceObject = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new GraphicsResourceObject(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_GraphicsResourceObject);
            LNGraphicsResourceObject_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _SceneNode = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new SceneNode(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_SceneNode);
            LNSceneNode_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _VisualNode = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new VisualNode(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_VisualNode);
            LNVisualNode_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _Sprite = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new Sprite(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_Sprite);
            LNSprite_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _Sprite2D = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new Sprite2D(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_Sprite2D);
            LNSprite2D_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _Sprite3D = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new Sprite3D(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_Sprite3D);
            LNSprite3D_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

            var _Component = new TypeInfo(){ Factory = (handle) =>
            {
                var obj = new Component(_LNInternal.InternalBlock);
                obj.SetHandle(handle);
                return obj;
            }};
            _typeInfos.Add(_Component);
            LNComponent_SetBindingTypeInfo((IntPtr)(_typeInfos.Count - 1));

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

            API.LNGameScene_OnStart_SetOverrideCaller(GameScene.OnStart_OverrideCallback);
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
        private static extern void LNTexture_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNTexture2D_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNGraphicsResourceObject_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNSceneNode_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNVisualNode_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNSprite_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNSprite2D_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNSprite3D_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNComponent_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNGameApplication_SetBindingTypeInfo(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNGameScene_SetBindingTypeInfo(IntPtr data);


    }
}
