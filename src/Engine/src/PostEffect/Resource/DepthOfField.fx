
#include <Lumino.fxh>

sampler2D _depthTex;
sampler2D _dofTex;

const float MaxMips = 8.0;

const float baseDepth = 10.0 / 100.0;


//==============================================================================
// Vertex shader

struct VSInput
{
    float3 Pos : POSITION0;
    float2 UV  : TEXCOORD0;
};

struct VSOutput
{
    float4 Pos : SV_POSITION;
    float2 UV  : TEXCOORD0;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.Pos = float4(input.Pos, 1.0);
    output.UV = input.UV;
    return output;
}

//==============================================================================
// Pixel shader

struct PSInput
{
    float2 UV : TEXCOORD0;
};


float4 GaussianFilteredColor5x5(sampler2D tex, float2 uv) {
    return float4(tex2D(tex, uv));
    float dx = 1.0 / 320.0;//ln_Resolution.z;
    float dy = 1.0 / 480.0;//ln_Resolution.w;
	float4 center = tex2D(tex, uv);
	float4 ret =
		tex2D(tex, uv + float2(-dx * 2, -dy * 2)) +
		tex2D(tex, uv + float2(-dx * 1, -dy * 2)) * 4 +
		tex2D(tex, uv + float2(-dx * 0, -dy * 2)) * 6 +
		tex2D(tex, uv + float2(dx * 1, -dy * 2)) * 4 +
		tex2D(tex, uv + float2(dx * 2, -dy * 2)) +

		tex2D(tex, uv + float2(-dx * 2, -dy * 1)) * 4 +
		tex2D(tex, uv + float2(-dx * 1, -dy * 1)) * 16 +
		tex2D(tex, uv + float2(-dx * 0, -dy * 1)) * 24 +
		tex2D(tex, uv + float2(dx * 1, -dy * 1)) * 16 +
		tex2D(tex, uv + float2(dx * 2, -dy * 1)) * 4 +

		tex2D(tex, uv + float2(-dx * 2, dy * 0)) * 6 +
		tex2D(tex, uv + float2(-dx * 1, dy * 0)) * 24 +
		center * 36 +
		tex2D(tex, uv + float2(dx * 1, dy * 0)) * 24 +
		tex2D(tex, uv + float2(dx * 2, dy * 0)) * 6 +

		tex2D(tex, uv + float2(-dx * 2, dy * 1)) * 4 +
		tex2D(tex, uv + float2(-dx * 1, dy * 1)) * 16 +
		tex2D(tex, uv + float2(-dx * 0, dy * 1)) * 24 +
		tex2D(tex, uv + float2(dx * 1, dy * 1)) * 16 +
		tex2D(tex, uv + float2(dx * 2, dy * 1)) * 4 +

		tex2D(tex, uv + float2(-dx * 2, dy * 2)) +
		tex2D(tex, uv + float2(-dx * 1, dy * 2)) * 4 +
		tex2D(tex, uv + float2(-dx * 0, dy * 2)) * 6 +
		tex2D(tex, uv + float2(dx * 1, dy * 2)) * 4 +
		tex2D(tex, uv + float2(dx * 2, dy * 2));
	return float4((ret.rgb / 256.0f), ret.a);

}

float4 PSMain(PSInput input) : SV_TARGET0
{
    float4 retcol = tex2D(ln_MaterialTexture, input.UV);

    float linearDepth = tex2D(_depthTex, input.UV).g;
    float d = distance(baseDepth, linearDepth);

    float t = pow(d, 1.05);//1.25);

    //return float4(t, 0, 0, 1);

    t *= 0.5;

    t *= MaxMips;
    float alpha, mipNo;
    alpha = modf(t, mipNo);

    float3 colA, colB;
    if (mipNo == 0) {
        //return float4(1, 0, 0, 1);
        colA = retcol.rgb;
        //colB = tex2D(_dofTex, input.UV * float2(1.0f, 0.5f)).rgb;
        colB = GaussianFilteredColor5x5(_dofTex, input.UV * float2(1.0f, 0.5f));
    }
    else {
        //colA = tex2D(_dofTex, input.UV*float2(1.0f, 0.5f)*pow(0.5, mipNo - 1) + float2(0, 1.0 - pow(0.5, mipNo - 1))).rgb;
        //colB = tex2D(_dofTex, input.UV*float2(1.0f, 0.5f)*pow(0.5, mipNo) + float2(0, 1 - pow(0.5, mipNo))).rgb;
        colA = GaussianFilteredColor5x5(_dofTex, input.UV * float2(1.0f, 0.5f) * pow(0.5, mipNo - 1.0) + float2(0, 1.0 - pow(0.5, mipNo - 1.0)));
        colB = GaussianFilteredColor5x5(_dofTex, input.UV * float2(1.0f, 0.5f) * pow(0.5, mipNo) + float2(0, 1.0 - pow(0.5, mipNo)));
    }
    retcol.rgb = lerp(colA, colB, alpha);

	return retcol;
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

