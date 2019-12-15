
require 'fiddle/import'
require 'rbconfig'

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
end
