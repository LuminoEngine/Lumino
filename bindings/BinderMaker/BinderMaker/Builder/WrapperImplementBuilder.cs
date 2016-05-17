using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Builder
{
    /// <summary>
    /// 
    /// </summary>
    class WrapperImplementBuilder : Builder
    {
        OutputBuffer _classText;
        OutputBuffer _outputText = new OutputBuffer();
       
        protected override bool OnClassLookedStart(CLClass classType)
        {
            // 以下の種類は Wrapper 対象ではない
            if (classType.IsStatic || 
                classType.IsStruct || 
                classType.IsExtension)
                return false;

            _classText = new OutputBuffer();

            _classText.AppendLine("class LNWI{0} : public ln::{0}", classType.Name);
            _classText.AppendLine("{");

            return true;
        }

        protected override void OnClassLookedEnd(CLClass classType)
        {
            _classText.AppendLine("};");
            _classText.NewLine();
            _outputText.AppendWithIndent(_classText.ToString());
        }

        protected override string OnMakeOutoutFileText()
        {
            return _outputText.ToString();
        }
    }
}
