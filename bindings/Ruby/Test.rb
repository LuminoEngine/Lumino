require './Lumino'
include Lumino

p Result::OUT_OF_MEMORY

vec1 = Vector3.new
vec2 = Vector3.new(1, 2, 3)

p vec1.x
p vec2.y
vec2.z = 100
p vec2.z

