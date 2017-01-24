#ifdef LN_GLSL_VERTEX
attribute vec4 ln_Vertex;
attribute vec2 ln_MultiTexCoord;
varying vec2 vTexUV;
void main() {
	vTexUV = ln_MultiTexCoord;
	gl_Position = ln_Vertex;
}
#endif

#ifdef LN_GLSL_FRAGMENT
uniform sampler2D g_Texture;
varying vec2 vTexUV;
void main()
{
	vec4 color = texture2D(g_Texture, vTexUV);
	gl_FragColor = color;
}
#endif

