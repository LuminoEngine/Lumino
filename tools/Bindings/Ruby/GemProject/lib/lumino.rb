
require 'fiddle/import'
require 'rbconfig'

$LUMINO_ENGINE_RESOURCES_DIR = __dir__ + "/../Tools/EngineResources"

extend Fiddle::Importer
if (RbConfig::CONFIG['host_os'] =~ /mswin|mingw|cygwin/)
  dlload __dir__ + "/../ext/LuminoEngine.dll"
else
  raise "Not supported platform."
end

require "lumino/version"
require "Lumino_RubyExt"

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
  #    puts 'Sprite self.inherited'
  #    Lumino::register_type(subclass)
  #  end
  #end
end

unless $LUMINO_DISABLE_INCLUDE
  include Lumino
end
