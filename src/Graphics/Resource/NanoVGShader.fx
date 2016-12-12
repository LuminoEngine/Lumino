

//==============================================================================
#ifdef LN_HLSL_DX9

#define EDGE_AA

static const int NANOVG_GL_UNIFORMARRAY_SIZE = 11;

float2 viewSize;

struct VSInput
{
	float2	vertex	: POSITION;
	float2	tcoord	: TEXCOORD0;
};

struct VSOutput
{
	float4	Pos		: POSITION;
	float2	ftcoord	: TEXCOORD0;
	float2	fpos	: TEXCOORD1;
};

struct PSInput
{
	float2	ftcoord	: TEXCOORD0;
	float2	fpos	: TEXCOORD1;
};

//------------------------------------------------------------------------------
VSOutput VSBasic(VSInput v)
{
	VSOutput o;
	o.ftcoord = v.tcoord;
	o.fpos = v.vertex;
	// スクリーン空間へ
	o.Pos = float4(2.0*v.vertex.x/viewSize.x - 1.0, 1.0 - 2.0*v.vertex.y/viewSize.y, 0, 1);
	return o;
}




float4 frag[NANOVG_GL_UNIFORMARRAY_SIZE];
texture tex;
//uniform sampler2D tex;
//varying vec2 ftcoord;	// ストロークの濃さ。AntiAlias のときに使う
//varying vec2 fpos;

#define scissorMat float3x3(frag[0].xyz, frag[1].xyz, frag[2].xyz)
#define paintMat float3x3(frag[3].xyz, frag[4].xyz, frag[5].xyz)	// glnvg__convertPaint() で作る
#define innerCol frag[6]										// InnerColor。glnvg__convertPaint() で作る
#define outerCol frag[7]										// OuterColor。glnvg__convertPaint() で作る
#define scissorExt frag[8].xy
#define scissorScale frag[8].zw
#define extent frag[9].xy
#define radius frag[9].z										// nvgRadialGradientのパラメータ
#define feather frag[9].w										// 各 Gradient のパラメータ
#define strokeMult frag[10].x
#define strokeThr frag[10].y
#define texType int(frag[10].z)		// 1:RGBA 2:A
#define type int(frag[10].w)


sampler		textureSampler = sampler_state
{
	texture = <tex>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

float sdroundrect(float2 pt, float2 ext, float rad)
{
	float2 ext2 = ext - float2(rad,rad);
	float2 d = abs(pt) - ext2;
	return min(max(d.x,d.y),0.0) + length(max(d,0.0)) - rad;
}
float scissorMask(float2 p)
{
	//float2 sc = (abs((scissorMat * float3(p,1.0)).xy) - scissorExt);
	float2 sc = (abs((  mul(float3(p,1.0),scissorMat)   ).xy) - scissorExt);
	sc = float2(0.5,0.5) - sc * scissorScale;
	return clamp(sc.x,0.0,1.0) * clamp(sc.y,0.0,1.0);
}
float strokeMask(float2 ftcoord)
{
	//return min(1.0, (1.0-abs(ftcoord.x*2.0-1.0))*strokeMult) * min(1.0, ftcoord.y);
	return min(1.0, mul(strokeMult,(1.0-abs(ftcoord.x*2.0-1.0)))    ) * min(1.0, ftcoord.y);
}
//------------------------------------------------------------------------------
float4 PSBasic(PSInput p) : COLOR0
{
	float4 result;
	float scissor = scissorMask(p.fpos);
#ifdef EDGE_AA
	float strokeAlpha = strokeMask(p.ftcoord);
#else
	float strokeAlpha = 1.0;
#endif
	if (type == 0) {			// Gradient	NSVG_SHADER_FILLGRAD
		// Calculate gradient color using box gradient
		//float2 pt = (   paintMat * float3(p.fpos,1.0)   ).xy;
		float2 pt = (   mul(float3(p.fpos,1.0), paintMat)   ).xy;
		float d = clamp((sdroundrect(pt, extent, radius) + feather*0.5) / feather, 0.0, 1.0);
		float4 color = lerp(innerCol,outerCol,d);
		// Combine alpha
		color *= strokeAlpha * scissor;
		result = color;
	} else if (type == 1) {		// Image	NSVG_SHADER_FILLIMG
		// Calculate color fron texture
		//float2 pt = (paintMat * float3(p.fpos,1.0)).xy / extent;
		float2 pt = (    mul(float3(p.fpos,1.0), paintMat)   ).xy / extent;
		float4 color = tex2D(textureSampler, pt);
		
		// A8 フォーマットは使わない
		color = float4(color.xyz*color.w,color.w);
		//if (texType == 1) color = float4(color.xyz*color.w,color.w);
		//if (texType == 2) color = float4(0, 0, 0, color.x);
		
		// Apply color tint and alpha.
		color *= innerCol;
		// Combine alpha
		color *= strokeAlpha * scissor;
		result = color;
	} else if (type == 2) {		// Stencil fill
		result = float4(1,1,1,1);
	} else if (type == 3) {		// Textured tris	・・・glnvg__renderTriangles < nvg__renderText ・・このとき ftcoord は普通にテクスチャUVをあらわす
		result = float4(0, 0, 0, 1);
	}
//#ifdef EDGE_AA
//	if (strokeAlpha < strokeThr) discard;
//#endif
	
	return result;
}

//------------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader = compile vs_3_0 VSBasic();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
}


#endif	// LN_HLSL_DX9


//=============================================================================
#ifdef LN_GLSL_VERTEX_Main


//#version 120

	uniform vec2 viewSize;
	
	attribute vec2	ln_Vertex;
	attribute vec2	ln_MultiTexCoord0;
	
	varying vec2 ftcoord;
	varying vec2 fpos;

void main(void) {
	ftcoord = ln_MultiTexCoord0;
	fpos = ln_Vertex;
	gl_Position = vec4(2.0*ln_Vertex.x/viewSize.x - 1.0, 1.0 - 2.0*ln_Vertex.y/viewSize.y, 0, 1);
}

#endif /* LN_GLSL_VERTEX_Main */

//=============================================================================
#ifdef LN_GLSL_FRAGMENT_Main

#define UNIFORMARRAY_SIZE 11

	uniform vec4 frag[UNIFORMARRAY_SIZE];
	uniform sampler2D tex;
	varying vec2 ftcoord;
	varying vec2 fpos;
	
//#ifndef USE_UNIFORMBUFFER
	#define scissorMat mat3(frag[0].xyz, frag[1].xyz, frag[2].xyz)
	#define paintMat mat3(frag[3].xyz, frag[4].xyz, frag[5].xyz)
	#define innerCol frag[6]
	#define outerCol frag[7]
	#define scissorExt frag[8].xy
	#define scissorScale frag[8].zw
	#define extent frag[9].xy
	#define radius frag[9].z
	#define feather frag[9].w
	#define strokeMult frag[10].x
	#define strokeThr frag[10].y
	#define texType int(frag[10].z)
	#define type int(frag[10].w)
//#endif

float sdroundrect(vec2 pt, vec2 ext, float rad) {
	vec2 ext2 = ext - vec2(rad,rad);
	vec2 d = abs(pt) - ext2;
	return min(max(d.x,d.y),0.0) + length(max(d,0.0)) - rad;
}

// Scissoring
float scissorMask(vec2 p) {
	vec2 sc = (abs((scissorMat * vec3(p,1.0)).xy) - scissorExt);
	sc = vec2(0.5,0.5) - sc * scissorScale;
	return clamp(sc.x,0.0,1.0) * clamp(sc.y,0.0,1.0);
}
#ifdef EDGE_AA
// Stroke - from [0..1] to clipped pyramid, where the slope is 1px.
float strokeMask() {
	return min(1.0, (1.0-abs(ftcoord.x*2.0-1.0))*strokeMult) * min(1.0, ftcoord.y);
}
#endif

void main(void) {
   vec4 result;
	float scissor = scissorMask(fpos);
#ifdef EDGE_AA
	float strokeAlpha = strokeMask();
#else
	float strokeAlpha = 1.0;
#endif
	if (type == 0) {			// Gradient
		// Calculate gradient color using box gradient
		vec2 pt = (paintMat * vec3(fpos,1.0)).xy;
		float d = clamp((sdroundrect(pt, extent, radius) + feather*0.5) / feather, 0.0, 1.0);
		vec4 color = mix(innerCol,outerCol,d);
		// Combine alpha
		color *= strokeAlpha * scissor;
		result = color;
	} else if (type == 1) {		// Image
		// Calculate color fron texture
		vec2 pt = (paintMat * vec3(fpos,1.0)).xy / extent;
		
		vec4 color = texture2D(tex, pt);
		
		if (texType == 1) color = vec4(color.xyz*color.w,color.w);
		if (texType == 2) color = vec4(color.x);
		// Apply color tint and alpha.
		color *= innerCol;
		// Combine alpha
		color *= strokeAlpha * scissor;
		result = color;
	} else if (type == 2) {		// Stencil fill
		result = vec4(1,1,1,1);
	} else if (type == 3) {		// Textured tris
		
		vec4 color = texture2D(tex, ftcoord);
		
		if (texType == 1) color = vec4(color.xyz*color.w,color.w);
		if (texType == 2) color = vec4(color.x);
		color *= scissor;
		result = color * innerCol;
	}
#ifdef EDGE_AA
	if (strokeAlpha < strokeThr) discard;
#endif
	
	gl_FragColor = result;
}

#endif /* LN_GLSL_FRAGMENT_Main */


