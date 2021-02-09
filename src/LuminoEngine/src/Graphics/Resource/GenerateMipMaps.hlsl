/*
 * fxc /T cs_5_1 /E GenerateMipMaps /Fh GenerateMipMaps.hlsl.inl GenerateMipMaps.hlsl
 */

SamplerState BilinearClamp : register(s0);
Texture2D<float4> SrcTexture : register(t0);
RWTexture2D<float4> DstTexture : register(u0);

cbuffer CB : register(b0)
{
	float2 TexelSize;	// 1.0 / destination dimension
	float GammaCurve;	// 1.0 for linear textures, 2.2 for srgb textures
}

[numthreads( 8, 8, 1 )]
void GenerateMipMaps(uint3 DTid : SV_DispatchThreadID)
{
	//DTid is the thread ID * the values from numthreads above and in this case correspond to the pixels location in number of pixels.
	//As a result texcoords (in 0-1 range) will point at the 4 pixels used for the mipmap.
	float4 color = pow(SrcTexture.SampleLevel(BilinearClamp, TexelSize * DTid.xy, 0), GammaCurve);
	color += pow(SrcTexture.SampleLevel(BilinearClamp, TexelSize * (DTid.xy + float2(1.0f, 0.0f)), 0), GammaCurve);
	color += pow(SrcTexture.SampleLevel(BilinearClamp, TexelSize * (DTid.xy + float2(0.0f, 1.0f)), 0), GammaCurve);
	color += pow(SrcTexture.SampleLevel(BilinearClamp, TexelSize * (DTid.xy + float2(1.0f, 1.0f)), 0), GammaCurve);
	color /= 4.0f;

	//Write the final color into the destination texture.
	DstTexture[DTid.xy] = pow(color, 1.0f/GammaCurve);
}
