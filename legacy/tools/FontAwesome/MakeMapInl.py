import json

text = open('icons.json', 'r')
data = json.load(text)

for glyph_key in data:
    glyph_info = data[glyph_key]
    print('{"' + glyph_key + '", 0x' + glyph_info['unicode'] + '},')


