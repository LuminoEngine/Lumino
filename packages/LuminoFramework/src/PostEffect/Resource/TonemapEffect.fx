
#include <Lumino.fxh>
//==============================================================================

float	paramA;  // shoulderStrength
float	paramB;  // linearStrength
float	paramCB;    // param.linearStrength * param.linearAngle
float	paramDE;    // param.toeStrength * param.toeNumerator
float	paramDF;    // param.toeStrength * param.toeDenominator
float	paramEperF;  // param.toeNumerator / param.toeDenominator
float	paramF_White;//

float Exposure = 0.0;
float4 _Tone;

const int isGamma = 1;


const float4 _Vignette_Color = float4(0, 0, 0, 1); 
const float2 _Vignette_Center = float2(0.5, 0.5);
const float4 _Vignette_Settings = float4(0.5, 0.5, 1.0, 0); // x: intensity, y: smoothness, z: roundness, w: rounded

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

float3 CalcUncharted2FilmicPreParam( float3 rgb,
	float paramA, float paramB, float paramCB,
	float paramDE, float paramDF, float paramEperF, float paramF_White )
{
	float3	ret = ((rgb * (paramA * rgb + paramCB) + paramDE)
		/ (rgb * (paramA * rgb + paramB) + paramDF))
		- paramEperF;
	return ret / paramF_White;
}

float4 PSMain(PSInput input) : SV_TARGET0
{
    float4 color = tex2D(ln_MaterialTexture, input.UV);
	float expBias = exp2( Exposure );
	float3 rgb = color.rgb * expBias;

    
	rgb = LN_CalculateToneColor(float4(rgb, 1.0), _Tone).rgb;

#if 0
    rgb = CalcUncharted2FilmicPreParam( rgb,
        paramA, paramB, paramCB, paramDE, paramDF, paramEperF, paramF_White );
#endif


#if 1
    float2 uvDistorted = input.UV;
    {

        float2 d = abs(uvDistorted - _Vignette_Center) * _Vignette_Settings.x;
        d *= 2.0;

        d.x *= lerp(1.0, ln_Resolution.x / ln_Resolution.y, _Vignette_Settings.w);
        d = pow(saturate(d), _Vignette_Settings.z); // Roundness
        //return float4(d, 0, 1);
        float vfactor = pow(saturate(1.0 - dot(d, d)), _Vignette_Settings.y);
        //return float4(vfactor, 0, 0, 1);

        rgb *= lerp(_Vignette_Color.rgb, float3(1.0, 1.0, 1.0), vfactor);

        //float a = lerp(1.0, color.a, vfactor);
	    //return float4(color.a, 0, 0, 1);

        //rgb = color.rgb * color.a;
    }
#endif


#if 0
    if (isGamma) {
        rgb = pow( rgb, 1.0 / 2.2 );
    }
#endif

	return float4(rgb, 1);
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

