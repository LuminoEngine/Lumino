/* Lumino ShadingModel combined HLSL file (RawIR:Intermediate Representation) */

//BEGIN_HLSL

#include <Lumino.fxh>
#include <LuminoForward.fxh>
#include <LuminoPBR.fxh>
#include <LuminoShadow.fxh>
#include <LuminoSkinning.fxh>

//------------------------------------------------------------------------------
// Lib (ClusteredForward)


float4	ln_AmbientColor;
float4	ln_AmbientSkyColor;
float4	ln_AmbientGroundColor;
float4	ln_FogColorAndDensity;

// .x : Start distance (distance from camera)
// .y : Lower height
// .z : Upper height
// .w : Height fog Density
float4	ln_FogParams;

float3 ln_CameraPosition;
float3 ln_MainLightDirection;	// ライトの向き。Sun から地表へ向かう。ライトの正面方向

float _LN_CalcFogFactor(float depth)
{
	float f = ln_FogParams.a * depth;
	return exp2(-f);
	//return exp2(-f*f);
}


float smoothstep(float a, float b, float x)
{
  float t = saturate((x - a)/(b - a));
  return t * t * (3.0 - (2.0 * t));
}

// return: fog density (0.0~)
float _LN_CalcFogFactor2(float depth, float samplePointY)
{
	float lower = ln_FogParams.y;//-30.0;
	float upper = ln_FogParams.z;//100.0;
	float lengthH = ln_FogParams.x;//5.0;	// 水平方向、どのあたりから Fog を生成するか
	float fogDensity = ln_FogColorAndDensity.a;//1.0 / 10.0;// 1.0;	// Fog の濃さ。1.0 の場合、2^(x*1.0) で、座標1進むと完全に Fog になる。
								// 0.5 だと座標2、0.25だと座標4という具合に、逆数で入力すると距離を測ることができる。

	// g(x)=-2^(x*x)+2
	float halfVolumeHeight = (upper - lower) / 2;
	float center = lower + halfVolumeHeight;
	//float d = abs(samplePointY - center) / halfVolumeHeight;	// Fog volume 中央からどれだけ離れているか (0.0中央 1.0:上端または下端 1.0~:Fogの外)
	//float density = -exp2(d * 0.5) + 2;
	//float density = -exp2(d*d) + 2;
	//float d = 1.0 - (abs(samplePointY - center) / halfVolumeHeight);	// Fog volume 外側境界からどれだけ離れているか (0.0:上端または下端, 1.0:中央 1.0~)
	float d = (halfVolumeHeight - abs(samplePointY - center));// / lengthH;
	//d = saturate(d);
	float densityV = exp2(d * ln_FogParams.w) - 1.0;
	//return saturate(densityV);

	// h(x)=2^((x+2)*0.5)
	float f = ((depth - lengthH));	// 0.0:視点位置, 1.0:フォグの出始め
	//ln_FogParams.a * depth;
	
	float densityH = exp2(f * fogDensity) - 1.0;

	// Input-density が小さいと境界が少し不自然に明るくなるが、ほとんどのケースでは問題ないのでこのままとする。
	// TODO: 何かいい補完方法思いついたら修正するかも。
	return saturate(densityH) * saturate(densityV);

	//float mu = saturate(densityH * densityV);
	//return saturate(mu * saturate(densityV));

	//return saturate(max(densityH, 0.0) * densityV);
	//return saturate(smoothstep(0.1, 0.9, densityH) * densityV);
	//return lerp(saturate(densityH * densityV), densityV, saturate(densityH));
	//return saturate(densityH * densityV);

	
	//return saturate(densityH) * saturate(densityV);

	//return saturate(densityH);

	//return saturate(density);


	//float lower = -5.0;
	//float upper = 0.0;
    //float d =  abs(height - lerp(upper, lower, 0.5));
    //float f = 0.5 * d;
	//return exp2(-f);
	//return exp2(-f*f);
}


const float3 _RayleighColorScale = float3(0.5, 0.2, 0.5);
const float turbidity = 1.0;
const float rayleigh = 0.5;
const float mieCoefficient = 0.005;
const float mieDirectionalG = 0.8;
const float luminance = 1.0;
const float3 up = float3( 0, 1, 0 );

const float e = 2.71828182845904523536028747135266249775724709369995957;


const float3 totalRayleigh = float3( 0.0000001, 0.0000001, 0.0000001);


const float3 lambda = float3( 680E-9, 550E-9, 450E-9 );
const float v = 4.0;
const float3 K = float3( 0.686, 0.678, 0.666 );
const float3 MieConst = LN_PI * pow( ( 2.0 * LN_PI ) / lambda, float3( v - 2.0 ) ) * K;
const float mieZenithLength = 1.25E3;
const float rayleighZenithLength = 8.4E3;

const float cutoffAngle = 1.6110731556870734;
const float steepness = 1.5;
const float EE = 1000.0;
float sunIntensity( float zenithAngleCos ) {
	zenithAngleCos = clamp( zenithAngleCos, -1.0, 1.0 );
	return EE * max( 0.0, 1.0 - pow( e, -( ( cutoffAngle - acos( zenithAngleCos ) ) / steepness ) ) );
}

// 3.0 / ( 16.0 * pi )
const float THREE_OVER_SIXTEENPI = 0.05968310365946075;
float rayleighPhase( float cosTheta ) {
	return THREE_OVER_SIXTEENPI * ( 1.0 + pow( cosTheta, 2.0 ) );
}

float3 totalMie( float T ) {
	float c = ( 0.2 * T ) * 10E-18;
	return 0.434 * c * MieConst;
}

// 1.0 / ( 4.0 * pi )
const float ONE_OVER_FOURPI = 0.07957747154594767;
float hgPhase( float cosTheta, float g ) {
	float g2 = pow( g, 2.0 );
	float inverse = 1.0 / pow( 1.0 - 2.0 * g * cosTheta + g2, 1.5 );
	return ONE_OVER_FOURPI * ( ( 1.0 - g2 ) * inverse );
}

// Filmic ToneMapping http://filmicgames.com/archives/75
const float A = 0.15;
const float B = 0.50;
const float C = 0.10;
const float D = 0.20;
const float E = 0.02;
const float F = 0.30;
const float whiteScale = 1.0748724675633854; // 1.0 / Uncharted2Tonemap(1000.0)
float3 Uncharted2Tonemap( float3 x ) {
	return ( ( x * ( A * x + C * B ) + D * E ) / ( x * ( A * x + B ) + D * F ) ) - E / F;
}

float3 LN_FogColor(float3 vWorldPosition)
{
	float3 cameraPos = ln_CameraPosition;
	float3 vSunDirection = -ln_MainLightDirection;
	float3 sunPosition = vSunDirection * 400000.0;

	float vSunfade = 1.0 - clamp( 1.0 - exp( ( sunPosition.y / 450000.0 ) ), 0.0, 1.0 );
	float vSunE = sunIntensity( dot( vSunDirection, up ) );
	float rayleighCoefficient = rayleigh - ( 1.0 * ( 1.0 - vSunfade ) );
	float vBetaR = (totalRayleigh * (float3(500.0, 500.0, 500.0) * _RayleighColorScale)) * rayleighCoefficient;
	float vBetaM = totalMie( turbidity ) * mieCoefficient;






	//float zenithAngle = acos( max( 0.0, dot( up, normalize( vWorldPosition - cameraPos ) ) ) );
	//float inverse = 1.0 / ( cos( zenithAngle ) + 0.15 * pow( 93.885 - ( ( zenithAngle * 180.0 ) / LN_PI ), -1.253 ) );
	//float sR = rayleighZenithLength * inverse;
	//float sM = mieZenithLength * inverse;
	float zenithAngle = acos( max( 0.0, dot( up, normalize( vWorldPosition - cameraPos ) ) ) );
	float inverse = 1.0 / ( cos( zenithAngle ) + 0.15 * pow( 93.885 - ( ( zenithAngle * 180.0 ) / LN_PI ), -1.253 ) );
	float sR = rayleighZenithLength * inverse;
	float sM = mieZenithLength * inverse;

	//float3 Fex = exp( -(vBetaM * sM ) );
	float3 Fex = exp( -( vBetaR * sR + vBetaM * sM ) );

	// in scattering
	float cosTheta = dot( normalize( vWorldPosition - cameraPos ), vSunDirection );

	float rPhase = rayleighPhase( cosTheta * 0.5 + 0.5 );
	float3 betaRTheta = vBetaR * rPhase;

	float mPhase = hgPhase( cosTheta, mieDirectionalG );
	float3 betaMTheta = vBetaM * mPhase;

	float3 Lin = pow( vSunE * ( ( betaRTheta + betaMTheta ) / ( vBetaR + vBetaM ) ) * ( 1.0 - Fex ), float3( 1.5 ) );
	Lin *= lerp( float3( 1.0 ), pow( vSunE * ( ( betaRTheta + betaMTheta ) / ( vBetaR + vBetaM ) ) * Fex, float3( 1.0 / 2.0 ) ), clamp( pow( 1.0 - dot( up, vSunDirection ), 5.0 ), 0.0, 1.0 ) );




	float3 texColor = ( Lin ) * 0.04;// + float3( 0.0, 0.0003, 0.00075 );

	float3 curr = Uncharted2Tonemap( ( log2( 2.0 / pow( luminance, 4.0 ) ) ) * texColor );
	float3 color = curr * whiteScale;

	float3 retColor = pow( color, float3( 1.0 / ( 1.2 + ( 1.2 * vSunfade ) ) ) );

	return float4( retColor, 1.0 );








	//return ln_FogColorAndDensity.rgb;
	return ln_MainLightDirection;
	return float3(1,0,0);
	//float mPhase = hgPhase( cosTheta, mieDirectionalG );
	//float3 betaMTheta = input.vBetaM * mPhase;

	//float3 Lin = pow( input.vSunE * ( ( betaRTheta + betaMTheta ) / ( input.vBetaR + input.vBetaM ) ) * ( 1.0 - Fex ), float3( 1.5 ) );
	//Lin *= lerp( float3( 1.0 ), pow( input.vSunE * ( ( betaRTheta + betaMTheta ) / ( input.vBetaR + input.vBetaM ) ) * Fex, float3( 1.0 / 2.0 ) ), clamp( pow( 1.0 - dot( up, input.vSunDirection ), 5.0 ), 0.0, 1.0 ) );
}



float4 _LN_PS_ClusteredForward_Default(
	//LN_PSInput_ClusteredForward extra,
	float3 worldPos,
	float3 vertexPos,
	LN_SurfaceOutput surface)
{
	// ビュー平面からの水平距離。視点からの距離ではないので注意
	float4 viewPos = mul(float4(worldPos, 1.0f), ln_View);
	
	// 頂点位置から視点位置へのベクトル
	float3 vViewPosition = -viewPos.xyz;
	
	_LN_LocalLightContext localLightContext;
	_LN_InitLocalLightContext(localLightContext, vertexPos, viewPos);
	
	float4 mc = surface.Albedo;
	
	/**/
	LN_PBRGeometry geometry;
	geometry.position = -vViewPosition;//-vViewPosition;
	geometry.normal = normalize(surface.Normal);//vNormal);
	geometry.viewDir = normalize(vViewPosition);//vViewPosition);

	/**/
	float metallic = ln_MaterialMetallic;//0.0;//0.5;	// TODO:
	float roughness = ln_MaterialRoughness;//1.0;//0.3;	// TODO:
	LN_PBRMaterial material;
	material.diffuseColor = lerp(surface.Albedo.xyz, float3(0, 0, 0), metallic);
	material.specularColor = lerp(float3(0.04, 0.04, 0.04), surface.Albedo.xyz, metallic);
	material.specularRoughness = clamp( roughness, 0.04, 1.0 );//roughness;

	float3 result = float3(0, 0, 0);

	
#if 0
	//result = float3(0, 0, 0);
	if (lightIndices[0] > 0) result.r += 1;
	if (lightIndices[1] > 0) result.g += 1;
	if (lightIndices[2] > 0) result.b += 1;
#endif


	/**/
	float opacity = 1.0;
	float3 outgoingLight = _LN_ComputePBRLocalLights(localLightContext, geometry, material);

	//return float4(surface.Emission + outgoingLight, opacity);
	result.rgb = surface.Emission + outgoingLight;
	
	// Shadow
    //float4 posInLight = extra.vInLightPosition;
    //outgoingLight *= LN_CalculateShadow(posInLight);
	
	// Fog
	float viewLength = length(ln_CameraPosition - worldPos);
	//result.rgb = lerp(ln_FogParams.rgb, result.rgb, _LN_CalcFogFactor(viewPos.z));
	//result.rgb = lerp(result.rgb, ln_FogParams.rgb, _LN_CalcFogFactor2(viewPos.z, worldPos.y));
	//result.rgb = lerp(result.rgb, ln_FogColorAndDensity.rgb, _LN_CalcFogFactor2(viewLength, worldPos.y));
	//result.rgb = lerp(result.rgb, ln_FogColorAndDensity.rgb, _LN_CalcFogFactor2(length(worldPos), worldPos.y));
	result.rgb = lerp(result.rgb, LN_FogColor(worldPos), _LN_CalcFogFactor2(viewLength, worldPos.y));
	//result.rgb = lerp(saturate(result.rgb), LN_FogColor(worldPos), _LN_CalcFogFactor2(viewLength, worldPos.y));
	//result.rgb = LN_FogColor(worldPos);

	//result.r = _LN_CalcFogFactor2(viewLength, worldPos.y);
	//result.g = 0;
	//result.b = 0;
	return float4(result.rgb, opacity);
	
	
	
	
	
	
	result.rgb = mc.rgb * result.rgb;
	
	// calc ambient color
	float3 ambient = float3(0, 0, 0);
	{
		// basic ambient light
		ambient = ln_AmbientColor.xyz * ln_AmbientColor.a;
		
		// hemisphere ambient light
		float hemisphere = (dot(surface.Normal, float3(0, 1, 0)) + 1.0) * 0.5;
		float4 c = lerp(ln_AmbientGroundColor, ln_AmbientSkyColor, hemisphere);
		ambient = saturate(ambient + c.xyz * c.a);
	}
	
	result.rgb += ambient;
	
	// fog
	result.rgb = lerp(ln_FogParams.rgb, result.rgb, _LN_CalcFogFactor(viewPos.z));
	
	
	return float4(result.rgb, 1.0);
}

//------------------------------------------------------------------------------
// User code

struct MyVFOutput
{
//	float4	WorldPos	: TEXCOORD5;
	float4	dummy	: TEXCOORD6;
};

struct MySSInput
{
	float4	Color		: COLOR0;		// defined
	float2	UV			: TEXCOORD0;	// defined
	float4	WorldPos	: TEXCOORD5;
};


void MyVFMain(LN_VSInput input, inout MyVFOutput output)	// ★ out 引数の型をパース
{
	output.dummy = float4(0, 0, 0, 0);
}

// Surface Shader
void MySSMain(MySSInput input, inout LN_SurfaceOutput output)
{
	//output.Albedo = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV) * input.Color;
	output.Albedo = tex2D(ln_MaterialTexture, input.UV) * input.Color;
	//output.Albedo = float4(1, 0, 0, 1);
}

//------------------------------------------------------------------------------
// (auto generation)
// ClusteredShading_Default.template.fx から生成する

struct _lngs_VSOutput
{
	float4	svPos		: SV_POSITION;

	// common
	float3	Normal		: NORMAL0;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;

	// clustered forward
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
};

// auto generation
_lngs_VSOutput _lngs_VS_ClusteredForward_Geometry(LN_VSInput vsi)
{
	LN_VSOutput_Common common = LN_ProcessVertex_Common(vsi);
	LN_VSOutput_ClusteredForward extra = LN_ProcessVertex_ClusteredForward(vsi);

	_lngs_VSOutput output;
	output.svPos = common.svPos;
	output.Normal = common.Normal;
	output.UV = common.UV;
	output.Color = common.Color;
	output.WorldPos = extra.WorldPos;
	output.VertexPos = extra.VertexPos;
	return output;
}
// auto generation

_lngs_VSOutput _lngs_VS_ClusteredForward_Geometry_SkinnedMesh(LN_VSInput vsi)
{
	LN_VSOutput_Common common = LN_ProcessVertex_SkinnedCommon(vsi);
	LN_VSOutput_ClusteredForward extra = LN_ProcessVertex_ClusteredForward(vsi);

	_lngs_VSOutput output;
	output.svPos = common.svPos;
	output.Normal = common.Normal;
	output.UV = common.UV;
	output.Color = common.Color;
	output.WorldPos = extra.WorldPos;
	output.VertexPos = extra.VertexPos;
	return output;

	//_lngs_VSOutput o;
	//o.common	= LN_ProcessVertex_SkinnedCommon(vsi);
	//o.extra		= LN_ProcessVertex_ClusteredForward(vsi);
	// ★ Scene固有のコードはここに直接生成する (ピクセルシェーダと書き方を合わせたい)
	//MyVFMain(vsi, o.user);	// ★ User定義呼び出し
	//return o;
}



struct _lngs_PSInput
{
	// common
	float3	Normal		: NORMAL0;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
	
	// clustered forward
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
};

struct _lngs_PSOutput
{
	float4 color0 : COLOR0;
	//float4 color1 : COLOR1;
};

_lngs_PSOutput _lngs_PS_ClusteredForward_Geometry(_lngs_PSInput input)
{
	LN_SurfaceOutput surface;
	_LN_InitSurfaceOutput(input.Normal, surface);

	//surface.Normal = input.Normal;
	
	// ★ライティングのコードはここに直接生成する (GBuffer生成などではマルチRT書き込みするため、戻り値も変えなければならない)
	// ・・・というより、ピクセルシェーダ全体を生成する。フラグメントの結合じゃダメ。
	

	// TODO: SurfaceShader を入れるのはこのあたり
	//surface.Albedo = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV) * input.Color;
	surface.Albedo = tex2D(ln_MaterialTexture, input.UV) * ln_MaterialColor * input.Color;

	surface.Emission = (ln_MaterialEmissive.rgb * ln_MaterialEmissive.a);
	
	_lngs_PSOutput o;
	o.color0 = _LN_PS_ClusteredForward_Default(input.WorldPos, input.VertexPos, surface);
	o.color0.a = surface.Albedo.a;
	//o.color0 = float4(1, 0, 0, 1);
	o.color0 = LN_GetBuiltinEffectColor(o.color0);

	//o.color0.r = ln_MaterialMetallic;
	
	return o;
}

float4	ln_MaterialAmbient;	// TODO: とりあえず MMD モデル用のために用意。
// ※MMM だと、GUI で設定したライトの「色」は Ambient に入ってくるようだ。Diffuse は常に 0 みたい。

float4 _lngs_PS_UnLighting(_lngs_PSInput input) : COLOR0
{
	//return float4(0, 1, 0, 1);
	float4 c = tex2D(ln_MaterialTexture, input.UV) * ln_MaterialColor * input.Color;
    clip(c.a - 0.0001);

	return LN_GetBuiltinEffectColor(c);
}

//------------------------------------------------------------------------------
/*
technique ClusteredForward
{
	pass ShadowCaster
	{
		VertexFactory	= MyVFMain;
		SurfaceShader	= MySSMain;
	}
	pass Geometry
	{
		VertexFactory	= MyVFMain;
		SurfaceShader	= MySSMain;
	}
}
*/
technique Forward_Geometry
{
	pass Pass1
	{
		VertexShader = _lngs_VS_ClusteredForward_Geometry;
		PixelShader	 = _lngs_PS_ClusteredForward_Geometry;
	}
}

technique Forward_Geometry_StaticMesh_UnLighting
{
	pass Pass1
	{
		VertexShader = _lngs_VS_ClusteredForward_Geometry;
		PixelShader	 = _lngs_PS_UnLighting;
	}
}

technique Forward_Geometry_SkinnedMesh
{
	pass Pass1
	{
		VertexShader = _lngs_VS_ClusteredForward_Geometry_SkinnedMesh;
		PixelShader	 = _lngs_PS_ClusteredForward_Geometry;
	}
}

technique Forward_Geometry_SkinnedMesh_UnLighting
{
	pass Pass1
	{
		VertexShader = _lngs_VS_ClusteredForward_Geometry_SkinnedMesh;
		PixelShader	 = _lngs_PS_UnLighting;
	}
}

//END_HLSL

