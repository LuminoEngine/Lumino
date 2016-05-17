using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using Lumino;

namespace UnitTest
{
    [TestClass]
    public class UnitTest1
    {
        [ClassInitialize()]
        public static void ClassInitialize(TestContext context)
        {
            Application.Initialize();
        }

        [ClassCleanup()]
        public static void ClassCleanup()
        {
            Application.Terminate();
        }

        [TestMethod]
        public void TestMethod1()
        {
            var tex1 = new Texture2D(TestDataFolder + "img1_BYTE_R8G8B8A8_20x20.png");
            var sprite = new Sprite2D();

            Application.UpdateFrame();
        }
    }
}
