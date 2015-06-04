using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace LNote
{


    public class GUIElement : CoreObject
    {

    }

    public class GUIControl : GUIElement
    {

    }

    public class GUIContentControl : GUIControl
    {

    }

    public class GUIRootPane : GUIContentControl
    {

    }

    public class GUIButton : GUIContentControl
    {
        public GUIButton()
            : base(_LNInternal.InternalBlock)
        {

        }
    }





    //---------------------------------------------------------------------------------------------
    
    internal enum _LNInternal
    {
        InternalBlock,
    }   


    /// <summary>
    /// 参照カウントを持つオブジェクトをラップする
    /// </summary>
    public class CoreObject : IDisposable
    {
        internal IntPtr _handle;

        internal CoreObject() { }
        //internal CoreObject(_LNInternal i) { }

        /// <summary>
        /// ハンドルの取得
        /// </summary>
        public IntPtr Handle { get { return _handle; } }

        /// <summary>
        /// デストラクタ
        /// </summary>
        ~CoreObject() { Dispose(); }

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
                if (CoreObjectManager.SystemInitialized)
                {
                    CoreObjectManager.UnregisterWrapperObject(this);
                    API.LNObject_Release(_handle);
                }
                _handle = IntPtr.Zero;
            }
            GC.SuppressFinalize(this);
        }
    }

    internal class TypeInfo
    {
        /// <summary>
        /// handle をラップするラップオブジェクトを生成するファクトリコールバック
        /// </summary>
        public delegate CoreObject ReferenceObjectFactory(IntPtr handle);

        public ReferenceObjectFactory Factory;

        private static List<TypeInfo> _typeInfos = new List<TypeInfo>();

        public static void Register()
        {
            // 自動生成1件分
            {

            }



            var _IntTable = new TypeInfo()
            {
                Factory = (handle) =>
                {
                    var obj = new IntTable(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_IntTable);
            LNIntTable_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));

            var _Random = new TypeInfo()
            {
                Factory = (handle) =>
                {
                    var obj = new Random(_LNInternal.InternalBlock);
                    obj.SetHandle(handle);
                    return obj;
                }
            };
            _typeInfos.Add(_Random);
            LNRandom_SetTypeUserData((IntPtr)(_typeInfos.Count - 1));

        }

        public static TypeInfo GetTypeInfoByHandle(IntPtr handle)
        {
            int index = (int)LNObject_GetTypeUserData(handle);
            return _typeInfos[index];
        }

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern IntPtr LNObject_GetTypeUserData(IntPtr handle);
        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNIntTable_SetTypeUserData(IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNRandom_SetTypeUserData(IntPtr data);


    }

    internal class UserData
    {
        public WeakReference RefObject;
    }

    internal static class CoreObjectManager
    {
        private const int InitialListSize = 1024;


        private static Stack<int> _userDataListIndexStack;
        private static List<UserData> _userDataList;           // LNObject_SetUserData で登録するのはこのインデックス。0 は無効値 (ユーザーデータ未割り当て)

        public static bool SystemInitialized = false;

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern void LNObject_SetUserData(IntPtr handle, IntPtr data);

        [DllImport(API.DLLName, CallingConvention = API.DefaultCallingConvention)]
        private static extern IntPtr LNObject_GetUserData(IntPtr handle);

        // 初期化
        public static void Initialize()
        {
            if (!SystemInitialized)
            {
                _userDataListIndexStack = new Stack<int>(InitialListSize);
                _userDataList = new List<UserData>(InitialListSize);

                // 管理リストの空き番号を詰める
                for (int i = 0; i < InitialListSize; i++)
                    _userDataListIndexStack.Push(i);

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
        //public static T GetWrapperObject<T>(IntPtr handle) where T : CoreObject
        public static T CheckRegisterAndGetWrapperObject<T>(IntPtr handle) where T : CoreObject
        {
            int index = (int)LNObject_GetUserData(handle);
            if (index == 0) // 新しく登録する
            {
                var obj = TypeInfo.GetTypeInfoByHandle(handle).Factory(handle);
                RegisterWrapperObject(obj, handle);
                return (T)obj;
                //return CreateWrapperObject<T>(handle);
            }
            else            // 登録済みのオブジェクトを返す
                return (T)_userDataList[index].RefObject.Target;
        }

        // 新しいハンドルを登録し、ラップオブジェクトを生成する
        //public static T CreateWrapperObject<T>(IntPtr handle) where T : ReferenceObject
        //{
        //    var obj = TypeInfo.GetTypeInfoByHandle(handle).Factory(handle);
        //    RegisterWrapperObject(obj, handle);
        //    return (T)obj;
        //}

        // handle を refObj にセットし、refObj を管理リストに追加する
        //  (ReferenceObject のコンストラクタからも呼ばれる)
        public static void RegisterWrapperObject(CoreObject refObj, IntPtr handle)
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
        public static void UnregisterWrapperObject(CoreObject refObj)
        {
            int index = (int)LNObject_GetUserData(refObj.Handle);
            _userDataList[index] = null;
            _userDataListIndexStack.Push(index);
        }
    }
}

