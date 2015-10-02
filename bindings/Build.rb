# このスクリプトは VisualStudio コマンドプロンプト上で実行する必要があります。

require 'fileutils'

version = "0.1.0"

def mkdir(path)
  FileUtils.mkdir_p(path)
end
def copy(filePath, dstDir)
  FileUtils.cp(filePath, dstDir)
end
def xcopy(srcDir, dstDir)
  FileUtils.cp_r(srcDir, dstDir)
end

c_api_dir = "src/C_API/"
lib_dir = "../lib/"
c_release_dir = "Release/LuminoC_" + version + "/"
dotnet_release_dir = "Release/LuminoDotNet_" + version + "/"
ruby_release_dir = "Release/LuminoRuby_" + version + "/"
hsp_release_dir = "Release/LuminoHSP_" + version + "/"

#--------------------------------------------------------------
# C
mkdir(c_release_dir)
copy("Readme.txt", c_release_dir)

c_Include_dir = c_release_dir + "Include/"
mkdir(c_Include_dir)
copy(c_api_dir + "LuminoC.h",       c_Include_dir)
copy(c_api_dir + "LNApplication.h", c_Include_dir)
copy(c_api_dir + "LNAudio.h",       c_Include_dir)
copy(c_api_dir + "LNBase.h",        c_Include_dir)
copy(c_api_dir + "LNCommon.h",      c_Include_dir)
copy(c_api_dir + "LNMath.h",        c_Include_dir)
copy(c_api_dir + "LNTypedef.h",     c_Include_dir)

c_lib_dir = c_release_dir + "Lib/"
mkdir(c_lib_dir)
copy(lib_dir + "LuminoC_x86MT.lib",   c_lib_dir)
copy(lib_dir + "LuminoC_x86MT.dll",   c_lib_dir)
copy(lib_dir + "LuminoC_x86uMT.lib",  c_lib_dir)
copy(lib_dir + "LuminoC_x86uMT.dll",  c_lib_dir)

c_samples_dir = c_release_dir + "Samples/"
mkdir(c_samples_dir)
xcopy("C/Samples", c_samples_dir)
xcopy("Common/Media", c_samples_dir)

#--------------------------------------------------------------
# DotNet
system("MSBuild ./DotNet/LuminoDotNet.sln /t:Rebuild /p:Configuration=\"Release\" /p:Platform=\"x86\" /m")

mkdir(dotnet_release_dir)
copy("Readme.txt", dotnet_release_dir)

dotnet_output_dir = "DotNet/LuminoDotNet/bin/x86/Release/"
copy(dotnet_output_dir + "LuminoDotNet.dll", dotnet_release_dir)
copy(dotnet_output_dir + "LuminoDotNet.XML", dotnet_release_dir)
copy(lib_dir + "LuminoC_x86uMT.dll",  dotnet_release_dir)

dotnet_samples_dir = dotnet_release_dir + "Samples/"
mkdir(dotnet_samples_dir)
xcopy("DotNet/Samples", dotnet_release_dir)
xcopy("Common/Media", dotnet_samples_dir)

#--------------------------------------------------------------
# Ruby
mkdir(ruby_release_dir)
copy("Readme.txt", ruby_release_dir)

#--------------------------------------------------------------
# HSP
mkdir(hsp_release_dir)
copy("Readme.txt", hsp_release_dir)

system("MSBuild ./HSP/LuminoHSP/LuminoHSP.sln /t:Rebuild /p:Configuration=\"Release\" /p:Platform=\"Win32\" /m")

