
#include <Lumino.fxh>

float _Factor;
float _Vague;       // 0=ぼかし無し, 1.0=黒から白まで全体をグラデーション
float4 _ColorScale;
sampler2D _OverrayTexture;
sampler2D _MaskTexture;

struct VS_Input
{
    float3    Pos        : POSITION;
    float2    UV        : TEXCOORD0;
};

struct VS_Output
{
    float4    Pos        : SV_POSITION;
    float2    UV        : TEXCOORD0;
};

struct PS_Input
{
    float2    UV        : TEXCOORD0;
};

VS_Output VS_Main(VS_Input input)
{
    VS_Output o;
    o.Pos = float4(input.Pos, 1.0);
    o.UV  = input.UV;
    return o;
}

float steprange(float gray)
{
    
    //float t = steprange(gray);
    float t = (1.0 - _Factor);// * (1.0 * _Vague);

    float range = 1.0 + _Vague;

    //t *= 1.0 / _Vague;  // ぼかし範囲が大きいほど時間を加速
    //t -= _Vague;
    t *= range;
    t -= _Vague;

    float a = (gray - t) / _Vague;
    //a += _Vague * 2;
    return a;


    // 1.0 / 255 = 0.003921568627451‬


    //float a = atan(_Vague);//lerp(255.0, 1.0, _Vague);
    //float x = gray - _Factor;
    //return clamp(a * x, 0.0, 1.0);
}

float4 PS_Main(PS_Input input) : COLOR0
{
    float gray = tex2D(_MaskTexture, input.UV).r;
    //float t = step(1.0 - _Factor, gray);//(gray + _Factor);// / _Vague;
    //return float4(t, 0, 0, 1);

    float a = (_Vague > 0.0) ? steprange(gray) : step(1.0 - _Factor, gray);


    float3 c = lerp(
        tex2D(ln_MaterialTexture, input.UV).rgb,
        tex2D(_OverrayTexture, input.UV).rgb * _ColorScale.rgb,
        saturate(a));
    return float4(c, 1.0);
}

technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass P0
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}

