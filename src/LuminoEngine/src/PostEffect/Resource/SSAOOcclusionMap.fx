
#include <Lumino.fxh>

// Note:
// http://developer.wonderpla.net/entry/blog/engineer/iPhone5S_SSAO/

//==============================================================================

#define ln_ViewportPixelSize ln_Resolution.xy

sampler2D _screenSampler;   // normalDepth
sampler2D _viewDepthMap;


const float bb = 1.00;
const float2 sampOffset[6] = {  // px 単位
	//float2(-1.0,  0.0) / ln_ViewportPixelSize,
	//float2(-0.5, -0.5) / ln_ViewportPixelSize,
	//float2( 0.5, -0.5) / ln_ViewportPixelSize,
	//float2( 1.0,  0.0) / ln_ViewportPixelSize,
	//float2( 0.5,  0.5) / ln_ViewportPixelSize,
	//float2(-0.5,  0.5) / ln_ViewportPixelSize,
	
	float2(-1.0,  0.0),// * bb,
	float2(-0.7,  0.7) * bb * 2,
	float2( 0.0,  1.0) * bb * 4,
	float2( 0.7,  0.7) * bb * 6,
	float2( 1.0,  0.0) * bb * 8,
	float2( 1.0,  1.0) * bb * 10,
};    // サンプリング点のオフセット値 

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
	float3 up = float3(lookAt.y, lookAt.z, lookAt.x);	// なんでもいい

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

float GetDepth(float2 uv)
{
    //tex2D(_screenSampler, input.UV).r
    return tex2D(_viewDepthMap, uv).g;
}

float4 PSMain(PSInput input) : SV_TARGET0
{
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

