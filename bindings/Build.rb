# このスクリプトは VisualStudio コマンドプロンプト上で実行する必要があります。

require 'fileutils'
require './Ruby/ZipFileGenerator.rb'  # gem 'rubyzip'

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
def compress(path)
  path = File.expand_path(path)
  parent = File.expand_path("..", path)
  name = File.basename(path)
  g = ZipFileGenerator.new(parent, name, path + ".zip")
  g.write
end



binder_root = File.expand_path(".") + "/"
c_api_dir = File.expand_path("src/C_API") + "/"
lib_dir = "../lib/"
c_release_dir = "Release/LuminoC_" + version + "/"
dotnet_release_dir = "Release/LuminoDotNet_" + version + "/"
ruby_release_dir = "Release/LuminoRuby_" + version + "/"
hsp_release_dir = "Release/LuminoHSP_" + version + "/"

ENV["PATH"] = binder_root + "../../DevKit-mingw64-32-4.7.2-20130224-1151-sfx/mingw/bin;" + binder_root + "../../DevKit-mingw64-32-4.7.2-20130224-1151-sfx/bin;" + ENV["PATH"]

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

# samples
c_samples_dir = c_release_dir + "Samples/"
mkdir(c_samples_dir)
xcopy("C/Samples", c_release_dir)
xcopy("Common/Media", c_samples_dir)

# zip
compress(c_release_dir)

#--------------------------------------------------------------
# DotNet
system("MSBuild ./DotNet/LuminoDotNet.sln /t:Rebuild /p:Configuration=\"Release\" /p:Platform=\"x86\" /m")

mkdir(dotnet_release_dir)
copy("Readme.txt", dotnet_release_dir)

dotnet_output_dir = "DotNet/LuminoDotNet/bin/x86/Release/"
copy(dotnet_output_dir + "LuminoDotNet.dll", dotnet_release_dir)
copy(dotnet_output_dir + "LuminoDotNet.XML", dotnet_release_dir)
copy(lib_dir + "LuminoC_x86uMT.dll",  dotnet_release_dir)

# samples
dotnet_samples_dir = dotnet_release_dir + "Samples/"
mkdir(dotnet_samples_dir)
xcopy("DotNet/Samples", dotnet_release_dir)
xcopy("Common/Media", dotnet_samples_dir)

# zip
compress(dotnet_release_dir)

#--------------------------------------------------------------
# Ruby

# 拡張ライブラリとリファレンスをビルドする
copy(lib_dir + "LuminoC_x86MT.dll",  "Ruby")
Dir.chdir("Ruby")
system("ruby extconf.rb")
system("make")
system("yardoc -o Help LuminoRubyDoc.rb")
Dir.chdir(binder_root)

# root
mkdir(ruby_release_dir)
copy("Readme.txt", ruby_release_dir)
copy("Ruby/Lumino.so", ruby_release_dir)
copy("Ruby/LuminoC_x86MT.dll", ruby_release_dir)

# Help
xcopy("Ruby/Help", ruby_release_dir)

# samples
ruby_samples_dir = ruby_release_dir + "Samples/"
mkdir(ruby_samples_dir)
xcopy("Ruby/Samples", ruby_release_dir)
xcopy("Common/Media", ruby_samples_dir)

# zip
compress(ruby_release_dir)

#--------------------------------------------------------------
# HSP
system("MSBuild ./HSP/LuminoHSP/LuminoHSP.sln /t:Build /p:Configuration=\"Release\" /p:Platform=\"Win32\" /m")

mkdir(hsp_release_dir)
copy("Readme.txt", hsp_release_dir)

# lib
copy("HSP/LuminoHSP.dll", hsp_release_dir)
copy("HSP/lumino.as", hsp_release_dir)
copy("HSP/lumino.hs", hsp_release_dir)
copy("HSP/CommandList.txt", hsp_release_dir)

# samples
hsp_samples_dir = hsp_release_dir + "Samples/"
mkdir(hsp_samples_dir)
xcopy("HSP/Samples", hsp_release_dir)
xcopy("Common/Media", hsp_samples_dir)

# zip
compress(hsp_release_dir)

