

binary = File.binread(ARGV[0])

output = ""
for i in 0...binary.size
  v = binary[i].ord
  output += format("0x%02x, ", v)
  output += "\n" if (i > 0) and (i % 16 == 15)
end

open(ARGV[0] + ".inl", "w") { |file| file.write(output) }

