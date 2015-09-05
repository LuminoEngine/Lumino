using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LN;

namespace Test
{
    class Program
    {
        static void Main(string[] args)
        {
            Application.Initialize();

            var s1 = new Sound(@"D:\Proj\LightNotePackage\Source\Cpp\Sample\Media\ln28ti.ogg");
            s1.Play();

            System.Windows.Forms.MessageBox.Show("test");

            Application.Terminate();


            // <Test> Struct コンストラクタ
            Vector3 v1 = new Vector3(1, 2, 3);

            // <Test> Struct プロパティ (getter)
            float f1 = v1.Length;

            // <Test> Struct メソッド
            // <Test> Struct メソッド オーバーロード
            v1.Normalize();

            // <Test> Struct static メソッド
            Vector3 v2 = Vector3.Normalize(v1);


            Console.WriteLine();
        }
    }
}
