using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Threading.Tasks;

namespace LuminoBuild
{
    class Program
    {

        static void Main(string[] args)
        {
            var builder = new Builder("../../../../");
            builder.BuildEngine();
            return;
            var cppRule = new CppRule();
            cppRule.Build(builder);
        }
    }
}
