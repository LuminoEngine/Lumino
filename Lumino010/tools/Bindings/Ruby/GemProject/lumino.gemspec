
lib = File.expand_path("../lib", __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require "lumino/version"

Gem::Specification.new do |spec|
  spec.name          = "lumino"
  spec.version       = Lumino::VERSION
  spec.authors       = ["lriki"]
  spec.email         = ["lriki.net@gmail.com"]

  spec.summary       = %q{Real-time graphics application framework.}
  spec.description   = %q{Real-time graphics application framework.}
  spec.homepage      = "https://github.com/LuminoEngine/Lumino"
  spec.license       = "MIT"
  spec.extensions    = %w[ext/extconf.rb]

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  #spec.files         = Dir.chdir(File.expand_path('..', __FILE__)) do
  #  `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  #end
  spec.files = Dir['{bin/*,ext/**/*,lib/**/*,test/**/*,Tools/**/*}']
  #spec.files.push('exe/lumino-rb.exe')  # see "lumino-rb file" command

  spec.bindir        = "exe"
  spec.executables   = ['lumino-rb']#spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["ext", "lib"]
  #spec.test_files    = spec.files.grep(%r{^(test|test_old|sample|testdata)/})

  #spec.add_development_dependency "bundler", "~> 2.1.4"
  #spec.add_development_dependency "rake", "~> 13.0"
  spec.add_dependency "thor"  # for CLI-tool
end
