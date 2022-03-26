
#include <Lumino.fxh>

struct VSOutput {
    float4 svPos        : SV_POSITION;
    float4 Color        : COLOR0;
};

//------------------------------------------------------------------------------
// VertexShader
//------------------------------------------------------------------------------
VSOutput VSMain(LN_VSInput input) {
    VSOutput output = (VSOutput)0;
    output.svPos = float4(input.Pos, 1.0);
#ifdef LN_USE_INSTANCING
    output.Color = input.Color * input.InstanceColorScale;
#else
    output.Color = input.Color;
#endif
    return output;
}

//------------------------------------------------------------------------------
// PixelShader
//------------------------------------------------------------------------------
float4 PSMain(VSOutput input): SV_TARGET {
    return input.Color;
}

//------------------------------------------------------------------------------
// Technique
//------------------------------------------------------------------------------
technique MainTech {
    MultiCompile = LN_USE_INSTANCING;
    pass Pass0
    {
        VertexShader = VSMain;
        PixelShader  = PSMain;
    }
}
