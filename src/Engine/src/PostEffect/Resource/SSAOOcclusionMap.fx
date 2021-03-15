/*
 * Note:
 * http://developer.wonderpla.net/entry/blog/engineer/iPhone5S_SSAO/
 * https://learnopengl.com/Advanced-Lighting/SSAO
 * https://github.com/boxerprogrammer/directx12_samples
 *
 * 色々参考にしたが結局 Range-check を簡単にするため、計算は ViewSpace 上で行うように全部自前で書き直している。
 */
#include <Lumino.fxh>

//==============================================================================
// Uniforms

sampler2D _viewSpaceNormalMap;
sampler2D _viewDepthMap;
sampler2D _randomSampler;

cbuffer LocalBuffer
{
    float _density;
    
    // https://qiita.com/mebiusbox2/items/2a18994a5caa6d584099#%E5%9F%BA%E6%9C%AC%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0
    // ViewSpace 上の、↑の図の最大半径
    float _scaling;
}

//==============================================================================

static const float SamplingRange = 0.7;

static const int SamplingCount = 16;

// 球内の一様分布
static const float3 SamplingOffsets[SamplingCount] = {
    float3(0.467928, -0.026220, -0.194384), // len:0.507375
    float3(-0.105969, 0.301316, -0.520884), // len:0.611017
    float3(0.043972, 0.077468, 0.154309),   // len:0.178175
    float3(-0.009717, 0.782814, -0.384311), // len:0.872116
    float3(0.511516, 0.048673, 0.802657),   // len:0.953036
    float3(0.186852, -0.643386, -0.561739), // len:0.874305
    float3(0.497528, 0.674539, -0.469116),  // len:0.960524
    float3(-0.504982, -0.349113, -0.160797),// len:0.634620
    float3(-0.204751, -0.766180, -0.053794),// len:0.794889
    float3(-0.615564, -0.319885, -0.511518),// len:0.861914
    float3(-0.605221, 0.184057, 0.717392),  // len:0.956463
    float3(-0.735380, 0.184512, 0.492802),  // len:0.904258
    float3(0.520004, -0.215251, -0.763622), // len:0.948607
    float3(0.615146, 0.603269, 0.170744),   // len:0.878346
    float3(0.727975, 0.164012, -0.133347),  // len:0.758043
    float3(0.029602, 0.484172, 0.003634),   // len:0.485090
};
/* ↑の生成コード
for (int i = 0; i < 60; i++) {
    const float cosTheta = -2.0 * Random::randFloat() + 1.0;
    const float sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);
    const float phi = 2.0 * Math::PI * Random::randFloat();
    const float radius = std::pow(Random::randFloat(), 1.0 / 3.0);
    auto v = Vector3(radius * sinTheta * std::cos(phi), radius * sinTheta * std::sin(phi), radius * cosTheta);
    printf("float3(%f, %f, %f), // len:%f\n", v.x, v.y, v.z, v.length());
}
*/

struct VSInput
{
    float3 Pos : POSITION0;
    float2 UV  : TEXCOORD0;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

//==============================================================================
// Vertex shader

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.Pos = float4(input.Pos, 1.0);
    output.UV = input.UV;
    return output;
}

//==============================================================================
// Pixel shader

float3x3 _LN_MakeLazyToRot(float3 lookAt)
{
	//Matrix Matrix::makeLookAtLH(const Vector3& position, const Vector3& lookAt, const Vector3& up)
	//float3 up = float3(lookAt.y, lookAt.z, lookAt.x);	// なんでもいい
    const float3 up = float3(0.0, 1.0, 0.0);

	//Vector3 xaxis, yaxis, zaxis;
	// 注視点からカメラ位置までのベクトルをZ軸とする
	float3 zaxis = lookAt;
	//zaxis -= position;
	//zaxis.normalize();
	// Z軸と上方向のベクトルの外積をとるとX軸が分かる
	float3 xaxis = cross(up, lookAt);
	//xaxis.normalize();
	// 2つの軸がわかったので、その2つの外積は残りの軸(Y軸)になる
	float3 yaxis = cross(zaxis, xaxis);
	
	return float3x3(
		xaxis.x, xaxis.y, xaxis.z,
		yaxis.x, yaxis.y, yaxis.z,
		zaxis.x, zaxis.y, zaxis.z);
	//return float3x3(
	//	xaxis.x, yaxis.x, zaxis.x,
	//	xaxis.y, yaxis.y, zaxis.y,
	//	xaxis.z, yaxis.z, zaxis.z);

}

float3x3 MakeRotation2DMatrix(float r)
{
    float s, c;
    sincos(r, s, c);
	return float3x3(
        c, s, 0.0,
        -s, c, 0.0,
        0.0, 0.0, 1.0);
}

float GetDepth(float2 uv)
{
    return tex2D(_viewDepthMap, uv).r;
}

float GetViewSpaceZ(float2 uv)
{
    return tex2D(_viewDepthMap, uv).g * (ln_FarClip - ln_NearClip) + ln_NearClip;
}

float3 GetNormal(float2 uv)
{
    float3 n = tex2D(_viewSpaceNormalMap, uv).xyz;
    return LN_UnpackNormal(n);
}

float3 GetViewPosition(float2 inputUV, float projectedZ) {
    // ClipSpace 上の座標を求める
    float4 clipPosition = float4(LN_UVToClipSpacePosition(inputUV), projectedZ, 1.0);
    float4 pos = (mul(clipPosition, ln_ProjectionI));
    return pos.xyz / pos.w;
}

float ExSdSSAO_GetZ(float2 uv) {
    return tex2D(_viewDepthMap, uv).g;
}

static const float rad = 0.05;
static const float strength = 5.0;
static const float falloff = 0.1;//1.0;
static const float3 ObjXYZ = float3(1, 1, 1);
#define invSamples (1.0f / SAMPLES)
#define rnd_offset 18.0f


float4 PSMain(PSInput input) : SV_TARGET0
{
    //return float4(_density, _scaling, 0, 1);
#if 1
    // UV 位置の深度。ProjI で ViewPos を求めるのにも使いたいので、Linear ではない。
    float projectedDepth = GetDepth(input.UV);

    // ViewSpace 上の点を求める
    float3 viewPos = GetViewPosition(input.UV, projectedDepth);
    float3 viewNormal = GetNormal(input.UV);
    //return float4(viewNormal, 1);

    float ao = 0.0f;
    float div = 0.0f;   // 平均計算のための分母
    if (projectedDepth < 1.0f) {
        for (int i = 0; i < SamplingCount; ++i) {
            float3 fres = tex2D(_randomSampler, input.UV + float2(0.01 * i, 0.0)).rgb;

            //float3 samplingViewNormal = normalize(mul(SamplingOffsets[i], _LN_MakeLazyToRot(viewNormal)));
            //float3 samplingOffset = SamplingOffsets[i];

            //float3 samplingOffset = SamplingOffsets[i] * (fres.z * 0.5 + 0.5);
            float3 samplingOffset = SamplingOffsets[i] * (fres);
            float3 samplingViewNormal = normalize(samplingOffset);



            // 法線の反対側に向いてた (始点ジオメトリの内側になっていたら) ら外側へ反転する
            float dt = dot(viewNormal, samplingViewNormal);
            float s = sign(dt);
            samplingViewNormal *= s;
            samplingOffset *= s;
            dt *= s;    // これで viewNormal と samplingViewNormal の向きの差を示す dt は、負値であることが無くなる。
                        // (同一方向なら1, 90度違っていれば0) -> 濃さとして使えるようになる。
            
            // サンプリング位置の clipPos を求める
            //float3 samplingViewPos = float3(viewPos + samplingViewNormal * fres);
            //float3 samplingViewPos = reflect(samplingViewNormal, fres);
            float3 samplingViewPos = float3(viewPos + samplingOffset * _scaling);
            float4 samplingClipPos = mul(float4(samplingViewPos, 1.0), ln_Projection);
            samplingClipPos.xyz /= samplingClipPos.w;

            // 計算結果が現在の場所の深度より奥に入ってるなら遮蔽されているという事なので加算
            float2 uv2 = LN_ClipSpacePositionToUV(samplingClipPos.xy);
            //return float4(GetDepth(uv2), 0, 0, 1);
            float samplingViewSpaceZ = GetViewSpaceZ(uv2);
            //float d = 1.0 - (abs(samplingDepth - samplingClipPos.z) / SamplingRange);
            //dt *= d;

            float depthDifference = samplingViewPos.z - samplingViewSpaceZ;  // ↓のstepで負値は一律 0 になるので、正の値だけ考えればよい

    // TODO: Range check
    // https://learnopengl.com/Advanced-Lighting/SSAO
            float rangeCheck = smoothstep(0.0, 1.0, _scaling / abs(depthDifference));
#if 0
            ao +=
                smoothstep(0, falloff_e, depthDifference) *
                (1.0f - pow(smoothstep(falloff_e, strength_e, depthDifference), 0.5)) *
                dt;
#else
            ao += 
            step(samplingViewSpaceZ, samplingViewPos.z) *   // step は、samplingClipPos.z の方が大きけ(奥にあれ)れば 1
            //smoothstep(0.0, 1.2, samplingDepth) * // 濃さ調整。ちゃんと考えたほうがいいかも
             //(1.0 - pow(smoothstep(0.0, 5.0, samplingDepth), 0.5)) *
             rangeCheck*
             dt;
            //div += dt;
#endif
        }
        //ao /= div;
        ao /= SamplingCount;
    }
    
    ao = max(0, ao);
    ao = pow(ao * 1.5, max(0.5, 1.5 - ao)) / 1.5; //やわらか関数
    ao *= _density;


    float result = 1.0f - ao;
    //return float4(LN_PackNormal(norm), 1.0);
    return float4(result, result, result, 1.0);


#else
#endif
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

