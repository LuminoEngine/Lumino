require './Lumino'
include Lumino

Application.initialize_audio
sound1 = Sound.new("D:/tmp/ZIGG-ZAGG.mp3")
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
