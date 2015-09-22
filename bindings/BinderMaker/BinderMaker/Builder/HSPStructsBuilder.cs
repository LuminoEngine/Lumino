using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    class HSPStructsBuilder : Builder
    {
        private static string RefFuncCaseTemplate = @"
case [ID]:
{
    static [TYPE] returnValue;
    if (CheclDefault()) {
        [DEFAULT]
    }
    else {
        [INIT]
    }
    *retValPtr = &returnValue;
    *typeRes = hsp[TYPE]_typeid();
    return true;
}
";

        public const int ConstIdBegin = 10;
        private OutputBuffer _allDefines = new OutputBuffer();
        private OutputBuffer _allRegisters = new OutputBuffer(1);
        private OutputBuffer _reffuncCase = new OutputBuffer(1);
        private int _idCount = ConstIdBegin;

        /// <summary>
        /// クラスor構造体 通知 (開始)
        /// </summary>
        /// <param name="classType"></param>
        /// <returns>false の場合このクラスの出力を無視する</returns>
        protected override bool OnClassLookedStart(CLClass classType)
        {
            if (!classType.IsStruct) return false;

            var originalName = classType.StructData.OriginalName;

            // 各種関数
            string t = GetTemplate("HSPOneStruct.txt");
            t = t.Replace("[TYPE]", originalName);
            _allDefines.AppendWithIndent(t);

            // 登録文
            _allRegisters.AppendLine("registvar(-1, hsp{0}_Init);", originalName);

            //-------------------------------------------------
            // reffunc (コンストラクタ的なものを定義する)

            // デフォルトの場合の初期化式
            string defaultExp = "memset(&returnValue, 0, sizeof(returnValue));";
            if (originalName == "LNMatrix")
                defaultExp = "LNMatrix_Identity(&returnValue);";   // 行列の場合は単位行列にする

            // 各メンバ代入式
            OutputBuffer initExp = new OutputBuffer(2);
            foreach (var member in classType.StructData.Members)
            {
                initExp.AppendLine("returnValue.{0} = {1};", member.Name, "GetParamDouble()");
            }

            // 結合
            t = RefFuncCaseTemplate.Trim();
            t = t.Replace("[ID]", string.Format("0x{0:X4}", _idCount));
            t = t.Replace("[TYPE]", originalName);
            t = t.Replace("[DEFAULT]", defaultExp);
            t = t.Replace("[INIT]", initExp.ToString().Trim());
            t += OutputBuffer.NewLineCode;
            _reffuncCase.AppendWithIndent(t);

            _idCount++;
            return true;
        }

        /// <summary>
        /// ファイルに出力するための最終文字列を生成する
        /// </summary>
        protected override string OnMakeOutoutFileText()
        {
            string t = GetTemplate("HSPStructs.txt");
            t = t.Replace("[DEFINES]", _allDefines.ToString());
            t = t.Replace("[REFFUNC_CASE]", _reffuncCase.ToString());
            t = t.Replace("[REGISTER]", _allRegisters.ToString());
            return t;
        }

        /// <summary>
        /// 出力ファイルのエンコーディング
        /// </summary>
        protected override Encoding GetOutputEncoding() { return Encoding.GetEncoding(932); }
    }
}
