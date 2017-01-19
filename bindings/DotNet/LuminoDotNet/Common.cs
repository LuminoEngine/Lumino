using System;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using System.Text;

namespace Lumino
{
    /// <summary>
    /// ライブラリ内部で発生した例外
    /// </summary>
    public class LuminoException : Exception
    {
        /// <summary>
        /// 結果コード
        /// </summary>
        public ResultCode ResultCode { get; private set; }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name=""resultCode"">結果コード</param>
        public LuminoException(ResultCode resultCode)
        {
            ResultCode = resultCode;
        }

        /// <summary>
        /// コンストラクタ
        /// </summary>
        /// <param name=""resultCode"">結果コード</param>
        public LuminoException(ResultCode resultCode, string message)
            : base(message)
        {
            ResultCode = resultCode;
        }

        internal static Exception MakeExceptionFromLastError(ResultCode result)
        {
            IntPtr errStr;
            int errStrLen;
            API.LNError_GetLastErrorMessage(out errStr);
            API.LCSInternal_GetIntPtrStringLength(errStr, out errStrLen);
            var errBuf = new StringBuilder(errStrLen);
            API.LCSInternal_GetIntPtrString(errStr, errBuf);
            return new LuminoException(result, errBuf.ToString());
        }
    }


    internal enum _LNInternal
    {
        InternalBlock,
    }

    /// <summary>
    /// 参照カウントを持つオブジェクトをラップする
    /// </summary>
    public class Object : IDisposable
    {
        internal IntPtr _handle;

        internal Object() { }
        internal Object(_LNInternal i) { }

        /// <summary>
        /// ハンドルの取得
        /// </summary>
        public IntPtr Handle { get { return _handle; } }

        /// <summary>
        /// デストラクタ
        /// </summary>
        ~Object() { Dispose(); }

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


    internal class UserData
    {
        public WeakReference RefObject;
    }

    internal static class InternalManager
    {
        private const int InitialListSize = 1024;

        //private static  List<ReferenceObject> _objectList;
        internal static Stack<int> _userDataListIndexStack;
        internal static List<UserData> _userDataList;           // LNObject_SetUserData で登録するのはこのインデックス。0 は無効値 (ユーザーデータ未割り当て)


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
                    _userDataList.Add(new UserData());
                    _userDataListIndexStack.Push(i);
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
        public static T GetWrapperObject<T>(IntPtr handle) where T : Object
        {
            int objectIndex = (int)LNObject_GetUserData(handle);
            if (objectIndex == 0) // 新しく登録する
                return CreateWrapperObject<T>(handle);
            else            // 登録済みのオブジェクトを返す
                return (T)_userDataList[objectIndex].RefObject.Target;
        }

        // 新しいハンドルを登録し、ラップオブジェクトを生成する
        public static T CreateWrapperObject<T>(IntPtr handle) where T : Object
        {
            if (handle == IntPtr.Zero) return null;
            var obj = TypeInfo.GetTypeInfoByHandle(handle).Factory(handle);
            RegisterWrapperObject(obj, handle);
            return (T)obj;
        }

        // handle を refObj にセットし、refObj を管理リストに追加する
        //  (ReferenceObject のコンストラクタからも呼ばれる)
        public static void RegisterWrapperObject(Object refObj, IntPtr handle)
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
        public static void UnregisterWrapperObject(Object refObj)
        {
            int index = (int)LNObject_GetUserData(refObj.Handle);
            _userDataList[index].RefObject = null;
            _userDataListIndexStack.Push(index);
        }
    }
    
    public static class TestInterface
    {
        public static int GetObjectWeakReferenceCount()
        {
            return InternalManager._userDataList.Count - InternalManager._userDataListIndexStack.Count;
        }
    }

    internal static class InternalAPI
    {
    	internal const string DLLName = API.DLLName;
        internal const CharSet DLLCharSet = API.DLLCharSet;
        internal const CallingConvention DefaultCallingConvention = API.DefaultCallingConvention;
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObjectList_GetCount(IntPtr listObject, out int outCount);
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObjectList_SetAt(IntPtr listObject, int index, IntPtr itemPtr);
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObjectList_GetAt(IntPtr listObject, int index, out IntPtr outItemPtr);
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObjectList_Add(IntPtr listObject, IntPtr itemPtr);
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObjectList_Clear(IntPtr listObject);
        
        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObjectList_Insert(IntPtr listObject, int index, IntPtr itemPtr);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObjectList_Remove(IntPtr listObject, IntPtr itemPtr, out bool outRemoved);

        [DllImport(DLLName, CharSet = DLLCharSet, CallingConvention = DefaultCallingConvention)]
        public extern static ResultCode LNObjectList_RemoveAt(IntPtr listObject, int index);
    }

    /// <summary>
    /// オブジェクトのコレクション
    /// </summary>
    public class ObjectList<T> : Object, IEnumerable<T>
        where T : Object
    {
        private List<T> _cacheList;

        internal ObjectList(_LNInternal i) { }

        internal override void SetHandle(IntPtr handle)
        {
            base.SetHandle(handle);

            _cacheList = new List<T>();
            SyncItems();

            //int count = Count;
            //for (int i = 0; i < count; i++)
            //{
            //    IntPtr item;
            //    API.LNObjectList_GetAt(_handle, i, out item);
            //    var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
            //    t.SetHandle(item);
            //    _cacheList.Add(t);
            //}
        }

        public int Count
        {
            get
            {
                int count;
                var result = InternalAPI.LNObjectList_GetCount(_handle, out count);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                return count;
            }
        }

        public T this[int index]
        {
            get
            {
                SyncItems();
                return (T)_cacheList[index];
            }
            set
            {
                SyncItems();
                var result = InternalAPI.LNObjectList_SetAt(_handle, index, value.Handle);
                if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
                _cacheList[index] = value;
            }
        }

        public void Add(T item)
        {
            SyncItems();
            var result = InternalAPI.LNObjectList_Add(_handle, item.Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            _cacheList.Add(item);
        }

        public void Clear()
        {
            var result = InternalAPI.LNObjectList_Clear(_handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            _cacheList.Clear();
        }

        public void Insert(int index, T item)
        {
            SyncItems();
            var result = InternalAPI.LNObjectList_Insert(_handle, index, item.Handle);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            _cacheList.Insert(index, item);
        }

        public bool Remove(T item)
        {
            SyncItems();
            bool removed;
            var result = InternalAPI.LNObjectList_Remove(_handle, item.Handle, out removed);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            _cacheList.Remove(item);
            return removed;
        }

        // TODO
        //public void RemoveAll(T item)
        //{
        //    SyncItems();
        //    var result = InternalAPI.LNObjectList_RemoveAll(_handle, item.Handle);
        //    if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
        //    _cacheList.RemoveAll((i) => i == item);
        //}

        public void RemoveAt(int index)
        {
            SyncItems();
            var result = InternalAPI.LNObjectList_RemoveAt(_handle, index);
            if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);
            _cacheList.RemoveAt(index);
        }

        private void SyncItems()
        {
            int count = Count;
            if (_cacheList.Count < count)
            {
                // 足りない分を詰める
                int ro = count - _cacheList.Count;
                for (int i = 0; i < ro; ++i)
                {
                    _cacheList.Add(null);
                }

                // リスト内容を同期する
                for (int i = 0; i < count; ++i)
                {
                    IntPtr item;
                    var result = InternalAPI.LNObjectList_GetAt(_handle, i, out item);
                    if (result != ResultCode.OK) throw LuminoException.MakeExceptionFromLastError(result);

                    if (_cacheList[i] == null || _cacheList[i].Handle != item)
                    {
                        var t = TypeInfo.GetTypeInfoByHandle(item).Factory(item);
                        t.SetHandle(item);
                        _cacheList[i] = ((T)t);
                    }
                }
            }
        }

        public IEnumerator<T> GetEnumerator()
        {
            return _cacheList.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return _cacheList.GetEnumerator();
        }
    }
}
