using System;
using System.IO;

namespace LuminoBindingsBuild
{
	class MainClass
	{
		public static void Main (string[] args)
		{

			var builder = new LuminoBuildTool.Builder();
			builder.RootDir = "../../../";	// .sln のあるフォルダ
			builder.LuminoLibDir = "../../../../lib/";

			var rubyBuilder = new LuminoRubyRule();
            var oldDir = Directory.GetCurrentDirectory();
			rubyBuilder.Build(builder);
            Directory.SetCurrentDirectory(oldDir);
		}
	}
}
