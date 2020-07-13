
#include <Lumino.fxh>

// Note:
// http://developer.wonderpla.net/entry/blog/engineer/iPhone5S_SSAO/

//==============================================================================

#define ln_ViewportPixelSize ln_Resolution.xy

// https://qiita.com/mebiusbox2/items/2a18994a5caa6d584099#%E5%9F%BA%E6%9C%AC%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0
// ViewSpace 上の、↑の図の最大半径
const float SamplingRange = 1.0;

sampler2D _screenSampler;   // normalDepth
sampler2D _viewDepthMap;
// TODO: move to lib
float3 LN_PackNormal(float3 normal)
{
	return (normal.xyz + 1.0) / 2.0;
}

const int SamplingCount = 8;

const float bb = 1.0 / SamplingCount;//1.0;
const float2 sampOffset[SamplingCount] = {  // px 単位
	//float2(-1.0,  0.0) / ln_ViewportPixelSize,
	//float2(-0.5, -0.5) / ln_ViewportPixelSize,
	//float2( 0.5, -0.5) / ln_ViewportPixelSize,
	//float2( 1.0,  0.0) / ln_ViewportPixelSize,
	//float2( 0.5,  0.5) / ln_ViewportPixelSize,
	//float2(-0.5,  0.5) / ln_ViewportPixelSize,
	
    /*
	float2(-1.0,  0.0),// * bb,
	float2(-0.7,  0.7) * bb * 2,
	float2( 0.0,  1.0) * bb * 4,
	float2( 0.7,  0.7) * bb * 6,
	float2( 1.0,  0.0) * bb * 8,
	float2( 1.0,  1.0) * bb * 10,
    */

    float2( 0.0, -1.0) * bb * 1,
    float2( 0.7, -0.7) * bb * 2,
    float2( 1.0,  0.0) * bb * 3,
    float2( 0.7,  0.7) * bb * 4,
    float2( 0.0,  1.0) * bb * 5,
    float2(-0.7,  0.7) * bb * 6,
    float2(-1.0,  0.0) * bb * 7,
    float2(-0.7, -0.7) * bb * 8,
};    // サンプリング点のオフセット値。最大距離だいたい 1.0

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

float tangent(float3 p, float3 s) {  
    return (p.z - s.z) / length(s.xy - p.xy);  
}  

const float EdgeBias = 0.04; 
const float PI = 3.14159;


float3 _LN_UnpackColorToNormal(float3 c)
{
	return (c * 2.0) - 1.0;
}

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

float3 GetNormal(float2 uv)
{
    float3 n = tex2D(_screenSampler, uv).xyz;
    return LN_UnpackNormal(n);
}

float3 GetViewPosition(float2 inputUV, float projectedZ) {
    // ClipSpace 上の座標を求める
    float4 clipPosition = float4(LN_UVToClipSpacePosition(inputUV), projectedZ, 1.0);
    float4 pos = (mul(clipPosition, ln_ProjectionI));
    return pos.xyz / pos.w;
}

float random(float2 uv) {
	return frac(sin(dot(uv, float2(12.9898f, 78.233f)))*43758.5453f);
}

float4 PSMain(PSInput input) : SV_TARGET0
{
#if 1
    // UV 位置の深度。ProjI で ViewPos を求めるのにも使いたいので、Linear ではない。
    float projectedDepth = GetDepth(input.UV);

    // ViewSpace 上の点を求める
    //float4 clipPos = float4(LN_UVToClipSpacePosition(input.UV), projectedDepth, 1.0);
    //float3 viewPos = mul(clipPos, ln_ProjectionI);
    float3 viewPos = GetViewPosition(input.UV, projectedDepth);
    float3 viewNormal = GetNormal(input.UV);

    float ao = 0.0f;
    float div = 0.0f;   // 平均計算のための分母
    if (projectedDepth < 1.0f) {
        for (int i = 0; i < SamplingCount; ++i) {
            // XY 平面上の offset を適当に回転する (TODO: 乱数にできるとよい)
            const float3 samplingOffset = mul(float3(sampOffset[i] * SamplingRange, 1.0), MakeRotation2DMatrix((float)i));

            // viewNormal が軸になるように samplingOffset を変換する
            float3 samplingViewNormal = normalize(mul(samplingOffset, _LN_MakeLazyToRot(viewNormal)));
            //float3 samplingViewNormal = normalize(float3(sampOffset[i], viewNormal.z));
            // TODO: samplingViewNormal を、viewNormal を軸にランダム回転する



            // 法線の反対側に向いてた (始点ジオメトリの内側になっていたら) ら外側へ反転する
            float dt = dot(viewNormal, samplingViewNormal);
            float s = sign(dt);
            samplingViewNormal *= s;
            dt *= s;    // これで viewNormal と samplingViewNormal の向きの差を示す dt は、負値であることが無くなる。
                        // (同一方向なら1, 90度違っていれば0) -> 濃さとして使えるようになる。
            
            // サンプリング位置の clipPos を求める
            float3 samplingViewPos = float3(viewPos + samplingViewNormal);
            float4 samplingClipPos = mul(float4(samplingViewPos, 1.0), ln_Projection);
            samplingClipPos.xyz /= samplingClipPos.w;

            // 計算結果が現在の場所の深度より奥に入ってるなら遮蔽されているという事なので加算
            float2 uv2 = LN_ClipSpacePositionToUV(samplingClipPos.xy);
            //return float4(GetDepth(uv2), 0, 0, 1);
            float samplingDepth = GetDepth(uv2);
            //float d = 1.0 - (abs(samplingDepth - samplingClipPos.z) / SamplingRange);
            //dt *= d;

            ao += step(samplingDepth, samplingClipPos.z) * dt;   // step は、rpos.z の方が大きけ(奥にあれ)れば 1
            //div += dt;
        }
        //ao /= div;
        ao /= SamplingCount;
    }
    float result = 1.0f - ao;
    //return float4(LN_PackNormal(norm), 1.0);
    return float4(result, result, result, 1.0);

#if 0
    float4 respos = float4(viewPos, 1.0);
    {
        float dp = projectedDepth;
        float dx = ln_Resolution.z;
        float dy = ln_Resolution.w;
        
        float div = 0.0f;
        float ao = 0.0f;
        float3 norm = GetNormal(input.UV);
        const int trycnt = 16;//シェーダコンパイルが遅い場合はこの数値を小さくしてください。
        const float radius = 0.05f;
        if (dp < 1.0f) {
            for (int i = 0; i < trycnt; ++i) {
                float rnd1 = random(float2(i*dx, i*dy)) * 2 - 1;
                float rnd2 = random(float2(rnd1, i*dy)) * 2 - 1;
                float rnd3 = random(float2(rnd2, rnd1)) * 2 - 1;
                float3 omega = normalize(float3(rnd1,rnd2,rnd3));
                //return float4(LN_PackNormal(omega), 1.0);
                omega = normalize(omega);
                //乱数の結果法線の反対側に向いてたら反転する
                float dt = dot(norm, omega);
                float sgn = sign(dt);
                omega *= sign(dt);
                //結果の座標を再び射影変換する
                float4 rpos = mul(float4(respos.xyz + omega * radius, 1), ln_Projection);
                rpos.xyz /= rpos.w;
                dt *= sgn;
                div += dt;
                //計算結果が現在の場所の深度より奥に入ってるなら遮蔽されているという事なので加算
                float2 uv2 = LN_ClipSpacePositionToUV(rpos.xy);
                //return float4(GetDepth(uv2), 0, 0, 1);
                ao += step(GetDepth(uv2), rpos.z)*dt;   // step は、rpos.z の方が大きけ(奥にあれ)れば 1
            }
            ao /= div;
        }
        float result = 1.0f - ao;
        //return float4(LN_PackNormal(norm), 1.0);
        return float4(result, result, result, 1.0);
    }
#endif

#else
    //return float4(ln_FarClip / 200, ln_NearClip, 0, 1);
    //return float4(ln_ViewportPixelSize.x / 1280, ln_ViewportPixelSize.y / 960, 0, 1);
	//float4 nd = tex2D(normalDepthSampler, input.UV);
	//return float4(_LN_UnpackColorToNormal(nd.xyz), 1.0);
	//float3 normal = _LN_UnpackColorToNormal(tex2D(normalDepthSampler, input.UV).xyz);
	
	//float3x3 toNormalRot = _LN_MakeLazyToRot(normal);
	
	float depth = GetDepth(input.UV);// tex2D(_screenSampler, input.UV).r;//texture(normalDepth, input.UV).w;  
    float3 pos = float3(input.UV.xy, depth);
	
	//return float4(depth, 0, 0, 1.0);
	
    float d = 0.0;  
    for( int i=0; i < 6; ++i ) {  
    	
    	//float3 offset = mul(float3(sampOffset[i], 0), toNormalRot);
        // サンプリングオフセット値の回転  
        //float2 samp = offset.xy;// * rot;  
    	float2 samp = sampOffset[i] / ln_ViewportPixelSize;
        //return float4(samp, 0, 1);
          
        // サンプリング点Aの深度値取得  
        float2 sl = input.UV + samp;  
        float3 pl = float3( sl.xy, GetDepth(sl));//tex2D(_screenSampler, sl).r);//tex2D(normalDepth, sl).w );  
        // pl.z は、near:0.0~far:1.0
          
        // モデルのエッジを検出  
        if( pl.z < pos.z - EdgeBias ) continue;  
		// pl が pos の手前にあるぞ
        //return float4(0, 1, 0, 1);
          
        // 中心点からの角度を計算  
        float tl = atan( tangent(pos, pl) );  
  
        // サンプリング点A’の深度値取得  
        float2 sr = input.UV - samp;  
        float3 pr = float3( sr.xy, GetDepth(sr));//tex2D(_screenSampler, sr).r );  //tex2D(normalDepth, sr).w );  
  
        if( pr.z < pos.z - EdgeBias ) continue;  
  
        float tr = atan( tangent(pos, pr) );  
        
        
		//float r = length(sampOffset[i]) / 2;
        
        
        
        // 遮蔽率を足し込む  
        d += clamp( ((tl+tr) / PI), 0.0, 1.0 );// * r;  
    }
    d = clamp( d / 6.0, 0.0, 1.0 ); 
	
    //return float4(tex2D(_viewDepthMap, input.UV).xy, 0, 1);
    //return float4(GetDepth(input.UV), 0, 0, 1);
    //return tex2D(_screenSampler, input.UV);
	return float4((1.0-d), (1.0-d), (1.0-d), 1.0);
	//return float4((1.0-d) * (1.0-depth), (1.0-d) * (1.0-depth), (1.0-d) * (1.0-depth), 1.0);
	//LN_CalculateToneColor(tex2D(_screenSampler, texUV), _Tone);
#endif
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

