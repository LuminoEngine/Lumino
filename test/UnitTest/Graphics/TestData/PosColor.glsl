#ifdef LN_GLSL_VERTEX
attribute vec4 ln_Vertex;
attribute vec4 ln_Color;
varying vec4 vColor;
void main() {
	vColor = ln_Color;
	gl_Position = ln_Vertex;
}
#endif

#ifdef LN_GLSL_FRAGMENT
varying vec4 vColor;
void main()
{
	gl_FragColor = vColor;
}
#endif