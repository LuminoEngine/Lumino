
require 'fiddle/import'
require 'rbconfig'

$LUMINO_ENGINE_RESOURCES_DIR = File.expand_path("../EngineResources", __FILE__)

extend Fiddle::Importer
if (RbConfig::CONFIG['host_os'] =~ /mswin|mingw|cygwin/)
  dlload __dir__ + "/../ext/LuminoEngine.dll"
else
  raise "Not supported platform."
end

require "lumino/version"
require "Lumino_RubyExt"

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
