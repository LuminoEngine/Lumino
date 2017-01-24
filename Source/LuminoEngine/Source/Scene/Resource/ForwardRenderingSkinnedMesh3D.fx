


// 座標変換行列
float4x4	WorldViewProjMatrix		: WORLDVIEWPROJECTION;
float4x4	WorldMatrix				: WORLD;
float4x4	ViewMatrix				: VIEW;
float4x4	ProjMatrix				: PROJECTION;

//ライト関連
bool		LightEnables[MMM_LightCount]		: LIGHTENABLES;		// 有効フラグ
float4x4	LightWVPMatrices[MMM_LightCount]	: LIGHTWVPMATRICES;	// 座標変換行列
float3		LightDirection[MMM_LightCount]		: LIGHTDIRECTIONS;	// 方向
float3		LightPositions[MMM_LightCount]		: LIGHTPOSITIONS;	// ライト位置
float		LightZFars[MMM_LightCount]			: LIGHTZFARS;		// ライトzFar値

//材質モーフ関連
float4	 AddingTexture = float4(0, 0, 0, 0)		  ;	// 材質モーフ加算Texture値
float4	 AddingSphere = float4(0, 0, 0, 0)		  ;	// 材質モーフ加算SphereTexture値
float4	 MultiplyTexture = float4(1, 1, 1, 1)	  ;	// 材質モーフ乗算Texture値
float4	 MultiplySphere	 = float4(1, 1, 1, 1)	  ;	// 材質モーフ乗算SphereTexture値

//カメラ位置
float3	 CameraPosition		: POSITION  < string Object = "Camera"; >;

// マテリアル色
float4   MaterialDiffuse	: DIFFUSE  < string Object = "Geometry"; >;
float3   MaterialAmbient	: AMBIENT  < string Object = "Geometry"; >;
float3   MaterialEmmisive	: EMISSIVE < string Object = "Geometry"; >;
float3   MaterialSpecular	: SPECULAR < string Object = "Geometry"; >;
float    SpecularPower		: SPECULARPOWER < string Object = "Geometry"; >;


// ライト色
float3   LightDiffuses[MMM_LightCount]      : LIGHTDIFFUSECOLORS;
float3   LightAmbients[MMM_LightCount]      : LIGHTAMBIENTCOLORS;
float3   LightSpeculars[MMM_LightCount]     : LIGHTSPECULARCOLORS;

// ライト色
static float4 DiffuseColor[3]  = { MaterialDiffuse * float4(LightDiffuses[0], 1.0f)
				 , MaterialDiffuse * float4(LightDiffuses[1], 1.0f)
				 , MaterialDiffuse * float4(LightDiffuses[2], 1.0f)};
static float3 AmbientColor[3]  = { saturate(MaterialAmbient * LightAmbients[0]) + MaterialEmmisive
				 , saturate(MaterialAmbient * LightAmbients[1]) + MaterialEmmisive
				 , saturate(MaterialAmbient * LightAmbients[2]) + MaterialEmmisive};
static float3 SpecularColor[3] = { MaterialSpecular * LightSpeculars[0]
				 , MaterialSpecular * LightSpeculars[1]
				 , MaterialSpecular * LightSpeculars[2]};



float           gOpacity            : OPACITY;
float4          gColorScale         : COLORSCALE;
float4          gBlendColor         : BLENDCOLOR;
float4          gTone               : TONE;
static float4   gColor              = float4( gColorScale.xyz, gColorScale.a * gOpacity );

// オブジェクトのテクスチャ
texture ObjectTexture: MATERIALTEXTURE;
sampler ObjTexSampler = sampler_state {
	texture = <ObjectTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

// スフィアマップのテクスチャ
texture ObjectSphereMap: MATERIALSPHEREMAP;
sampler ObjSphareSampler = sampler_state {
	texture = <ObjectSphereMap>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VS_OUTPUT
{
	float4 Pos		: POSITION;		// 射影変換座標
	float2 Tex		: TEXCOORD0;	// テクスチャ
	float4 SubTex	: TEXCOORD1;	// サブテクスチャ/スフィアマップテクスチャ座標
	float3 Normal	: TEXCOORD2;	// 法線
	float3 Eye		: TEXCOORD3;	// カメラとの相対位置
#if 0
	float4 SS_UV1   : TEXCOORD4;	// セルフシャドウテクスチャ座標
	float4 SS_UV2   : TEXCOORD5;	// セルフシャドウテクスチャ座標
	float4 SS_UV3   : TEXCOORD6;	// セルフシャドウテクスチャ座標
#endif
	float4 Color	: COLOR0;		// ライト0による色
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(MMM_SKINNING_INPUT IN, uniform bool useTexture, uniform bool useSphereMap, uniform bool useToon, uniform bool useSelfShadow)
{
	VS_OUTPUT output;
	
	// 座標と法線を取得する
	MMM_SKINNING_OUTPUT skinOut = MMM_SkinnedPositionNormal(IN.Pos, IN.Normal, IN.BlendWeight, IN.BlendIndices, IN.SdefC, IN.SdefR0, IN.SdefR1);
	
	// カメラとの相対位置
	output.Eye = CameraPosition - mul( skinOut.Position, WorldMatrix ).xyz;
	// 頂点法線
	output.Normal = normalize( mul( skinOut.Normal, (float3x3)WorldMatrix ) );
	
	// 頂点座標
	//if (MMM_IsDinamicProjection)
	//{
	//	float4x4 wvpmat = mul(mul(WorldMatrix, ViewMatrix), MMM_DynamicFov(ProjMatrix, length(output.Eye)));
	//	output.Pos = mul( Skinoutput.Position, wvpmat );
	//}
	//else
	//{
		output.Pos = mul( skinOut.Position, WorldViewProjMatrix );
	//}
	
	// ディフューズ色＋アンビエント色 計算
	float3 color = float3(0, 0, 0);
	float3 ambient = float3(0, 0, 0);
	float count = 0;
	for (int i = 0; i < 3; i++)
	{
		if (LightEnables[i])
		{
			color += (float3(1,1,1) - color) * (max(0, DiffuseColor[i].rgb * dot(output.Normal, -LightDirection[i])));
			ambient += AmbientColor[i];
			count = count + 1.0;
		}
	}
	output.Color.rgb = saturate(ambient / count + color);
	output.Color.a = MaterialDiffuse.a;

	// テクスチャ座標
	output.Tex = IN.Tex;
	output.SubTex = float4(IN.AddUV1.xy, 0, 0);
	
	if ( useSphereMap )
	{
		// スフィアマップテクスチャ座標
		float2 NormalWV = mul( output.Normal, (float3x3)ViewMatrix );
		output.SubTex.z = NormalWV.x * 0.5f + 0.5f;
		output.SubTex.w = NormalWV.y * -0.5f + 0.5f;
	}
	
	//output.Color.r = IN.BlendWeight.x;
	//output.Color.g = IN.BlendWeight.y;
	//output.Color.b = IN.BlendWeight.z;
	//output.Color.a = 1;

#if 0
	if (useSelfShadow)
	{
		float4 dpos = mul(skinOut.Position, WorldMatrix);
		//デプスマップテクスチャ座標
		output.SS_UV1 = mul(dpos, LightWVPMatrices[0]);
		output.SS_UV2 = mul(dpos, LightWVPMatrices[1]);
		output.SS_UV3 = mul(dpos, LightWVPMatrices[2]);
		
		output.SS_UV1.y = -output.SS_UV1.y;
		output.SS_UV2.y = -output.SS_UV2.y;
		output.SS_UV3.y = -output.SS_UV3.y;

		output.SS_UV1.z = (length(LightPositions[0] - skinOut.Position) / LightZFars[0]);
		output.SS_UV2.z = (length(LightPositions[1] - skinOut.Position) / LightZFars[1]);
		output.SS_UV3.z = (length(LightPositions[2] - skinOut.Position) / LightZFars[2]);
	}
#endif

    return output;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(VS_OUTPUT IN, uniform bool useTexture, uniform bool useSphereMap, uniform bool useToon, uniform bool useSelfShadow) : COLOR0
{
	return tex2D(ObjTexSampler, IN.Tex);//IN.Color;//float4(1, 0, 0, 1);// 
#if 0
	float4 Color = IN.Color;
	float4 texColor = float4(1,1,1,1);
	float  texAlpha = MultiplyTexture.a + AddingTexture.a;

	//スペキュラ色計算
	float3 HalfVector;
	float3 Specular = 0;
	for (int i = 0; i < 3; i++) {
		if (LightEnables[i]) {
			HalfVector = normalize( normalize(IN.Eye) + -LightDirection[i] );
			Specular += pow( max(0,dot( HalfVector, normalize(IN.Normal) )), SpecularPower ) * SpecularColor[i];
		}
	}

	// テクスチャ適用
	if (useTexture) {
		texColor = tex2D(ObjTexSampler, IN.Tex);
		texColor.rgb = (texColor.rgb * MultiplyTexture.rgb + AddingTexture.rgb) * texAlpha + (1.0 - texAlpha);
	}
	Color.rgb *= texColor.rgb;

	// スフィアマップ適用
	//if ( useSphereMap ) {
	//	// スフィアマップ適用
	//	if(spadd) Color.rgb = Color.rgb + (tex2D(ObjSphareSampler,IN.SubTex.zw).rgb * MultiplySphere.rgb + AddingSphere.rgb);
	//	else      Color.rgb = Color.rgb * (tex2D(ObjSphareSampler,IN.SubTex.zw).rgb * MultiplySphere.rgb + AddingSphere.rgb);
	//}
	// アルファ適用
	Color.a = IN.Color.a * texColor.a;

	// セルフシャドウなしのトゥーン適用
#if 0
	float3 color;
	if (!useSelfShadow && useToon && usetoontexturemap ) {
		//================================================================================
		// MikuMikuMovingデフォルトのトゥーン色を取得する(MMM_GetToonColor)
		//================================================================================
		color = MMM_GetToonColor(MaterialToon, IN.Normal, LightDirection[0], LightDirection[1], LightDirection[2]);
		Color.rgb *= color;
	}
	// セルフシャドウ
	if (useSelfShadow) {

		if (useToon && usetoontexturemap) {
			//================================================================================
			// MikuMikuMovingデフォルトのセルフシャドウ色を取得する(MMM_GetSelfShadowToonColor)
			//================================================================================
			float3 shadow = MMM_GetToonColor(MaterialToon, IN.Normal, LightDirection[0], LightDirection[1], LightDirection[2]);
			color = MMM_GetSelfShadowToonColor(MaterialToon, IN.Normal, IN.SS_UV1, IN.SS_UV2, IN.SS_UV3, false, useToon);

			Color.rgb *= min(shadow, color);
		}
		else {
			Color.rgb *= MMM_GetSelfShadowToonColor(MaterialToon, IN.Normal, IN.SS_UV1, IN.SS_UV2, IN.SS_UV3, false, useToon);
		}
	}
#endif
	
	// スペキュラ適用
	Color.rgb += Specular;

	return Color;
#endif
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
    pass P0
    {
        VertexShader = compile vs_3_0 vsBasic(true, false, false, false);
        PixelShader	 = compile ps_3_0 psBasic(true, false, false, false);
        CullMode = None;
    }
}

//=============================================================================
//
//=============================================================================
