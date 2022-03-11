
require "fiddle/import"
require "rbconfig"

$LUMINO_ENGINE_RESOURCES_DIR = __dir__ + "/../tools/EngineResources"


if (RbConfig::CONFIG["host_os"] =~ /mswin|mingw|cygwin/)
  # 拡張ライブラリの .so を require すると、Ruby ランタイムはその .so が依存している DLL もロードしようとするが、
  # .so と同じフォルダに保存しているだけではロードしてくれない。
  # "LoadError 126: Module not found." が発生する。
  # DLL のある場所へロードパスを通す必要がある。
  module LNWin32API
    extend Fiddle::Importer
    dlload "Kernel32.dll"
    extern "int AddDllDirectory(const wchar_t*)"
  end
  dll_dir = File.join(__dir__, "/../ext/lumino_ext")
  LNWin32API.AddDllDirectory(dll_dir.encode("UTF-16LE"))
  
  # 明示的に現在プロセスに対して DLL をロードしておく。
  # 上記の通り、Ruby ランタイムは .so が別の DLL に依存している場合ロードしようとするが、
  # エラーチェックはしてくれるものの実際にはロードしてくれない。(エラーチェックといっても Win32 の例外をそのまま print してるだけのようだが)
  # 実行はできるが、 export した関数を呼び出すときにクラッシュする。
  module LuminoFFIDll
    extend Fiddle::Importer
    dlload "LuminoFFI.dll"
  end
else
  raise "Not supported platform."
end

require "lumino/version"
require __dir__ + "/lumino_ext/lumino_ext.so"

current_assets_dir = File.expand_path("assets")
if File.exist?(current_assets_dir)
  Lumino::EngineSettings.add_asset_directory(current_assets_dir)
end

module Lumino
  #class Error < StandardError; end

  class Application
    def run
      Lumino::Application.run_app_internal(self)
    end
  end

  #class Sprite
  #  def self.inherited(subclass)
  #    puts "Sprite self.inherited"
  #    Lumino::register_type(subclass)
  #  end
  #end
end

unless $LUMINO_DISABLE_INCLUDE
  include Lumino
end
