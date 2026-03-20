
sampler2D _Texture;
//Texture2D _Texture;
//SamplerState _SamplerState;

struct VS_INPUT
{
    float4 Pos : POSITION;
    float2 UV  : TEXCOORD0;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 UV  : TEXCOORD0;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    output.UV = input.UV;
    return output;
}

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 UV  : TEXCOORD0;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    float4 color = tex2D(_Texture, input.UV);//_Texture.Sample(_SamplerState, input.UV);
    return color;
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

