=begin
EUC-JP の補助文字部分のテーブルを作成する。
マッピングは以下のファイル。
ftp://ftp.unicode.org/Public/MAPPINGS/OBSOLETE/EASTASIA/JIS/JIS0212.TXT
=end

$output = ""

$table = Array.new(65536)

# EUC → Unicode
for i in 0...$table.size
  $table[i] = "0x0000"
end
$output += "// EUC-JP EXT -> UTF16\n"
$output += "const unsigned short g_EUCJPEXTToUTF16Table[65536] = {\n"
File.readlines("JIS0212.TXT").each do |line|
  if line[0] != '#'
    cols = line.split(" ")
    $table[cols[0].hex] = cols[1]
  end
end

for i in 0...$table.size
  $output += $table[i] + ", "
  $output += "\n" if i % 16 == 15
end
$output += "};\n\n"


# EUC → Unicode
for i in 0...$table.size
  $table[i] = "0x0000"
end
$output += "// UTF16 -> EUC-JP EXT\n"
$output += "const unsigned short g_UTF16ToEUCJPEXTTable[65536] = {\n"
File.readlines("JIS0212.TXT").each do |line|
  if line[0] != '#'
    cols = line.split(" ")
    $table[cols[1].hex] = cols[0]
  end
end

for i in 0...$table.size
  $output += $table[i] + ", "
  $output += "\n" if i % 16 == 15
end
$output += "};\n\n"
    
open("ConvertTable_EUCJP_EXT.c", "w") {|f| f.write($output) }

