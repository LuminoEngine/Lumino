
require "fiddle/import"
require "rbconfig"

$LUMINO_ENGINE_RESOURCES_DIR = __dir__ + "/../Tools/EngineResources"

module LNWin32API
  extend Fiddle::Importer
  dlload "Kernel32.dll"
  extern "int AddDllDirectory(const wchar_t*)"
end


if (RbConfig::CONFIG["host_os"] =~ /mswin|mingw|cygwin/)
  dll_dir = File.join(__dir__, "lumino_ext")
  p dll_dir
  LNWin32API.AddDllDirectory(dll_dir.encode("UTF-16LE"))

  extend Fiddle::Importer
  dlload "LuminoFFI.dll"
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
