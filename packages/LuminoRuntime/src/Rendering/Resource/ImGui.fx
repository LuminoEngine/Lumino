float4x4 _Projection;
sampler2D _Texture;

//==============================================================================
// Vertex shader

struct VSInput {
    float3 Pos      : POSITION;
    float2 UV       : TEXCOORD0;
    float4 Color    : COLOR0;
};

struct VSOutput {
    float4 Pos      : SV_POSITION;
    float2 UV       : TEXCOORD0;
    float4 Color    : COLOR0;
};

VSOutput VSMain(VSInput input) {
    VSOutput output;
    output.Pos = mul(float4(input.Pos, 1.0), _Projection);
    output.UV = input.UV;
    output.Color = input.Color;
    return output;
}

//==============================================================================
// Pixel shader

struct PSInput {
    float2 UV       : TEXCOORD0;
    float4 Color    : COLOR0;
};

float4 PSMain(PSInput input) : SV_TARGET {
    return tex2D(_Texture, input.UV) * input.Color;
}

//==============================================================================
// Technique

technique Default
{
    ShadingModel = Unlit;
    pass Pass0
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}
