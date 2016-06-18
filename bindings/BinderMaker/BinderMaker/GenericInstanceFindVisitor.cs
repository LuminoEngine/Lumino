using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BinderMaker.Decls;

namespace BinderMaker
{
    class GenericInstanceFindVisitor : Decls.DeclsTreeVisitor
    {
        /// <summary>
        /// インスタンス化するクラス名と、型実引数リストのマップ (LN付き)
        /// </summary>
        public Dictionary<string, List<string>> GenericInstanceMap = new Dictionary<string, List<string>>();

        public List<ClassDecl> AllClasses = new List<ClassDecl>();
        
        protected override void OnModule(ModuleDecl m)
        {
        }

        protected override void OnClass(ClassDecl c)
        {
            AllClasses.Add(c);
        }

        protected override void OnFunc(FuncDecl f)
        {
        }

        protected override void OnParam(ParamDecl p)
        {
            if (p.IsGeneric)
            {
                GenericInstanceMap.Add(p.GenericClassName, p.GenericTypeParams);
            }
        }
    }
}
