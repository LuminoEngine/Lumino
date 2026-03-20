#include <Lumino.fxh>
 
struct VSOutput
{
    LN_VS_OUTPUT_DECLARE;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
};

VSOutput VSMain(LN_VSInput input)
{
    VSOutput output = (VSOutput)0;
    LN_ProcessVertex(input, output);
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return float4(1, 0, 0, 1);
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

