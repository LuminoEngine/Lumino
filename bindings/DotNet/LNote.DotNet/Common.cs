using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace LN
{
    /// <summary>
    /// ライブラリ内部で発生した例外
    /// </summary>
    public class LuminoException : Exception
    {
        /// <summary>
        /// 結果コード
        /// </summary>
        public Result ResultCode { get; private set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name=""resultCode"">結果コード</param>
        public LuminoException(Result resultCode)
        {
            ResultCode = resultCode;
        }
    }

    internal class UserData
    {
        public WeakReference RefObject;
    }

    internal static class InternalManager
    {
        private const int InitialListSize = 1024;

        //private static  List<ReferenceObject> _objectList;
        private static Stack<int> _userDataListIndexStack;
        private static List<UserData> _userDataList;           // LNObject_SetUserData で登録するのはこのインデックス。0 は無効値 (ユーザーデータ未割り当て)


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
                    _userDataList.Add(new UserData());
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
            // 管理リストが一杯の時は拡張する
            if (_userDataListIndexStack.Count == 0)
            {
                int growCount = _userDataList.Count;
                _userDataList.Capacity = growCount + InitialListSize;
                for (int i = 0; i < growCount; i++)
                {
                    _userDataList.Add(new UserData());
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
            _userDataList[index].RefObject = null;
            _userDataListIndexStack.Push(index);
        }
    }
}
