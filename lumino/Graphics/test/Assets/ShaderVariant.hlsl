//------------------------------------------------------------------------------
// ConstantBuffer
//------------------------------------------------------------------------------
cbuffer ConstBuff: register(b0) {
    float4 _Color;
};

//sampler2D g_texture1;

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
    //return _Color * tex2D(g_texture1, float2(0, 0));
}

//------------------------------------------------------------------------------
// Technique
//------------------------------------------------------------------------------
technique MainTech {
    MultiCompile = LN_USE_INSTANCING;
    MultiCompile = LN_USE_SKINNING;
    MultiCompile = _,LN_USE_NORMALMAP,LN_USE_TEST_HEIGHTMAP;
    MultiCompile = LN_USE_ROUGHNESS_MAP;
    MultiCompile = LN_PHASE_FORWARD,LN_PHASE_SHADOW_CASTER,LN_PHASE_FORWARD_PREPASS;
    pass Pass0 {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}
