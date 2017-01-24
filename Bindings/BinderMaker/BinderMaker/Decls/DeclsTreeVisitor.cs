using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace BinderMaker.Decls
{
    abstract class DeclsTreeVisitor
    {
        public void Run(List<ModuleDecl> modules)
        {
            foreach (var m in modules)
            {
                OnModule(m);
                foreach (var c in m.Classes)
                {
                    OnClass(c);
                    foreach (var f in c.FuncDecls)
                    {
                        OnFunc(f);
                        foreach (var p in f.Params)
                        {
                            OnParam(p);
                        }
                    }
                }
            }
        }

        protected abstract void OnModule(ModuleDecl m);

        protected abstract void OnClass(ClassDecl c);

        protected abstract void OnFunc(FuncDecl f);

        protected abstract void OnParam(ParamDecl p);
    }
}
