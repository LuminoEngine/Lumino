#! ruby -EWindows-31J
# -*- mode:ruby; coding:Windows-31J -*-
require './Lumino'
include Lumino

#p "sssssssssssssssssssssssssssssssssssssssssssssssss"
#p "にほんご"

#raise "あああ"

#Config.register_archive("audio.lna", "test")
Application.initialize_audio
sound1 = Sound.new("audio/ZIGG-ZAGG.mp3")
sound1.pitch = 105
sound1.play


gets

Application.finalize

p Result::OUT_OF_MEMORY

vec1 = Vector3.new
vec2 = Vector3.new(1, 2, 3)

p vec1.x
p vec2.y
vec2.z = 100
p vec2.z


vec3 = Vector3.normalize(vec2)
p vec3.x, vec3.y, vec3.z

p vec2.x, vec2.y, vec2.z
vec2.normalize
p vec2.x, vec2.y, vec2.z
