using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using BinderMaker.Decls;

namespace BinderMaker
{
    class GenericInstanceInfo
    {
        public string GenericClassName;
        public List<string> TypeArgs;
    }

    class GenericInstanceFindVisitor : Decls.DeclsTreeVisitor
    {
        /// <summary>
        /// インスタンス化するクラス名と、型実引数リストのマップ (LN付き)
        /// </summary>
        public List<GenericInstanceInfo> GenericInstanceInfoList = new List<GenericInstanceInfo>();

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
            if (p.IsGenericInstance)
            {
                GenericInstanceInfoList.Add(
                    new GenericInstanceInfo()
                    {
                        GenericClassName = p.GenericClassName,
                        TypeArgs = p.GenericTypeArgs,
                    });
            }
        }
    }
}
