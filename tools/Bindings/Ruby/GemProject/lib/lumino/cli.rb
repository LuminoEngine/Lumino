require "thor"

module LuminoCLI
  class CLI < Thor
    desc "test {snake_case_string}", "test {snake_case_string} to {camelCaseString}"
    def test #(str)
      p "Lumino cli test."
    end
  end
end
