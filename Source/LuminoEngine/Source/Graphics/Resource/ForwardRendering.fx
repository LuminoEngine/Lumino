
//==============================================================================
#ifdef LN_HLSL_DX9

struct LN_VS_INPUT
{
	float3	Pos		: POSITION;		// 位置
	float2	Tex		: TEXCOORD0;	// テクスチャ座標
	float3	Normal	: NORMAL0;//TEXCOORD1;	// 法線
	float4	Color	: COLOR0;		// 頂点色
};


// オブジェクトのテクスチャ
sampler		ObjTexSampler		= sampler_state
{
	texture = <ln_MaterialTexture>;
};

struct VS_OUTPUT
{
	float4	Pos		: POSITION;		// 位置
	float2	Tex		: TEXCOORD0;	// テクスチャ座標
	float3	Normal	: TEXCOORD1;	// 法線
	float4	Color	: COLOR0;		// 色
};

//------------------------------------------------------------------------------
// 頂点シェーダ
VS_OUTPUT Basic_VS(LN_VS_INPUT input)
{
	VS_OUTPUT output;
	
	// 頂点位置をスクリーン空間に変換
	output.Pos = mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
	
	// 頂点法線をワールド空間に変換
	output.Normal = normalize(mul(input.Normal, (float3x3)ln_World));
	
	// テクスチャ座標
	output.Tex = input.Tex;
	
	// 頂点カラー
	output.Color = LN_GetLambertVertexColor(output.Normal) * input.Color;
	
	return output;
}

//------------------------------------------------------------------------------
// ピクセルシェーダ
float4 Basic_PS(VS_OUTPUT input) : COLOR0
{
	float4 color;
	
	float4 texColor = tex2D(ObjTexSampler, input.Tex);
	color = input.Color * texColor;//MaterialDiffuse;//input.Color * texColor;
	
	return color;
}

//------------------------------------------------------------------------------
// 描画テクニック
technique MainTec0
{
	pass P0
	{
		VertexShader = compile vs_3_0 Basic_VS();
		PixelShader	 = compile ps_3_0 Basic_PS();
	}
}


#endif /* LN_HLSL_DX9 */

//=============================================================================
#ifdef LN_GLSL_VERTEX_Main
attribute vec3	ln_Vertex;			// POSITION
attribute vec2	ln_MultiTexCoord0;	// TEXCOORD0
attribute vec3	ln_Normal0;			// NORMAL0
attribute vec4	ln_Color0;			// COLOR0

varying vec2	v_TexUV;
varying vec4	v_Color;

void main()
{
	// 頂点位置をスクリーン空間に変換
	gl_Position		= ln_WorldViewProjection * vec4(ln_Vertex, 1.0);
	
	// 頂点法線をワールド空間に変換
	vec3 normal		= normalize(mat3(ln_WorldViewProjection) * ln_Normal0);
	
	// テクスチャ座標
	v_TexUV			= LN_FlipTexCoord(ln_MultiTexCoord0);
	
	// 頂点カラー
	v_Color			= LN_GetLambertVertexColor(normal) * ln_Color0;
}
#endif /* LN_GLSL_VERTEX_Main */

//=============================================================================
#ifdef LN_GLSL_FRAGMENT_Main
varying vec4	v_Color;
varying vec2	v_TexUV;

void main()
{
	gl_FragColor = texture2D(ln_MaterialTexture, v_TexUV) * v_Color;
}
#endif /* LN_GLSL_FRAGMENT_Main */


