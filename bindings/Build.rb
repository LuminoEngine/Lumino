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
def xcopyHelper(srcDir, dstDir, entries)
  entries.each { |e|
    src = File.join(srcDir, e)
    dst = File.join(dstDir, e)
    if File.directory?(src)
      mkdir(dst)
      subdir =Dir.entries(src); subdir.delete("."); subdir.delete("..")
      xcopyHelper(src, dst, subdir)
    else
      FileUtils.cp(src, File.join(dstDir, e))
    end
  }
end
def xcopy(srcDir, dstDir, overwrite=false)
  if overwrite#File.directory?(dstDir)
    entries = Dir.entries(srcDir); entries.delete("."); entries.delete("..")
    xcopyHelper(srcDir, dstDir, entries)
  else
    FileUtils.cp_r(srcDir, dstDir)
  end
end
def compress(path)
  path = File.expand_path(path)
  parent = File.expand_path("..", path)
  name = File.basename(path)
  g = ZipFileGenerator.new(parent, name, path + ".zip")
  g.write
end

shfb_install_dir = "D:\\Program Files (x86)\\EWSoftware\\Sandcastle Help File Builder"

binder_root = File.expand_path(".") + "/"
c_api_dir = File.expand_path("C_API/include") + "/"
lib_dir = "../lib/"
c_release_dir = "Release/LuminoC_" + version + "/"
dotnet_release_dir = "Release/LuminoDotNet_" + version + "/"
ruby_release_dir = "Release/LuminoRuby_" + version + "/"
hsp_release_dir = "Release/LuminoHSP_" + version + "/"

ENV["PATH"] = binder_root + "../../DevKit-mingw64-32-4.7.2-20130224-1151-sfx/mingw/bin;" + binder_root + "../../DevKit-mingw64-32-4.7.2-20130224-1151-sfx/bin;" + ENV["PATH"]

#--------------------------------------------------------------
# C
=begin
mkdir(c_release_dir)
copy("Readme.txt", c_release_dir)

c_Include_dir = c_release_dir + "Include/"
mkdir(c_Include_dir)
xcopy(c_api_dir, c_Include_dir)

c_lib_dir = c_release_dir + "Lib/"
mkdir(c_lib_dir)
copy(lib_dir + "LuminoC_x86MT.lib",   c_lib_dir)
copy(lib_dir + "LuminoC_x86MT.dll",   c_lib_dir)
copy(lib_dir + "LuminoC_x86uMT.lib",  c_lib_dir)
copy(lib_dir + "LuminoC_x86uMT.dll",  c_lib_dir)

# help
Dir.chdir("C")
system("doxygen Doxyfile")
xcopy("../../external/Lumino.Core/doc/src/theme", "Help/html", true)  # テーマファイル
Dir.chdir(binder_root)
xcopy("C/Help/html", c_release_dir + "Help/")

# samples
c_samples_dir = c_release_dir + "Samples/"
mkdir(c_samples_dir)
xcopy("C/Samples", c_release_dir)
xcopy("Common/Media", c_samples_dir)

# zip
compress(c_release_dir)

#--------------------------------------------------------------
# DotNet

# クラスライブラリとヘルプをビルドする
Dir.chdir("DotNet")
#system("MSBuild /t:Rebuild /p:Configuration=\"Release\" /p:Platform=\"x86\" /m LuminoDotNet.sln")
#system("MSBuild /p:Configuration=Release /property:SHFBROOT=\"" + shfb_install_dir + "\" LuminoDotNet.shfbproj")
Dir.chdir(binder_root)

mkdir(dotnet_release_dir)
copy("Readme.txt", dotnet_release_dir)

dotnet_output_dir = "DotNet/LuminoDotNet/bin/x86/Release/"
copy(dotnet_output_dir + "LuminoDotNet.dll", dotnet_release_dir)
copy(dotnet_output_dir + "LuminoDotNet.XML", dotnet_release_dir)
copy(lib_dir + "LuminoC_x86uMT.dll",  dotnet_release_dir)

# Help
xcopy("DotNet/Help", dotnet_release_dir)

# samples
dotnet_samples_dir = dotnet_release_dir + "Samples/"
mkdir(dotnet_samples_dir)
xcopy("DotNet/Samples", dotnet_release_dir)
xcopy("Common/Media", dotnet_samples_dir)

# zip
compress(dotnet_release_dir)

=end

#--------------------------------------------------------------
# Ruby

# 拡張ライブラリとヘルプをビルドする
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
=begin
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

=end
