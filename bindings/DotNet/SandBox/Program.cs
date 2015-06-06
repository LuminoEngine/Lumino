using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using LNote;

namespace SandBox
{

    class Program
    {

        static void Main(string[] args)
        {
            LNote.API.LNApplication_Initialize();
            LNote.CoreObjectManager.Initialize();

            var pane = GUIRootPane.DefaultRootPane;
            var button1 = new GUIButton();
            pane.Content = button1;

            bool endRequested = false;
            do
            {
                LNote.API.LNApplication_Update();
                LNote.API.LNApplication_IsEndRequested(out endRequested);
            } while (!endRequested);

            LNote.API.LNApplication_Terminate();
        }
    }
}
