/*
 *  音声ファイルをループ再生します。
 */
using System;
using Lumino;

namespace PlaySound
{
    class Program
    {
        [STAThread] // for OpenFileDialog
        static void Main(string[] args)
        {
            // ファイルを開く
            var dlg = new System.Windows.Forms.OpenFileDialog();
            if (dlg.ShowDialog() != System.Windows.Forms.DialogResult.OK) return;
            
            // 音声機能を初期化する
            Application.InitializeAudio();

            // 音声ファイルから Sound オブジェクトを作る
            var sound = new Sound(dlg.FileName);
            sound.Volume = 1.0f;			// 音量
            sound.Pitch = 1.0f;				// ピッチ
            sound.IsLoopEnabled = true;	    // ループON

            // 再生
            sound.Play();

            // Enter 待ち
            Console.ReadKey();

            // 停止
            sound.Stop();

            // ライブラリの終了処理
            Application.Terminate();
        }
    }
}
