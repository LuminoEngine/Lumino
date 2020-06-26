
#include <Lumino.fxh>

sampler2D _depthTex;
sampler2D _dofTex;

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

float4 PSMain(PSInput input) : SV_TARGET0
{
    float4 retcol = tex2D(ln_MaterialTexture, input.UV);

    float t = pow(distance(baseDepth, tex2D(_depthTex, input.UV).g), 0.5f);// *8.0f;

    //return float4(t, 0, 0, 1);

    t *= 8.0f;
    float alpha, no;
    alpha = modf(t, no);

    float3 colA, colB;
    if (no == 0) {
        colA = retcol.rgb;
        colB = tex2D(_dofTex, input.UV * float2(1.0f, 0.5f)).rgb;
        //colB = GaussianFilteredColor5x5(_dofTex, smp, input.UV*float2(1.0f, 0.5f), dx, dy);
    }
    else {
        colA = tex2D(_dofTex, input.UV*float2(1.0f, 0.5f)*pow(0.5, no - 1) + float2(0, 1.0 - pow(0.5, no - 1))).rgb;
        colB = tex2D(_dofTex, input.UV*float2(1.0f, 0.5f)*pow(0.5, no) + float2(0, 1 - pow(0.5, no))).rgb;
        //colA = GaussianFilteredColor5x5(_dofTex, smp, input.UV*float2(1.0f, 0.5f)*pow(0.5, no - 1) + float2(0, 1.0 - pow(0.5, no - 1)), dx, dy);
        //colB = GaussianFilteredColor5x5(_dofTex, smp, input.UV*float2(1.0f, 0.5f)*pow(0.5, no) + float2(0, 1 - pow(0.5, no)), dx, dy);
    }
    retcol.rgb = lerp(colA, colB, alpha);

	return retcol;
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

