using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    /// <summary>
    /// 各言語ソース生成クラスのベース
    /// </summary>
    abstract class Builder
    {
        /// <summary>
        /// 中間コード管理クラス
        /// </summary>
        protected CLManager Manager { get; private set; }

        /// <summary>
        /// LangContext
        /// </summary>
        protected LangContext Context { get; private set; }

        /// <summary>
        /// ソースファイル作成
        /// </summary>
        /// <param name="analyzer"></param>
        public void Build(CLManager manager, LangContext context, string outputFilePath)
        {
            Manager = manager;
            Context = context;

            OnInitialize();

            // enum
            foreach (var e in Manager.AllEnums)
            {
                OnEnumLooked(e);
            }

            // クラス
            foreach (var classType in Manager.AllClasses)
            {
                // 組み込みクラスは対象としない
                if (classType.IsPreDefined) continue;

                // 無視するクラスか？
                if (Context.IsIgnoredClass(classType)) continue;

                if (OnClassLookedStart(classType))
                {
                    // コンストラクタ (並び順的に先頭の方にしたい)
                    foreach (var method in classType.Methods)
                    {
                        if (method.IsRefObjectConstructor)
                        {
                            OnMethodLooked(method);
                        }
                    }

                    // プロパティ
                    foreach (var prop in classType.Properties)
                    {
                        OnPropertyLooked(prop);
                    }

                    // メソッド
                    foreach (var method in classType.Methods)
                    {
                        // プロパティ や internal は出力しない
                        if (method.PropertyNameType == PropertyNameType.NotProperty &&
                            method.Modifier != MethodModifier.Internal &&
                            !method.IsRefObjectConstructor)
                        {
                            OnMethodLooked(method);
                        }
                    }

                    OnClassLookedEnd(classType);
                }
            }

            // ファイルに出力
            string output = OnMakeOutoutFileText();
            output = output.Replace("\r\n", "\n").Replace("\n", "\r\n");
            System.IO.File.WriteAllText(outputFilePath, output, GetOutputEncoding());
        }

        /// <summary>
        /// テンプレートファイルを読み込む
        /// </summary>
        /// <returns></returns>
        protected string GetTemplate(string fileName)
        {
            return System.IO.File.ReadAllText("../../Builder/Templates/" + fileName);
        }

        /// <summary>
        /// ビルド開始前(初期化)通知
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnInitialize() { }

        /// <summary>
        /// enum 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnEnumLooked(CLEnum enumType) { }

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected virtual bool OnClassLookedStart(CLClass classType) { return true; }

        /// <summary>
        /// クラスor構造体 通知 (終了)
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnClassLookedEnd(CLClass classType) { }

        /// <summary>
        /// プロパティ 通知
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnPropertyLooked(CLProperty prop) { }

        /// <summary>
        /// メソッド 通知 (プロパティや internal は通知されない)
        /// </summary>
        /// <param name="enumType"></param>
        protected virtual void OnMethodLooked(CLMethod method) { }
        
        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected abstract string OnMakeOutoutFileText();

        /// <summary>
        /// 出力ファイルのエンコーディング
        /// </summary>
        protected virtual Encoding GetOutputEncoding() { return Encoding.UTF8; }
    }
}
