using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using BinderMaker.Decls;

namespace BinderMaker.Parser2
{
    class ApiDeclParser
    {
        /// <summary>
        /// 解析実行
        /// </summary>
        /// <param name="filePath"></param>
        public List<ModuleDecl> Analyze(string[] filePaths)
        {
            var list = new List<ModuleDecl>();
            foreach (var path in filePaths)
            {
                list.Add(AnalyzeOneFile(path));
            }
            return list;
        }

        private ModuleDecl AnalyzeOneFile(string filePath)
        {
            string text = System.IO.File.ReadAllText(filePath, Encoding.UTF8);
            text = text.Replace("\r\n", "\n");
            return ApiModule.DoParse(text);
        }
    }
}
