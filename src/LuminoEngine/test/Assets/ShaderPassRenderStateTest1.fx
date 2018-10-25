
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
};
 
struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};
 
VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = input.Pos;
    output.Color = input.Color;
    return output;
}

struct PS_INPUT
{
    float4 Color : COLOR;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    return input.Color;
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
    pass Pass1
    {
        // RenderTargetBlendDesc
        blendEnable = true;
        sourceBlend = One;
        destinationBlend = One;
        blendOp = Add;
        sourceBlendAlpha = One;
        destinationBlendAlpha = One;
        blendOpAlpha = Add;
        
        // RasterizerStateDesc
        fillMode = Wireframe;
        cullMode = None;
        
        // DepthStencilStateDesc
        depthTestFunc = Never;
        depthWriteEnabled = false;

        // StencilOpDesc
        stencilEnabled = false;
        stencilReferenceValue = 255;
        stencilFailOp = Keep;
        stencilDepthFailOp = Keep;
        stencilPassOp = Keep;
        stencilFunc = Never;
        
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}

