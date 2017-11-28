
texture2D	ln_MaterialTexture;
static float2	_MaterialTextureSize = float2(640, 480);	// TODO:
static float2 RcpMaterialTextureSize = float2(
	1.0 / _MaterialTextureSize.x,
	1.0 / _MaterialTextureSize.y);

sampler2D ScreenSampler = sampler_state
{ 
	Texture = <ln_MaterialTexture>;
    MinFilter = Point; 
    MagFilter = Point;
    MipFilter = None;
    AddressU = Clamp;
    AddressV = Clamp;
};


float FxaaLuma(float3 rgb)
{
    const float3 luma = float3(0.299, 0.587, 0.114);
    return dot(rgb, luma);
}

float4 FxaaTexOff(sampler2D tex, float2 pos, float2 offset)
{
    return tex2D(tex, pos + offset * RcpMaterialTextureSize);
}

float4 FxaaTexLod(sampler2D tex, float2 pos)
{
    return tex2D(tex, pos);
}

float4 FxaaPixelShader(sampler2D tex, float4 posPos, float2 rcpFrame)
{
    // NOTE:
    //     NW N NE
    //     W  M  E
    //     SW S SE

    float3 rgbNW = FxaaTexOff(tex, posPos.zw, float2(0, 0)).xyz;
    float3 rgbNE = FxaaTexOff(tex, posPos.zw, float2(1, 0)).xyz;
    float3 rgbSW = FxaaTexOff(tex, posPos.zw, float2(0, 1)).xyz;
    float3 rgbSE = FxaaTexOff(tex, posPos.zw, float2(1, 1)).xyz;
    float3 rgbM  = FxaaTexLod(tex, posPos.xy).xyz;

    float lumaNW = FxaaLuma(rgbNW);
    float lumaNE = FxaaLuma(rgbNE);
    float lumaSW = FxaaLuma(rgbSW);
    float lumaSE = FxaaLuma(rgbSE);
    float lumaM  = FxaaLuma(rgbM);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    float dirSwMinusNe = lumaSW - lumaNE;
    float dirSeMinusNw = lumaSE - lumaNW;

    float2 dir = float2(dirSwMinusNe + dirSeMinusNw, dirSwMinusNe - dirSeMinusNw);

    const float FxaaReduceMin = 1.0/128.0;
    const float FxaaReduceMul = 1.0/8.0;
    const float FxaaSpanMax = 8.0;

    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FxaaReduceMul),
        FxaaReduceMin);

    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = clamp(mul(dir, rcpDirMin), float2(-FxaaSpanMax, -FxaaSpanMax), float2(FxaaSpanMax, FxaaSpanMax)) * rcpFrame.xy;

    float3 rgbN1 = FxaaTexLod(tex, posPos.xy + dir * (1.0/3.0 - 0.5)).xyz;
    float3 rgbP1 = FxaaTexLod(tex, posPos.xy + dir * (2.0/3.0 - 0.5)).xyz;

    float3 rgbN2 = FxaaTexLod(tex, posPos.xy + dir * (0.0/3.0 - 0.5)).xyz;
    float3 rgbP2 = FxaaTexLod(tex, posPos.xy + dir * (3.0/3.0 - 0.5)).xyz;

    float3 rgbA = (rgbN1 + rgbP1);
    float3 rgbB = ((rgbN2 + rgbP2) * 0.25) + (rgbA * 0.25);

    float lumaB = FxaaLuma(rgbB);
    if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
        rgbB.xyz = rgbA.xyz * 0.5;
    }

    return float4(rgbB, 1.0);
}


//----------

float4x4	ln_WorldView;
float4x4	ln_Projection;
float2		ln_ViewportPixelSize;

static float2 ViewportOffset = (float2( 0.5, 0.5 ) / ln_ViewportPixelSize);

static float2 RcpViewportPixelSize = float2(
	1.0 / ln_ViewportPixelSize.x,
	1.0 / ln_ViewportPixelSize.y);

const float FxaaSubpixShift = 1.0/4.0;

struct VSInput
{
	float3	Position		: POSITION;
	float2	TextureCoord	: TEXCOORD0;
};

struct VSOutput
{
	float4	svPosition		: POSITION;
	float4	TextureCoord	: TEXCOORD0;
};

struct PSInput
{
	float4	TextureCoord		: TEXCOORD0;
};

VSOutput VS_FXAA(VSInput input)
{
	VSOutput output;
	output.svPosition	= float4(input.Position, 1.0f);

	output.TextureCoord = float4(
		input.TextureCoord.xy,
		input.TextureCoord.xy - (RcpViewportPixelSize.xy * (0.5 + FxaaSubpixShift)));
	
	return output;
}

float4 PS_FXAA(PSInput input) : COLOR0
{
    return FxaaPixelShader(ScreenSampler, input.TextureCoord, RcpViewportPixelSize);
}

technique Main
{
	pass ShadowCaster
	{
		VertexShader = compile vs_3_0 VS_FXAA();
		PixelShader	 = compile ps_3_0 PS_FXAA();
	}
}
