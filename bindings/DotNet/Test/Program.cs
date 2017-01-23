using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Lumino;

namespace Test
{
    class Program
    {
#if true
        //--------------------------------------------------------------
        static void Test_Struct()
        {
            // デフォルトコンストラクタ
            var vec1 = new Vector3();
            AssertEq(0.0f, vec1.X);
            AssertEq(0.0f, vec1.Y);
            AssertEq(0.0f, vec1.Z);

            // フィールド初期値指定のコンストラクタ
            var vec2 = new Vector3(1, 2, 3);
            AssertEq(1, vec2.X);
            AssertEq(2, vec2.Y);
            AssertEq(3, vec2.Z);

            // フィールドへの代入
            vec2.Z = 100;
            AssertEq(100, vec2.Z);

            // インスタンスメソッドの呼び出し
            vec2.Normalize();
            AssertEq(true, vec2.X > 0.009);
            AssertEq(true, vec2.Y > 0.01);
            AssertEq(true, vec2.Z > 0.999);

            // クラスメソッドの呼び出し（インスタンスメソッドとのオーバーライド）
            var vec3 = Vector3.Normalize(vec2);
            AssertEq(true, vec3.X > 0.009);
            AssertEq(true, vec3.Y > 0.01);
            AssertEq(true, vec3.Z > 0.999);
        }

        //--------------------------------------------------------------
        static void Test_RefObjectConstructorOverload()
        {
            var tex1 = new Texture2D(32, 32);

            // 実行エラーになら名ければOK
            var spr1 = new Sprite2D();
            var spr2 = new Sprite2D(tex1);
        }

        //--------------------------------------------------------------
        static void Test_RefObjectGetSetValue()
        {
            var spr1 = new Sprite2D();

            // get
            AssertEq(true, spr1.IsVisible);

            // set
            spr1.IsVisible = false;
            AssertEq(false, spr1.IsVisible);
        }

        //--------------------------------------------------------------
        static void Test_RefObjectGetSetStruct()
        {
            var spr1 = new Sprite2D();

            // get
            var pos = spr1.Position;
            AssertEq(0.0, pos.X);
            AssertEq(0.0, pos.Y);
            AssertEq(0.0, pos.Z);

            // set
            spr1.Position = new Vector3(1.0f, 2.0f, 3.0f);
            AssertEq(1.0f, spr1.Position.X);
            AssertEq(2.0f, spr1.Position.Y);
            AssertEq(3.0f, spr1.Position.Z);
        }

        //--------------------------------------------------------------
        static void Test_RefObjectGetSetRefObject()
        {
            var tex1 = new Texture2D(32, 32);
            var spr1 = new Sprite2D();

            // set したもとの get で取得したものは同一
            spr1.Texture = tex1;
            AssertEq(tex1, spr1.Texture);

            // null にしてみる
            spr1.Texture = null;
            AssertEq(null, spr1.Texture);

            // コンストラクタにセットしたものと同一
            var spr2 = new Sprite2D(tex1);
            AssertEq(tex1, spr2.Texture);
        }

        //--------------------------------------------------------------
        // 明示的破棄のテスト
        static void Test_Dispose()
        {
            int c = Diag.GetHandleCount();

            // 1つ作ると Handle が1つ増える
            var tex1 = new Texture2D(32, 32);
            AssertEq(c + 1, Diag.GetHandleCount());

            // 明示的に Dispose するとHandle が1つ減る
            tex1.Dispose();
            AssertEq(c, Diag.GetHandleCount());
        }

        //--------------------------------------------------------------
        // オブジェクトリストのテスト
        static void Test_ObjectList()
        {
            var tex1 = new Texture2D(32, 32);
            var spr1 = new Sprite2D(tex1);
            var spr2 = new Sprite2D(tex1);
            var spr3 = new Sprite2D(tex1);
            var children = spr1.Children;

            // とりあえず機能を呼び出してみる
            AssertEq(0, children.Count);

            // 要素を追加する
            children.Add(spr2);
            
            // 要素数が増えている
            AssertEq(1, children.Count);

            // 要素を get してみる
            AssertEq(spr2, children[0]);

            // 要素を set してみる
            children[0] = spr3;

            // 要素を get してみる
            AssertEq(spr3, children[0]);

            // 要素を削除してみる
            children.RemoveAt(0);
            AssertEq(0, children.Count);

            // foreach
            children.Add(spr2);
            children.Add(spr3);
            int i = 0;
            foreach (var item in children)
            {
                if (i == 0) AssertEq(spr2, item);
                if (i == 1) AssertEq(spr3, item);
                i++;
            }

            // 別の変数に取り出してみる
            var children2 = spr1.Children;
            AssertEq(children, children2);
        }

        //--------------------------------------------------------------
        // デフォルトで作成されるオブジェクトリストのテスト
        static void Test_DefaultObjectList()
        {
            var vp1 = Viewport.MainViewport;
            var list1 = vp1.Layers;

            // 最初から何個か入っている
            AssertEq(true, list1.Count > 0);

            // 要素を get してみる
            var l1 = list1[0];

            // もう一度 get してみる
            AssertEq(l1, list1[0]);
        }

        static void Main(string[] args)
        {
            Engine.Initialize();
            Test_Struct();
            Test_RefObjectConstructorOverload();
            Test_RefObjectGetSetValue();
            Test_RefObjectGetSetStruct();
            Test_RefObjectGetSetRefObject();
            Test_Dispose();
            Test_ObjectList();
            Test_DefaultObjectList();
            Engine.Terminate();
            Console.WriteLine("Test succeeded.");
        }

        static void AssertEq<T>(T expected, T actual)
        {
            if (expected == null)
            {
                if (actual != null)
                    throw new InvalidOperationException();
            }
            else
            {
                if (!expected.Equals(actual))
                    throw new InvalidOperationException();
            }
        }
#endif
#if false

        class MyScene : GameScene
        {
            protected override void OnStart()
            {
                base.OnStart();
                Console.WriteLine("OnStart");
                var s1 = new Sound(@"D:\tmp\light_song_instrumental_0.mp3");
                s1.Pitch = 1.1f;
                s1.Play();
            }
        }

        static void Main(string[] args)
        {
            Engine.Initialize();

            //GameAudio.PlayBGM("D:/tmp/GrandSky.mp3");

            //var tex1 = new Texture2D(TestDataFolder + "img1_BYTE_R8G8B8A8_20x20.png");
            //Console.WriteLine(tex1.Size.Width);

            //var sp1 = new Sprite2D(tex1);

            //while (Engine.Update())
            //{
            //}
            var app = new GameApplication();
            app.Run(new MyScene());

            Engine.Terminate();

#if false
            var s1 = new Sound(@"D:\tmp\light_song_instrumental_0.mp3");
            s1.Pitch = 105;
            Console.WriteLine(s1.Pitch);
            Console.WriteLine(s1.Volume);
            s1.Is3DEnabled = true;
            s1.EmitterPosition = new Vector3(10, 0, 0);
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
#endif

        }
#endif
    }
}
