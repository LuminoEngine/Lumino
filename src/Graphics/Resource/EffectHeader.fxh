

#ifdef LN_HLSL_DX9

//==============================================================================
// Lumino
//==============================================================================

static const int LN_LightCount = 3;

// 座標変換行列
float4x4	ln_WorldViewProjection;
float4x4	ln_World;
float4x4	ln_View;
float4x4	ln_Projection;
float2		ln_ViewportPixelSize;

//ライト関連
bool		ln_LightEnables[LN_LightCount];		// 有効フラグ
float3		ln_LightDirections[LN_LightCount];		// 方向
float3		ln_LightPositions[LN_LightCount];		// ライト位置
float		ln_LightZFars[LN_LightCount];			// ライトzFar値

// マテリアル色
float4		ln_MaterialDiffuse;
float3		ln_MaterialAmbient;
float3		ln_MaterialEmmisive;
float3		ln_MaterialSpecular;
float		ln_MaterialSpecularPower;
texture		ln_MaterialTexture;

// ライト色
float3		ln_LightDiffuses[LN_LightCount];
float3		ln_LightAmbients[LN_LightCount];
float3		ln_LightSpeculars[LN_LightCount];

// ライト色
static float4 ln_DiffuseColor[LN_LightCount] =
{
	ln_MaterialDiffuse * float4(ln_LightDiffuses[0], 1.0f),
	ln_MaterialDiffuse * float4(ln_LightDiffuses[1], 1.0f),
	ln_MaterialDiffuse * float4(ln_LightDiffuses[2], 1.0f),
};
static float3 ln_AmbientColor[LN_LightCount] =
{
	saturate(ln_MaterialAmbient * ln_LightAmbients[0]) + ln_MaterialEmmisive,
	saturate(ln_MaterialAmbient * ln_LightAmbients[1]) + ln_MaterialEmmisive,
	saturate(ln_MaterialAmbient * ln_LightAmbients[2]) + ln_MaterialEmmisive,
};
static float3 ln_SpecularColor[LN_LightCount] =
{
	ln_MaterialSpecular * ln_LightSpeculars[0],
	ln_MaterialSpecular * ln_LightSpeculars[1],
	ln_MaterialSpecular * ln_LightSpeculars[2],
};


//------------------------------------------------------------------------------
float4 LN_GetLambertVertexColor(float3 normal)
{
	// ディフューズ色＋アンビエント色 計算
	float3 color = float3(0, 0, 0);
	float3 ambient = float3(0, 0, 0);
	float count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (ln_LightEnables[i])
		{
			color += (float3(1,1,1) - color) * (max(0, ln_DiffuseColor[i].rgb * dot(normal, -ln_LightDirections[i])));
			//output.Color.rgb = float3(dot(output.Normal, -LightDirection[i]), 0, 0);
			ambient += ln_AmbientColor[i];
			count = count + 1.0;
		}
	}
	
	float4 outColor;
	outColor.rgb = saturate(ambient / count + color);
	outColor.a = ln_MaterialDiffuse.a;
	return outColor;
}


//==============================================================================
// MikuMikuMoving
//==============================================================================

//ライト数
static const int MMM_LightCount = 3;

float	 MMM_modelsize = 1.0;

// ボーンテクスチャサイズの逆数
float2 lnBoneTextureReciprocalSize = float2( 1.0/4.0, 1.0/256.0 );

// ボーン用頂点テクスチャサンプラー宣言
texture lnBoneTexture;
sampler2D lnBoneSampler = sampler_state
{
	Texture = <lnBoneTexture>;
	MinFilter = Point;  // 殆どのGPUでは以下のようなステート設定にしないと
	MagFilter = Point;  // 頂点テクスチャのフェッチがうまくいかない
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};
texture lnBoneLocalQuaternionTexture;
sampler2D lnBoneLocalQuaternionSampler = sampler_state
{
	Texture = <lnBoneLocalQuaternionTexture>;
	MinFilter = Point; 
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};

struct MMM_SKINNING_INPUT
{
	float4	Pos				: POSITION;
	float4	BlendWeight		: BLENDWEIGHT;
	float4	BlendIndices	: BLENDINDICES;
	float4	Color			: COLOR0;		// 独自
	float3	Normal			: NORMAL;
	float2	Tex				: TEXCOORD0;
	float4	AddUV1			: TEXCOORD1;
	float4	AddUV2			: TEXCOORD2;
	float4	AddUV3			: TEXCOORD3;
	float4	AddUV4			: TEXCOORD4;
	float4	SdefC			: TEXCOORD5;
	float3	SdefR0			: TEXCOORD6;
	float3	SdefR1			: TEXCOORD7;
	float	EdgeWeight		: TEXCOORD8;
	float	Index			: PSIZE15;
};

struct MMM_SKINNING_OUTPUT
{
	float4	Position;
	float3	Normal;
};

#define MaxIndices 4


//------------------------------------------------------------------------------
float4x3 LN_GetBoneMatrix(int boneIndex)
{
	float2 uv = lnBoneTextureReciprocalSize;
	float4 tc0 = float4( 0.5f * uv.x, (boneIndex + 0.5f) * uv.y, 0, 1 );	// +0.5 は半ピクセル分
	float4 tc1 = float4( 1.5f * uv.x, (boneIndex + 0.5f) * uv.y, 0, 1 );
	float4 tc2 = float4( 2.5f * uv.x, (boneIndex + 0.5f) * uv.y, 0, 1 );
	//float4 tc3 = float4( 3.5f * uv.x, (boneIndex + 0.5f) * uv.y, 0, 1 );
	return float4x3(
		tex2Dlod( lnBoneSampler, tc0 ),
		tex2Dlod( lnBoneSampler, tc1 ),
		tex2Dlod( lnBoneSampler, tc2 )/*,
		tex2Dlod( lnBoneSampler, tc3 )*/ );
}

//------------------------------------------------------------------------------
float4 LN_GetBoneLocalQuaternion(int boneIndex)
{
	float2 uv = lnBoneTextureReciprocalSize;
	float4 tc0 = float4(0.5f, (boneIndex + 0.5f) * uv.y, 0, 1);	// +0.5 は半ピクセル分
	return tex2Dlod(lnBoneLocalQuaternionSampler, tc0);
}

//------------------------------------------------------------------------------
float4 LN_QuaternionSlerp(float4 q1, float4 q2, float t)
{
	float4 quat;

	float opposite, inverse;
	float tt = 1;
	float dot = q1.w * q2.w + q1.x * q2.x + q1.y * q2.y + q1.z * q2.z;

	if (dot < 0)
	{
		tt = -1;
		dot = -dot;
	}

	if (dot > 0.999999)
	{
		inverse = 1.0 - t;
		opposite = t * tt;
	}
	else
	{
		float acosf = acos(dot);
		float invSin = 1.0 / sin(acosf);

		inverse = sin((1.0 - t) * acosf) * invSin;
		opposite = sin(t * acosf) * invSin * tt;
	}

	return float4(inverse * q1.x + opposite * q2.x, inverse * q1.y + opposite * q2.y, inverse * q1.z + opposite * q2.z, inverse * q1.w + opposite * q2.w);
}

//------------------------------------------------------------------------------
float4x4 LN_QuaternionToMatrix(float4 quat)
{
	float4x4 mat;

	float xy = quat.x * quat.y;
	float zz = quat.z * quat.z;
	float wx = quat.w * quat.x;
	float wy = quat.w * quat.y;
	float yz = quat.y * quat.z;
	float wz = quat.w * quat.z;

	mat._11 = 1.0f - 2.0f * quat.y * quat.y - 2.0f * zz;
	mat._12 = 2.0f * xy + 2.0f * wz;
	mat._13 = 2.0f * quat.x * quat.z - 2.0f * wy;
	mat._14 = 0;

	mat._21 = 2.0f * xy - 2.0f * wz;
	mat._22 = 1.0f - 2.0f * quat.x * quat.x - 2.0f * zz;
	mat._23 = 2.0f * yz + 2.0f * wx;
	mat._24 = 0;

	mat._31 = 2.0f * quat.x * quat.z + 2.0f * wy;
	mat._32 = 2.0f * yz - 2.0f * wx;
	mat._33 = 1.0f - 2.0f * quat.x * quat.x - 2.0f * quat.y * quat.y;
	mat._34 = 0;

	mat._41 = 0;
	mat._42 = 0;
	mat._43 = 0;
	mat._44 = 1.0f;

	return mat;
}


MMM_SKINNING_OUTPUT MMM_SkinnedPositionNormal(
	float4	position,
	float3	normal,
	float4	weights,
	float4	indices,
	float4	sdefC,
	float3	sdefR0,
	float3	sdefR1 )
{
	MMM_SKINNING_OUTPUT o;
	float4 pos = 0.0;
	float4 nml = 0.0;

	float blendWeights[4] = (float[4])weights;
	float BlendIndices[4] = (float[4])indices;
	
	if (sdefC.w >= 0)
	{
		float4 center = float4(sdefC.xyz, 1.0f);
		float4 r0 = float4(sdefR0, 1.0f);
		float4 r1 = float4(sdefR1, 1.0f);

		float3 pp0 = mul(r0, LN_GetBoneMatrix(BlendIndices[0]));
		float3 pp1 = mul(r1, LN_GetBoneMatrix(BlendIndices[1]));

		float3 prc = pp0 * blendWeights[0] + pp1 * blendWeights[1];

		float3 v1, diff;

		v1 = mul(center, LN_GetBoneMatrix(BlendIndices[0]));
		diff = (v1 - center.xyz) * blendWeights[0];

		v1 = mul(center, LN_GetBoneMatrix(BlendIndices[1]));
		diff += (v1 - center.xyz) * blendWeights[1];

		float3 trans = center.xyz + diff;
		trans = (trans + prc) * 0.5;

		float4 quat = LN_QuaternionSlerp(LN_GetBoneLocalQuaternion(BlendIndices[0]), LN_GetBoneLocalQuaternion(BlendIndices[1]), blendWeights[1]);
		float4x4 mat = LN_QuaternionToMatrix(quat);
		
		pos = mul(position - center, mat) * MMM_modelsize + float4(trans, 0.0f);
		nml = mul(float4(normal, 1.0f), mat) * MMM_modelsize;
		
		
	}
	else
	{
		float lastWeight = 0.0f;

		for (int i = 0; i < 3; i++)
		{
			lastWeight += blendWeights[i];
			pos.xyz += mul(position.xyz, LN_GetBoneMatrix(BlendIndices[i])) * blendWeights[i];
			nml.xyz += mul(normal.xyz, LN_GetBoneMatrix(BlendIndices[i])) * blendWeights[i];
		}
		lastWeight = 1.0f - lastWeight;
		pos.xyz += mul(position.xyz, LN_GetBoneMatrix(BlendIndices[3])) * lastWeight;
		nml.xyz += mul(normal.xyz, LN_GetBoneMatrix(BlendIndices[3])) * lastWeight;
		
		pos.w = 1.0f;
	}
	
	o.Position = pos;
	o.Normal = nml.xyz;
	
	return o;
}
/*
float4x4 World;
float4x4 View;
float4x4 Projection;
 
struct VertexShaderInput
{
    float4 Position : POSITION0;
};
 
///省略///
 
VertexShaderInput VertexShaderFunction(VertexShaderInput input)
{
///省略///
return input;
}
 
float4 PixelShaderFunction(VertexShaderInput input) : COLOR0
{
    return float4(1, 0, 0, 1);
}
*/


#endif // LN_HLSL_DX9

