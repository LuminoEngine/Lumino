using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker
{
    static class Diag
    {
        public static void ReportError(CLMethod method, string format, params object[] args)
        {
            ReportError(method.FuncDecl.OriginalFullName, format, args);
        }

        public static void ReportError(string funcName, string format, params object[] args)
        {
            throw new InvalidOperationException(funcName + ": " + string.Format(format, args));
        }

        public static void ReportError(string format, params object[] args)
        {
            throw new InvalidOperationException(string.Format(format, args));
        }
    }
}
