using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Lumino;

namespace Test
{
    class Sandbox
    {
        public void Main()
        {
            Engine.Initialize();

            var box1 = new BoxMesh();

            while (Engine.Update())
            {
            }

            Engine.Terminate();
        }
    }
}
