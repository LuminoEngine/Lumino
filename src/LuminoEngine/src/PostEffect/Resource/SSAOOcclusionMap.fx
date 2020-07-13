
#include <Lumino.fxh>

// Note:
// http://developer.wonderpla.net/entry/blog/engineer/iPhone5S_SSAO/

//==============================================================================

#define ln_ViewportPixelSize ln_Resolution.xy

// https://qiita.com/mebiusbox2/items/2a18994a5caa6d584099#%E5%9F%BA%E6%9C%AC%E3%82%A2%E3%83%AB%E3%82%B4%E3%83%AA%E3%82%BA%E3%83%A0
// ViewSpace 上の、↑の図の最大半径
const float SamplingRange = 0.7;

const float radius = 0.005;

sampler2D _screenSampler;   // normalDepth
sampler2D _viewDepthMap;
sampler2D _randomSampler;

// TODO: move to lib
float3 LN_PackNormal(float3 normal)
{
	return (normal.xyz + 1.0) / 2.0;
}

const int SamplingCount = 16;

const float bb = 1.0 / SamplingCount;//1.0;
const float3 sampOffset[SamplingCount] = {  // px 単位
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

/*
    float3( 0.0, -1.0, 0.0) * bb * 1,
    float3( 0.7, -0.7, 0.0) * bb * 2,
    float3( 1.0,  0.0, 0.0) * bb * 3,
    float3( 0.7,  0.7, 0.0) * bb * 4,
    float3( 0.0,  1.0, 0.0) * bb * 5,
    float3(-0.7,  0.7, 0.0) * bb * 6,
    float3(-1.0,  0.0, 0.0) * bb * 7,
    float3(-0.7, -0.7, 0.0) * bb * 8,
    */
    
    float3(0.53812504, 0.18565957, -0.43192),
    float3(0.13790712, 0.24864247, 0.44301823),
    float3(0.33715037, 0.56794053, -0.005789503),
    float3(-0.6999805, -0.04511441, -0.0019965635),
    float3(0.06896307, -0.15983082, -0.85477847),
    float3(0.056099437, 0.006954967, -0.1843352),
    float3(-0.014653638, 0.14027752, 0.0762037),
    float3(0.010019933, -0.1924225, -0.034443386),
    float3(-0.35775623, -0.5301969, -0.43581226),
    float3(-0.3169221, 0.106360726, 0.015860917),
    float3(0.010350345, -0.58698344, 0.0046293875),
    float3(-0.08972908, -0.49408212, 0.3287904),
    float3(0.7119986, -0.0154690035, -0.09183723),
    float3(-0.053382345, 0.059675813, -0.5411899),
    float3(0.035267662, -0.063188605, 0.54602677),
    float3(-0.47761092, 0.2847911, -0.0271716)
    
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




#define SAMPLES    10    // SSAO 影判定のサンプル数

#if SAMPLES == 16
float3 samples[16] = {
    float3(0.53812504, 0.18565957, -0.43192),
    float3(0.13790712, 0.24864247, 0.44301823),
    float3(0.33715037, 0.56794053, -0.005789503),
    float3(-0.6999805, -0.04511441, -0.0019965635),
    float3(0.06896307, -0.15983082, -0.85477847),
    float3(0.056099437, 0.006954967, -0.1843352),
    float3(-0.014653638, 0.14027752, 0.0762037),
    float3(0.010019933, -0.1924225, -0.034443386),
    float3(-0.35775623, -0.5301969, -0.43581226),
    float3(-0.3169221, 0.106360726, 0.015860917),
    float3(0.010350345, -0.58698344, 0.0046293875),
    float3(-0.08972908, -0.49408212, 0.3287904),
    float3(0.7119986, -0.0154690035, -0.09183723),
    float3(-0.053382345, 0.059675813, -0.5411899),
    float3(0.035267662, -0.063188605, 0.54602677),
    float3(-0.47761092, 0.2847911, -0.0271716)
};

#elif SAMPLES == 12
float3 samples[12] = {
    float3(-0.13657719, 0.30651027, 0.16118456),
    float3(-0.14714938, 0.33245975, -0.113095455),
    float3(0.030659059, 0.27887347, -0.7332209),
    float3(0.009913514, -0.89884496, 0.07381549),
    float3(0.040318526, 0.40091, 0.6847858),
    float3(0.22311053, -0.3039437, -0.19340435),
    float3(0.36235332, 0.21894878, -0.05407306),
    float3(-0.15198798, -0.38409665, -0.46785462),
    float3(-0.013492276, -0.5345803, 0.11307949),
    float3(-0.4972847, 0.037064247, -0.4381323),
    float3(-0.024175806, -0.008928787, 0.17719103),
    float3(0.694014, -0.122672155, 0.33098832)
};

#elif SAMPLES == 8
float3 samples[8] = {
    float3(0.24710192, 0.6445882, 0.033550154),
    float3(0.00991752, -0.21947019, 0.7196721),
    float3(0.25109035, -0.1787317, -0.011580509),
    float3(-0.08781511, 0.44514698, 0.56647956),
    float3(-0.011737816, -0.0643377, 0.16030222),
    float3(0.035941467, 0.04990871, -0.46533614),
    float3(-0.058801126, 0.7347013, -0.25399926),
    float3(-0.24799341, -0.022052078, -0.13399573)
};

#else
float3 samples[10] = {
    float3(-0.010735935, 0.01647018, 0.0062425877),
    float3(-0.06533369, 0.3647007, -0.13746321),
    float3(-0.6539235, -0.016726388, -0.53000957),
    float3(0.40958285, 0.0052428036, -0.5591124),
    float3(-0.1465366, 0.09899267, 0.15571679),
    float3(-0.44122112, -0.5458797, 0.04912532),
    float3(0.03755566, -0.10961345, -0.33040273),
    float3(0.019100213, 0.29652783, 0.066237666),
    float3(0.8765323, 0.011236004, 0.28265962),
    float3(0.29264435, -0.40794238, 0.15964167)
};
#endif


float ExSdSSAO_GetZ(float2 uv) {
    return tex2D(_viewDepthMap, uv).g;
}

const float rad = 0.05;
const float density = 2.0;
const float strength = 5.0;
const float falloff = 0.1;//1.0;
const float scaling = 0.1;
const float3 ObjXYZ = float3(1, 1, 1);
#define invSamples (1.0f / SAMPLES)
#define rnd_offset 18.0f

float4 PS_SSAO(float2 inTex)
{
    float3 fres = normalize((tex2D(_randomSampler, inTex * rnd_offset).xyz * 2.0f) - 1.0f);
    
    float  currentPixelDepth = /*LinearZぽい*/ExSdSSAO_GetZ(inTex);
    float4 currentPixelSample = tex2D(_screenSampler, inTex);//tex2D(NormalMap, inTex);
    
    float  w1 = GetDepth(inTex);/* projectedZ ぽい*/ //tex2D(DepthMap, inTex).g;
    float  w2 = pow(max(0.5, w1), 0.4);
    
    float  radD = rad * (5.0 / w2 + 0.3);
    float3 norm = normalize(currentPixelSample.xyz * 2.0f - 1.0f);
    
    float  occluderDepth, depthDifference, normDiff, bl = 0.0f, finalColor = 0.0f;
    float2 se;
    float3 ray, occNorm;
    
    
    float falloff_e = (w1 * 0.005 + w2 * 0.05) * falloff * ObjXYZ.y;
    float strength_e = (w1 * 0.05 + w2 * 2) * strength * ObjXYZ.x;
    
    for(int i = 0; i < SAMPLES ; i++)
    {
        ray = radD * reflect(samples[i], fres);
        se = inTex + sign(dot(ray, norm)) * ray.xy * float2(1.0f, -1.0f);
        //occNorm = normalize(tex2D(NormalMap, se).xyz * 2.0f - 1.0f);
        occNorm = normalize(GetNormal(se));
        
        depthDifference = currentPixelDepth - ExSdSSAO_GetZ(se);
        normDiff = 1.0f - dot(occNorm, norm);
        
        bl += smoothstep(0, falloff_e, depthDifference)
            * (1.0f - pow(smoothstep(falloff_e, strength_e, depthDifference), 0.5)) * normDiff;
        
    }
    
    float ao = density * bl * invSamples * scaling;
    ao *= (1 + min(w1 / 120, 2)); //遠方強調
    ao = max(0, ao);
    ao = pow(ao * 1.5, max(0.5, 1.5 - ao)) / 1.5; //やわらか関数
    
    
    //ao = 1.0f - sp;
    
    return  float4(ao, ao, ao, 1.0f);
    
}



float4 PSMain(PSInput input) : SV_TARGET0
{

    //return PS_SSAO(input.UV);
    //return tex2D(_randomSampler, input.UV);
#if 1
    // UV 位置の深度。ProjI で ViewPos を求めるのにも使いたいので、Linear ではない。
    float projectedDepth = GetDepth(input.UV);

    // ViewSpace 上の点を求める
    //float4 clipPos = float4(LN_UVToClipSpacePosition(input.UV), projectedDepth, 1.0);
    //float3 viewPos = mul(clipPos, ln_ProjectionI);
    float3 viewPos = GetViewPosition(input.UV, projectedDepth);
    float3 viewNormal = GetNormal(input.UV);

#if 0
    float  w1 = 1.0 / (GetDepth(input.UV));/* projectedZ ぽい*/ //tex2D(DepthMap, inTex).g;
    float  w2 = pow(max(0.5, w1), 0.4);
    float falloff_e = (w1 * 0.005 + w2 * 0.05) * falloff;// * ObjXYZ.y;
    float strength_e = (w1 * 0.05 + w2 * 2) * strength;// * ObjXYZ.x;
#endif

    float ao = 0.0f;
    float div = 0.0f;   // 平均計算のための分母
    if (projectedDepth < 1.0f) {
        for (int i = 0; i < SamplingCount; ++i) {
            float3 fres = tex2D(_randomSampler, input.UV + float2(0.01 * i, 0.0)).rgb;
#if 1
            //float3 samplingViewNormal = normalize(mul(sampOffset[i], _LN_MakeLazyToRot(viewNormal)));
            //float3 samplingOffset = sampOffset[i];
            float3 samplingViewNormal = normalize(sampOffset[i]);
#elif 0
            const float3 samplingOffset = mul(float3(sampOffset[i].xy, 0.0f), MakeRotation2DMatrix((float)i));
            float3 samplingViewNormal = normalize(float3(samplingOffset.xy, fres.z + 1.0f));
            //float3 samplingViewNormal = reflect(samples[i], fres);
#else
            // XY 平面上の offset を適当に回転する (TODO: 乱数にできるとよい)
            const float3 samplingOffset = mul(float3(sampOffset[i] * SamplingRange, 1.0), MakeRotation2DMatrix((float)i));

            // viewNormal が軸になるように samplingOffset を変換する
            float3 samplingViewNormal = normalize(mul(samplingOffset, _LN_MakeLazyToRot(viewNormal)));
            //float3 samplingViewNormal = normalize(float3(sampOffset[i], viewNormal.z));
            // TODO: samplingViewNormal を、viewNormal を軸にランダム回転する
#endif


            // 法線の反対側に向いてた (始点ジオメトリの内側になっていたら) ら外側へ反転する
            float dt = dot(viewNormal, samplingViewNormal);
            float s = sign(dt);
            samplingViewNormal *= s;
            //samplingOffset *= s;
            dt *= s;    // これで viewNormal と samplingViewNormal の向きの差を示す dt は、負値であることが無くなる。
                        // (同一方向なら1, 90度違っていれば0) -> 濃さとして使えるようになる。
            
            // サンプリング位置の clipPos を求める
            //float3 samplingViewPos = float3(viewPos + samplingViewNormal * fres);
            //float3 samplingViewPos = reflect(samplingViewNormal, fres);
            float3 samplingViewPos = float3(viewPos + (samplingViewNormal * (fres.z * 0.5 + 0.5)) * SamplingRange);
            float4 samplingClipPos = mul(float4(samplingViewPos, 1.0), ln_Projection);
            samplingClipPos.xyz /= samplingClipPos.w;

            // 計算結果が現在の場所の深度より奥に入ってるなら遮蔽されているという事なので加算
            float2 uv2 = LN_ClipSpacePositionToUV(samplingClipPos.xy);
            //return float4(GetDepth(uv2), 0, 0, 1);
            float samplingDepth = GetDepth(uv2);
            //float d = 1.0 - (abs(samplingDepth - samplingClipPos.z) / SamplingRange);
            //dt *= d;

            float depthDifference = samplingClipPos.z - samplingDepth;  // ↓のstepで負値は一律 0 になるので、正の値だけ考えればよい

    // TODO: Range check
    // https://learnopengl.com/Advanced-Lighting/SSAO
            float rangeCheck = smoothstep(0.0, 1.0, radius / abs(depthDifference));
#if 0
            ao +=
                smoothstep(0, falloff_e, depthDifference) *
                (1.0f - pow(smoothstep(falloff_e, strength_e, depthDifference), 0.5)) *
                dt;
#else
            ao += 
            step(samplingDepth, samplingClipPos.z) *   // step は、samplingClipPos.z の方が大きけ(奥にあれ)れば 1
            smoothstep(0.0, 1.2, samplingDepth) * // 濃さ調整。ちゃんと考えたほうがいいかも
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

