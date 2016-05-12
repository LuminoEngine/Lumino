using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild
{
    abstract class ModuleRule
    {
        public abstract void Build(Builder builder);
    }
}
