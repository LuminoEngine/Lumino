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

            //var box1 = StaticMesh.CreateBox();//new BoxMesh();

            var col1 = new BoxCollisionShape(new Vector3(1, 1, 1));
            var col2 = new BoxCollisionShape(new Vector3(1, 1, 1));
            var body1 = new Collider();
            var body2 = new Collider();
            body1.AddShape(col1);
            body2.AddShape(col2);
            body1.IsTrigger = true;
            body2.IsTrigger = true;
            body1.TriggerEnter += TriggerEnterCallback;
            body2.TriggerEnter += TriggerEnterCallback;

            while (Engine.Update())
            {
            }

            Engine.Terminate();
        }

        static void TriggerEnterCallback(PhysicsObject obj)
        {
            Console.WriteLine();
        }
    }
}
