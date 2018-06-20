// simple.frag

uniform vec4 g_color;

void main (void)
{
  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0) + g_color;
}
