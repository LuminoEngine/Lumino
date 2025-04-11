
#define EDGE_AA 1

//==============================================================================
// Vertex shader

cbuffer Constants
{
    
    //float4x4 scissorMat_;
    //float4x4 paintMat_;
    float3x3 scissorMat;
    float3x3 paintMat;  // 48
    
    float4 viewSize;    // actualy .x, .y       // [96]

    float4 innerCol;
    float4 outerCol;
    float2 scissorExt;      // 144
    float2 scissorScale;    // 152
    float2 extent;
    float radius;
    float feather;
    float strokeMult;   // 176
    float strokeThr;    // 180
    int texType;        // 
    int type;           // 188
};

sampler2D tex;

struct VSInput
{
    float2 vertex   : POSITION0;
    float2 tcoord   : TEXCOORD0;
};

struct VSOutput
{
    float4 pos      : SV_POSITION;
    float2 ftcoord  : TEXCOORD0;
    float2 fpos     : TEXCOORD1;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.pos = float4(2.0*input.vertex.x/viewSize.x - 1.0, 1.0 - 2.0*input.vertex.y/viewSize.y, 0, 1);
    output.ftcoord = input.tcoord;
    output.fpos = input.vertex;
    return output;
}

//==============================================================================
// Pixel shader

//#define scissorMat (float3x3)scissorMat_
//#define paintMat (float3x3)paintMat_

#define my_mul(a, b) mul(b, a)

struct PSInput
{
    float2 ftcoord  : TEXCOORD0;
    float2 fpos     : TEXCOORD1;
};


float sdroundrect(float2 pt, float2 ext, float rad) {
    float2 ext2 = ext - float2(rad,rad);
    float2 d = abs(pt) - ext2;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0)) - rad;
}

// Scissoring
float scissorMask(float2 p) {
    //float2 sc = (abs((scissorMat * float3(p,1.0)).xy) - scissorExt);
    float2 sc = (abs(my_mul(scissorMat, float3(p,1.0)).xy) - scissorExt);
    sc = float2(0.5,0.5) - sc * scissorScale;
    return clamp(sc.x,0.0,1.0) * clamp(sc.y,0.0,1.0);
}

#ifdef EDGE_AA
// Stroke - from [0..1] to clipped pyramid, where the slope is 1px.
float strokeMask(PSInput input) {
    return min(1.0, (1.0-abs(input.ftcoord.x*2.0-1.0))*strokeMult) * min(1.0, input.ftcoord.y);
}
#endif

float4 PSMain(PSInput input) : SV_TARGET
{
    float2 fpos = input.fpos;

    float4 result;
    float scissor = scissorMask(fpos);
#ifdef EDGE_AA
    float strokeAlpha = strokeMask(input);
    if (strokeAlpha < strokeThr) discard;
#else
    float strokeAlpha = 1.0;
#endif
    if (type == 0) {            // Gradient
        // Calculate gradient color using box gradient
        //float2 pt = (paintMat * float3(fpos,1.0)).xy;
        float2 pt = my_mul(paintMat, float3(fpos,1.0)).xy;
        float d = clamp((sdroundrect(pt, extent, radius) + feather*0.5) / feather, 0.0, 1.0);
        float4 color = lerp(innerCol,outerCol,d);
        // Combine alpha
        color *= strokeAlpha * scissor;
        result = color;
    }
    else if (type == 1) {        // Image
        // Calculate color fron texture
        //float2 pt = (paintMat * float3(fpos,1.0)).xy / extent;
        float2 pt = my_mul(paintMat, float3(fpos,1.0)).xy / extent;

        float4 color = tex2D(tex, pt);

        if (texType == 1) color = float4(color.xyz*color.w,color.w);
        if (texType == 2) color = float4(color.x, color.x, color.x, color.x);
        // Apply color tint and alpha.
        color *= innerCol;
        // Combine alpha
        color *= strokeAlpha * scissor;
        result = color;
    } else if (type == 2) {        // Stencil fill
        result = float4(1,1,1,1);
    } else if (type == 3) {        // Textured tris

        float4 color = tex2D(tex, input.ftcoord);

        if (texType == 1) color = float4(color.xyz*color.w,color.w);
        if (texType == 2) color = float4(color.x, color.x, color.x, color.x);
        color *= scissor;
        result = color * innerCol;
    }

    return result;
}


//==============================================================================
// Technique

technique Default
{
    pass Pass0
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}


