using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using LN = Lumino;

namespace GameAudio
{
    /// <summary>
    /// MainWindow.xaml の相互作用ロジック
    /// </summary>
    public partial class MainWindow : Window
    {
        const string BGMFile = "../../../Media/ln21.mid";
        const string BGSFile = "../../../Media/water02.wav";
        const string MEFile = "../../../Media/lnme_victory1.ogg";
        const string SEFile = "../../../Media/ln_cursor_1.wav";

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            IntPtr handle = new WindowInteropHelper(this).Handle;
            LN.Config.SetUserWindowHandle(handle);
            LN.Config.SetDirectMusicMode(LN.DirectMusicMode.Normal);
            LN.Application.InitializeAudio();
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            LN.Application.Terminate();
        }

        private void Button_Click_PlayBGM(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.PlayBGM(BGMFile);
        }

        private void Button_Click_StopBGM(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.StopBGM();
        }

        private void Button_Click_FadeoutBGM(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.StopBGM(5.0);
        }

        private void Button_Click_PlayBGS(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.PlayBGS(BGSFile);
        }

        private void Button_Click_StopBGS(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.StopBGS();
        }

        private void Button_Click_FadeoutBGS(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.StopBGS(5.0);
        }

        private void Button_Click_PlayME(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.PlayME(MEFile);
        }

        private void Button_Click_StopME(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.StopME();
        }

        private void Button_Click_PlaySE(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.PlaySE(SEFile);
        }

        private void Button_Click_StopSE(object sender, RoutedEventArgs e)
        {
            LN.GameAudio.StopSE();
        }
    }
}
