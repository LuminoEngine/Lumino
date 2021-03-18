// Copyright (c) 2020+ lriki. Distributed under the MIT license.

// For render object black color filled.

#include <Lumino.fxh>

//==============================================================================
// Vertex shader

struct VS_Input
{
    float3 Pos : POSITION;
};

struct VS_Output
{
    float4 Pos : SV_POSITION;
};

VS_Output VS_Main(LN_VSInput input)
{
    VS_Output output;
    output.Pos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    return output;
}

//==============================================================================
// Pixel shader

float4 PS_Main() : SV_TARGET
{
    return float4(0, 0, 0, 1);
}

//==============================================================================
// Technique

technique Default
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
