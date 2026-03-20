//------------------------------------------------------------------------------
// ConstantBuffer
//------------------------------------------------------------------------------
cbuffer ConstBuff: register(b0) {
    float4 _Color;
};

//------------------------------------------------------------------------------
// VertexShader
//------------------------------------------------------------------------------
struct VSInput {
    float4 Pos: POSITION;
};
 
struct VSOutput {
    float4 Pos: SV_POSITION;
};
 
VSOutput VSMain(VSInput input) {
    VSOutput output = (VSOutput)0;
    output.Pos = input.Pos;
    return output;
}

//------------------------------------------------------------------------------
// PixelShader
//------------------------------------------------------------------------------
struct PSInput {
    float4 Pos: SV_POSITION;
};

float4 PSMain(PSInput input): SV_TARGET {
    return _Color;
}

//------------------------------------------------------------------------------
// Technique
//------------------------------------------------------------------------------
technique MainTech {
    pass Pass0 {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}
